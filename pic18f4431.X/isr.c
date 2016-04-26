#include <xc.h>
#include "func.h"

void low_priority interrupt low_isr (void);
void high_priority interrupt high_isr (void);



void low_priority interrupt low_isr(void)
{
//    if (PIR3bits.TMR5IF==1)
//    {
//      LATBbits.LATB6=1;
//      PIR3bits.TMR5IF =0;
//    }
}

void high_priority interrupt high_isr(void)
{
    
    if (PIR3bits.TMR5IF==1)
    {
        LATBbits.LATB7=~LATBbits.LATB7;
        PIR3bits.TMR5IF=0;
    }
 
    if (PIR1bits.ADIF==1)
    {
        LATBbits.LATB6 = 0;
        while (!ADCON1bits.BFEMT);
        adc_get();
        LATBbits.LATB6 = 1;
        PIR1bits.ADIF=0;
    }
    
}




