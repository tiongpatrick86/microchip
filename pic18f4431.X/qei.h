/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#pragma warning disable 752
#pragma warning disable 520

#include <xc.h> // include processor files - each processor file is guarded.  
// TODO Insert appropriate #include <>
#include <stdint.h>
#include <stdbool.h>
// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// QEICON SFR CONFIGURATION, Use "AND MASK"
#define QEI_CONFIG_VELOCITY_ON              0b01111111
#define QEI_CONFIG_VELOCITY_OFF             0b11111111
#define QEI_CONFIG_4XUPDATE_POSCOUNT_RESET  0b11111011
#define QEI_CONFIG_4XUPDATE_INDEX_RESET     0b11110111
#define QEI_CONFIG_2XUPDATE_POSCOUNT_RESET  0b11101011
#define QEI_CONFIG_2XUPDATE_INDEX_RESET     0b11100111
#define QEI_CONFIG_OFF                      0b11100011
#define QEI_CONFIG_VELOCITY_REDUCTION_1_64   0b11111111
#define QEI_CONFIG_VELOCITY_REDUCTION_1_16   0b11111110
#define QEI_CONFIG_VELOCITY_REDUCTION_1_4    0b11111101
#define QEI_CONFIG_VELOCITY_REDUCTION__1_1   0b11111100


#define QEI_INTERRUPT_ENABLE()             (PIE3bits.PTIE=1)
#define QEI_INTERRUPT_DISABLE()            (PIE3bits.PTIE=0)

#define QEI_INTERRUPT_HIGH_PRIORITY()      (PIR3bits.PTIF=1)
#define QEI_INTERRUPT_LOW_PRIORITY()       (PIR3bits.PTIF=0)

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

typedef union {
    struct{
        uint8_t POSCNT_LSB;
        uint8_t POSCNT_MSB;
    }POSCNT_8bit;
    uint16_t POSCNT_16bit;
}QEI_POSCNT;



#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

void QEI_Init (uint8_t QEI_CONFIG);    
void QEI_Update_Pos (uint16_t qeipos);
void QEI_Set_MaxCounter (uint16_t qeipos);
QEI_POSCNT QEI_Get_Pos (void); 
bool QEI_Error_Flag (void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

