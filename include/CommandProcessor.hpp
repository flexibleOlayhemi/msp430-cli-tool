/*
 * CommandProcessor.hpp
 *
 *  Created on: Jan 6, 2026
 *      Author: User
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
            if (std::strcmp(cmd.action,"enable") == 0){
                handleEnable(cmd);
            }
            else if (std::strcmp(cmd.action,"disable") == 0){
                handleDisable(cmd);
                        }
            else if (std::strcmp(cmd.action, "read") == 0){
                handleRead(cmd);
            }
            else if (std::strcmp(cmd.action, "play") == 0){
                handlePlay(cmd);
            }
            else {
                //send Error Message
                Config::Console::println("ERR: Unknown Command");
            }
        }



    private:
        static void handleEnable(const Command& cmd){
            if(cmd.target != nullptr && std::strcmp(cmd.target, "D1") == 0){
                Config::StatusLed::on(); //Dispatch and execute

                if (cmd.verbose){
                    Config::Console::println("LED D1 enabled."); //send feedback if verbose
                }
                Config::Console::println("OK"); // Success feedback
            } else{
                Config::Console::println("ERR: Invalid Target");  //Execution Error
            }
        }

        static void handleDisable(const Command& cmd){
            if(cmd.target != nullptr && std::strcmp(cmd.target, "D1") == 0){
                Config::StatusLed::off(); //Dispatch and execute

                if (cmd.verbose){
                    Config::Console::println("LED D1 Disabled."); //send feedback if verbose
                }
                Config::Console::println("OK"); // Success feedback
            } else{
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
                bool pressed = Config::UserBtn::isPressed();

                    if (cmd.verbose) {

                        Config::Console::println(pressed ? "BTN: PRESSED" : "BTN: RELEASED");
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
