/*
 * jukebox.hpp
 *
 *  Created on: Jan 17, 2026
 *      Author: Olayemi
 */

#pragma once
#include <msp430.h>
#include "HardwareConfig.hpp"

namespace App{
    class Jukebox{

    public:

        static constexpr uint16_t PAUSE_NOTE = 0;
        static bool isPlaying;

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
            //static const uint16_t PAUSE_NOTE = 0xFFFF;

            //  Melody 1: Twinkle Twinkle
            static const uint16_t  m1_notes[] = {
                NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
                NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4, PAUSE_NOTE
            };

            static const uint16_t m1_durations[] = {
                300, 300, 300, 300, 300, 300, 600,
                300, 300, 300, 300, 300, 300, 600, 300
            };

            //static const uint8_t m1_lenght = 14;

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


            //static const uint8_t m2_length = 13;

            if (id == 1){
                setup(m1_notes,m1_durations,sizeof(m1_notes)/sizeof(m1_notes[0]));
            }
            else if (id == 2) {
                setup(m2_notes,m2_durations, sizeof(m2_notes)/sizeof(m2_notes[0]));
            }
            else
            {
                Config::Console::println("No Melody configured for your selection");
                return;
            }

        }

        static void setup(const uint16_t* notes, const uint16_t* durations, uint8_t length){
            currentNotes = notes;
            currentDuration = durations;
            currentLength = length;
            currentIndex = 0;
            isPlaying = true;
            startNote();  // start the first note
        }

        static void startNote(){
            if (currentIndex >= currentLength){
                stop();
                return;
            }
            //otherwise play next index
                uint16_t freq = currentNotes[currentIndex];

                if (freq == PAUSE_NOTE) {
                    Config::Buzzer::stop(); //Silence
                } else {
                Config::Buzzer::playPeriod(freq); // set PWM for note
                }
                msCountdown = currentDuration[currentIndex];
        }

        static void stop(){
            isPlaying = false;
            Config::Buzzer::stop();
            currentIndex = 0;
            msCountdown = 0;
        }

        // called by timer Interrupt Service
        static void tick(){
            if (!isPlaying) return;

            if(msCountdown > 0){
                msCountdown--;
            } else{
                currentIndex++;
                if (currentIndex < currentLength){
                startNote(); // move to next note
                } else {
                    stop();
                }
            }
        }

    private:

        //Blocking method
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

        //Interrupt method
        //Pointers to keep track of the current melody being played
        static const uint16_t* currentNotes;
        static const uint16_t* currentDuration;
        static uint8_t currentLength;
        static uint8_t currentIndex;
        static uint16_t msCountdown;



    };
}









