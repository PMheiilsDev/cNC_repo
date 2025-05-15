#include <Arduino.h>

// #define DEBUG
#ifdef DEBUG 
    #include "avr8-stub.h" 
    #include "app_api.h" 
#endif

#define LED_AMT 4
#define PWM_T 1000000/500
#define PWM_STEPS_AMT 30
#define FADE_TIME 1000e3

uint32_t start_time_us = 0;
uint32_t led_start_time_us[LED_AMT]={};

uint32_t pwm_start_time = 0;
uint8_t pwm_value[LED_AMT] = 
{
   0* 0*255/3,
   0* 1*255/3,
   0* 2*255/3,
   0* 3*255/3
};

bool pwm_state[LED_AMT] =
{
    0,0,0,0
};

uint8_t led_pin[4] = 
{
    11,10,5,6
};

uint8_t brightness[LED_AMT] = {};

void setup()
{
    #ifdef DEBUG
    debug_init();
    #endif

    for( uint8_t i = 0; i < LED_AMT; i++ )
    {
        pinMode(led_pin[i],OUTPUT);
    }

    // Serial.begin(9600);
}

void loop()
{
    uint32_t time_us = micros();
    
    // pwm handler 
    if ( time_us - pwm_start_time >= PWM_T )
    {
        pwm_start_time = time_us;

        for ( uint8_t i = 0; i < LED_AMT; i++ )
        {
            pwm_state[i] = 1;
        }
    }
    for ( uint8_t i = 0; i < LED_AMT; i++ )
    {
        if( time_us - pwm_start_time >= pwm_value[i] * (PWM_T/255) )
        {
            pwm_state[i] = 0;
        }
        
        digitalWrite(led_pin[i],!pwm_state[i]);
        
    }

    static bool up = 0;

    // change the values of the pwm handler
    if ( time_us - start_time_us >= FADE_TIME )
    {
        start_time_us = time_us;

        if ( pwm_value[0] == 255 || pwm_value[0] == 0 )
        {
            up = !up;
        }
        if ( up )
            pwm_value[0] ++;
        else
            pwm_value[0] --;
    }
}
