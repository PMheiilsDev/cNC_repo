#ifndef _DISPLAY_H
#define _DISPLAY_H

// At no point should 0-4 be LOW and 8-15 be HIGH
// as then the LEDs would be in reverse what they might not be made for 

#include <inttypes.h>

#include "Arduino.h"

#include "TimerOne.h"

#define DISPLAY_SDI 8
#define DISPLAY_RCLK 4
#define DISPLAY_SRCLK 7

#define NUM_AMT 4

// note that a frame rate of 25 is not enough as one number will only be on for 1/NUM_AMT of the time so here 1/4 
#define FRAME_RATE_Hz 100  

extern uint8_t display_ctr;
extern uint8_t display_data[NUM_AMT];

extern uint8_t seg_7_numbers[10];

/// @brief initialises the display and turns all the pins to 5V so that all LEDs are off 
void init_display();

/// @brief writes a uint to the display removing leading 0s 
/// @param number 0-9999 as only 4 digits  
void write_number( uint16_t number );

#endif

