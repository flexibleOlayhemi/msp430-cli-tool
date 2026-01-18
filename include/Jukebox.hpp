/*
 * jukebox.hpp
 *
 *  Created on: Jan 17, 2026
 *      Author: User
 */

#pragma once
#include <msp430.h>
#include "HardwareConfig.hpp"

namespace App{
    class Jukebox{

    public:

        static void play(uint8_t id){
            // Note Frequencies TA0CCR0 Period values for 1MHz SMCLK
            static const uint16_t NOTE_C4 = 3822;
            static const uint16_t NOTE_D4 = 3405;
            static const uint16_t NOTE_E4 = 3034;
            static const uint16_t NOTE_F4 = 2863;
            static const uint16_t NOTE_G4 = 2551;
            static const uint16_t NOTE_A4 = 2272;
            static const uint16_t NOTE_B4 = 2024;
            static const uint16_t NOTE_C5 = 1911;
            static const uint16_t PAUSE_NOTE = 0xFFFF;

            //  Melody 1: Twinkle Twinkle
            static const uint16_t  m1_notes[] = {
                NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
                NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4, PAUSE_NOTE
            };

            static const uint16_t m1_durations[] = {
                300, 300, 300, 300, 300, 300, 600,
                300, 300, 300, 300, 300, 300, 600, 300
            };

            static const uint8_t m1_lenght = 14;

            // Melody 2: Mary Had a Little Lamb
            static const uint16_t m2_notes[] = {
                NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4,
                NOTE_E4, NOTE_E4, NOTE_E4,
                NOTE_D4, NOTE_D4, NOTE_D4,
                NOTE_E4, NOTE_G4, NOTE_G4, PAUSE_NOTE
            };
            static const uint16_t m2_durations[] = {
                300, 300, 300, 300,
                300, 300, 600,
                300, 300, 600,
                300, 300, 600, 300
            };


            static const uint8_t m2_length = 13;

            if (id == 1){
                run(m1_notes,m1_durations,m1_lenght);
            }
            else if (id == 2) {
                run(m2_notes,m2_durations, m2_length);
            }
            else
            {
                Config::Console::println("No Melody configured for your selection");
                return;
            }
        }

    private:

        static void run(const uint16_t* notes, const uint16_t* durations,uint8_t length){
            for (uint8_t i = 0; i < length; i++){
                Config::Buzzer::playPeriod(notes[i]);

                for (uint16_t d= 0; d < durations[i]; d++){
                    __delay_cycles(1000); //1Mz delay 1000 is 1ms
                }

                Config::Buzzer::stop();
                __delay_cycles(50000); //50ms silence gap
            }
        }


    };
}






