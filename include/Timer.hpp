/*
 * Timer.hpp
 *
 *  Created on: Jan 5, 2026
 *      Author: Olayemi
 */

#pragma once
#include <msp430.h>
#include <cstdint>

namespace Hardware{
    enum class TimerClock {SMCLK , ACLK, TACLK};

    template<TimerClock Source>
    class TimerA {
    public:
        static void init(volatile uint16_t& CTL, volatile uint16_t& CCR0,volatile uint16_t& CCTL0, uint16_t period){
            //Configure: Up Mode , Divider /8, Interrupt Enabled
            CTL = getSourceBits() | MC_1 | ID_0 | TACLR;
            CCR0 = period;
            CCTL0 = CCIE;

        }
    private:
        //This logic stripped away by the compiler
        static constexpr uint16_t getSourceBits(){
            if (Source == TimerClock::SMCLK) return TASSEL_2;
            if (Source == TimerClock::ACLK) return TASSEL_1;
            return TASSEL_0;
        }
    };
}
