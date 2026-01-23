/*
 * CommandProcessor.hpp
 *
 *  Created on: Jan 6, 2026
 *      Author: Olayemi
 */

#pragma once
#include "HardwareConfig.hpp"
#include "CommandParser.hpp"
#include "Jukebox.hpp"
#include <cstring>

namespace App{
    class CommandProcessor{
    public:
        static void execute(const App::Command& cmd){
            //check if command is valid
            if (cmd.action == nullptr) return;

            //Dispatch and execute command
            if (std::strcmp(cmd.action,"ENABLE") == 0){
                handleEnable(cmd);
            }
            else if (std::strcmp(cmd.action,"DISABLE") == 0){
                handleDisable(cmd);
                        }
            else if (std::strcmp(cmd.action, "READ") == 0){
                handleRead(cmd);
            }
            else if (std::strcmp(cmd.action, "PLAY") == 0){
                handlePlay(cmd);
            }
            else if (std::strcmp(cmd.action, "STOP") == 0) {
                if (App::Jukebox::isPlaying){
                App::Jukebox::stop();
                Config::Console::println("Melody halted.");
                } else
                Config::Console::println("Halt only when Melody is playing!");
            }
            else {
                //send Error Message
                Config::Console::println("ERR: Unknown Command");
            }
        }



    private:
        //persistent state for the 4 shift registers LEDs D1 - D4
        static uint8_t sregLedState;

        static void handleEnable(const Command& cmd){
           bool recognized = false;
           bool isHeater = false;
           if (cmd.target != nullptr && cmd.target[0]=='D' && cmd.target[1] >= '1' && cmd.target[1] <= '4'){
               uint8_t index = cmd.target[1] - '1';
               sregLedState |= (1 << index);
               Config::BoardUI::writeLeds(sregLedState);
               recognized = true;
           }
           else if(cmd.target != nullptr && std::strcmp(cmd.target, "D5") == 0){
                Config::Led5::on(); //Dispatch and execute
                recognized = true;
           }
           else if (cmd.target != nullptr && std::strcmp(cmd.target, "D6") == 0){
                Config::Led6::on(); //Dispatch and execute
                recognized = true;
           }
           else if (cmd.target != nullptr && std::strcmp(cmd.target, "HEATER") == 0){
                Config::Heater::on(); //Dispatch and execute
                recognized = true;
                isHeater = true;
           }

           if (recognized){
                if (cmd.verbose){
                    Config::Console::print(isHeater ? "" : "LED ");
                    Config::Console::print(cmd.target);
                    Config::Console::println("  enabled."); //send feedback if verbose
                }
                Config::Console::println("OK"); // Success feedback
            }
            else{
                Config::Console::println("ERR: Invalid Target");  //Execution Error
            }
        }

        static void handleDisable(const Command& cmd){
            bool recognized = false;
            bool isHeater = false;
            if (cmd.target != nullptr && cmd.target[0]=='D' && cmd.target[1] >= '1' && cmd.target[1] <= '4'){
                // If sregLedState was 0111 (D1,D2,D3 on)
                // disable D3 (id=3):
                // 1 << (3-1) is 0100
                // ~(0100) is 1011
                // 0111 & 1011 = 0011 (D3 is now off, others stay same)
                uint8_t index = cmd.target[1] - '1';
                sregLedState &= ~(1 << index);
                Config::BoardUI::writeLeds(sregLedState);
                recognized = true;
            }
            else if(cmd.target != nullptr && std::strcmp(cmd.target, "D5") == 0){
                 Config::Led5::off(); //Dispatch and execute
                 recognized = true;
             }
             else if (cmd.target != nullptr && std::strcmp(cmd.target, "D6") == 0){
                 Config::Led6::off(); //Dispatch and execute
                 recognized = true;
             }
             else if (cmd.target != nullptr && std::strcmp(cmd.target, "HEATER") == 0){
                 Config::Heater::off(); //Dispatch and execute
                 recognized = true;
                 isHeater = true;
             }
             if (recognized){
                 if (cmd.verbose){
                     Config::Console::print(isHeater ? "" : "LED ");  // print message as HEATER or LED
                     Config::Console::print(cmd.target);
                     Config::Console::println("  disabled."); //send feedback if verbose
                 }
                 Config::Console::println("OK"); // Success feedback
             }
             else{
                 Config::Console::println("ERR: Invalid Target");  //Execution Error
             }
        }

        static void handlePlay(const Command& cmd){
            if (cmd.target == nullptr){
                Config::Console::println("ERR: play should be followed by TONE or MELODY");
                return;
            }

            if (std::strcmp(cmd.target, "TONE") == 0) {
                if (cmd.args[0] ==  0){
                    Config::Console::println("ERR: Please specify frequency! e.g 440 ");
                    return;
                }
                uint16_t frequency = cmd.args[0];
                uint16_t duration = (cmd.argCount > 1) ? cmd.args[1] : 100; // if duration not specified , default to 100ms

                Config::Buzzer::play(frequency);

                for (uint16_t i = 0; i < duration; i++) {
                    __delay_cycles(1000);
                }
                if (cmd.verbose) {

                    Config::Console::print("TONE played at frequency: ");
                    Config::Console::printNumber(frequency);
                    Config::Console::println("");
                }
                Config::Buzzer::stop();
                Config::Console::println("OK");
            }
            else if (std::strcmp(cmd.target, "MELODY") == 0 ){
                if (cmd.args[0] ==  0){
                     Config::Console::println("ERR: Please specify the Melody e.g 1 or 2");
                     return;
                 }
                App::Jukebox::play((uint8_t)cmd.args[0]);
                if (cmd.verbose) {

                    Config::Console::print("Melody ");
                    Config::Console::printNumber((uint8_t)cmd.args[0]);
                    Config::Console::println("is Done playing.");
                }
                Config::Console::println("OK");

            }
            else{
                Config::Console::println("ERR: Invalid play Command ");
            }
        }

        static void handleRead(const Command& cmd) {
                if (cmd.target == nullptr){
                    Config::Console::println("ERR: NO Sensor specified");
                    return;
                }

                if (std::strcmp(cmd.target, "BUTTON") == 0) {
                    int btnId = cmd.args[0];
                    bool pressed = false;

                    if (btnId >= 1 && btnId <= 4){
                        uint8_t btns = Config::BoardUI::readButtons();
                        pressed = (btns & (1 << (btnId - 1)));
                    }
                    else if (btnId == 5) {
                        pressed =  Config::Btn5::isPressed();
                    }
                    else if (btnId == 6) {
                        pressed =  Config::Btn6::isPressed();
                    }else {
                        Config::Console::println("ERR: Available  BTN ID 1 - 6 ");
                        return;
                    }

                    if (cmd.verbose) {
                        Config::Console::print("Button ");
                        Config::Console::printNumber(btnId);
                        Config::Console::println(pressed ? " is PRESSED" : " is RELEASED");
                    }
                    else{
                        Config::Console::println(pressed ? "1" : "0");
                    }
                    Config::Console::println("OK");
                }
                else if (std::strcmp(cmd.target, "POT") == 0){
                    uint16_t val = Config::Potentiometer::read();
                    if (cmd.verbose){
                        Config::Console::print("Current POT value is: ");
                    }

                    Config::Console::printNumber(val);
                    Config::Console::println("");
                    Config::Console::println("OK");
                }
                else if (std::strcmp(cmd.target, "LDR") == 0){
                    uint16_t val = Config::LightSensor::read();
                    if (cmd.verbose){
                        Config::Console::print("Current LDR value is: ");
                    }

                    Config::Console::printNumber(val);
                    Config::Console::println("");
                    Config::Console::println("OK");
                }
                else if (std::strcmp(cmd.target, "NTC") == 0){
                    uint16_t val = Config::TempSensor::read();
                    if(cmd.verbose){
                        Config::Console::print("Current NTC value is: ");
                    }
                    Config::Console::printNumber(val);
                    Config::Console::println("");
                    Config::Console::println("OK");
                }
                else{
                    Config::Console::println("ERR: Invalid Sensor");
                }
        }

    };
}
