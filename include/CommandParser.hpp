/*
 * CommandParser.hpp
 *
 *  Created on: Jan 5, 2026
 *      Author: User
 */

#pragma once
#include <cstring>
#include <cstdlib>

namespace App {
    struct Command {
        char* action; // e.g "enable"
        char* target; // e.g "D1"
        unsigned int args[2]; // store numeric params (freq, duration)
        uint8_t argCount; // track the count of param
        bool verbose;
    };

    class CommandParser {
    public:
        static Command parse(char* str){
            Command cmd = {nullptr, nullptr,{0,0},0, false};

            if (str == nullptr || str[0] == '\0') return cmd;

            //Get action
            cmd.action = strtok(str," ");

            //Get Target
            if (cmd.action != nullptr){
                cmd.target = strtok(nullptr, " ");
            }

            //search for flags like "-v" and numeric param
            char* next = strtok(nullptr, " ");
            while (next != nullptr){
                if (std::strcmp(next, "-V") == 0){
                    cmd.verbose = true;
                }
                else if ( cmd.argCount < 2 && next[0] >= '0' && next[0] <= '9' ) {
                    cmd.args[cmd.argCount++] = std::atoi(next); //get the numeric value as int
                }

                next = strtok(nullptr, " ");
            }

            return cmd;
        }
    };
}
