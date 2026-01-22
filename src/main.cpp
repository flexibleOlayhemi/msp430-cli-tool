/*
 * main.cpp
 *
 *  Created on: Jan 5, 2026
 *      Author: Olayemi
 */


#include <msp430.h>
#include "include/HardwareConfig.hpp"
#include "include/CommandBuffer.hpp"
#include "include/CommandParser.hpp"
#include "include/CommandProcessor.hpp"

using namespace Config;

App::CommandBuffer cmdLine;

// Initialize the persistent LED memory
uint8_t App::CommandProcessor::sregLedState = 0x00;

// Initialize jukebox in memory
const uint16_t* App::Jukebox::currentNotes = nullptr;
const uint16_t* App::Jukebox::currentDuration =  nullptr;
uint8_t App::Jukebox::currentLength = 0;
uint8_t App::Jukebox::currentIndex = 0;
 uint16_t App::Jukebox::msCountdown = 0;
bool App::Jukebox::isPlaying = false;

int main() {
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog

    //Initialize Peripherals
    SystemPower::initLowPower();
    Console::init();
    Led5::init();Led6::init();
    Btn5::init();Btn6::init();
    BoardUI::init();
    Potentiometer::init();
    LightSensor::init();
    TempSensor::init();
    //initialize timer 1 for melody at 1ms : Period 1000 = 1ms at 1Mhz SMCLK
    Timer::init(TA1CTL,TA1CCR0,TA1CCTL0,1000);
    Heater::init();
    Buzzer::init();



    __bis_SR_register(GIE);



    //clear command buffer
    cmdLine.clear();


    Console::println("MSP430 CLI Tool v4.0 Ready");
    Console::print("> ");  //Command prompt for user

    while(1){
        //Check if serial character received
        //checking the UART status
        if (Console::available()){
            char rxChar = Console::read();

            //echo back character
            Console::sendChar(rxChar);

            //Read character and Append
            if(cmdLine.addChar(rxChar)){
                //Parse command string
                App::Command currentCmd = App::CommandParser::parse(const_cast<char*>(cmdLine.getContents()));

                //Dispatch and execute command
                //Print response on new line
                Console::print("\r\n");
                App::CommandProcessor::execute(currentCmd);

                //v2.0 Reset for next command
                cmdLine.clear();
                Console::print("> ");
            }
        }
    }
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_ISR(void){
    App::Jukebox::tick();
}

