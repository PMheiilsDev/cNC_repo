#include <Arduino.h>
#include "display.hpp"


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

void button_task(button_t* button);


void setup()
{
    pinMode(6,OUTPUT);
    fader_1.Ctr = 0;
    fader_1.up = 1;

    button_1.gpio = A1;
    pinMode(button_1.gpio, INPUT);
    
    init_display();

    for ( uint8_t j = 0; j < 3; j++ )
    {
        for ( uint8_t i = 0; i < 8; i++ )
        {
            delay(1000);

            display_send(1<<i);
        }
    }
    display_send(0);
}

void loop()
{
    if( fader_1.Ctr == 0 ) 
        fader_1.up = 1;
    else if ( fader_1.Ctr == 255 )
        fader_1.up = 0;
    
    fader_1.up?fader_1.Ctr++:fader_1.Ctr--;
    
    analogWrite(6,~fader_1.Ctr);
    delay(10);

    button_task(&button_1);
}

void button_task(button_t* button)
{
    button->state_pref = button->state;
    button->state = !digitalRead(button->gpio);

    if( button->state && ! button->state_pref)
    {
        fader_1.Ctr = 0;
        fader_1.up = 1;
    }
}

