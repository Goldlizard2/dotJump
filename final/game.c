#include "system.h"
#include "tinygl.h"
#include "pacer.h"
#include "../fonts/font5x5_1.h"
#include "navswitch.h"
#include "button.h"
#include "pio.h"
#include <avr/io.h>

#define MESSAGE_RATE 60
#define PACER_RATE 1000
#define MENU_TEXT "WELCOME TO LIGHT JUMP"
#define END_TEXT "GAME OVER"

void initialize(void)
{
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


/** Define PIO pins driving LED matrix columns.  */
static const pio_t cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};

/*static void display_column (uint8_t row_pattern[5], uint8_t current_column)
{


}*/

static void delay (uint16_t milliseconds) {
    TCNT1 = 0;
    uint16_t time = milliseconds/0.128;
    while (TCNT1 < time) {
        continue;
    }

}

static void clearDisplay(void)
{
    for (uint8_t i = 0; i < 5; i++) {
        pio_config_set(cols[i], PIO_OUTPUT_HIGH);
    }
    for (uint8_t j = 0; j<7 ; j++) {
         pio_config_set(rows[j], PIO_OUTPUT_HIGH);
    }
}

static void characterObject(void)
/*three led high object for a character model*/
{
    clearDisplay();
    pio_config_set(rows[2], PIO_OUTPUT_LOW);
    for (uint8_t i = 2; i < 5; i++) {
        pio_config_set(cols[i], PIO_OUTPUT_LOW);
    }
}

static void setLedMatrix(void)
{
    for (uint8_t i = 0; i < LEDMAT_ROWS_NUM; i++) {
        pio_config_set (rows[i], PIO_OUTPUT_HIGH);

        if (i < 7) {
            pio_config_set (cols[i], PIO_OUTPUT_HIGH);
        }
    }
}

static void highObject(uint8_t row)
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

static void lowObject(uint8_t row)
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

static void jump(void)
/*jump moves the character object up two leds then falls*/
{
    clearDisplay();
    pio_config_set(rows[2], PIO_OUTPUT_LOW);
    // TODO check if this loop can be removed
    for (uint8_t i = 0; i < 5; i++) {
        pio_config_set(cols[i], PIO_OUTPUT_HIGH);
    }
    for (uint8_t j = 0; j<3 ; j++) {
         pio_config_set(cols[j], PIO_OUTPUT_LOW);
    }
}

static void duck(void)
/*duck moves the character object to lowCharacherObject and then resets to tallCharacterObject*/
{
    clearDisplay();
    pio_config_set(rows[2], PIO_OUTPUT_LOW);
    // TODO check if this loop can be removed
    for (uint8_t i = 0; i < 5; i++) {
        pio_config_set(cols[i], PIO_OUTPUT_HIGH);
    }
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

bool navSwitchMoved(void)
{
    return (navswitch_push_event_p(NAVSWITCH_PUSH) || navswitch_push_event_p(NAVSWITCH_NORTH) || navswitch_push_event_p(NAVSWITCH_SOUTH) || navswitch_push_event_p(NAVSWITCH_EAST) || navswitch_push_event_p(NAVSWITCH_WEST));
}

int main (void)
{
    // Full game loop
    while(1) {

        system_init ();
        navswitch_init ();
        initialize();
        setLedMatrix();
        TCCR1A = 0x00;
        TCCR1B = 0x05;
        TCCR1C = 0x00;
        uint8_t highObjectLoc = 9;
        uint8_t lowObjectLoc = 6;
        uint16_t objectCounter = 0;
        uint8_t moveCounter = 0;
        bool jumping = false;
        bool ducking = false;

        tinygl_text(MENU_TEXT);
        // Start Menu
        while (!navSwitchMoved())
        {
            pacer_wait();
            tinygl_update();
            navswitch_update();
        }


        // Main game loop (note - delay should be kept to ~16ms)
        while (1)
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
            if (navswitch_push_event_p (NAVSWITCH_WEST) && !ducking)
            {
                jumping = true;
                moveCounter = 0;
            }

            // Check is player is trying to duck
            if (navswitch_push_event_p (NAVSWITCH_EAST) && !jumping)
            {
                ducking = true;
                moveCounter = 0;
            }

            // Check if a collision occured (TODO check if this could be done when object is updated)
            if(collision(lowObjectLoc, highObjectLoc, jumping, ducking)) {
                break;
            }

            // Update obstacles
            if (objectCounter == 30) {
                objectCounter = 0;
                lowObjectLoc--;
                highObjectLoc--;
                if (lowObjectLoc>10){
                    lowObjectLoc = 8;
                }
                if (highObjectLoc>10){
                    highObjectLoc = 8;
                }
            }

            // Reset player state
            if (moveCounter == 70) {
                moveCounter = 0;
                jumping = false;
                ducking = false;
            }

        }

        // Game over screen
        tinygl_text(END_TEXT);
        while (!navSwitchMoved()) {
            pacer_wait();
            tinygl_update();
            navswitch_update();
        }
        // Delay to prevent accidental extra press
        delay(100);
        navswitch_update();
    }

}
