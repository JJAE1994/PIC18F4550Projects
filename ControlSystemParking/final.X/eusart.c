#include "config.h"

void SER_ESCRIBE_CADENA(const char *cadena, unsigned int tam) {      
    unsigned char x=0;
    for(x=0;x<tam;x++) {
        TXREG = cadena[x];
        while(TXSTAbits.TRMT == 0);
    }
}

void SER_ESCRIBE_CARACTER(const char caracter) {
    TXREG = caracter;
    while(TXSTAbits.TRMT==0);
}

char SER_READ(void) {
    while(PIR1bits.RCIF==0);
    if(RCSTAbits.OERR ==1) {
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }
    PIR1bits.RCIF=0;
    return RCREG;
}

void NEXT_LINE(void) {
    TXREG = 0x0A;
    while(TXSTAbits.TRMT==0);
    TXREG = 0x0D;
    while(TXSTAbits.TRMT==0);
}

void EUSART_config(void){
    SPBRGH = 0;                 
    SPBRG = 8;                 
    TRISCbits.RC6 = 0;
    TRISCbits.RC7 = 1;  
    RCSTAbits.SPEN = 1;        
    TXSTAbits.TXEN = 1;
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCONbits.BRG16 = 0;
    //HABILITANDO RECEPCION
    RCSTAbits.RC9 = 0;  
    RCSTAbits.CREN = 1;         
}   
