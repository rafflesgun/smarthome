//
//  livoloWrapper.c
//  SmartHome
//
//  Created by Raffles Gun on 10/4/15.
//  Copyright (c) 2015 Raffles Gun. All rights reserved.
//

#include "livolo.h"
#include "livoloWrapper.h"
#include <stdio.h>

extern "C" {
    Livolo* newLivolo(unsigned char pin) {
        //printf("Inside newRCSwitch");
        return new Livolo(pin);
    }
    
    void Livolo_SendButton(Livolo* v, unsigned int remoteID, unsigned char keycode) {
        //printf("Inside RCSwitch_send");
        v->sendButton(remoteID, keycode);
    }
    
    void deleteLivolo(Livolo* v) {
        delete v;
    }
}