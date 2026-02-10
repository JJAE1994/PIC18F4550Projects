#include "config.h"
#include "eusart.h"
#include <pic18f4550.h>

unsigned int millar, centena, decena, unidad;
unsigned char display[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
/*==========================================================================================================
 ===========================================================================================================*/
void Configuracion_Registros (void);  // Función para configurar registros de inicio.
void convierte(unsigned int numero);

/*==========================================================================================================
 ===========================================================================================================*/
void main(void)                       // Función Principal.
{   
    unsigned int aforo=0;
    unsigned int state=0;
    unsigned int cifras=0;
    unsigned int recepcion[10];
    unsigned int recibido=100;
    unsigned int i;
    Configuracion_Registros();        // Llamamos a la función de configuración de registros.
    EUSART_config();
    SER_ESCRIBE_CADENA("\n\r",2);
    SER_ESCRIBE_CADENA("MALL DEL SUR",12);
    __delay_ms(1000);
    SER_ESCRIBE_CADENA("\n\r",2);

    SER_ESCRIBE_CADENA("INGRESAR AFORO MAX. <20,150> : ",31);
    
    while(state==0) {
        recepcion[cifras]=SER_READ();        
        if(recepcion[cifras]=='-') {            
            if(cifras==2) {
                recibido=10*(recepcion[0]-0x30)+(recepcion[1]-0x30);
                state=1;
            }else if(cifras==3) {
                recibido=100*(recepcion[0]-0x30)+10*(recepcion[1]-0x30)+(recepcion[2]-0x30);
                state=1;
            }else{
                state=0;
                cifras=0;
                for(i=0;i<cifras;i++) {
                    recepcion[i]=0;
                }            
            }            
        }else{
            SER_ESCRIBE_CARACTER(recepcion[cifras]);
            //SER_ESCRIBE_CARACTER(digitos+0x30);
            cifras++;
        }
        if((recibido<20)|| (recibido >150)) {
                state=0;
                cifras=0;
                for(i=0;i<cifras;i++) {
                    recepcion[i]=0;
                }     
                recibido=100;
        }
    }
    SER_ESCRIBE_CADENA("\n\r",2);
    while(1)
    {    

        if(PORTAbits.RA0 ==1 && PORTAbits.RA1 ==0) {
              LATEbits.LE0 = 1;
              __delay_us(1000);
              LATEbits.LE0 = 0;
              __delay_us(19000);
              SER_ESCRIBE_CADENA("PUEDE PASAR  ...\n\r",18);
              convierte(aforo);
              LATBbits.LATB7=1;
              LATDbits.LATD0=1;
              LATDbits.LATD1=1;
            while(PORTAbits.RA0 ==1){
                LATB=display[unidad];
                LATBbits.LATB7=1;
                LATDbits.LATD1=0;
                __delay_ms(1);
                LATDbits.LATD0=1;
                LATDbits.LATD1=1;
                LATB=display[decena];
                LATBbits.LATB7=1;
                LATDbits.LATD0=0;
                __delay_ms(1);
                LATBbits.LATB7=1;
                LATDbits.LATD0=1;
                LATDbits.LATD1=1;       
                LATB=display[centena];
                LATBbits.LATB7=0;
                __delay_ms(1);
                LATBbits.LATB7=1;
                LATDbits.LATD0=1;
                LATDbits.LATD1=1;
            }                       
            if(aforo<recibido) {
                aforo++;
            }else{
                SER_ESCRIBE_CADENA("AFORO AL LIMITE!\n\r",18);
            }            
        }
        
        if(PORTAbits.RA2 ==1 && PORTAbits.RA3 ==0) {
              LATEbits.LE1 = 1;
              __delay_us(1000);
              LATEbits.LE1 = 0;
              __delay_us(19000);
              convierte(aforo);
                LATBbits.LATB7=1;
                LATDbits.LATD0=1;
                LATDbits.LATD1=1;
            while(PORTAbits.RA2 ==1){

                LATB=display[unidad];
                LATBbits.LATB7=1;
                LATDbits.LATD1=0;
                __delay_ms(1);
                LATDbits.LATD0=1;
                LATDbits.LATD1=1;
                LATB=display[decena];
                LATBbits.LATB7=1;
                LATDbits.LATD0=0;
                __delay_ms(1);
                LATBbits.LATB7=1;
                LATDbits.LATD0=1;
                LATDbits.LATD1=1;       
                LATB=display[centena];
                LATBbits.LATB7=0;
                __delay_ms(1);
                LATBbits.LATB7=1;
                LATDbits.LATD0=1;
                LATDbits.LATD1=1;
            }                       
            if(aforo>0) {
                aforo--;
            }    
        }
        
        if(PORTAbits.RA1 ==1) {
              SER_ESCRIBE_CADENA("ESPERE SU TURNO\n\r",17);
              LATEbits.LE0 =1; // se pone a 90 grados EL SERVO
              __delay_us(1500);
              LATEbits.LE0=0;
              __delay_us(18500);
              convierte(aforo);
                LATBbits.LATB7=1;
                LATDbits.LATD0=1;
                LATDbits.LATD1=1;
              while(PORTAbits.RA1 ==1){
                LATB=display[unidad];
                LATBbits.LATB7=1;
                LATDbits.LATD1=0;
                __delay_ms(1);
                LATDbits.LATD0=1;
                LATDbits.LATD1=1;
                LATB=display[decena];
                LATBbits.LATB7=1;
                LATDbits.LATD0=0;
                __delay_ms(1);
                LATBbits.LATB7=1;
                LATDbits.LATD0=1;
                LATDbits.LATD1=1;       
                LATB=display[centena];
                LATBbits.LATB7=0;
                __delay_ms(1);
                LATBbits.LATB7=1;
                LATDbits.LATD0=1;
                LATDbits.LATD1=1;
            }                       
              
        }
        if(PORTAbits.RA3 ==1) {
            LATEbits.LE1 =1; // se pone a 90 grados EL SERVO
            __delay_us(1500);
            LATEbits.LE1=0;
            __delay_us(18500);
            convierte(aforo);
            LATBbits.LATB7=1;
            LATDbits.LATD0=1;
            LATDbits.LATD1=1;
             while(PORTAbits.RA3 ==1){
                LATB=display[unidad];
                LATBbits.LATB7=1;
                LATDbits.LATD1=0;
                __delay_ms(1);
                LATDbits.LATD0=1;
                LATDbits.LATD1=1;
                LATB=display[decena];
                LATBbits.LATB7=1;
                LATDbits.LATD0=0;
                __delay_ms(1);
                LATBbits.LATB7=1;
                LATDbits.LATD0=1;
                LATDbits.LATD1=1;       
                LATB=display[centena];
                LATBbits.LATB7=0;
                __delay_ms(1);
                LATBbits.LATB7=1;
                LATDbits.LATD0=1;
                LATDbits.LATD1=1;
            }                       
        }
                LATBbits.LATB7=1;
                LATDbits.LATD0=1;
                LATDbits.LATD1=1;
                convierte(aforo);
                LATB=display[unidad];
                LATBbits.LATB7=1;
                LATDbits.LATD1=0;
                __delay_ms(1);
                LATDbits.LATD0=1;
                LATDbits.LATD1=1;
                LATB=display[decena];
                LATBbits.LATB7=1;
                LATDbits.LATD0=0;
                __delay_ms(1);
                LATBbits.LATB7=1;
                LATDbits.LATD0=1;
                LATDbits.LATD1=1;       
                LATB=display[centena];
                LATBbits.LATB7=0;
                __delay_ms(1);
                LATBbits.LATB7=1;
                LATDbits.LATD0=1;
                LATDbits.LATD1=1;


    }
    return;
}

/*==========================================================================================================
 ===========================================================================================================*/
void Configuracion_Registros (void)   //Función para configurar registros de inicio. 
{
    ADCON1bits.PCFG=0b1111;           // Deshabilitamos las entradas analógicas de los puerto A y B
    TRISAbits.RA0 = 1;
    TRISAbits.RA1 = 1;
    TRISAbits.RA2 = 1;
    TRISAbits.RA3 = 1;
    TRISB=0x00;
    TRISD=0;
    TRISEbits.RE0 = 0;
    TRISEbits.RE1 = 0;
    LATB &= 0x80;
    LATD = 0x03;
    LATEbits.LATE0 = 0;
    LATEbits.LATE1 = 0;
}

void convierte(unsigned int numero){
    millar = numero/1000;
    centena = (numero % 1000) / 100;
    decena =  (numero % 100) / 10;
    unidad = numero % 10;
}
