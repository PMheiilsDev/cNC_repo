#include <Arduino.h>

#define DISPLAY_SDI 8
#define DISPLAY_RCLK 4
#define DISPLAY_SRCLK 7

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
        digitalWrite(DISPLAY_SDI,0);
        digitalWrite(DISPLAY_SRCLK,HIGH);
        delay(100);
    }
    for( uint8_t i = 8; i < 16; i++ )
    {
        digitalWrite(DISPLAY_SRCLK,LOW);
        digitalWrite(DISPLAY_SDI,1);
        digitalWrite(DISPLAY_SRCLK,HIGH);
        delay(100);
    }
    
    
    digitalWrite(DISPLAY_RCLK,HIGH);
    
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

