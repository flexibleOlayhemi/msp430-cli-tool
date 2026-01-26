/*****************************************************************************************************
 * main.cpp
 *
 * Command Line Tool for MSP430 Configuration
 *
 *  Created on: Jan 5, 2026
 *      Author: Olayemi
 *
 *PIN CONFIGURATION
 *  LED5 P1.0
 *  LED6 P1.7
 *  BUTTON5 P3.2
 *  BUTTON6 P3.3
 *  POT P1.4 INCH_4
 *  LDR P1.5 INCH5
 *  NTC P1.3
 *  Heater P1.6
 *  Buzzer P3.6
 *
 *  Shift register connection
 *  SREG1 Controls BUTTON 1 - 4
 *  SREG2 Controls LED 1 - 4
 *
 *  QD_DATA_PIN      P2.7
 *  SR_DATA_PIN      P2.6
 *  CLR_PIN          P2.5
 *  CK_PIN           P2.4
 *  S1_SREG1_PIN     P2.3
 *  S0_SREG1_PIN     P2.2
 *  S1_SREG2_PIN     P2.1
 *  S0_SREG2_PIN     P2.0
 *
 *COMMAND PROMTS
 *  The CLI supports high-level control with optional verbose feedback using the -v flag.
 *  All commands are processed case-insensitively using bitwise masking.
 *  Device Control (LEDs, Heater) ENABLE/DISABLE: Sets the logic state of digital pins.
 *       Syntax: ENABLE <device> [-v]
 *           Example: ENABLE D1 -v (Turns on LED 1 and confirms via Serial)
 *           Example: DISABLE D1 (Turns off LED 1 silently)
 *           Example: ENABLE Heater -v (Turns on Heater and confirms via Serial)
 *  Sensor Acquisition and Button status READ: Samples the ADC or a GPIO/Shift Register pin.
 *       Syntax: READ <sensor|button> [-v]
 *           Example: READ BUTTON 1 (Returns 0 or 1)
 *           Example: READ LDR (Returns 10-bit light intensity value)
 *           Example: READ POT -v (Returns analog value with descriptive text)
 *  Audio & Background Tasks PLAY: Orchestrates background music or individual frequencies.
 *       Syntax: PLAY MELODY <id> | PLAY TONE <freq> <duration>
 *           Example: PLAY MELODY 1 (Starts Twinkle Twinkle in background)
 *           Example: PLAY TONE 400 300 (Plays 400Hz for 300ms)
 *           STOP: Immediate hardware kill-switch. Logic: Sets isPlaying = false and calls TA0CTL &= ~MC_3.
 ***********************************************************************************************************/


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
volatile bool App::Jukebox::isPlaying = false;

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

    // Wait for any character (like Enter) before starting to ensure serial communication is ready
        while (!(IFG2 & UCA0RXIFG));
        uint8_t dummy = UCA0RXBUF;


    Console::println("MSP430 CLI Tool Ready");
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

//ISR for CPU multitasking while playing melody
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_ISR(void){
    App::Jukebox::tick();
}

