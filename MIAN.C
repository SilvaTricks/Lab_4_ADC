//******************************************************************************
//   UNIVERSIDAD DEL VALLE DE GUATEMALA
//   IE2023 PROGRAAMACIÓN DE MICROCONTROLADORES 
//   AUTOR: JORGE SILVA
//   COMPILADOR: XC8 (v1.41), MPLAB X IDE (v6.00)
//   PROYECTO: LABORATORIO 4, CONTADOR
//   HARDWARE: PIC16F887
//   CREADO: 10/10/2022
//   ÚLTIMA MODIFCACIÓN: 18/10/2022
//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT
#pragma config WDTE = OFF       
#pragma config PWRTE = ON      
#pragma config MCLRE = OFF      
#pragma config CP = OFF        
#pragma config CPD = OFF        
#pragma config BOREN = OFF      
#pragma config IESO = OFF       
#pragma config FCMEN = OFF     
#pragma config LVP = OFF       

// CONFIG2
#pragma config BOR4V = BOR40V   
#pragma config WRT = OFF
//******************************************************************************
#include <xc.h>

#define _XTAL_FREQ 4000000

void setup(void);
void setupADC(void);
void main(void);

unsigned int unidades = 0;
unsigned int decimales = 0;
unsigned char displayCC[] = {0b00111111, 0b00000110, 0b01011011, 0b01001111,
0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111, 0b01110111,
0b01111100, 0b00111001, 0b01011110, 0b01111001, 0b01110001};

//******************************************************************************
// Código Principal
//******************************************************************************
void main(void) {
    
    setup();
    setupADC();
    
    while(1){ 
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO == 1);
        ADIF = 0;
        PORTC = ADRESH;   //Mostramos valor del ADC en puerto c
        __delay_ms(10);

        if(PORTEbits.RE0 == 1){
            //INCREMENTAR, WHILE PARA ANTIRREBOTE
            while(RE0 == 1){}
            PORTB++;
             //SI HACE OVERFLOW, SE REINICIA
            if (PORTB>=257){
                PORTB = 0;
            }
        }
        if(PORTEbits.RE1 == 1){
            while(RE1 == 1){}
            PORTB--;
             //SI HACE OVERFLOW, SE REINICIA
            if (PORTB<0){
                PORTB = 256;
            }
        }
            
        if(PORTC > PORTB){
            PORTEbits.RE2 = 1;
        }
        
        if(PORTC < PORTB){
            PORTEbits.RE2 = 0;
        }
    }
}
//******************************************************************************
// Función para configurar GPIOs
//******************************************************************************
void setup(void){
    //PONEMOS PINES DIGITALES
    ANSEL = 0;
    ANSELH = 0;
    
    //PUERTO B, C, Y D COMO SALIDA
    TRISB = 0;
    PORTB = 0;
    TRISC = 0;
    PORTC = 0;
    TRISD = 0;
    PORTD = 0;
    
    //PINES 0 Y 1 DEL PUERTO E COMO ENTRADAS Y EL 2 COMO SALIDA
    TRISEbits.TRISE0 = 1;
    TRISEbits.TRISE1 = 1;
    TRISEbits.TRISE2 = 0;
    PORTE = 0;
    
    
}
//******************************************************************************
// Función para configurar ADC
//******************************************************************************
void setupADC(void){
    
    // Paso 1 Seleccionar puerto de entrada
    //TRISAbits.TRISA0 = 1;
    TRISA = TRISA | 0x01;
    ANSEL = ANSEL | 0x01;
    
    // Paso 2 Configurar módulo ADC
    
    ADCON0bits.ADCS1 = 0;
    ADCON0bits.ADCS0 = 1;       // Fosc/ 8
    
    ADCON1bits.VCFG1 = 0;       // Ref VSS
    ADCON1bits.VCFG0 = 0;       // Ref VDD
    
    ADCON1bits.ADFM = 0;        // Justificado hacia izquierda
    
    ADCON0bits.CHS3 = 0;
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS0 = 0;        // Canal AN0
    
    ADCON0bits.ADON = 1;        // Habilitamos el ADC
    __delay_us(100);
}
