#include <Arduino.h>

// #define DEBUG
#ifdef DEBUG 
    #include "avr8-stub.h" 
    #include "app_api.h" 
#endif

#define LED_AMT 6
#define PWM_T 2000
#define FADE_STEPS_AMT 30
#define FADE_TIME 1000e3

uint32_t start_time_us = 0;
uint32_t led_start_time_us[LED_AMT]={};

uint8_t pwm_value[LED_AMT] = 
{
    0* 0*255/3,
    0* 1*255/3,
    0* 2*255/3,
    0* 3*255/3,
};


uint8_t led_pin[LED_AMT] = 
{
    6,5,10,11,12,13
};


void setup()
{
    #ifdef DEBUG
    debug_init();
    #endif

    for( uint8_t i = 0; i < LED_AMT; i++ )
    {
        pinMode(led_pin[i],OUTPUT);
    }
    
    Serial.begin(9600);
}

void loop()
{   

    // turn all LEDs off
    for ( uint8_t i = 0; i < LED_AMT; i++ )
        digitalWrite(led_pin[i], HIGH);

    // alle LEDs
    for ( uint8_t i = 0; i < LED_AMT; i++ )
    {
        // alle Helligkeiten
        for ( uint8_t j = 0; j < FADE_STEPS_AMT; j++ )
        {
            // alle PWM Zyklen
            for ( uint16_t k = 0; k < (FADE_TIME/PWM_T/FADE_STEPS_AMT) ; k++ )
            {
                // turn LEDs off 
                digitalWrite(led_pin[i],HIGH);
                delayMicroseconds( PWM_T - (j * (PWM_T/FADE_STEPS_AMT)) );
                
                // turn LEDs on 
                digitalWrite(led_pin[i],LOW);
                delayMicroseconds( j * (PWM_T/FADE_STEPS_AMT) );
            }
        }
    }
}

