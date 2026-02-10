#include "init.h"

uint16_t display[]={0x08f3, 0x00f9, 0x00ff, 0x00f9, 0x0039, 0x00f7, 0x0000,  0x0000, 0x0000,  0x0000,0x0000,  
0x0000,0x0000,  0x0000, 0x0536, 0x003f, 0x0936, 0x1200, 0x0039, 0x00f7, 0x0000,  0x0000, 0x0000,  0x0000,0x0000,  
0x0000,0x0000,  0x0000, 0x0536, 0x00f7, 0x08f3, 0x1200, 0x003f, 0x0000,  0x0000, 0x0000,  0x0000,0x0000,  0x0000,
0x0000,  0x00ff, 0x00f9, 0x08f3, 0x0936, 0x00f7, 0x00ff, 0x00f9, 0x0000,  0x0000, 0x0000,  0x0000,0x0000,  0x0000,0x0000};//54

void main(void) {
    uint8_t i=0,k, m, contador_140ms=0;
    uint16_t temporal;
    
    TRISB = 0;
    TRISD = 0;
    TRISA = 0;
    LATB = 0xFF;
    LATC = 0;
    LATD = 0;

    while(1) {
        m=0;
        
        for(k=i;k<i+7;k++) {            
            temporal=display[k];
            LATA = temporal & (0x3F);
            LATD = ((temporal & (0xFFC0))>> 6);
            LATB &= ~(1<<m);
            __delay_ms(20);
            LATB = 0xFF;            
            m++;
        }
        
        if(contador_140ms==7) {
            contador_140ms=0;
            if(i==47) {
               i=0;
           }else{
               i++;
           }
        }else{
            contador_140ms++;
        }
        
    }
    
    
    return;
}
