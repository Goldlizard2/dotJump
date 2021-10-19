/** @file   game.c
    @authors Okoko Anainga, Daniel Pallesen
    @date   17/10/2021
    @brief  The main game header file for the game
*/

#ifndef GAME_H
#define GAME_H

#include "system.h"

//initializes the clock and the text
void initialize(void);

//delay timer
void delay (uint16_t milliseconds);

// generates a random number for using to select a high or low object
uint8_t randomNumberGenerator(void);

// clears the display
void clearDisplay(void);

// creates the three high character object on row two
void characterObject(void);

//turns on the leds to represent the high object
void highObject(uint8_t row);

//turns on the leds to represent the low object
void lowObject(uint8_t row);

//makes the character jump 2 blocks high to avoid low object
void jump(void);

<<<<<<< HEAD
// makes the charchter duck one object lower so it can avoid a high object
void duck(void);
=======
// makes the character duck one object lower so it can avoid a high object
static void duck(void);
>>>>>>> 155e4595ee4f7c0c96257c24b2162fa6ef27cccb

// returns a boolean object of a collision
bool collision(uint8_t lowObjectLoc, uint8_t highObjectLoc, bool jump, bool duck);

// returns the boolean object of a dodge
bool dodge(uint8_t lowObjectLoc, uint8_t highObjectLoc, bool jump, bool duck);

// returns a boolean object on weather or no the navigation switch was altered
bool navSwitchMoved(void);

// runs the main game fuction
int main (void);

#endif
