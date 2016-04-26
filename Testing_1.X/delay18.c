/*******************************************************************************
* This file provides the functions of delay for PIC18F on PTK40A
*
* Author: Ober Choo Sui Hong @ Cytron Technologies Sdn. Bhd.
*******************************************************************************/

#include <p18cxxx.h>	//include the PIC18F model of PIC header file
#include <delays.h>
#include "system.h"
#include "delay18.h"

/*******************************************************************************
* PUBLIC FUNCTION: delay_10us
*
* PARAMETERS:
* ~ ui_10microsecond
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* delay for for period 10 microsecond base on ui_10microsecond
* based on _XTAL_FREQ value, ui_10microsecond range from 0-65,535.
*******************************************************************************/
void delay_10us(unsigned int ui_10microsecond)
{
	for( ; ui_10microsecond > 0; ui_10microsecond --)
	{
		#if (_OSC_FREQ == 20000000)
		Delay10TCYx(5);	//50 instruction cycle with 20MHz oscillator is 10us
        #elif (_OSC_FREQ == 40000000)
        Delay10TCYx(10);
		#elif (_OSC_FREQ == 48000000)
		Delay10TCYx(12);	//120 instruction cycle with 48MHz oscillator is 10us
		#endif
	}	
}


/*******************************************************************************
* PUBLIC FUNCTION: delay_ms
*
* PARAMETERS:
* ~ ui_millisecond
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* delay for for period millisecond base on ui_millisecond
* based on _XTAL_FREQ value, ui_millisecond range from 0-65,535.
*******************************************************************************/
void delay_ms(unsigned int ui_millisecond)
{
	for( ; ui_millisecond > 0; ui_millisecond --)
	{
		#if (_OSC_FREQ == 20000000)
		Delay1KTCYx(5);	//5K instruction cycle with 20MHz oscillator is 1ms
        #elif (_OSC_FREQ == 40000000)
        Delay1KTCYx(10);
		#elif (_OSC_FREQ == 48000000)
		Delay1KTCYx(12);	//12K instruction cycle with 48MHz oscillator is 1ms
		#endif
	}		
}	

void PrintValue(unsigned short int data)
{
  unsigned char hundred_thousand;
  unsigned char ten_thousand;
  unsigned char thousand;
  unsigned char hundred;
  unsigned char tenth;

  hundred_thousand = data/100000;         
  data = data % 100000;
  ten_thousand = data/10000;
  data = data % 10000;
  thousand = data / 1000;
  data = data % 1000;
  hundred = data / 100;
  data = data % 100;
  tenth = data / 10;
  data = data % 10;



//  if(hundred_thousand>0)
//  {
//    uart_transmit(hundred_thousand +0x30); //0x30 added to become ASCII code
//    uart_transmit(ten_thousand +0x30);
//    uart_transmit(thousand +0x30);
//    uart_transmit(hundred +0x30);
//    uart_transmit(tenth +0x30);
//    uart_transmit(data+0x30);
//  }
//
//  else if(ten_thousand>0) 
//  {
//    uart_transmit(0x30); //0x30 added to become ASCII code
//    uart_transmit(ten_thousand+0x30);
//    uart_transmit(thousand+0x30);
//    uart_transmit(hundred+0x30);
//    uart_transmit(tenth+0x30);
//    uart_transmit(data+0x30);
//  }
//  else if(thousand>0)
//  {
//    uart_transmit(0x30); //0x30 added to become ASCII code
//    uart_transmit(0x30);
//    uart_transmit(thousand+0x30);
//    uart_transmit(hundred+0x30);
//    uart_transmit(tenth+0x30);
//    uart_transmit(data+0x30);
//  }
//  else if(hundred>0)
//  {
//    uart_transmit(0x30); //0x30 added to become ASCII code
//    uart_transmit(0x30);
//    uart_transmit(0x30);
//    uart_transmit(hundred+0x30);
//    uart_transmit(tenth+0x30);
//    uart_transmit(data+0x30);
//  }
//  else if(tenth>0)
//  {
//    uart_transmit(0x30); //0x30 added to become ASCII code
//    uart_transmit(0x30);
//    uart_transmit(0x30);
//    uart_transmit(0x30);
//    uart_transmit(tenth+0x30);
//    uart_transmit(data+0x30);
//  }
//  else 
//  {
//    uart_transmit(0x30); //0x30 added to become ASCII code
//    uart_transmit(0x30);
//    uart_transmit(0x30);
//    uart_transmit(0x30);
//    uart_transmit(0x30);
//    uart_transmit(data+0x30);
//  }

}


