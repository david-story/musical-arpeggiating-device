/*
 * File:   adc.c
 * Author: David
 *
 * Created on May 2, 2019, 8:48 AM
 */
#include "xc.h"

int discretize_adc()
{
    int val = 0;
    
    return val;
}

unsigned int read_adc1()
{
    int val = 0x00;                                 // sets val to 0
    int low = 0x00;                                 // sets low to 0
    int high = 0x00;                                // sets high to 0
    
    ADCON0bits.GODONE = 1;                          // sets ADC to GO
    while(ADCON0bits.GODONE == 1);                  // while not done
    
    low = ADRESL;                                   // result low to low
    high = ADRESH;                                  // result high to high
    high = (high << 8);                             // high left shifted 8
    val = high + low;                               // new int val of shifted high + low
            
    return val;                                     // returns int value of adc
}

unsigned int read_adc0()
{
    int val = 0x00;                                 // sets val to 0
    int low = 0x00;                                 // sets low to 0
    int high = 0x00;                                // sets high to 0
    
    ADCON0bits.GODONE = 1;                          // sets ADC to GO
    while(ADCON0bits.GODONE == 1);                  // while not done
    
    low = ADRESL;                                   // result low to low
    high = ADRESH;                                  // result high to high
    high = (high << 8);                             // high left shifted 8
    val = high + low;                               // new int val of shifted high + low
            
    return val;                                     // returns int value of adc
}

void adc0_init()
{
    TRISA = 0xFF;
    ANSEL = 0;
    ANSELbits.ANS2 = 1;
    ADCON0 = 0b00000001;                            //select channel AN0 and turn on A/D module
    ADCON1 = 0B00000000;                            //select Vdd and Vss as reference voltaages and set up RA0/AN0 for analog input
    ADCON2 = 0b10101101;                            //Conversion reading is right justified, Tad = 12 and conversion frequency = Fosc/16
}

void adc1_init()
{
    TRISA = 0xFF;
    ANSEL = 0;
    ANSELbits.ANS1 = 1;
    ADCON0 = 0b00001001;                            //select channel AN0 and turn on A/D module
    ADCON1 = 0B00000000;                            //select Vdd and Vss as reference voltaages and set up RA0/AN0 for analog input
    ADCON2 = 0b10101101;                            //Conversion reading is right justified, Tad = 12 and conversion frequency = Fosc/16
}