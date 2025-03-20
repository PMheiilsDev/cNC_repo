
// At no point should 0-4 be LOW and 8-15 be HIGH
// as then the LEDs would be in reverse what they might not be made for 

#include "display.hpp" 

uint8_t display_ctr;
uint8_t display_data[NUM_AMT];

uint8_t seg_7_numbers[10] = 
{
    //ABCDEFG.
    0b11111100,  
    0b01100000,  
    0b11011010,  
    0b11110010,  
    0b01100110,  
    0b10110110,  
    0b10111110,  
    0b11100000,  
    0b11111110,  
    0b11110110  
};

/// @brief sends the date to the display this shold never be used 
/// @param data each bit is one LED of a segment
/// @param num the data is displayed on all numbers that are 1 and all that are 0 are turned off  
static void display_send(uint8_t data, uint8_t num);

/// @brief call back for the interal interrupt that handles all number after each other 
static void display_callback();

void init_display()
{

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
        digitalWrite(DISPLAY_SDI,1);
        digitalWrite(DISPLAY_SRCLK,HIGH);
    }

    for( uint8_t i = 8; i < 16; i++ )
    {
        digitalWrite(DISPLAY_SRCLK,LOW);
        digitalWrite(DISPLAY_SDI,1);
        digitalWrite(DISPLAY_SRCLK,HIGH);
    }

    digitalWrite(DISPLAY_RCLK,HIGH);

    display_ctr = 0;
    memset( display_data, 0x00, 4 );

    Timer1.initialize(1e6/(FRAME_RATE_Hz*NUM_AMT));
    Timer1.attachInterrupt(display_callback);
}

static void display_send(uint8_t data, uint8_t num)
{
    // initiate data sending 
    digitalWrite(DISPLAY_RCLK,LOW);

    // send the data 
    for( uint8_t i = 0; i < 8; i++ )
    {
        digitalWrite(DISPLAY_SRCLK,LOW);
        digitalWrite(DISPLAY_SDI, ((data>>i)&1)?LOW:HIGH );
        digitalWrite(DISPLAY_SRCLK,HIGH);
    }

    // set the 4 unused pins 
    for( uint8_t i = 0; i < 4; i++ )
    {
        digitalWrite(DISPLAY_SRCLK,LOW);
        digitalWrite(DISPLAY_SDI,LOW);
        digitalWrite(DISPLAY_SRCLK,HIGH);
    }

    // sent the common pins to high so that all leds can be on 
    for( uint8_t i = 0; i < 4; i++ )
    {
        digitalWrite(DISPLAY_SRCLK,LOW);
        digitalWrite(DISPLAY_SDI,((~num>>i)&1)?LOW:HIGH);
        digitalWrite(DISPLAY_SRCLK,HIGH);
    }

    digitalWrite(DISPLAY_RCLK,HIGH);
    
}

static void display_callback()
{
    display_send(display_data[display_ctr],1<<display_ctr);
    display_ctr++;
    if (display_ctr > NUM_AMT)
    {
        display_ctr = 0;
    }
}

void display_write_number( uint16_t number )
{
    if (number >= 10000) 
        return;

    char buffer[4+1];

    sprintf(buffer, "%04d", number);

    int8_t val[4];

    val[0] = buffer[3] - '0';
    val[1] = buffer[2] - '0';
    val[2] = buffer[1] - '0';
    val[3] = buffer[0] - '0';

    memset( display_data, 0x00, 4 ); 

    bool might_lead = true;

    for ( int8_t i = 3; i > 0; i-- )
    {
        if ( might_lead && ( val[i] == 0 ))
        {    
            display_data[i] = 0x00;
        }
        else // meaning that it is not a leading 0 
        {
            display_data[i] = seg_7_numbers[val[i]];
            might_lead = false;
        }
    }
    display_data[0] = seg_7_numbers[val[0]];

}

