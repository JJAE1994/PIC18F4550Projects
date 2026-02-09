#include "config.h"
#include "eusart.h"
#include "reloj.h"
#include "lcd.h"
#include <xc.h>
#include <stdint.h>
#include <pic18f4550.h>

void GPIO_Init (void);

void main(void)                 
{   
    char pass[] = {'1','9','9','4'};//CONTRASEÑA
    unsigned int recepcion[4];//ARREGLO PARA INGRESAR DIGITOS
    unsigned int i;
    unsigned int pass_correct=1;
    GPIO_Init();
    EUSART_config();
    LCD_Init_4_Bits();
    
    while(1)
    {    
        LCD_Tx_Command(LCD_CLEAR); 
        LCD_Goto_XY(1,1);
        LCD_Put_String("    INGRESAR    ");
        LCD_Goto_XY(1,2);
        LCD_Put_String("   CONTRASENA   ");
        __delay_ms(150);
        for(i=0;i<4;i++) {
            recepcion[i]=SER_READ();
            SER_ESCRIBE_CARACTER(recepcion[i]);
            if(pass_correct==1) {//COMPARANDO DIGITOS INGRESADOS CON CONTRASEÑA
                if(recepcion[i]!= pass[i]) {
                    pass_correct=0;
                }
            }
        }    
        SER_ESCRIBE_CADENA("\n\r",2);
        if(pass_correct==0) {
            LCD_Tx_Command(LCD_CLEAR); 
            LCD_Goto_XY(1,1);
            LCD_Put_String("   CONTRASENA   ");
            LCD_Goto_XY(2,1);
            LCD_Put_String("   INCORRECTA   ");
            LATBbits.LATB1 = 1;
            __delay_ms(2000);
            LATBbits.LATB1 = 0;
        }else{
            LCD_Tx_Command(LCD_CLEAR); 
            LCD_Goto_XY(1,1);
            LCD_Put_String("   CONTRASENA   ");
            LCD_Goto_XY(2,1);
            LCD_Put_String("    CORRECTA    ");
            LATBbits.LATB0 = 1;
            SER_ESCRIBE_CADENA("hola\n\r",6);
            __delay_ms(3000);
            LATBbits.LATB0 = 0;            
        }     
    }
    return;
}

void GPIO_Init (void)  
{
    TRISBbits.RB0 = 0;//RB0 como salida
    TRISBbits.RB1 = 0;//RB1 como salida  
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
}
