/*
 * File:   ssp.c
 * Author: patrick
 *
 * Created on April 25, 2016, 10:43 AM
 */


#include <xc.h>
#include "SSP.h"
#include <stdint.h>

I2C_Data  I2C_data;

void I2C_Receive(uint8_t *SSP_BUF)
{
    
    if (SSPSTATbits.BF==1)
    {
        while(!SSPSTATbits.P)
        {
            while(!SSPSTATbits.BF);
            *SSP_BUF++=SSPBUF;
        }
        
    }
}

void I2c_read_char(uint8_t *buffer)
{
    if (SSPSTATbits.BF) *buffer =SSPBUF;
//    while(!SSPSTATbits.BF);
//    *buffer = SSPBUF;   
}


void I2c_read_char2(void)
{
    if (SSPSTATbits.BF) I2C_data.INDEX =SSPBUF;
//    while(!SSPSTATbits.BF);
//    *buffer = SSPBUF;   
}




void I2c_send_char(uint8_t buffer)
{
    if (SSPSTATbits.P)I2C_data.INDEX=0;
    
    SSPCONbits.CKP =0;
    SSPBUF = buffer;
    while(!SSPSTATbits.BF);
    SSPCONbits.CKP =1;
}

void I2c_send_array(uint8_t *buffer)
{
    if (SSPSTATbits.P)I2C_data.INDEX=0;
    
    SSPCONbits.CKP =0;
    SSPBUF = buffer[I2C_data.INDEX++];
    while(!SSPSTATbits.BF);
    SSPCONbits.CKP =1;
}

