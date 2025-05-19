#include <Arduino.h>

// #define DEBUG
#ifdef DEBUG 
    #include "avr8-stub.h" 
    #include "app_api.h" 
#endif

#define ADC_PIN A0 
#define READ_AMT 20 

uint32_t result = 0;

void setup()
{
    #ifdef DEBUG
    debug_init();
    #endif

    pinMode(ADC_PIN,INPUT);

    Serial.begin(9600);
}

void loop()
{   
    result = 0;
    for ( uint8_t i = 0; i < READ_AMT; i++ )
    {
        result += analogRead(ADC_PIN);
    }
    Serial.println(result/READ_AMT);
    
}

