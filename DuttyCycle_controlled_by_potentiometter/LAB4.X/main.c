
#include "reloj.h"
#include <stdint.h>
#include <xc.h>
#include "pic18f4550.h"
//#include <stdio.h>
#include "lcd.h"

volatile uint32_t valor_digital=0;
unsigned int digdec, diguni;

void ADC_init(void);
void Timer2_init(void);
void ISR_Init(void);
void CPP_Init(void);
void Extraer_Digitos_Entero(unsigned int num);

void main(void) {
   
   unsigned int ccp_value, dc;
   LCD_Init_4_Bits();
   ISR_Init();
   ADC_init();   
   CPP_Init();
   while(1)
   {  
   ADCON0bits.DONE=1;
   while(ADCON0bits.GO_nDONE==1);
   valor_digital = (ADRESH<<8)|ADRESL;
   ccp_value = 4+((valor_digital*94)/1023);//4 < CCPR2L < 98
   CCPR2L = ccp_value;   
   dc = (ccp_value*100)/(PR2 + 1);//CICLO DE TRABAJO
   Extraer_Digitos_Entero(dc);
   LCD_Tx_Command(LCD_CLEAR); 
   LCD_Goto_XY(1,1);
   LCD_Put_String("DC: ");
   LCD_Put_Char(digdec+0x30);
   LCD_Put_Char(diguni+0x30);
   LCD_Put_String("%");      
   __delay_ms(150);
   }
    return;
}

void ADC_init(void)
{
    ADCON0bits.ADON=0;//ADC deshabilitado
    //AN3, AN2, AN1, AN0 analogicos
    ADCON1bits.PCFG = 0b1011;
    //VREF+ = 5V, VREF-=0V
    ADCON1bits.VCFG0=0;
    ADCON1bits.VCFG1=0;
    //FOSC/32
    ADCON2bits.ADCS=0b010;
    //Adquisition Time 4TAD
    ADCON2bits.ACQT=0b010;
    //Justificado a la derecha
    ADCON2bits.ADFM=1;
    //Seleccionamos AN3
    ADCON0bits.CHS=0b0011;
    //ADC Habilitado
    ADCON0bits.ADON=1;
}

void Timer2_init(void){
    //Deshabilitamos el Timer2
    T2CONbits.TMR2ON = 0;
    //Prescaler /4
    T2CONbits.T2CKPS = 0b01;
    //Postcaler /1
    T2CONbits.TOUTPS = 0b0000;
    //TOP = 0.5ms/(1/(4MHz/4*Prescaler)) = 125
    PR2 = 125;
    //Habilitamos Timer2
    T2CONbits.TMR2ON = 1;

}

void CPP_Init(void) {
    /*     
     * 3 < DC < 77
     * 3 < (CCPR2L/(PR2 + 1)) *100 < 77
     * 3 < (CCPR2L/126) *100 < 77
     *3.78 < (CCPR2L/126) *100 < 97.02
     *4 < CCPR2L < 98
     */
    TRISCbits.RC1 = 0;//pin RC1 como salida
    Timer2_init();
    CCPR2L = 4;//ciclo de trabajo minimo
    CCP2CONbits.DC2B = 0b00;
    //Modo PWM
    CCP2CONbits.CCP2M3 = 1;
    CCP2CONbits.CCP2M2 = 1;
}


void ISR_Init(void) {
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
}

void Extraer_Digitos_Entero(unsigned int num){
        digdec = num / 10;
        num -= (digdec * 10);
        diguni = num;
}

 