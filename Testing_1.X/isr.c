/*******************************************************************************
* This file provides the interrupt service routine, ISR for PTK40A using PIC18F
*
* Author: Ober Choo Sui Hong @ Cytron Technologies Sdn. Bhd.
*******************************************************************************/

#include <p18cxxx.h>	//include the PIC18F model of PIC header file
#include "system.h"
#include "spi.h"
#include "delay18.h"
#include "uart.h"

unsigned char byte=0;
// Declare for local function prototypes
void ISRHigh(void);

/*******************************************************************************
* Interrupt Service Routine
*
* DESCRIPTIONS:
* This is the main ISR 
* There is 2 interrupt vectors in PIC18F, we are using high priority interrupt only
*******************************************************************************/

#pragma	code InterruptVectorHigh = 0x08
void InterruptVectorHigh(void)
{
 	_asm
		goto ISRHigh		// jump to interrupt routine
	_endasm
}
#pragma code

#pragma	code InterruptVectorLow = 0x18
void InterruptVectorLow(void)
{
 	_asm
		goto ISRHigh			// jump to interrupt routine
	_endasm
}
#pragma code



#pragma interrupt ISRHigh
void ISRHigh(void)
{
	   if (PIR1bits.RCIF ==1) 
	   {
           STATUS_RECEIVING =1;
           STATUS_RECEIVING =0;
	   }
       
       if (PIR1bits.SSPIF==1)
       {
           STATUS_BUSY=1;
           uc_send_receive_spi(0xAA);
           STATUS_BUSY=0;
           PIR1bits.SSPIF =0;
       }
       
       if (PIR3bits.PTIF==1)
       {
//           LATBbits.LATB6=~LATBbits.LATB6;
           PIR3bits.PTIF=0;
       }
       
       if (PIR1bits.ADIF==1)
       {
//           LATBbits.LATB6 =1;
//           ADCON0bits.GO =1;
//           while(ADCON0bits.DONE==1);
//           uart_transmit((unsigned char)ADRES);
           
           PIR1bits.ADIF=0;
       }
}


