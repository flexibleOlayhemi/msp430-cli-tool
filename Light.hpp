/*
 * Light.hpp
 *
 *  Created on: Dec 27, 2025
 *      Author: User
 */
#pragma once

#include <msp430.h>
#include <cstdint>


namespace Hardware{

    template<typename PortType, PortType OutReg, PortType DirReg,uint8_t BitMask>
    class Led{
    public:
        //static constexpr uint8_t LED = BIT0;

        static  void init(){
            *DirReg  |= BitMask;
            *OutReg &= ~BitMask;
        }

        static void toggle(){
            *OutReg ^= BitMask;
        }

        static void on(){
            *OutReg |= BitMask;
        }

        static void off(){
            *OutReg &= ~BitMask;
        }

        static void set(bool state){
            if(state) *OutReg |= BitMask;
            else *OutReg &= ~BitMask;
        }
    };

}





