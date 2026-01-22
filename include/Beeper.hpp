/*
 * Beeper.hpp
 *
 *  Created on: Jan 14, 2026
 *      Author: Olayemi
 */

#pragma once
#include <msp430.h>
#include <cstdint>

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
            uint16_t cycles = 1000000 / freq;

            TA0CCR0 = cycles; //Set the pitch
            TA0CCR2 = cycles / 2; //50% duty cycle // shift right 1 i.e div by 2

            //start timer
            TA0CTL = TASSEL_2 | MC_1 | TACLR; // SMCLK upmode

            //Toggle every time timer hits CCR0
            TA0CCTL2 = OUTMOD_7;  //toggle mode for PWM2 pin


        }

        static void playPeriod(uint16_t period){
            if (period == 0xFFFF || period == 0) {
                stop();
                return;
            }

            TA0CCR0 = period;
            TA0CCR2 = period >> 1;
            TA0CCTL2 = OUTMOD_7;
            TA0CTL = TASSEL_2 | MC_1 | TACLR;
        }

        static void stop(){
            TA0CTL &= ~MC_3; //stop timer
            TA0CCTL2 &= ~OUTMOD_7; // reset output mode
            P1OUT &= ~BIT6;  // ensure pin is low
        }

    };

}
