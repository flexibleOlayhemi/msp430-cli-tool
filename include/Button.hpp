/*
 * Button.hpp
 *
 *  Created on: Jan 1, 2026
 *      Author: User
 */

#pragma once
#include <msp430.h>
#include <cstdint>

namespace Hardware {
  template<typename Reg,
  Reg InReg,  // for reading state (&P1IN)
  Reg DirReg, // for setting input (&PIDIR)
  Reg RenReg,  // for enabling resistor (&P1REN)
  Reg OutReg,  // for selecting pull-up/down (&P1OUT)
  Reg IeReg,Reg IesReg,Reg IfgReg , // for interrupt
  uint8_t BitMask
  >
  class Button {
  public:
      static void init(){
          *DirReg &= ~BitMask;
          *RenReg |= BitMask;
          *OutReg |= BitMask;  // pull up high

          *IesReg |= BitMask;
          *IfgReg &= ~BitMask;
          *IeReg |= BitMask;

      }

      static void clearFlag(){
          *IfgReg &= ~BitMask;
      }

      static bool isSource(){
          return *IfgReg & BitMask;
      }

      static bool isPressed(){
          //0 when pressed , pull-up
          //Return TRUE if the bit is 0
          return !(*InReg & BitMask);
      }


  };
}
