/*
 * HardwareConfig.hpp
 *
 *  Created on: Dec 29, 2025
 *      Author: User
 */

#pragma once
#include "Light.hpp"
#include "Button.hpp"
#include "Uart.hpp"
#include "Timer.hpp"
#include "System.hpp"

namespace Config{

    using RegPtr = volatile uint8_t*;

    using SystemPower = Hardware::System;

    using StatusLed = Hardware::Led<RegPtr,&P1OUT,&P1DIR,BIT0>;
    using ErrorLed = Hardware::Led<RegPtr,&P1OUT,&P1DIR,BIT3>;

    //using StatusLed = Hardware::Led<Pins::RedLed>;

    using UserBtn = Hardware::Button<RegPtr,&P1IN,&P1DIR,&P1REN,&P1OUT,
            &P1IE,&P1IES,&P1IFG,BIT4>;
    using Console = Hardware::Uart;
    using SystemTimer = Hardware::TimerA<Hardware::TimerClock::SMCLK>;
}
