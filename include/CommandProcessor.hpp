/*
 * CommandProcessor.hpp
 *
 *  Created on: Jan 6, 2026
 *      Author: User
 */

#pragma once
#include "HardwareConfig.hpp"
#include "CommandParser.hpp"
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
            else if (std::strcmp(cmd.action, "read") == 0){
                handleRead(cmd);
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

        static void handleRead(const Command& cmd) {
            if (cmd.target != nullptr && std::strcmp(cmd.target, "BUTTON") == 0) {
                bool pressed = Config::UserBtn::isPressed();

                if (cmd.verbose) {

                    Config::Console::println(pressed ? "BTN: PRESSED" : "BTN: RELEASED");
                }
                else{
                    Config::Console::println(pressed ? "1" : "0");
                }
                Config::Console::println("OK");
            }
            else{
                Config::Console::println("ERR: Invalid Sensor");
            }
        }

    };
}
