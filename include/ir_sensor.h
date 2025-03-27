#ifndef _IR_SENSOR_H
#define _IR_SENSOR_H

#include "Arduino.h"

#define IR_SENSOR_PIN 2 

#define IR_SENSOR_VISUALISE_PIN 10 

typedef enum 
{
    IR_DATA_ERR = -1,
    IR_DATA_LOW, 
    IR_DATA_HIGH, 
    IR_DATA_START, 
}
ir_data_t;

void init_ir_sensor();

void ir_sensor_task();

ir_data_t ir_range(uint32_t value);


#endif
