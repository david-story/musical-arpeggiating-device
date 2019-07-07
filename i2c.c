/*
 * File:   i2c.c
 * Author: David
 *
 * Created on May 2, 2019, 8:57 AM
 */


#include <xc.h>

void i2c_init()
{
    // Configure both data and clock as input pins
    TRISCbits.RC3 = 1;  // Configures SCL as input
    TRISCbits.RC4 = 1;  // Configures SDA as input
    
    // Set up for Master Mode
    SSPCON1bits.SSPM = 0x08;
    SSPCON1bits.SSPEN = 0x01;   
    
    return;
}

void i2c_send(char val, char address)
{
    // Start condition
    SSPCON2bits.SEN = 1;
    // SSPIF
    SSPIF = 1;
    return;
}

char i2c_recieve(char address)
{
    char val; 
    val = 0x00;
    return val;
}