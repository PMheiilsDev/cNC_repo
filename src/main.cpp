#include <Arduino.h>

// #define DEBUG
#ifdef DEBUG 
    #include "avr8-stub.h" 
    #include "app_api.h" 
#endif

#include "display.hpp"
#include "imperial_march.hpp"

typedef struct
{
    uint16_t Ctr:8, up:1;
}
fader_t;

typedef struct 
{
    char state:1, state_pref:1;
    uint8_t gpio;
}
button_t;

fader_t fader_1;
button_t button_1;
button_t button_buzzer;

uint8_t button_task(button_t* button);


void setup()
{
    #ifdef DEBUG
    debug_init();
    #endif

    init_display();

    pinMode(A1,INPUT);

    // Serial.begin(9600);
}

uint8_t i = 0;

bool mode = 1;

bool state = 0;
bool state_pref = 0;
uint32_t rising_time = 0;
uint32_t toggle_time = 0;

void loop()
{

    state_pref = state;
    state = digitalRead(A1);
    if ( !state && state_pref )
    {
        rising_time = millis();
    }
    if ( state && !state_pref )
    {
        uint32_t dt = millis() - rising_time;
        if ( dt < 500 )
        {
            if( mode )
            {
                i++;
                i = i%6;
            }
        }
        else if ( dt >= 500 )
        {
            mode = !mode;
        }
    }
    if ( !mode )
    {
        if ( millis() - toggle_time >= 1000 )
        {
            toggle_time = millis();
            i++;
            i = i%6;
        }
    }

    display_data[0] = (i==0)?(1<<7):(1<<(i+1));
    
    display_data[1] = (i==0)?(1<<7):1<<(7-i);
    display_data[2] = seg_7_numbers[i];

    display_data[3] = seg_7_numbers[mode];
        
}

