/* 
 * File:   SSP.h
 * Author: patrick
 *
 * Created on April 25, 2016, 10:43 AM
 */

#ifndef SSP_H
#define	SSP_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct 
{
    uint8_t BUF[10];
    uint8_t INDEX;        
}I2C_Data;
    
    
    
void I2C_Receive(uint8_t *SSP_BUF); 
void I2c_read_char2(void);
void I2c_read_char(uint8_t *buffer);
void I2c_send_char(uint8_t buffer);
void I2c_send_array(uint8_t *buffer);

#ifdef	__cplusplus
}
#endif

#endif	/* SSP_H */

