/*
 * CommandParser.hpp
 *
 *  Created on: Jan 5, 2026
 *      Author: User
 */

#pragma once
#include <cstring>

namespace App {
    struct Command {
        char* action; // e.g "enable"
        char* target; // e.g "D1"
        bool verbose;
    };

    class CommandParser {
    public:
        static Command parse(char* str){
            Command cmd = {nullptr, nullptr, false};

            if (str == nullptr || str[0] == '\0') return cmd;

            //Get action
            cmd.action = strtok(str," ");

            //Get Target
            if (cmd.action != nullptr){
                cmd.target = strtok(nullptr, " ");
            }

            //search for flags like "-v"
            char* next = strtok(nullptr, " ");
            while (next != nullptr){
                if (std::strcmp(next, "-v") == 0){
                    cmd.verbose = true;
                }

                next = strtok(nullptr, " ");
            }

            return cmd;
        }
    };
}
