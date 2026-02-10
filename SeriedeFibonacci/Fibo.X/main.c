#include <xc.h>
#include <stdint.h>
#include "configuracion.h"

#define _XTAL_FREQ 4000000


void main(void)
{
    uint8_t i;
    uint8_t fn, fn_1, fn_2;
    TRISD = 0x00;
    LATD = 0X00;
    
    while(1)
    {   
        

        if(i==0) {
            fn=0;
        }else if(i==1) {
            fn=1;
            fn_1=0;
        }else{
            fn = fn_1 + fn_2;
        }
        LATD = fn;
        __delay_ms(500);
        if(i==13) {
            i=0;
            fn_2=0;
            fn_1=0;
            fn=0;            
        }else{
            i++;
            fn_2 = fn_1;
            fn_1 = fn;        
        }
                    
     
    }
    
    
    return;
}
