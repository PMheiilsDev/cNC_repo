#include <Arduino.h>

// #define DEBUG
#ifdef DEBUG 
    #include "avr8-stub.h" 
    #include "app_api.h" 
#endif

#define ADC_PIN A0 
#define READ_AMT 20 


int main_()
{
    init();
    Serial.begin(9600);
    // eigener Code

    uint16_t res[60];

    uint32_t last_measure_time = 0;

    uint8_t ctr = 0;

    while(1)
    {
        uint32_t now = millis();
        if ( now - last_measure_time >= (uint32_t)60*1000 )
        {
            last_measure_time = now;
            res[ctr] = analogRead(A0);
            ctr++;
            if ( ctr >= 60 )
            {
                uint32_t sum = 0;
                for ( uint8_t i = 0; i < sizeof(res)/sizeof(res[0]); i++ )
                {
                    sum += res[i]; 
                }
                ctr = 0;
                Serial.print((double)sum/sizeof(res)*sizeof(res[0])/1023*5e3);
                Serial.println(" mV");
            }
        }
        
    }

    // 
}


void setup(){main_();};

