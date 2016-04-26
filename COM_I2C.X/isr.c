/*
 * File:   isr.c
 * Author: patrick
 *
 * Created on January 20, 2016, 7:54 AM
 */

#include <p18cxxx.h>
#include <xc.h>
#include "SSP.h"

void interrupt low_priority ISRLow (void);
void interrupt high_priority ISRHigh (void);
#define _XTAL_FREQ 40000000

uint8_t buffer[10]={0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x40};

/*******************************************************************************
* Interrupt Service Routine
*
* DESCRIPTIONS:
* This is the main ISR 
* There is 2 interrupt vectors in PIC18F, we are using high priority interrupt only
*******************************************************************************/
void interrupt low_priority ISRLow (void)
{
    if (PIR1bits.SSPIF)
    {
        
        LATBbits.LATB6=1;
        SSPCONbits.CKP=0;
        __delay_ms(10);
        SSPCONbits.CKP=1;
        if (SSPSTATbits.R_nW==0)I2c_read_char2();
        else I2c_send_array(&buffer);
        LATBbits.LATB6=0;
        PIR1bits.SSPIF=0;
    }
}

void interrupt high_priority ISRHigh (void)
{
    if (PIR1bits.SSPIF)
    {
        LATBbits.LATB7=1;
        PIR1bits.SSPIF=0;
    }
}



