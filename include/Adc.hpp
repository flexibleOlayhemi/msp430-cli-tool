/*
 * Adc.hpp
 *
 *  Created on: Jan 10, 2026
 *      Author: User
 */

#pragma once
#include <msp430.h>
#include <cstdint>
//#include "Uart.hpp"


namespace Hardware {
  template <uint16_t Channel , uint16_t PinBit>
  class Adc {
  public:
      static void init(){
          //ADC10ON: Power on ADC
          //ADC10SHT_2: Sample and hold time (16 clock cycles)
          ADC10CTL0 |= ADC10SHT_2 | ADC10ON;

          //Select  the channel [INCH_4 or A4]
          ADC10CTL1 = Channel;

          //Ebable the anolog input for this specific pin
          ADC10AE0 |= PinBit;
      }

      static uint16_t read() {
          ADC10CTL0 &= ~ENC;             // Disable conversion so we can change channel next time
          ADC10CTL1 = Channel;           // reset channel before conversion
          ADC10CTL0 |= ENC | ADC10SC;    // Enable start conversion
          // Temporary debug check
          //Hardware::Uart::print("Reading Channel: ");
          //Hardware::Uart::printNumber(Channel >> 12);
          //Hardware::Uart::println("");
          while (ADC10CTL1 & ADC10BUSY); // wait for hardware to finish
          return ADC10MEM;               // Read result (0 - 1023)

      }
  };
}
