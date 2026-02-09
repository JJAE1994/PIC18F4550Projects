
/*El Sensor ultrasonico HC-SR04 trabaja a una frecuencia de 40KHz, mientras que el oido humano esta entre el rango de 20Hz a 20KHz.
 Rango de medición de 2 cm a 400 cm.
 Velocidad del sonido 340 m/s >>> 0.034cm/us   >>>>> (20*C).
 Formula: d=v*t  >>> Distancia = Velocidad del sonido*Tiempo de ida
 Distancia = (0.034cm/us)*(Tiempo/2)
 Distancia = 0.017*Tiempo de ida. */

#include <pic18f4550.h>

#include "configuracion.h"

#define Pin_Trig_aum    LATAbits.LATA0    // Asignamos el nombre "Pin_Trig_aum" al pin RA0 (salida).
#define Pin_Echo_aum    PORTAbits.RA1     // Asignamos el nombre "Pin_Echo_aum" al pin RA1 (entrada).
#define Pin_Trig_des    LATAbits.LATA2    // Asignamos el nombre "Pin_Trig_des" al pin RA0 (salida).
#define Pin_Echo_des    PORTAbits.RA3     // Asignamos el nombre "Pin_Echo_des" al pin RA1 (entrada).

unsigned int centena, decena, unidad;
unsigned int display[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
/*==========================================================================================================
 ===========================================================================================================*/
void Configuracion_Registros (void);  // Función para configurar registros de inicio.
void EUSART_config(void);  // Función para configurar eusart 37600 bps.
void convierte(unsigned int numero);
void Timer1_Init (void);              // Función para inicializar el Timer 1.
uint16_t Obtener_Distancia_Aum (void);    // Función para obtener la distancia que aumenta la cuenta.
uint16_t Obtener_Distancia_Des (void);    // Función para obtener la distancia que reduce la cuenta..
void SER_ESCRIBE_CADENA(const char *cadena, unsigned int tam);
void SER_ESCRIBE_CARACTER(const char caracter);
char SER_READ(void);
void NEXT_LINE(void);


/*==========================================================================================================
 ===========================================================================================================*/
void main(void)                       // Función Principal.
{
    unsigned int Distancia_aum, Distancia_des;
    unsigned int aforo=0, estado=0, digitos=0;
    unsigned int recepcion[10];
    unsigned int recibido=100;
    unsigned int i;
    Configuracion_Registros();        // Llamamos a la función de configuración de registros.
    EUSART_config();                //configuracion inicial del eusart
    Timer1_Init();                    // Inicializamos la configuración del Timer1.
    SER_ESCRIBE_CADENA("Ingrese el aforo maximo (entre 20 y 150): ",42);
    //BUCLE PARA PEDIR AFORO MAXIMO
    while(estado==0) {
        recepcion[digitos]=SER_READ();        
        if(recepcion[digitos]==' ') {//cuando se haya insertado un espacio            
            if(digitos==2) {//si se insertaron dos digitos
                recibido=10*(recepcion[0]-0x30)+(recepcion[1]-0x30);
                estado=1;
            }else if(digitos==3) {//si se insertaron tress digitos
                recibido=100*(recepcion[0]-0x30)+10*(recepcion[1]-0x30)+(recepcion[2]-0x30);
                estado=1;
            }else{//caso contrario
                estado=0;
                digitos=0;
                for(i=0;i<digitos;i++) {
                    recepcion[i]=0;
                }            
            }            
        }else{
            SER_ESCRIBE_CARACTER(recepcion[digitos]);
            //SER_ESCRIBE_CARACTER(digitos+0x30);
            digitos++;
        }
        if((recibido<20)|| (recibido >150)) {
                estado=0;
                digitos=0;
                for(i=0;i<digitos;i++) {
                    recepcion[i]=0;
                }     
                recibido=100;
        }
    }
    while(1)
    {
        Distancia_aum=Obtener_Distancia_Aum();// Cargamos la variable "Distancia_aum" con el valor de distancia capturado por el primer sensor HC-SR04.
        Distancia_des=Obtener_Distancia_Des();// Cargamos la variable "Distancia_des" con el valor de distancia capturado por el segundo sensor HC-SR04.
        if(Distancia_aum<50) {//si es menor que 50
            if(aforo<recibido) {//si es menor que el aforo
                aforo++;
                LATCbits.LATC0=0;
            }else{//de lo contrario se activa alarma
                LATCbits.LATC0=1;
            }
            
        }
        if(Distancia_des<50) {//si es menor que 50
            if(aforo>0) {//si es menor que el aforo
                aforo--;
                LATCbits.LATC0=0;
            }
        }
        convierte(Distancia_aum);
        SER_ESCRIBE_CADENA("SENSOR1: ",9);
        SER_ESCRIBE_CARACTER(centena+0x30);
        SER_ESCRIBE_CARACTER(decena+0x30);
        SER_ESCRIBE_CARACTER(unidad+0x30);
        NEXT_LINE();
        convierte(Distancia_des);
        SER_ESCRIBE_CADENA("SENSOR2: ",9);
        SER_ESCRIBE_CARACTER(centena+0x30);
        SER_ESCRIBE_CARACTER(decena+0x30);
        SER_ESCRIBE_CARACTER(unidad+0x30);
        NEXT_LINE();
        convierte(aforo);
        //MULTIPLEXANDO DISPLAYS
        LATD=0x00;
        LATB=0x00;
        LATB=display[unidad];
        LATD=0x01;
        __delay_ms(10);
        LATD=0x00;
        LATB=0x00;
        LATB=display[decena];
        LATD=0x02;
        __delay_ms(10);
        LATD=0x00;
        LATB=0x00;
        LATB=display[centena];
        LATD=0x04;
        __delay_ms(10);

    }
    return;
}

/*==========================================================================================================
 ===========================================================================================================*/
void Configuracion_Registros (void)   //Función para configurar registros de inicio. 
{
    ADCON1bits.PCFG=0b1111;           // Deshabilitamos las entradas analógicas de los puerto A y B
    TRISAbits.RA0 = 0;                   // Configuramos el pin RA0 y RA2 como salida digital.
    TRISAbits.RA2 = 0;
    TRISAbits.RA1 = 1;                   // Configuramos el pin RA0 y RA2 como salida digital.
    TRISAbits.RA3 = 1;                   // Configuramos el pin RA1 y RA3 como entrada digital.
    TRISB=0x00;
    TRISCbits.RC0 = 0;
    TRISDbits.RD0=0;
    TRISDbits.RD1=0;
    TRISDbits.RD2=0;
    LATD=0x00;
    LATB=0x00;
    LATCbits.LATC0=0;
}

void Timer1_Init (void)               // Función de configuración del Timer 1.
{
    T1CONbits.RD16=1;                 // Timer1 trabajando a 16 bits.
    T1CONbits.T1CKPS=0b00;            // Timer1  Pre-escaler=0.
    T1CONbits.TMR1CS=0;               // Internal clock (Fosc/4).
    TMR1=0;                           // Cargamos el registro TMR1 con el valor de 0.
    TMR1ON=0;                         // Temporizador Timer1 detenido.
}

uint16_t Obtener_Distancia_Aum (void) // Función para obtener la distancia.
{
    uint16_t Duracion;         
    uint16_t Distancia;
    uint16_t Timer_1;
    
    Pin_Trig_aum=1;                       // Ponemos a nivel alto el pin RA0.
    __delay_us(10);                   // Retardo de 10 us.
    Pin_Trig_aum=0;                       // Ponemos a nivel bajo el pin RA0.
    while(Pin_Echo_aum==0);               // Esperamos mientras el pin RA1 sea igual a 0.
    T1CONbits.TMR1ON=1;               // Activamos el temporizador del Timer1.
    while(Pin_Echo_aum==1);               // Esperamos mientras el pin RA1 sea igual a 1.
    T1CONbits.TMR1ON=0;               // Detenemos el temporizador del Timer1.
    Timer_1=TMR1;                     // Cargamos la variable "Timer_1" con el valor del registro TMR1. (Tiempo en us)
    Duracion=Timer_1/2;               // Cargamos el valor de la variable "Timer_1"/2 (debido que con 8MHz se generan 0.5us) en la variable "Duracion".
    Distancia=Duracion/58;            // Valor de la distancia en cm. (formula aplicada para us)
    TMR1=0;                           // Reiniciamos el valor del registro TMR1.
    return Distancia;                 // Retornamos el valor de distancia.
}

uint16_t Obtener_Distancia_Des (void) // Función para obtener la distancia.
{
    uint16_t Duracion;         
    uint16_t Distancia;
    uint16_t Timer_1;
    
    Pin_Trig_des=1;                       // Ponemos a nivel alto el pin RA0.
    __delay_us(10);                   // Retardo de 10 us.
    Pin_Trig_des=0;                       // Ponemos a nivel bajo el pin RA0.
    while(Pin_Echo_des==0);               // Esperamos mientras el pin RA1 sea igual a 0.
    T1CONbits.TMR1ON=1;               // Activamos el temporizador del Timer1.
    while(Pin_Echo_des==1);               // Esperamos mientras el pin RA1 sea igual a 1.
    T1CONbits.TMR1ON=0;               // Detenemos el temporizador del Timer1.
    Timer_1=TMR1;                     // Cargamos la variable "Timer_1" con el valor del registro TMR1. (Tiempo en us)
    Duracion=Timer_1/2;               // Cargamos el valor de la variable "Timer_1"/2 (debido que con 8MHz se generan 0.5us) en la variable "Duracion".
    Distancia=Duracion/58;            // Valor de la distancia en cm. (formula aplicada para us)
    TMR1=0;                           // Reiniciamos el valor del registro TMR1.
    return Distancia;                 // Retornamos el valor de distancia.
}

void EUSART_config(void){
    SPBRGH = 0;                 
    SPBRG = 8; //37600 bps                
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

void convierte(unsigned int numero){
    centena = (numero % 1000) / 100;
    decena =  (numero % 100) / 10;
    unidad = numero % 10;
}

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

