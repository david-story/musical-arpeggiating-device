/*
 * File:   lcd.c
 * Author: David
 *
 * Created on May 2, 2019, 6:35 PM
 */


#include <xc.h>
#include "config.h"

char perm_screen_1[6] = {'n','o','t','e',':',' '};
char perm_screen_2[6] = {' ','b','p','m',':',' '};

//Initialization to use LCD with wanted settings

void lcdcmd(char cmdout)
{
    lcdport=cmdout;
    rs=0;
    rw=0;
    en=1;
    __delay_ms(10);
    en=0;
    return;
}

void lcd_init()
{
    TRISB=0x03;                             // Configure Port B as output port
    PORTB=0;                                // Set PORTB 0x00
    TRISD=0;                                // Configure Port D as output port
    PORTD=0;                                // Set PORTD 0x00
    lcdcmd(0x38); //Set to 2-line 16 char, 8-bit mode
    lcdcmd(0x0C); //Make cursor invisible
    lcdcmd(0x01); //Clear the screen
    lcdcmd(0x06);
    lcdcmd(0x80); //Set cursor to top line and first character
    return;
}

void lcddata(char dataout)
{
    lcdport=dataout;
    rs=1;
    rw=0;
    en=1;
    __delay_ms(10);
    en=0;
    return;
}

void screen_setup()
{
    // clears the display
    lcdcmd(0x01);
    // return home
    lcdcmd(0x02);
    lcdcmd(0x80);
    
    // write the permanent contents of array 1
    for(int i = 0; i < 6; i++)
    {
        lcddata(perm_screen_1[i]);
    }
   
    lcdcmd(0xC0);
    
    for(int i = 0; i < 6; i++)
    {
        lcddata(perm_screen_2[i]);
    }
    return;
}