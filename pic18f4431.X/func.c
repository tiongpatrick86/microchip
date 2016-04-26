#include "func.h"
#include <xc.h>

unsigned short Buffer;


void adc_get (void)
{
    Buffer = ADRES;
}

