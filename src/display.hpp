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

uint8_t display_ctr;
uint8_t display_data[NUM_AMT];
void display_callback();


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

    display_ctr = 0;
    memset( display_data, 0x00, 4 );

    Timer1.initialize(1e6/(FRAME_RATE_Hz*NUM_AMT));
    Timer1.attachInterrupt(display_callback);
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

void display_callback()
{
    display_send(display_data[display_ctr],1<<display_ctr);
    display_ctr++;
    if (display_ctr > NUM_AMT)
        display_ctr = 0;
}

#endif

