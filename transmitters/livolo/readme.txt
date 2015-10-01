This is a library to control Livolo branded wireless switches. 

Features:

- emulates buttons 1 to 10 and ALL OFF of Livolo remote controller

Usage:

Basically you need two things to get it to work:

1) Create Livolo instance
2) Use sendButton (unsigned int remoteID, byte keycode) function to "push" the buttons

sendButton function uses to arguments: remote ID and keycode. Typically, remote IDs are 16 bit unsigned values, but
not all of them are valid (maybe there are some IDs reserved only for system use or there is something I don't know).

Tested remote IDs: 

- read from real remote IDs: 6400; 19303
- "virtual" remote IDs: 10550; 8500; 7400
- Fullsize remote ID has this pattern: remoteID = 1 + N*3;
  where N is any integer number you choose starting from 0 to 21884 (so that the output doesn't exceed 65535)
  Example result: 1,4,7,10,13,...,6400,6403,6406....

You can try and find new IDs as well: put your switch into learning mode and start sendButton with remote ID you wish to use. If
it is a valid ID, switch will accept it.

Keycodes read from real remote:
Fullsize remote:
#1: 0, #2: 96, #3: 120, #4: 24, #5: 80, #6: 48, #7: 108, #8: 12, #9: 72; #10: 40, #OFF: 106

Mini remote:
#A: 8, #B: 16, #C: 56, #OFF (a.k.a #D): 42

Keycodes are 7 bit values (actually I use 8 bit values, just skip most significant (leftmost) bit), but other keycodes
could be reserved for system use (dimmer, for example).

For an example sketch see blink.ino under examples folder.