/*******************************************************************************
* This file provides the functions for the SPI module on PTK40A, using PIC18F
*
* Author: Ober Choo Sui Hong @ Cytron Technologies Sdn. Bhd.
*******************************************************************************/

#include <p18cxxx.h>	//include the PIC18F model of PIC header file
#include "system.h"
#include "spi.h"
#include "delay18.h"


/*******************************************************************************
* PUBLIC FUNCTION: spi_initialize
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Initialize the SPI module.
*
*******************************************************************************/
void spi_initialize(void)
{
    unsigned char dummy;
	// Set the SDO as output. SDI is automatically controlled by the SPI module.
	#if defined (__18F4520)
	TRISCbits.TRISC5 = 0;
	#elif defined (__18F4550)
	TRISCbits.TRISC7 = 0;	//PIC18F4550 SDO pin is located at RC7, 
						//but PTK40A cannot use SPI if PIC18F4550 is used
    #elif defined (__18F4431)
    TRISDbits.TRISD1 =0;
    TRISDbits.TRISD2 =1;
    TRISDbits.TRISD3 =0;
    //TRISCbits.TRISC6 =0;
	#endif
	
	// deactivate Chip Select, the EEPROM is not activate
//	SPI_EEPROM_CSN = 1;
	
	// Input data sampled at middle of data output time.
	SSPSTATbits.SMP = 1;
	
	// Select SPI mode 1, 1.
	SSPSTATbits.CKE = 1;		// Output data changes on transition from active to idle clock state.
					// Rising edge
    
    #if defined (__18F4520)^(__18F4550)    
	SSPCON1bits.CKP = 0;		// Idle state for clock is a low level.
	
	// SPI Master mode, clock = FOSC/4.
	SSPCON1bits.SSPM3 = 0;
	SSPCON1bits.SSPM2 = 0;
	SSPCON1bits.SSPM1 = 0;
	SSPCON1bits.SSPM0 = 0;
	
	// Clear the Write Collision Detect bit.
	SSPCON1bits.WCOL = 0;
	
	// Enable the MSSP module.
	SSPCON1bits.SSPEN = 1;
    
    #elif defined (__18F4431)^(__18F2331)
    SSPCONbits.CKP = 1;		// Idle state for clock is a low level.
	
	// SPI Master mode, clock = FOSC/4.
	SSPCONbits.SSPM3 = 0;
	SSPCONbits.SSPM2 = 0;
	SSPCONbits.SSPM1 = 1;
	SSPCONbits.SSPM0 = 0;
	
	// Clear the Write Collision Detect bit.
	SSPCONbits.WCOL = 0;
	
	// Enable the MSSP module.
    SSPCONbits.SSPOV=1;
    SSPCONbits.SSPOV=0;
    
	SSPCONbits.SSPEN = 1;
    
    dummy=SSPBUF;
    dummy=SSPBUF;
    dummy=SSPBUF;
    #endif
    
}


/*******************************************************************************
* PUBLIC FUNCTION: spi_initialize
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Initialize the SPI module.
*
*******************************************************************************/
void spi_initialize_interrupt(void)
{
	// Set the SDO as output. SDI is automatically controlled by the SPI module.
	#if defined (__18F4520)
	TRISCbits.TRISC5 = 0;
	#elif defined (__18F4550)
	TRISCbits.TRISC7 = 0;	//PIC18F4550 SDO pin is located at RC7, 
						//but PTK40A cannot use SPI if PIC18F4550 is used
    #elif defined (__18F4431)
    TRISDbits.TRISD1 =0;
    TRISDbits.TRISD2 =1;
    TRISDbits.TRISD3 =1;
    TRISCbits.TRISC6 =1;
	#endif
	
	// deactivate Chip Select, the EEPROM is not activate
    //	SPI_EEPROM_CSN = 1;
	
    RCONbits.IPEN=1;
    INTCONbits.GIE=1;
    INTCONbits.PEIE=1;
    IPR1bits.SSPIP =1;
    PIE1bits.SSPIE=1;
    
    // Input data sampled at middle of data output time.
	SSPSTATbits.SMP = 0;
	
	// Select SPI mode 1, 1.
	SSPSTATbits.CKE = 1;		// Output data changes on transition from active to idle clock state.
					// Rising edge
    
    #if defined (__18F4520)^(__18F4550)    
	SSPCON1bits.CKP = 0;		// Idle state for clock is a low level.
	
	// SPI Slave mode, clock = FOSC/4.
	SSPCON1bits.SSPM3 = 0;
	SSPCON1bits.SSPM2 = 1;
	SSPCON1bits.SSPM1 = 0;
	SSPCON1bits.SSPM0 = 0;
	
	// Clear the Write Collision Detect bit.
	SSPCON1bits.WCOL = 0;
	
	// Enable the MSSP module.
	SSPCON1bits.SSPEN = 1;
    
    #elif defined (__18F4431)^(__18F2331)
    SSPCONbits.CKP = 0;		// Idle state for clock is a low level.
	
	// SPI Master mode, clock = FOSC/4.
	SSPCONbits.SSPM3 = 0;
	SSPCONbits.SSPM2 = 0;
	SSPCONbits.SSPM1 = 0;
	SSPCONbits.SSPM0 = 0;
	
	// Clear the Write Collision Detect bit.
	SSPCONbits.WCOL = 0;
	
	// Enable the MSSP module.
	SSPCONbits.SSPEN = 1;
    #endif
    
}









/*******************************************************************************
* PUBLIC FUNCTION: uc_send_receive_spi
*
* PARAMETERS:
* ~ uc_data		- The data to send.
*
* RETURN:
* ~ The data received from the SPI.
*
* DESCRIPTIONS:
* Send and receive the data using the SPI.
*
*******************************************************************************/
unsigned char uc_send_receive_spi(unsigned char uc_data)
{	
	// Send the data
    Slave1 =0;
    delay_10us(10);
	SSPBUF = uc_data;
	// Wait for the SPI module to finish sending / receiving.
	while(SSPSTATbits.BF == 0);
	
	// Return the received data.
//    SSPBUFFER=SSPBUF;
    Slave1 =1;
//	
    return SSPBUF;
}

unsigned char array_send_receive_spi(unsigned char *uc_data)
{	
	// Send the data
    unsigned char i=0;
    
    Slave1 =0;
    delay_10us(10);
	SSPBUF = *uc_data;
	// Wait for the SPI module to finish sending / receiving.
	while(SSPSTATbits.BF == 0);
	
    for (i=0;i<10;i++)
    {
        while (SSPSTATbits.BF==0);
        
    }
	// Return the received data.
//    SSPBUFFER=SSPBUF;
    Slave1 =1;
//	
    return SSPBUF;
}




unsigned int ptr_send_receive_spi(unsigned char* ptr_sent_array, unsigned char* ptr_receive_array)
{
    // Send the data
    unsigned int z=0;
    unsigned char buffer=0;
    for (z=0;z<16;z++)
    {
        buffer=*ptr_sent_array; 
        Slave1 =0;
        delay_10us(1);
        if (SSPCONbits.SSPOV) LATAbits.LATA0 =1;
        SSPBUF = buffer;
        // Wait for the SPI module to finish sending / receiving.
        while(SSPSTATbits.BF == 0);

        // Return the received data.
        *ptr_receive_array=SSPBUF;
        delay_10us(1);           
        Slave1 =1;
        *ptr_receive_array++;
        *ptr_sent_array++;
    }
    return 0x01;
}




unsigned char uc_receive_spi(void)
{	
    unsigned char dummy=0;
	// Send the data
	dummy = SSPBUF;
	
	// Wait for the SPI module to finish sending / receiving.
	while(SSPSTATbits.BF == 0);
	
	// Return the received data.
	return SSPBUF;
}





