/*
 * File:   qei.c
 * Author: tiongpatrick
 *
 * Created on April 27, 2016, 4:26 AM
 */


#include <xc.h>
#include "qei.h"


bool QEI_Error_Flag (void)
{
    return(QEICONbits.QERR);
}

void QEI_Init (uint8_t QEI_CONFIG)
{
#if defined (__18F2331) || defined (__18F2431)||\
    defined (__18F4331)||defined (__18F4431)
    TRISA = TRISA|0b00011100;
    #endif

    QEICON = QEI_CONFIG;
    
}

void QEI_Update_Pos (uint16_t qeipos)
{
    POSCNTH = (uint8_t)(qeipos>>8);
    POSCNTL = (uint8_t)(qeipos);    
}

void QEI_Set_MaxCounter (uint16_t qeipos)
{
    MAXCNTH = (uint8_t)(qeipos>>8);
    MAXCNTL = (uint8_t)(qeipos);   
}

QEI_POSCNT QEI_Get_Pos (void)
{
    QEI_POSCNT CurrentQEI;
    CurrentQEI.POSCNT_8bit.POSCNT_LSB = POSCNTL;
    CurrentQEI.POSCNT_8bit.POSCNT_MSB = POSCNTH;    
    return (CurrentQEI);
}

