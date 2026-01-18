/*
 * main.cpp
 *
 *  Created on: Jan 5, 2026
 *      Author: User
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

int main() {
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog

    //Initialize Peripherals
    SystemPower::initLowPower();
    Console::init();
    //StatusLed::init();
    Led5::init();Led6::init();
    //UserBtn::init();
    Btn5::init();Btn6::init();
    BoardUI::init();
    SystemTimer::init(50000);
    Potentiometer::init();
    LightSensor::init();
    TempSensor::init();
    Buzzer::init();



    //clear command buffer
    cmdLine.clear();

    //wait 2secs for initialization
    //__delay_cycles(2000000);


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

