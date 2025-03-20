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
button_t button_buzzer;

uint8_t button_task(button_t* button);


void setup()
{
    pinMode(6,OUTPUT);
    fader_1.Ctr = 0;
    fader_1.up = 1;

    button_1.gpio = A1;
    pinMode(button_1.gpio, INPUT);
    
    button_buzzer.gpio = A2;
    pinMode(button_buzzer.gpio, INPUT);

    pinMode(3, OUTPUT);
    digitalWrite(3,LOW);

    init_display();

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
    if( button_task(&button_buzzer) )
    {
        for ( uint8_t i = 1; i < 100; i++ )
        {
            for( uint8_t j = 0; j < 200/i; j++ )
            {
                digitalWrite(3, HIGH);
                delayMicroseconds(i*10);
            
                digitalWrite(3, LOW);
                delayMicroseconds(i*10);
            }
        }
        analogWrite(3,0);
    }
}

uint8_t button_task(button_t* button)
{
    button->state_pref = button->state;
    button->state = !digitalRead(button->gpio);

    return( button->state && ! button->state_pref);
}

