#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
//#include <delays.h>
#include <time.h>

#include <math.h>
#include "adc.h"
#include "lcd.h"
#include "config.h"
#include <pic18f45k20.h>

// {'F','2',' '},{'C','#','2'},{'D','2',' '}

// FUNCTION PROTOS
void CCP_timer_init(void);
void __interrupt(high_priority) arp_gen();

// ! ! UPDATE THIS AS NEEDED ! ! //     //      //      //
//
// Pre-calculated pitch-gen counter values
// for 16 MHz clock, no TMR1 prescale
//  C2 - E4, chromatic
//
//  

const int tone_counts[] = {30576, 28860, 27241, 25714, 24269,
    //               C2      C#       D      D#     E
    22907, 21622, 20408, 19262, 18182, 17161, 16198, 15289,
    // F    F#       G      G#     A     A#     B     C3
    14431, 13621, 12857, 12135, 11454, 10811, 10204, 9632,
    //  C#   D      D#     E     F     F#      G    G#
    9091, 8581, 8099, 7644, 7216,  6811, 6428, 6067,
    //   A      A#   B      C4    C#    D    D#     E
    5727, 5406, 5102, 4816, 4545, 4290, 4050, 3822,
    // F   F#    G      G#    A     A#    B     C5
    3608, 3405, 3214, 3034, 2863, 2703, 2551, 2408}
    //C#    D    D#    E      F    F#    G     G#

;

// Push-button pattern, in 1/4-steps (1 - tonic, 4 - maj third, 7 - fifth, etc)

// 
int seq_length = 6;


// PB1
const int sequence_maj[] = {0, 7, 12, 16, 12, 7}; // major triad w/ octave

const int sequence_min[] = {0, 7, 12, 15, 12, 7}; // major triad w/ octave

//PB2

//      //      //      //      //      //      //

// GLOBAL LITERALS

int prescale_tmr0 = 64;
int prescale_tmr1 = 1;
float fosc_val = 16000000;

float notediv = 3; // val = notes per 1/4 note

// GLOBAL DECLARES

int seq_pointer; // points to index of sequence tone 
int load_tmr0; // stores counter value for note durations
int bpm = 120;
int base_tone = 0; // points to tonic count val (e.g 0 - C2, 12 - C3)
int value_1 = 0;

//  //  //  //  //  //  //  //  //  //  //  //  //


// INTERRUPT SERVICE ROUTINES

void __interrupt(high_priority) arp_gen() {
    if (PIR1bits.CCP1IF) {
        PIR1bits.CCP1IF = 0;
        WRITETIMER1(0);
    }

    if (INTCONbits.TMR0IF) {
        INTCONbits.TMR0IF = 0;
        PIR1bits.CCP1IF = 0;

        seq_pointer++;
        if (seq_pointer == seq_length) {
            seq_pointer = 0;
            base_tone = value_1;
        }
        // change note
        if (!RB0) {
            CCPR1 = tone_counts[(base_tone) + sequence_maj[seq_pointer]];
        } 
        else if (!RB1) {
            CCPR1 = tone_counts[(base_tone) + sequence_min[seq_pointer]];
        }
        TMR0 = load_tmr0;
    }
}

unsigned char second_octave[12][3] = {{'C','2',' '},{'C','#','2'},{'D','2',' '},{'D','#','2'},{'E','2',' '},{'F','2',' '},{'F','#','2'},{'G','2',' '},{'G','#','2'},{'A','2',' '}, {'A','#','2'},{'B','2',' '}};
unsigned char third_octave[12][3] = {{'C','3',' '},{'C','#','3'},{'D','3',' '},{'D','#','3'},{'E','3',' '},{'F','3',' '},{'F','#','3'},{'G','3',' '},{'G','#','3'},{'A','3',' '},{'A','#','3'},{'B','3',' '}};
unsigned char fourth_octave[12][3] = {{'C','4',' '},{'C','#','4'},{'D','4',' '},{'D','#','4'},{'E','4',' '}};

void main(void) {
    
    OSCCON = 0b11111100; // configure oscillator (to 16 MHz)
    RCONbits.IPEN = 1; // enable interrupt priorities
    INTCONbits.GIEH = 1; // enable HP interrupts
    INTCONbits.GIEL = 1; //enable LP interrupts

    TRISB = 3; // Configure Port B <1:0> as input, <7:2> as output
    PORTB = 0;
    INTCON2bits.RBPU = 0; // internal pullups on
    TRISD = 0; // Configure Port D as output
    PORTD = 0;
    TRISA = 0xFF; // Configure Port A as input

    TRISC = 0x03; // Configure RC<1:0> as input, RC<7:2> as output
    PORTC = 0;

    //    adc_init(); // ADC initialization
    //    lcd_init(); // LCD initialization
    //    char data[4]; // set up voltage display array 
    //    char message[4]; // set up message display array 

    CCP_timer_init();
    float voltage;
    unsigned int value_2;
    int array_value;
    lcd_init();
    
    screen_setup();
    // Main Program Loop
     // TO BE PARAMETERIZED and moved to global declares
    seq_pointer = 0;
    //CCPR1 = (int) round(fosc_val / (8 * prescale_tmr1 * freq2)); // TIMER1 toggle count for tone gen - old version of CCPR1 loading
    CCPR1 = tone_counts[base_tone + sequence_maj[seq_pointer]];
    //CCPR1 = 4545;
    
    while(1)
    {
        // PORTCbits.RC3 = !PORTCbits.RC3;
        // __delay_ms(500);

        char data[3] = {' ', ' ', ' '};
        char bpm_array[5];
        // pin RA0, channel AN0
        adc0_init();
        value_1 = 0;
        value_1 = read_adc0();                         // read the ADC at RA0 into value
        value_1 = value_1 / 35;
        
        // pin RA1, channel AN1
        adc1_init();
        value_2 = 0;
        value_2 = read_adc1();                         // read the ADC at RA0 into value
        value_2 = value_2 / 24;
        value_2 = value_2 * 10;
        value_2 = value_2 + 30;
        bpm = value_2;
        
        T1CONbits.TMR1ON = !(RB0 && RB1); // TMR1 (for tone) runs only when button ON (PB grounded)
        T0CONbits.TMR0ON = !(RB0 && RB1); // TMR0 (for note change) runs only when button ON (PB grounded)
        
        float duration = 60 / (bpm * notediv);
        load_tmr0 = (int) round((2^16) - duration * fosc_val / (4 * prescale_tmr0)); // count val for TMR0
        
        if (RB0 && RB1) {
            
            // if button not pressed, keep reloading TMR0 val to prevent playing from middle of sequence
            // also update base tone and reset sequence pointer
            //base_tone = tone_counts[12];
            TMR0 = load_tmr0;
            PORTC = 0x00;
            base_tone = value_1;  // ADC-translated tone value not loaded until break in sound
            seq_pointer = 0;
            CCPR1 = tone_counts[(base_tone) + sequence_maj[seq_pointer]];
            CCP1CONbits.CCP1M1 = 0;
            CCP1CONbits.CCP1M1 = 1;
            
        }
        
        sprintf(bpm_array, "%u", value_2);
        //sprintf(data, "%u", value_1);
        
        if(value_1 < 12)
        {
            sprintf(data, "%s", second_octave[value_1]);

        }
        else 
        {
            if(value_1 < 24)
            {
                sprintf(data, "%s", third_octave[(value_1)-12]);
            }
            else
            {
                sprintf(data, "%s", fourth_octave[(value_1)-24]);
            }
        }
        lcdcmd(0x02);
        for(int k = 0; k < 6; k++)
        {
            lcdcmd(0x14);
        }
        for(int i = 0; i < 3; i++)
        {
            lcddata(data[i]);
        }
        
        lcdcmd(0xC0);
        for(int k = 0; k < 6; k++)
        {
            lcdcmd(0x14);
        }    
        
        int iter;
        if(value_2 < 10)
        {
            iter = 2;
            data[2] = ']';
        }
        else if (value_2 >= 10)
        {
            iter = 3;
        }
        else
        {
            iter = 1;
        }
         
        for(int i = 0; i < iter; i++)
        {   
            if((iter == 2) && (i > 2))
            {
                lcddata(0x20);
            }
            else if (iter == 2)
            {
            lcddata(bpm_array[i]);
            }
            
            if((iter == 3) && (i > 3))
            {
                lcddata(0x20);
            }
            else if (iter == 3)
            {
            lcddata(bpm_array[i]);
            }
            
        }
        lcdcmd(0x02);

                    }
         
    return;
}

void CCP_timer_init() {
    CCP1CONbits.P1M = 0b00; // P1A assigned as CC i/o (default for current CCP1M setting))
    CCP1CONbits.DC1B = 0b00; // unused for compare mode
    CCP1CONbits.CCP1M = 0b0010; // CCP1 - compare mode, toggle on match

    T1CONbits.RD16 = 0b1; // TMR1 set up as 16-bits
    T1CONbits.T1RUN = 0b0; // Main system clock derived from source other than TMR1
    T1CONbits.T1CKPS = 0b00; // 1:1 prescale
    T1CONbits.T1OSCEN = 0b1; // TMR1 oscillator not enabled (n/a when exclusively using internal osc)
    T1CONbits.T1SYNC = 0b0; // ignored when using internal osc
    T1CONbits.TMR1CS = 0b0; // use internal osc (FOSC/4)
    T1CONbits.TMR1ON = 0b0; // TMR1 stopped (to be started by other peripherals)

    //T0CON = 0b1000101; // enable TMR0, 16-bit, internal clock, 1:64 prescale 
    T0CONbits.TMR0ON = 0b0; // TMR0 stopped (to be started by other peripherals)
    T0CONbits.T08BIT = 0b0; // TMR0 set up as 8-bits
    T0CONbits.T0CS = 0b0; // select internal clock
    T0CONbits.T0SE = 0b0; // increment TMR0 on rising edge of clock
    T0CONbits.PSA = 0b0; // prescaler is assigned
    T0CONbits.T0PS = 0b101; // 1:64 prescale

    T3CONbits.RD16 = 0b0; // TMR3 set up as 8-bits (n/a, TMR3 unused in program)
    T3CONbits.T3CCP2 = 0b0; // TMR1 -> CCP1, TMR3 -> CCP2 // IMPORTANT FOR TMR1
    T3CONbits.T3CCP1 = 0b1; //
    T3CONbits.T3CKPS = 0b00; // 1:1 prescale
    T3CONbits.T3SYNC = 0b0; // ignored when using internal osc
    T3CONbits.TMR3CS = 0b0; // use internal osc (FOSC/4)
    T3CONbits.TMR3ON = 0b0; // TMR3 stopped

    PIE1bits.CCP1IE = 1; // enable CCP1 interrupt
    PIE1bits.TMR1IE = 0; // disable TMR1 overflow interrupt (since relying on CCP1)
    IPR1bits.CCP1IP = 1; // set CCP1 low  priority interrupt
    PIR1bits.CCP1IF = 0; // clear CCP1 interrupt flag (will be handled in software)

    INTCONbits.TMR0IE = 0; // TMR0 interrupt enable
    INTCON2bits.TMR0IP = 1; // set TMR0 low priority interrupt
    INTCONbits.TMR0IF = 0; // clear TMR0 interrupt flag
}

