/*
 * Uart.hpp
 *
 *  Created on: Jan 4, 2026
 *      Author: User
 */

#pragma once
#include <msp430.h>
#include <cstdint>

namespace Hardware{
    class Uart{
    public:
        static void init() {
            // 1. Setup the Clock (DCO) to 1MHz
            // If calibration constants were erased, we stop here (safety check)
            //if (CALBC1_1MHZ==0xFF) while(1);

            DCOCTL = 0;             // Select lowest DCO settings
            BCSCTL1 = CALBC1_1MHZ;  // Set range
            DCOCTL = CALDCO_1MHZ;   // Set DCO step + modulation

            // 2. Setup Pins (P1.1=RXD, P1.2=TXD)
            P1SEL  = BIT1 | BIT2;
            P1SEL2 = BIT1 | BIT2;

            // 3. Configure UART
            UCA0CTL1 |= UCSWRST;    // Hold in reset
            UCA0CTL1 |= UCSSEL_2;   // Use SMCLK (now locked at 1MHz)

            // Baud Rate math: 1,000,000 / 9600 = 104.16...
            UCA0BR0 = 104;
            UCA0BR1 = 0;
            UCA0MCTL = UCBRS_1;     // Modulation UCBRS_1 is better for 104.16

            UCA0CTL1 &= ~UCSWRST;   // Release reset
        }

        static void sendChar(char c){
            #pragma diag_suppress 1528 // Suppress ULP 3.1 advisor for this loop
            while (!(IFG2 & UCA0TXIFG)); // Wait for TX buffer
            UCA0TXBUF = c;
            #pragma diag_default 1528 // Re-enable it for rest of the project
        }

        static void print(const char* s){
            while (*s) sendChar(*s++);
        }

        static void println(const char* s){
            print(s);
            print("\r\n");
        }

        //check if the RX interrupt flag is set
        static bool available(){
            return (IFG2 & UCA0RXIFG);
        }

        static char read(){
            return UCA0RXBUF;
        }
    };
}
