
#include "ir_sensor.h"

uint32_t now = 0;
uint32_t last_time_us = 0;
uint8_t curr_index = 0;

uint32_t ir_times[50];

static void ir_sensor_callback()
{
    bool state = digitalRead( IR_SENSOR_PIN );
    if ( state )
    {
        last_time_us = now;
        now = micros(); 
        ir_times[curr_index] = now - last_time_us;
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
    for ( uint8_t i = 0; i < curr_index; i ++ )
    {
        Serial.println(ir_times[i]);
    }
    curr_index = 0;
}

