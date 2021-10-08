#include "system.h"
#include "tinygl.h"
#include "pacer.h"
#include "../fonts/font5x7_1.h"
#include "navswitch.h"
#include "button.h"
#include "pio.h"

#define MESSAGE_RATE 10
#define PACER_RATE 500
#define MENU_TEXT "WELCOME"


#define BUTTON_PIO PIO_DEFINE(PORT_D, 7)
int button_pressed_p (void)
{
    return pio_input_get(BUTTON_PIO);
}



void initialize(void)
{
    tinygl_init(PACER_RATE);
    pacer_init (PACER_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    // Init button 1
    pio_config_set(BUTTON_PIO, PIO_INPUT);
}

void displayMenu(void)
{
    tinygl_text(MENU_TEXT);
}

void tallCharacterObject(void)
/*three led high object for a character model*/
{

}

void lowCharacherObject(void)
/*two led high state that is entered when in the ducking state from pulling LED down*/
{

}

void highObject(void)
/*3 led object from the roof that can be ducked under*/
{

}

void lowObject(void)
/*one led object along the ground that can be jumped*/
{

}

void jump(void)
/*jump moves the character object up two leds then falls*/
{

}

void duck(void)
/*duck moves the character object to lowCharacherObject and then resets to tallCharacterObject*/
{

}


int main (void)
{
    // Initalize game and display menu
    system_init();
    initialize();
    displayMenu();

    // Update menu
    while (!navswitch_push_event_p(NAVSWITCH_PUSH) && !button_pressed_p())
    {
        pacer_wait();
        tinygl_update();
        navswitch_update();
        button_update();
    }

    // GAME LOGIC HERE
    tinygl_text("GAME");
    while (1) {
        pacer_wait();
        tinygl_update();
    }
}
