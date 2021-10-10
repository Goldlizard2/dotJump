#include "system.h"
#include "tinygl.h"
#include "pacer.h"
#include "../fonts/font5x7_1.h"
#include "navswitch.h"
#include "button.h"
#include "pio.h"
#include <avr/io.h>

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

static void display_column (uint8_t row_pattern[5], uint8_t current_column)
{


}

static void delay (uint16_t milliseconds) {
    TCNT1 = 0;
    uint16_t time = milliseconds/0.128;
    while (TCNT1 < time) {
        continue;
    }
    
}



static void tallCharacterObject(void)
/*three led high object for a character model*/ 
{
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

static void highObject(void)
/*3 led object from the roof that can be ducked under*/
{

}

static void lowObject(void)
/*one led object along the ground that can be jumped*/ 
{

}

static void jump(void)
/*jump moves the character object up two leds then falls*/
{
    pio_config_set(rows[2], PIO_OUTPUT_LOW);
    for (uint8_t i = 0; i < 5; i++) {
        pio_config_set(cols[i], PIO_OUTPUT_HIGH);
    }
    for (uint8_t j = 0; j<3 ; j++) {
         pio_config_set(cols[j], PIO_OUTPUT_LOW);
    }
    delay(500);
    for (uint8_t i = 0; i < 5; i++) {
        pio_config_set(cols[i], PIO_OUTPUT_HIGH);
    }
    tallCharacterObject();

}

static void duck(void)
/*duck moves the character object to lowCharacherObject and then resets to tallCharacterObject*/
{
    pio_config_set(rows[2], PIO_OUTPUT_LOW);
    for (uint8_t i = 0; i < 5; i++) {
        pio_config_set(cols[i], PIO_OUTPUT_HIGH);
    }
    for (uint8_t j = 3; j<5 ; j++) {
         pio_config_set(cols[j], PIO_OUTPUT_LOW);
    }
    delay(500);
    for (uint8_t i = 3; i < 5; i++) {
        pio_config_set(cols[i], PIO_OUTPUT_HIGH);
    }
    tallCharacterObject();
}


int main (void)
{
    //uint8_t current_column = 0;
  
    system_init ();
    navswitch_init ();
    pacer_init(1000);
    setLedMatrix();
    TCCR1A = 0x00;
    TCCR1B = 0x05;
    TCCR1C = 0x00;
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
    
    while (1)
    {
    
        tallCharacterObject();
        navswitch_update ();

        if (navswitch_push_event_p (NAVSWITCH_WEST))
        {
            jump();        
        }
        if (navswitch_push_event_p (NAVSWITCH_EAST))
        {
            duck();        
        }
        
        
        
        



    }
}
