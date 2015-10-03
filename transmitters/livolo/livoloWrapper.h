//
//  livoloWrapper.h
//  SmartHome
//
//  Created by Raffles Gun on 10/4/15.
//  Copyright (c) 2015 Raffles Gun. All rights reserved.
//

#ifndef LivoloWrapper_H
#define LivoloWrapper_H

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct Livolo Livolo;
    
    Livolo* newLivolo(unsigned char pin);
    
    void Livolo_SendButton(Livolo* v, unsigned int remoteID, unsigned char keycode);
    
    void deleteLivolo(Livolo* v);
    
#ifdef __cplusplus
}
#endif
#endif