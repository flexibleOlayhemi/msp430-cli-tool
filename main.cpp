/*
 * main.cpp
 *
 *  Created on: Jan 5, 2026
 *      Author: User
 */


#include <msp430.h>
#include "HardwareConfig.hpp"
#include "CommandBuffer.hpp"

using namespace Config;

App::CommandBuffer cmdLine;

int main() {
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog

    //Initialize Peripherals
    SystemPower::initLowPower();
    Console::init();
    cmdLine.clear();


    Console::println("MSP430 CLI Tool v1.0 Ready");

    while(1){
        //Check if serial character received
        //checking the UART status
        if (Console::available()){
            char rxChar = Console::read();

            //Read character and Append
            if(cmdLine.addChar(rxChar)){
                //line end received
                Console::print("Received Command: ");
                Console::println(cmdLine.getContents());

                //v1.0 clear and restart the loop
                cmdLine.clear();
            }
        }
    }
}

