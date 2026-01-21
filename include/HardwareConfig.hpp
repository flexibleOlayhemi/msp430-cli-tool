/*
 * HardwareConfig.hpp
 *
 *  Created on: Dec 29, 2025
 *      Author: Olayemi
 */

#pragma once
#include "Light.hpp"
#include "Button.hpp"
#include "Uart.hpp"
#include "Timer.hpp"
#include "System.hpp"
#include "Adc.hpp"
#include "Beeper.hpp"
#include "ShiftRegister.hpp"

namespace Config{

    using RegPtr = volatile uint8_t*;

    using SystemPower = Hardware::System;

    using StatusLed = Hardware::Led<RegPtr,&P1OUT,&P1DIR,BIT0>;
    using Led5 = Hardware::Led<RegPtr,&P1OUT,&P1DIR,BIT0>;
    using Led6 = Hardware::Led<RegPtr,&P1OUT,&P1DIR,BIT7>;


    using UserBtn = Hardware::Button<RegPtr,&P3IN,&P3DIR,&P3REN,&P3OUT,
            nullptr,nullptr,nullptr,BIT2>;
    using Btn5 = Hardware::Button<RegPtr,&P3IN,&P3DIR,&P3REN,&P3OUT,
            nullptr,nullptr,nullptr,BIT2>;
    using Btn6 = Hardware::Button<RegPtr,&P3IN,&P3DIR,&P3REN,&P3OUT,
            nullptr,nullptr,nullptr,BIT3>;


    using Console = Hardware::Uart;
    using SystemTimer = Hardware::TimerA<Hardware::TimerClock::SMCLK>;
    using Potentiometer = Hardware::Adc<INCH_4, BIT4>;
    using LightSensor   = Hardware::Adc<INCH_5, BIT5>;
    using TempSensor = Hardware::Adc<INCH_3, BIT3>;
    using Heater =  Hardware::Led<RegPtr,&P1OUT,&P1DIR,BIT6>; //reusing Led GPIO template class
    using Buzzer = Hardware::Beeper;

    using BoardUI = App::ShiftRegister;


}
