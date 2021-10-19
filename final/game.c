/** @file   game.c
    @authors Okoko Anainga, Daniel Pallesen
    @date   17/10/2021
    @brief  The main game file for LIGHT RUNNER
*/

#include "system.h"
#include "tinygl.h"
#include "pacer.h"
#include "../fonts/font5x5_1.h"
#include "navswitch.h"
#include "button.h"
#include "pio.h"
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "game.h"


#define PACER_RATE 1000
#define MESSAGE_RATE 50

void initialize(void)
{
    srand(time(NULL));
    tinygl_init(PACER_RATE);
    pacer_init (PACER_RATE);
    tinygl_font_set (&font5x5_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
}

/** Define PIO pins driving LED matrix rows.  */
static const pio_t rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/* Define PIO pins driving LED matrix columns.  */
static const pio_t cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};

void delay (uint16_t milliseconds)
{
    TCNT1 = 0;
    uint16_t time = milliseconds/0.128;
    while (TCNT1 < time) {
        continue;
    }

}

uint8_t randomNumberGenerator(void)
{
    uint8_t randomItem = 0;
    randomItem = rand() % 2;
    return randomItem;
}

void clearDisplay(void)
{
    for (uint8_t i = 0; i < 5; i++) {
        pio_config_set(cols[i], PIO_OUTPUT_HIGH);
    }
    for (uint8_t j = 0; j<7 ; j++) {
         pio_config_set(rows[j], PIO_OUTPUT_HIGH);
    }
}

void characterObject(void)
/*three led high object for a character model*/
{
    clearDisplay();
    pio_config_set(rows[2], PIO_OUTPUT_LOW);
    for (uint8_t i = 2; i < 5; i++) {
        pio_config_set(cols[i], PIO_OUTPUT_LOW);
    }
}

void highObject(uint8_t row)
/*3 led object from the roof that can be ducked under*/
{
    if (row<=6) {
        clearDisplay();
        pio_config_set(rows[row], PIO_OUTPUT_LOW);
        for (uint8_t j = 0; j<3 ; j++) {
             pio_config_set(cols[j], PIO_OUTPUT_LOW);
        }
    }
}

void lowObject(uint8_t row)
/*one led object along the ground that can be jumped*/
{
    if (row<=6) {
        clearDisplay();
        pio_config_set(rows[row], PIO_OUTPUT_LOW);
        for (uint8_t i = 3; i < 5; i++) {
            pio_config_set(cols[i], PIO_OUTPUT_LOW);
        }
    }
}

void jump(void)
/*jump moves the character object up two leds then falls*/
{
    clearDisplay();
    pio_config_set(rows[2], PIO_OUTPUT_LOW);
    for (uint8_t j = 0; j<3 ; j++) {
         pio_config_set(cols[j], PIO_OUTPUT_LOW);
    }
}

void duck(void)
/*duck moves the character object to lowCharacherObject and then resets to tallCharacterObject*/
{
    clearDisplay();
    pio_config_set(rows[2], PIO_OUTPUT_LOW);
    for (uint8_t j = 3; j<5 ; j++) {
         pio_config_set(cols[j], PIO_OUTPUT_LOW);
    }
}

bool collision(uint8_t lowObjectLoc, uint8_t highObjectLoc, bool jump, bool duck)
/* Checks if high and low objects are at player position and if so that
the player is performing the correct action to dodge */
{
    return ((lowObjectLoc == 2 && !jump) || (highObjectLoc == 2 && !duck));
}

bool dodge(uint8_t lowObjectLoc, uint8_t highObjectLoc, bool jump, bool duck)
/* Checks if player is currently dodging an obstacle */
{
    return ((lowObjectLoc == 2 && jump) || (highObjectLoc == 2 && duck));
}

bool navSwitchMoved(void)
{
    return (navswitch_push_event_p(NAVSWITCH_PUSH) || navswitch_push_event_p(NAVSWITCH_NORTH) || navswitch_push_event_p(NAVSWITCH_SOUTH) || navswitch_push_event_p(NAVSWITCH_EAST) || navswitch_push_event_p(NAVSWITCH_WEST));
}

