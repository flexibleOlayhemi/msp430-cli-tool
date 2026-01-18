/*
 * shiftRegister.hpp
 *
 *  Created on: Jan 18, 2026
 *      Author: Olayemi
 *
 *P2.4: Clock (CLK)
 *P2.5: /Clear (Reset)
 *P2.6: Serial Data In (for LEDs)
 *P2.7: Serial Data Out (for Buttons)
 *P2.0/P2.1: Mode Select for LED Register
 *P2.2/P2.3: Mode Select for Button Register
 */

#pragma once
#include <msp430.h>
#include <stdint.h>

namespace App {
    class ShiftRegister{
    public:
        static void init(){
            P2DIR |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6);
            P2DIR &= ~BIT7;
            // Modify XTAL pins to be I / O
            P2SEL &= ~( BIT6 | BIT7 ) ;

            P2OUT &= ~BIT5; // Reset /CLR
            __delay_cycles(100);
            P2OUT |= BIT5;
            P2OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3); // Hold mode
        }

        static void writeLeds(uint8_t data){
            //(S0=1 , S1=0)
            P2OUT |= BIT0; P2OUT &= ~BIT1; // SREG2 shif Right
            //MSB first
            for (int i = 3; i >= 0; i--){
                if (data & (0x01 << i)) P2OUT |= BIT6; else P2OUT &= ~ BIT6;
                pulse();
            }
            P2OUT &= ~(BIT0 | BIT1);
        }

        static uint8_t readButtons(){
            uint8_t val = 0;
            //(S01=1, S1= 1) to capture PB1-PB4
            P2OUT |= (BIT2 | BIT3); pulse(); // SREG1 Parallel load

            //(S0=1, S1=0) SREG1 Shift Right
            P2OUT &= ~BIT3;
            for (int i = 3; i >= 0; i--){
                if (P2IN & BIT7) val |= (0x01 << i);
                pulse();
            }
            P2OUT &= ~(BIT2 | BIT3); //Return to hold
            return val;
        }

    private:
        static void pulse(){
            P2OUT |= BIT4; // clock High
            P2OUT &= ~BIT4;  //clock Low
        }

    };
}
