/** @file   gameEnvironment.c
    @authors Okoko Anainga, Daniel Pallesen
    @date   19/10/2021
    @brief  Contains the main game loop for LIGHT RUNNER
*/


#include "system.h"
#include "tinygl.h"
#include "pacer.h"
#include "navswitch.h"
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "game.h"


#define MENU_TEXT "WELCOME TO LIGHT JUMP"
#define END_TEXT "POINTS"
#define PLAYER_UPDATE_COUNT 60
#define OBJECT_UPDATE_COUNT 30

int main (void)
{
    // Full game loop
    uint8_t randomItem = randomNumberGenerator();
    while(1) {

        system_init ();
        navswitch_init ();
        initialize();
        clearDisplay();
        TCCR1A = 0x00;
        TCCR1B = 0x05;
        TCCR1C = 0x00;
        uint8_t highObjectLoc = 7;
        uint8_t lowObjectLoc = 9;
        uint8_t objectCounter = 0;
        uint8_t moveCounter = 0;
        uint8_t speedIncrease = 0;
        uint16_t score = 0;
        bool jumping = false;
        bool ducking = false;
        bool gameOver = false;

        tinygl_text(MENU_TEXT);
        // Start Menu
        while (!navSwitchMoved())
        {
            pacer_wait();
            tinygl_update();
            navswitch_update();
        }


        // Main game loop (note - delay should be kept to ~16ms)
        while (!gameOver)
        {
            // Display player in current state
            if (jumping) {
                jump();
            } else if (ducking) {
                duck();
            } else {
                characterObject();
            }

            // Update counters
            objectCounter++;
            moveCounter++;

            // Display objects and player
            delay(5);
            lowObject(lowObjectLoc);
            delay(5);
            highObject(highObjectLoc);
            delay(5);

            navswitch_update ();


            // Check if player is trying to jump
            if (navswitch_push_event_p (NAVSWITCH_WEST) && !ducking && !jumping)
            {
                jumping = true;
                moveCounter = 0;
            }

            // Check is player is trying to duck
            if (navswitch_push_event_p (NAVSWITCH_EAST) && !jumping && !ducking)
            {
                ducking = true;
                moveCounter = 0;
            }


            // Reset player state
            if (moveCounter == PLAYER_UPDATE_COUNT - floor(speedIncrease/2)) {
                moveCounter = 0;
                jumping = false;
                ducking = false;
            }

            // Update obstacles
            if (objectCounter == (OBJECT_UPDATE_COUNT - speedIncrease)) {
                // Check if a collision occured
                gameOver = collision(lowObjectLoc, highObjectLoc, jumping, ducking);
                if (dodge(lowObjectLoc, highObjectLoc, jumping, ducking)) {
                    score++;
                }
                if (speedIncrease < 22) {
                        speedIncrease = floor(score / 2);
                }
                objectCounter = 0;

                // Creates low object
                if (randomItem == 1) {
                    lowObjectLoc--;
                    if (lowObjectLoc>20) {
                        lowObjectLoc = 7;
                        randomItem = randomNumberGenerator();

                    }

                }
                // Creates high object
                if (randomItem == 0) {
                    highObjectLoc--;
                    if (highObjectLoc>20) {
                        highObjectLoc = 7;
                        randomItem = randomNumberGenerator();
                    }
                }
            }


        }

        // Game over screen
        // Convert score to a string
        char sscore[8];
        sprintf(sscore, " %d ", score);
        // Set end game text
        tinygl_text(strcat(sscore, END_TEXT));
        // Display end game screen
        while (!navSwitchMoved()) {
            pacer_wait();
            tinygl_update();
            navswitch_update();
        }
        // Delay to prevent accidental extra navswitch press
        delay(50);
        navswitch_update();
    }
}
