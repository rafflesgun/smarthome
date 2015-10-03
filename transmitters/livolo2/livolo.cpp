/*
  Livolo.cpp - Library for Livolo wireless switches.
  Created by Sergey Chernov, October 25, 2013.
  Released into the public domain.
  
  01/12/2013 - code optimization, thanks Maarten! http://forum.arduino.cc/index.php?topic=153525.msg1489857#msg1489857
  
*/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "livolo.h"
#include "Livolo.h"

Livolo::Livolo(byte pin)
{
  pinMode(pin, OUTPUT);
  txPin = pin;
}

// keycodes #1: 0, #2: 96, #3: 120, #4: 24, #5: 80, #6: 48, #7: 108, #8: 12, #9: 72; #10: 40, #OFF: 106
// real remote IDs: 6400; 19303
// tested "virtual" remote IDs: 10550; 8500; 7400
// other IDs could work too, as long as they do not exceed 16 bit
// known issue: not all 16 bit remote ID are valid
// have not tested other buttons, but as there is dimmer control, some keycodes could be strictly system
// use: sendButton(remoteID, keycode), see example blink.ino; 


void Livolo::sendButton(unsigned int remoteID, byte keycode) {

  for (pulse= 0; pulse <= 180; pulse = pulse+1) { // how many times to transmit a command
  sendPulse(1); // Start  
  high = true; // first pulse is always high

  for (i = 16; i>0; i--) { // transmit remoteID
    byte txPulse=bitRead(remoteID, i-1); // read bits from remote ID
    selectPulse(txPulse);    
    }

  for (i = 7; i>0; i--) { // transmit keycode
    byte txPulse=bitRead(keycode, i-1); // read bits from keycode
    selectPulse(txPulse);    
    }    
  }
   digitalWrite(txPin, LOW);
}

// build transmit sequence so that every high pulse is followed by low and vice versa

void Livolo::selectPulse(byte inBit) {
  
      switch (inBit) {
      case 0: 
       for (byte ii=1; ii<3; ii++) {
        if (high == true) {   // if current pulse should be high, send High Zero
          sendPulse(2); 
        } else {              // else send Low Zero
                sendPulse(4);
        }
        high=!high; // invert next pulse
       }
        break;
      case 1:                // if current pulse should be high, send High One
        if (high == true) {
          sendPulse(3);
        } else {             // else send Low One
                sendPulse(5);
        }
        high=!high; // invert next pulse
        break;        
      }
}

// transmit pulses
// slightly corrected pulse length, use old (commented out) values if these not working for you

void Livolo::sendPulse(byte txPulse) {

  switch(txPulse) { // transmit pulse
   case 1: // Start
   digitalWrite(txPin, HIGH);
   delayMicroseconds(500); // 550
   // digitalWrite(txPin, LOW); 
   break;
   case 2: // "High Zero"
   digitalWrite(txPin, LOW);
   delayMicroseconds(100); // 110
   digitalWrite(txPin, HIGH);
   break;   
   case 3: // "High One"
   digitalWrite(txPin, LOW);
   delayMicroseconds(300); // 303
   digitalWrite(txPin, HIGH);
   break;      
   case 4: // "Low Zero"
   digitalWrite(txPin, HIGH);
   delayMicroseconds(100); // 110
   digitalWrite(txPin, LOW);
   break;      
   case 5: // "Low One"
   digitalWrite(txPin, HIGH);
   delayMicroseconds(300); // 290
   digitalWrite(txPin, LOW);
   break;      
  } 
}

// =============================================================================================
// This is the main program part.
//
//

int main(int argc, char **argv)
{
    int cflg, tflg, dflg, iflg, lflg, kflg, pflg;
    int verbose=0;
    fflg = 0;
    int errflg=0;
    int c, j;
    
    if (wiringPiSetup () == -1)
        exit (1) ;
    
    // Sort out the options first!
    //
    // ./livolo -g <gid> -n <dev> on/off
    
    while ((c = getopt(argc, argv, ":b:fg:il:n:p:r:s:t:v")) != -1) {
        switch(c) {
            case 'b':						// Timing for the start pulse (Base Begin)
                p_start = atoi(optarg);
                break;
            case 'f':						// fake flag ...
                fflg = 1;
                break;
            case 'g':						// Group
                group = atoi(optarg);
                break;
            case 'i':						// Inverted codes. Experience has shown that this does not really work
                inverted=true;
                break;
            case 'l':						// Timing for the long pulse (typical around 300 uSec)
                p_long= atoi(optarg);
                break;
                
            case 'n':						// Device Number
                device = atoi(optarg);
                break;
            case 'p':						// output pin of wiringPi
                output_pin = atoi(optarg);;
                break;
            case 'r':						// Pulse repeats
                repeats=atoi(optarg);
                break;
            case 's':						// short pulse length
                p_short=atoi(optarg);
                break;
            case 't':						// Loop mode
                loops = atoi(optarg);
                if (loops<1) errflg=1;
                break;
            case 'v':						// Verbose, output long timing/bit strings
                verbose = 1;
                break;
            case ':':       				// -f or -o without operand
                fprintf(stderr,"Option -%c requires an operand\n", optopt);
                errflg++;
                break;
            case '?':
                fprintf(stderr, "Unrecognized option: -%c\n", optopt);
                errflg++;
        }
    }
    
    // Check for additional command such as on or off
    while (optind < argc) {
        if (verbose==1) printf("Additional Arguments: %s\n",argv[optind]);
        // we must be sure that off is not toggled between ON/OFF
        if (! strcmp(argv[optind], "off" )) device = 42;						// off -> ALL OFF
        optind++;
    }
    
    // If there is an error, display the correct usage of the command
    if (errflg) {
        fprintf(stderr, "usage: argv[0] (options) \n");
        
        fprintf(stderr, "\nSettings:\n");
        fprintf(stderr, "\t\t; This setting will affect other timing settings as well\n");
        fprintf(stderr, "-t\t\t; Test mode, will output received code from remote\n");
        fprintf(stderr, "-v\t\t; Verbose, will output more information about the received codes\n");
        fprintf(stderr, "-l\t\t; Long pulse time in uSec\n");
        fprintf(stderr, "-s\t\t; Short pulse time in uSec\n");
        fprintf(stderr, "-r\t\t; Repeats per train pulse time in uSec\n");
        
        exit (2);
    }
    
    // If the -v verbose flag is specified, output more information than usual
    if (verbose == 1) {
        
        printf("The following options have been set:\n\n");
        printf("-v\t; Verbose option\n");
        if (tflg>0) printf("-t\t; Test option\n");
        if (fflg>0) printf("-f\t; Fake option\n");
        printf("\n");
        printf("-g\tgroup  : %d\n",group);
        printf("-n\tdevice : %d\n",device);
        printf("-r\trepeats: %d\n",repeats);
        printf("-b\tp_begin: %d\n",p_start);
        printf("-s\tp_short: %d\n",p_short);
        printf("-l\tp_long : %d\n",p_long);
        
        printf("\n");
    }
    
    Livolo liv(output_pin);
    
    // 
    // Main LOOP
    //
    for ( j=1; j<=loops; j++)
    {
        
        if (verbose==1) {
            printf("Sending: j: %d, grp: %d, code: %d ...",j,group,device);
            printf(", start: %d, short: %d, long: %d ...",p_start,p_short,p_long);
            fflush(stdout);
        }
        liv.sendButton(group, device);	
        if (verbose==1) {						// Do the transmission
            printf(" ... done\n"); fflush(stdout);
        }
        if (j<loops) sleep(5);
    }
}
