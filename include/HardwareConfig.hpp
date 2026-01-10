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
#include "Adc.hpp"

namespace Config{

    using RegPtr = volatile uint8_t*;

    using SystemPower = Hardware::System;

    using StatusLed = Hardware::Led<RegPtr,&P1OUT,&P1DIR,BIT0>;
    using ErrorLed = Hardware::Led<RegPtr,&P1OUT,&P1DIR,BIT3>;

    //using StatusLed = Hardware::Led<Pins::RedLed>;

    using UserBtn = Hardware::Button<RegPtr,&P2IN,&P2DIR,&P2REN,&P2OUT,
            &P2IE,&P2IES,&P2IFG,BIT0>;
    using Console = Hardware::Uart;
    using SystemTimer = Hardware::TimerA<Hardware::TimerClock::SMCLK>;
    using Potentiometer = Hardware::Adc<INCH_4, BIT4>;
    using LightSensor   = Hardware::Adc<INCH_5, BIT5>;
}
