/*
 * CommandBuffer.hpp
 *
 *  Created on: Jan 5, 2026
 *      Author: User
 */

#pragma once
#include <cstdint>

namespace App {
    class CommandBuffer{
    private:
        static constexpr uint8_t MAX_SIZE = 32; //Limit to 32 chars to save RAM
        char buffer[MAX_SIZE];
        uint8_t index = 0;
    public:
        //clear command buffer
        void clear(){
            // Count down to 0 for better power efficiency (ULP 13.1)
            uint8_t i = MAX_SIZE;
            while(i > 0)
            //for (uint8_t i=0; i < MAX_SIZE; i++)buffer[i] = '\0';
            {
                buffer[--i] = '\0';
            }
            index=0;
        }

        //read character and append
        //return true if line end received
        bool addChar(char c){
            //convert input to UPPERCASE
            if ( c >= 'a' && c <='z') {
                  c &= ~0x20; // clear bit 5 to convert to uppercase
            }

            //handle backspace
            if(c == '\b' || c == 127){
                if (index > 0) index--;
                return false;
            }

            if (c == '\r' || c == '\n'){
                buffer[index] = '\0'; // null terminate the string
                return true;
            }

            //Append if there is space
            if(index < (MAX_SIZE - 1)){
                  buffer[index++] = c;
            }
            return false;
        }

        const char* getContents() const {return buffer;}
    };
}
