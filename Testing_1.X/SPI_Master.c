/* 
 * File:   slave.c
 * Author: tiongpatrick
 *
 * Created on May 17, 2015, 8:09 AM
 */
#include <p18cxxx.h>
#include <delays.h>
#include <spi.h>
#include "system.h"
#include "delay18.h"
#include "spi.h"
#include "uart.h"


/******************************************************************************
 * Configuration bits
 * 
 * 
 * 
 *****************************************************************************/

	//#pragma config OSC = HS		//if 20MHz external crystal is used, need to select HS
	#pragma config OSC = HSPLL	// Maximum Fosc is 40MHz, so HSPLL is select, please change external crystal to 10MHz
	//#pragma config OSC = INTIO67	// Using internal 8MHz Oscillator, RA6 and RA7 as normal IO pin
	#pragma config FCMEN = OFF 	// Fail-Safe Clock Monitor Enable bit
	#pragma config IESO = OFF		// Internal/External Oscillator Switchover Enable bit
	#pragma config PWRTEN = ON		// Power Up Timer Enable bit 
	#pragma config BOREN = OFF 	// Brown-out Reset Enable bits
	#pragma config WDTEN = OFF 		// Watch-Dog Timer Enable bit
//	#pragma config CCP2MX = PORTC	//CCP2 at RC1
//	#pragma config PBADEN = ON	// PORTB analog pin as analog input after Reset	
//	#pragma config LPT1OSC = OFF	// Timer 1 configured for high power operation 
	#pragma config MCLRE = ON		// MCLR pin enable, RE3 input disabled
	#pragma config STVREN = ON	//Stack overflow will cause Reset
	#pragma config LVP = OFF 		// Low voltage programming disable
//	#pragma config XINST = OFF		// Instruction set extension and Indexed Addressing mode enabled 
	#pragma config DEBUG = OFF	// Debug disable, RB6 and RB7 as normal IO

#if defined (__18F4431)
    #pragma config SSPMX = RD1
#endif


/******************************************************************************
 * 
 * Function Prototype
 * 
 * ****************************************************************************/

unsigned int i=0;
unsigned char ant =0;
unsigned char receive_byte[16]={0x00};
unsigned char send_byte[16]={'0','1','2','3','4','5','6','7','8','9','0','0','0','0','0','0'};
unsigned char send_byte1[16]={'*','A','B','C','D','E','F','G','H','I','J','K','\0','2','3','4'};
unsigned short duty_cycle[5]={0x2ffe,0x1fff,0x0fff,0x0eff,0x09ff};
void getsSPI_modified( unsigned char *rdptr, unsigned char length );
//static unsigned char send_byte[16]={'\0'};
unsigned char text;
void blinkLED(void);
void stepper_state (void);
void ADC_init (void);
unsigned char state = 4;

static unsigned char InnerBuffer[Buffer_size]= {'\0'};

void main(){

#if defined (__18F2331)
    PORTA =0;
    PORTB =0;
    PORTC =0;
    
    TRISA = 0b00011100;
    TRISB = 0b00000000;
    TRISC = 0b10110000;
    
#elif defined (__18F4431)
    //Initialization of I/O
	PORTA = 0;
	PORTB = 0;
	PORTC = 0;
	PORTD = 0;
	PORTE = 0;

	TRISA = 0b00111111;
	TRISB = 0b00000000;
	TRISC = 0b11110011;
	TRISD = 0b00011100;
	TRISE = 0;
#endif

    
    uart_initialize();
    
    
    //PWM Time base output Postscaler
    PTCON0bits.PTOPS = 0b0000;
    
    //PWM Time base output prescaler
    PTCON0bits.PTCKPS = 0b00;
    
    //PWM time base mode: 00 Free Running Mode 01 Single Shot Mode 
    //10 Continuous Up/Down Counting mode 11 Continuous Up/Down with double updates Counting mode
    PTCON0bits.PTMOD = 0b00;
    
    //PWM Time Base timer Enable bits
    PTCON1bits.PTEN = 0;
    
    //PWM Time Base timer count direction
    PTCON1bits.PTDIR = 0;
    
    //PWM Module enable bits
    PWMCON0bits.PWMEN = 0b101;
    
    //PWM output pair mode (0 complementary/1 Independent)
    PWMCON0bits.PMOD0 = 0;//(PWM0,PWM1)
    PWMCON0bits.PMOD1 = 0;//(PWM2,PWM3)
    PWMCON0bits.PMOD2 = 0;//(PWM4,PWM5)
    PWMCON0bits.PMOD3 = 0;//(PWM6,PWM7)
    
    //PWM Special Event Trigger, PWM Update Disable, PWM Output Override Synchronization
    PWMCON1bits.SEVOPS = 0b0000; //PWM Special Event Trigger postscaler bit
    PWMCON1bits.SEVTDIR = 0; //PWM Special Event Trigger count direction
    
    PWMCON1bits.UDIS = 0;//update for duty cycle and period buffer is enabled 
    PWMCON1bits.OSYNC = 0; // Output overrides via OVDCON is synchronized
    
    PTPERH = 0x0f;
    PTPERL = 0xff;
    
    PDC0H=0x2f;
    PDC0L=0xfe;
    
    PDC1H=0x05;
    PDC1L=0x00;
    
    PDC2H=0x05;
    PDC2L=0x00;
    
    PDC3H=0x05;
    PDC3L=0x00;
    
    OVDCOND=0b11111111;
    OVDCONS=0b11111111;
    
    //Dead time control
    DTCONbits.DTAPS = 0b00;
    DTCON = 0b01111111;

    //Interrupt
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    IPR3bits.PTIP = 1;
    PIE3bits.PTIE = 1;
    
    PTCON1bits.PTEN = 1;
   
    ADC_init();
    ADCON0bits.ADON =1;
    
    
   
    while (1)
   {
//        ADCON0bits.GO =1;
//        while(ADCON0bits.DONE==1);   
//        uart_transmit('A');
//        uart_transmit((unsigned int)ADRES);
        state++;
        if (state>8) state =1;
        blinkLED();
        stepper_state();
   }
}

void blinkLED(void)
	{
        if (i>8)i=0;
        if (state==1||state==2||state==3||state==4)LATBbits.LATB7=1;
        else LATBbits.LATB7 = 0;
        PDC0L = (unsigned char)duty_cycle[i];
        PDC0H = (unsigned char)(duty_cycle[i]>>8);
        i++;
//        delay_ms(1);
        delay_10us(50);
	}


void stepper_state (void)
{
    switch (state)
    {
        case 1:
        {
            OVDCOND = 0b01010110;
            break;
        }
        case 2:
        {
            OVDCOND = ~0b10011001;
            break;
        }
        case 3:
        {
            OVDCOND = ~0b10011010;
            break;
        }
        case 4:
        {
            OVDCOND =~ 0b10010110;
            break;
        }
        case 5:
        {
            OVDCOND =~ 0b10100110;
            break;
        }
        case 6:
        {
            OVDCOND =~ 0b01100110;
            break;
        }
        case 7:
        {
            OVDCOND =~ 0b01101010;
            break;
        }
        case 8:
        {
            OVDCOND =~ 0b01101001;
            break;
        }
        default:
        {
            break;
        }
    }
}


void ADC_init (void)
{
    ANSEL0 = 0b00000011;
    ANSEL1 = 0b00000000;
    
    //VREF+ = AVDD, VREF- = AVSS,
    ADCON1bits.VCFG=0b00;
    //FIFO Buffer Enable
    ADCON1bits.FIFOEN =0;
    
    //Continuous loop auto-conversion 
    ADCON0bits.ACONV =0;
    //Multichannel auto-conversion
    ADCON0bits.ACSCH =0;
    ADCON0bits.ACMOD = 0b00;
    
    //Conversion clock 64bits
    ADCON2bits.ADCS =0b110;
    ADCON2bits.ACQT =0b0011;
    
    //Conversion trigger-Power Control PWM rising edge
    ADCON3bits.SSRC = 0b00000;
    
    //Interrupts 
    ADCON3bits.ADRS = 0b11;
    
    ADCHS = 0b00000000;
    ADCON0bits.ADON =0;
    
    
//    INTCONbits.PEIE = 
    
//    ADCON0 = 0b00111000;
    

//    //Right justified
//    ADCON2bits.ADFM = 1;
//    
//    ADCON2bits.ACQT = 0b0110;
//    ADCON2bits.ADCS = 0b110;
//    
//    ADCON3bits.ADRS == 0b00;
//    ADCON3bits.SSRC = 0b10000;
//    
//    ADCHS = 0b00000000;
   
}

void getsSPI_modified( unsigned char *rdptr, unsigned char length )
{
  while ( *rdptr++ != length )                // stay in loop until length = 0
  {
    *rdptr++ = getcSPI();         // read a single byte
                                 // reduce string length count by 1
  }
}


