/*
 * System.hpp
 *
 *  Created on: Dec 28, 2025
 *      Author: Olayemi
 */
#pragma once
#include <msp430.h>

namespace Hardware{
class System{
public:
    static void initLowPower(){
        //Setting all port to output and driving them to low , to avoid floating and for low power consumption
        P1DIR = 0xFF;
        P1OUT = 0x00;

        P2DIR = 0xFF;
        P2OUT = 0x00;

        P3DIR = 0xFF;
        P3OUT = 0x00;
    }
};
}



