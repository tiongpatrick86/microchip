/*
 * File:   COM.c
 * Author: patrick
 *
 * Created on April 25, 2016, 10:00 AM
 */
    #include <xc.h>
    #include <stdint.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <i2c.h>
    #include "SSP.h"
    #include <GenericTypeDefs.h>

    #pragma config OSC = HSPLL	// Maximum Fosc is 40MHz, so HSPLL is select, please change external crystal to 10MHz
	#pragma config FCMEN = OFF 	// Fail-Safe Clock Monitor Enable bit
	#pragma config IESO = OFF		// Internal/External Oscillator Switchover Enable bit
	#pragma config PWRTEN = ON		// Power Up Timer Enable bit 
	#pragma config BOREN = OFF 	// Brown-out Reset Enable bits
    #pragma config BORV = 45
	#pragma config WDTEN = OFF 		// Watch-Dog Timer Enable bit
//CONFIG3L @ 0x300004
    #pragma config T1OSCMX =OFF
    #pragma config PWMPIN =OFF
    #pragma config HPOL =LOW
    #pragma config LPOL =LOW

//CONFIG3H @ 0x300005
	#pragma config MCLRE = ON		// MCLR pin enable, RE3 input disabled

//CONFIG4L @ 0x300006
	#pragma config STVREN = ON	//Stack overflow will cause Reset
	#pragma config DEBUG = OFF	// Debug disable, RB6 and RB7 as normal IO
	#pragma config LVP = OFF 		// Low voltage programming disable

//CONFIG5L @ 0x300008
    #pragma config CP0 =OFF
    #pragma config CP1 =OFF

//CONFIG5H @ 0x300009
    #pragma config CPB =OFF
    #pragma config CPD =OFF

// CONFIG6L @ 0x30000A
    #pragma config WRT0 =OFF
    #pragma config WRT1 =OFF

// CONFIG6H @ 0x30000B
    #pragma config WRTB =OFF
    #pragma config WRTC =OFF
    #pragma config WRTD =OFF

// CONFIG7L @ 0x30000C
    #pragma config EBTR0 =OFF
    #pragma config EBTR1 =OFF

// CONFIG7H @ 0x30000D
    #pragma config EBTRB =OFF

#if defined (__18F4431)
    #pragma config SSPMX = RD1
#endif


/******************************************************************************
 *
 *          Private Function Prototypes
 *
 *
 *****************************************************************************/
#define _XTAL_FREQ 40000000
//#define ENABLE_SSP_INTERUPT (SSPCONbits.SSPEN=1);



void SYSTEM_INT(void);
void I2C_Slave_config(void);

uint8_t Buffer;
/******************************************************************************
 *
 *          Private Function 
 *
 *
 *****************************************************************************/

void SYSTEM_INT (void)
{
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
        TRISD = 0b00001100;
        TRISE = 0;
    #endif
    
    ADCON1bits.VCFG= 0;         //Reference voltage is not used
    ANSEL0=0;                   //All are digital input or output
    
}

void I2C_Slave_config (void)
{
    uint8_t I2c_mode,I2c_slave_add;
    
    I2c_mode = 0b00001110;
    I2c_slave_add = 0x27;
// Configure SCL and SDA as input pin
    TRISD |= 0b00001100;
//    TRISDbits.RD3=1;
//    TRISDbits.RD2=1;
    SSPADD = (uint8_t)(I2c_slave_add<<1);
    SSPCONbits.WCOL =0;
    SSPCONbits.CKP = 1;
    SSPCONbits.SSPM = 0b0110;

// SSP Enable bits    
    SSPCONbits.SSPEN = 1;
    
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    
    IPR1bits.SSPIP = 0;
    PIE1bits.SSPIE = 1;
}


void main(void) {
    SYSTEM_INT();
    I2C_Slave_config();

    while (TRUE)
    {

    }
    return;
}
