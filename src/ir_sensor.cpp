
#include "ir_sensor.h"

uint32_t now = 0;
uint32_t last_time_us = 0;
uint8_t curr_index = 0;

uint32_t ir_times[50];
bool something_ready = 0;

bool bit_array[50];

static void ir_sensor_callback()
{
    bool state = digitalRead( IR_SENSOR_PIN );
    if ( state )
    {
        last_time_us = now;
        now = micros(); 
        ir_times[curr_index] = now - last_time_us;

        // if there is a start bit or error length => set to 0 assuming that task is called often enough
        ir_data_t res = ir_range(ir_times[curr_index]); 
        bit_array[curr_index] = ((res == IR_DATA_HIGH)?(bool)true:(bool)false);
        if ( res == IR_DATA_START || res == IR_DATA_ERR ) 
            curr_index = 0;
        curr_index++;
        digitalWrite(IR_SENSOR_VISUALISE_PIN, HIGH );
    }
    else 
    {
        digitalWrite(IR_SENSOR_VISUALISE_PIN, LOW);
    }
}

void init_ir_sensor()
{
    pinMode(IR_SENSOR_PIN, INPUT);
    
    attachInterrupt(digitalPinToInterrupt(IR_SENSOR_PIN), ir_sensor_callback, RISING | FALLING );
    
    pinMode(IR_SENSOR_VISUALISE_PIN,OUTPUT);
}

void ir_sensor_task()
{
    if ( curr_index >= 16 ) 
    {
        // for ( uint8_t i = 0; i < curr_index; i ++ )
        // {
        //     Serial.print(ir_times[i]);
        //     Serial.print(",");
        // }
        // Serial.println();
        for ( uint8_t i = 0; i < 16; i++ )
        {
            Serial.print(bit_array[i]);
            Serial.print(",");
        }
        Serial.println();
        curr_index = 0;
        something_ready = false;
    }
}


ir_data_t ir_range(uint32_t value)
{
    if ( value < 316 )
        return IR_DATA_ERR;
    if ( value <= 526 )
        return IR_DATA_LOW;
    if ( value <= 947 )
        return IR_DATA_HIGH;
    if ( value <= 1597 )
        return IR_DATA_START; 
    
    return IR_DATA_ERR;
    
}

