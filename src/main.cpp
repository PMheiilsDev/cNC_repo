#include <Arduino.h>

// #define DEBUG
#ifdef DEBUG 
    #include "avr8-stub.h" 
    #include "app_api.h" 
#endif

#define LED_AMT 4
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


uint8_t led_pin[4] = 
{
    11,10,5,6
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
    unsigned long time_us = micros();
    
    // Serial.println(time_us%PWM_T);

    // for all pwm leds
    for ( uint8_t i = 0; i < LED_AMT; i++ )
    {
        if( time_us % PWM_T >= (unsigned long) pwm_value[i] * PWM_T /255 )
        {
            digitalWrite(led_pin[i],HIGH);
        }
        else
        {
            digitalWrite(led_pin[i], LOW);
        }
    }

    delayMicroseconds(rand()%20);
    

    static bool up = 0;

    // change the values of the pwm handler
    if ( (time_us - start_time_us) >= (unsigned long)(FADE_TIME/FADE_STEPS_AMT) )
    {
        // Serial.println(pwm_value[3]);

        start_time_us = time_us;

        if ( pwm_value[3] == 255 || pwm_value[3] == 0 )
        {
            up = !up;
        }
        if ( up )
            pwm_value[3] ++;
        else
            pwm_value[3] --;
    }
}
