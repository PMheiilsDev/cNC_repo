#ifndef _DISPLAY_H
#define _DISPLAY_H

// At no point should 0-4 be LOW and 8-15 be HIGH
// as then the LEDs would be in reverse what they might not be made for 

#include "Arduino.h"

#define DISPLAY_SDI 8
#define DISPLAY_RCLK 4
#define DISPLAY_SRCLK 7


/// @brief initialises the display and turns all the pins to 5V so that all LEDs are off 
void init_display()
{

    pinMode(DISPLAY_SDI, OUTPUT);
    pinMode(DISPLAY_RCLK, OUTPUT);
    pinMode(DISPLAY_SRCLK, OUTPUT);

    digitalWrite(DISPLAY_SDI, LOW);
    digitalWrite(DISPLAY_RCLK, LOW);
    digitalWrite(DISPLAY_SRCLK, LOW);

    digitalWrite(DISPLAY_RCLK,LOW);

    for( uint8_t i = 0; i < 8; i++ )
    {
        digitalWrite(DISPLAY_SRCLK,LOW);
        digitalWrite(DISPLAY_SDI,1);
        digitalWrite(DISPLAY_SRCLK,HIGH);
    }

    for( uint8_t i = 8; i < 16; i++ )
    {
        digitalWrite(DISPLAY_SRCLK,LOW);
        digitalWrite(DISPLAY_SDI,1);
        digitalWrite(DISPLAY_SRCLK,HIGH);
    }

    digitalWrite(DISPLAY_RCLK,HIGH);

}

void display_send(uint8_t data, uint8_t num)
{
    // initiate data sending 
    digitalWrite(DISPLAY_RCLK,LOW);

    // send the data 
    for( uint8_t i = 0; i < 8; i++ )
    {
        digitalWrite(DISPLAY_SRCLK,LOW);
        digitalWrite(DISPLAY_SDI, ((data>>i)&1)?LOW:HIGH );
        digitalWrite(DISPLAY_SRCLK,HIGH);
    }

    // set the 4 unused pins 
    for( uint8_t i = 0; i < 4; i++ )
    {
        digitalWrite(DISPLAY_SRCLK,LOW);
        digitalWrite(DISPLAY_SDI,LOW);
        digitalWrite(DISPLAY_SRCLK,HIGH);
    }

    // sent the common pins to high so that all leds can be on 
    for( uint8_t i = 0; i < 4; i++ )
    {
        digitalWrite(DISPLAY_SRCLK,LOW);
        digitalWrite(DISPLAY_SDI,((~num>>i)&1)?LOW:HIGH);
        digitalWrite(DISPLAY_SRCLK,HIGH);
    }

    digitalWrite(DISPLAY_RCLK,HIGH);
    
}

#endif

