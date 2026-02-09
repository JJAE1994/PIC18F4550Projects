#include "config.h"
#include "reloj.h"

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
    /*
     * Si se usaba modo low speed el error era de 1.73%, por ello
     * se uso el modo high speed que nos daba un error de 0.16&
     * X = ((10000000/9600)/16)-1=64.1-->64
    Baud Rate calculado = 10000000/16/(64+1) = 9615
     */
    SPBRGH = 0;                 
    SPBRG = 64;                 
    TRISCbits.RC6 = 0;//pin tx como salida
    TRISCbits.RC7 = 1;//pin rx como entrada  
    RCSTAbits.SPEN = 1;//Habilitamos puerto serial        
    TXSTAbits.TXEN = 1;//Habilitamos transmision
    TXSTAbits.SYNC = 0;//modo asincrono
    TXSTAbits.BRGH = 1;//high speed mode /16
    BAUDCONbits.BRG16 = 0;//8 bit baudrate generator
    //HABILITANDO RECEPCION
    RCSTAbits.RC9 = 0;  
    RCSTAbits.CREN = 1;         
}   
