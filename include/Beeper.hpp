/*
 * Beeper.hpp
 *
 *  Created on: Jan 14, 2026
 *      Author: User
 */

#pragma once
#include <msp430.h>

namespace Hardware{
    class Beeper {
    public:
        static void init(){
            P3DIR |= BIT6;
            P3SEL |= BIT6; //Select TA0.1 function
        }

        static void play(uint16_t freq){
            if (freq == 0){
                stop();
                return;
            }

            //Up mode, count from 0 to CCRO
            //frequency = SMCLK / (2 * CCR0)
            // with 1MHz SMCLK , for 440Hz: CCRO = 1,136
            uint16_t cycles = 1000000 / freq;

            TA1CCR0 = cycles; //Set the pitch
            TA1CCR2 = cycles >> 1; //50% duty cycle // shift right 1 i.e div by 2


            //Toggle every time timer hits CCR0
            TA1CCTL2 = OUTMOD_7;  //toggle mode for PWM2 pin

            //start timer
            TA1CTL = TASSEL_2 | MC_1 | TACLR; // SMCLK up-mode clear
        }

        static void stop(){
            TA1CTL = MC_0; //stop timer
            TA0CCTL1 &= ~OUTMOD_7; // reset output mode
            P1OUT &= ~BIT6;  // ensure pin is low
        }

    };

}
