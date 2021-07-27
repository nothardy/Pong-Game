/* 
 * File:   PongDobleMatriz.c
 * Author: julianpiniel
 *
 * Created on 1 de julio de 2018, 19:54
 */

#include <stdio.h>
#include <stdlib.h>
#define _XTAL_FREQ 20000000 // Indicamos a que frecuencia de reloj esta funcionando el micro
#include <xc.h> // Librer�a XC8
#include "LCD.h"

#define FCY (_XTAL_FREQ/4)      //Frecuencia de trabajo
// PIC16F887 Configuration Bit Settings
// CONFIG1
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//Asigno las salidas del pic a las entradas del LCD

#define chipenable PORTCbits.RC0
#define chipenable2 PORTCbits.RC7
#define Inicio1 PORTBbits.RB2
#define subir2 PORTBbits.RB0
#define bajar2 PORTBbits.RB1
#define subir1 PORTBbits.RB6
#define bajar1 PORTBbits.RB7
#define Inicio2 PORTBbits.RB5
unsigned char linea1[16],linea2[16],bufer[16],primerinicio=1,banderainicio1=0,banderainicio2=0,BanderaSprintf,saque=0, Contadorseg=0,seg=0,Contadormin=0,min=0, gol1=1,gol2=0,ledball=0x08,filaball=0x02,ledbarra1=0x08,ledbarra2=0x08,puntaje1=0,puntaje2=0,banderagol=0, Trayectoria=1;
unsigned char bandera1,bandera2,bandera3,bandera4,banderajuego;
unsigned char contador;
unsigned char tiempo=100;

//Prototipo de funciones
void InitES(void);
void InitTMR1(void);
void InitSPI(void);
void enviardato(unsigned char, unsigned char);
void enviardato22(unsigned char, unsigned char);
void enviardato2(unsigned char, unsigned char);
void enviardato1(unsigned char, unsigned char);
void enviardato0(unsigned char, unsigned char);
void Posicioninicial(void);
void Juego(void);
void trayectoria();
void Saque();
void Gol();
void Marcador();
void barras(unsigned char,unsigned char);
void moverbarra();
// Funcion de interrupciones
void interrupt rutinainterrupciones()
{
//    if(INTCONbits.RBIF==1){
//        INTCONbits.RBIF=0;
//        if(subir1==1){
//            bandera1=1;
//        }
//        if(bajar1==1){
//            bandera2=1;
//        }
//        if(subir2==1){
//            bandera3=1;
//        }
//        if(bajar2){
//            bandera4=1;
//        }
//    }
//    if(PIR1bits.TMR1IF==1){
//        PIR1bits.TMR1IF=0;
//        TMR1=62411;
//        Marcador();
//        
//        
//    }
    
    
    //unsigned char puertob=0;
    if(INTCONbits.RBIF==1 )
    {
       // Marcador();
        INTCONbits.RBIF=0;
        if (subir1==1)
        {
            __delay_ms(40);
            if(subir1==1)
            {
                //while(subir1==1);
                __delay_ms(5);
                //if(subir1==0)
                //{
                    if(ledbarra1!=0x40)
                    {
                        ledbarra1=ledbarra1*2;
                        barras(ledbarra1,ledbarra2);
                    }
                //}
            }
        }
        else if (bajar1==1)
        {
            __delay_ms(40);
            if(bajar1==1)
            {
                //while(bajar1==1);
                __delay_ms(5);
               // if(bajar1==0)
                //{
                    if(ledbarra1!=0x02)
                    {
                        ledbarra1=ledbarra1/2;
                        barras(ledbarra1,ledbarra2);
                    }
                //}
            }
        }
        else if (subir2==1)
        {
            __delay_ms(40);
            if(subir2==1)
            {
                //while(subir2==1);
                __delay_ms(5);
              //  if(subir2==0)
               // {
                    if(ledbarra2!=0x40)
                    {
                        ledbarra2=ledbarra2*2;
                        barras(ledbarra1,ledbarra2);
                    }
               // }
            }
        }
        else if (bajar2==1)
        {
            __delay_ms(40);
            if(bajar2==1)
            {
               // while(bajar2==1);
                __delay_ms(5);
             //   if(bajar2==0)
               // {
                    if(ledbarra2!=0x02)
                    {
                        ledbarra2=ledbarra2/2;
                        barras(ledbarra1,ledbarra2);
                    }
               // }
            }
        }
    }
//    if(PIR1bits.TMR1IF==1)
//    {
//        PIR1bits.TMR1IF=0;
//        Contadorseg++;
//        Contadormin++;
//        TMR1=3036;
//        if(Contadorseg==10)
//        {
//            Contadorseg=0;
//            seg++;
//        }
//        if(Contadormin==600)
//        {
//            Contadormin=0;
//            min++;
//        }
//    }
}



void main(void) 
{
  InitES();
  inicializacion();
  InitSPI();
  barras(ledbarra1,ledbarra2);
  enviardato(filaball,ledball);
  InitTMR1();
  while(1)
   {
      INTCONbits.RBIE=0;
      Posicioninicial();
      //Marcador();// ajusto donde arranca la barra
      if(Inicio1==1 || Inicio2==1 )
      {
          gol1=0;
          gol2=0;
        while(saque==0)
        {
            INTCONbits.RBIE=1;
            Juego();
            primerinicio=0;
        } 
        saque=0;
      }
      if(puntaje1==5  || puntaje2==5)
      {
          tiempo=70;
      }
      if(puntaje1==10 || puntaje2==10){
          
          INTCONbits.RBIE=0;
    
    enviardato0(0x01,0); //limpio pantallas
    enviardato0(0x02,0);
    enviardato0(0x03,0);
    enviardato0(0x04,0);
    enviardato0(0x05,0);
    enviardato0(0x06,0);
    enviardato0(0x07,0);
    enviardato0(0x08,0);
    
    //letra w
    enviardato2(0x07,0b01111110);
    enviardato2(0x06,0x02);
    enviardato2(0x05,0b00001110);
    enviardato2(0x04,0x02);
    enviardato2(0x03,0b01111110);
    
    //letra I
    enviardato2(0x01,0b01111110);
    enviardato(0x08,0);
    
    //letra N}
    enviardato(0x07,0b01111110);
    enviardato(0x06,0x20);
    enviardato(0x05,0x10);
    enviardato(0x04,0x08);
    enviardato(0x03,0x04);
    enviardato(0x02,0b01111110);
          
    //Marcador();
          while(1);
      }
   }
}

void InitES(void)
{
    ANSEL=0;
    ANSELH=0;
    TRISCbits.TRISC0=0;
    TRISCbits.TRISC3=0;
    TRISCbits.TRISC4=1;
    TRISCbits.TRISC5=0;
    TRISCbits.TRISC7=0;
    TRISB=0b11111111;
    PORTB=0;
    PORTC=0;
    chipenable=1;
    chipenable2=1;
    TRISD=0;
    PORTD=0;
}

void InitTMR1(void)
{
    T1CON=T1CON | 0x30;    //Pre escalador en 8
    T1CONbits.TMR1CS = 0;    //Se utiliza FCY para temporizaci�n
    T1CONbits.TMR1GE = 0;    //No se usa gate
    TMR1=62411;            //en 3036. Cuenta hasta 65535
    PIR1bits.TMR1IF = 0;     //Se baja bandera de interrupci�n por las dudas
    INTCONbits.GIE = 1;      //Se activan interrupciones globales
    INTCONbits.PEIE = 1;     //Se activan interrupciones de perifEricos
    INTCONbits.RBIE=1;
    INTCONbits.RBIF=0;
    PIE1bits.TMR1IE = 0;     //Se activa interrupci�n por desborde de timer 1
    T1CONbits.TMR1ON = 0;    //Se enciende timer 1
    IOCB=0b11000011;
}
void InitSPI(void)
{
//    SSPSTAT=0b00000000; //config proteus
//    SSPCON=0b11100010;
    SSPSTAT=0b11000000; //config placa
    SSPCON=0b00110010;
    //limpio la pantalla
    enviardato0(0x0C,0x00); // saco el shutdown mode
    enviardato0(0x01,0);
    enviardato0(0x02,0);
    enviardato0(0x03,0);
    enviardato0(0x04,0);
    enviardato0(0x05,0);
    enviardato0(0x06,0);
    enviardato0(0x07,0);
    enviardato0(0x08,0);
    //comienza0 config inicialde la matriz
    enviardato0(0x0B,0x07);// scan limit puesto a 8, lee todos los digitos
    enviardato0(0x09,0); // saco el decodificador bcd
    enviardato0(0x0C,0x01); // saco el shutdown mode
    enviardato0(0x0F,0); // corre en modo normal
    enviardato0(0x0A,0x0F);
    
    
    
    
}

void enviardato0(unsigned char direccion , unsigned char dato)
{
    chipenable=0;
    chipenable2=0;
    
    SSPBUF=direccion;
    while(SSPSTATbits.BF==0);
    SSPBUF=dato;
    while(SSPSTATbits.BF==0);
    __delay_ms(1);
    
    chipenable=1;
    chipenable2=1;
}

void enviardato(unsigned char direccion , unsigned char dato)
{
    chipenable=0;
    
    SSPBUF=direccion;
    while(SSPSTATbits.BF==0);
    SSPBUF=dato;
    while(SSPSTATbits.BF==0);
    __delay_ms(1);
    
    chipenable=1;
    

}

void enviardato22(unsigned char direccion , unsigned char dato) //ELIJO LA COLUMNA DE LA SEGUNDA MATRIZ
{   
    chipenable=0;
    
    SSPBUF=direccion;
    while(SSPSTATbits.BF==0);
    SSPBUF=dato;
    while(SSPSTATbits.BF==0);
    
    SSPBUF=0x00;
    while(SSPSTATbits.BF==0);
    SSPBUF=0x00;
    while(SSPSTATbits.BF==0);   
    
    chipenable=1;
}

void enviardato2(unsigned char direccion, unsigned char dato)
{
    chipenable2=0;
    
    SSPBUF=direccion;
    while(SSPSTATbits.BF==0);
    SSPBUF=dato;
    while(SSPSTATbits.BF==0);
    __delay_ms(1);
    
    chipenable2=1;
}

void enviardato1(unsigned char direccion , unsigned char dato) //ELIJO LA COLUMNA DE LA PRIMER MATRIZ
{   
    chipenable=0;
    
    SSPBUF=0x00;
    while(SSPSTATbits.BF==0);
    SSPBUF=0x00;
    while(SSPSTATbits.BF==0);  
    
    SSPBUF=direccion;
    while(SSPSTATbits.BF==0);
    SSPBUF=dato;
    while(SSPSTATbits.BF==0);
    __delay_ms(1);

    chipenable=1;
}
void Posicioninicial()
{
    INTCONbits.RBIE=0;
    if(gol1==1 || primerinicio==1) // poner interrupciones
    {
        if (subir1==1)
        {
            __delay_ms(20);
            if(subir1==1)
            {
                while(subir1==1);
                __delay_ms(20);
                if(subir1==0)
                {
                    if(ledbarra1!=0x40)
                    {
                        ledbarra1=ledbarra1*2;
                        barras(ledbarra1,ledbarra2);
                        ledball=ledbarra1;
                        enviardato(filaball,ledball);
                        
                    }
                        
                }
            }
        }
        if (bajar1==1)
        {
            __delay_ms(20);
            if(bajar1==1)
            {
                while(bajar1==1);
                __delay_ms(20);
                if(bajar1==0)
                {
                    if(ledbarra1!=0x02)
                    {
                        ledbarra1=ledbarra1/2;
                        barras(ledbarra1,ledbarra2);
                        ledball=ledbarra1;
                        enviardato(filaball,ledball);
                    }
                        
                }
            }
        }
    }
    if(gol2==1)
    {
        
        if (subir2==1)
        {
            __delay_ms(20);
            if(subir2==1)
            {
                while(subir2==1);
                __delay_ms(20);
                if(subir2==0)
                {
                    if(ledbarra2!=0x40)
                    {
                        ledbarra2=ledbarra2*2;
                        barras(ledbarra1,ledbarra2);
                        ledball=ledbarra2;
                        enviardato2(0x07,ledbarra2);
                    }
                        
                }
            }
        }
        if (bajar2==1)
        {
            __delay_ms(20);
            if(bajar2==1)
            {
                while(bajar2==1);
                __delay_ms(20);
                if(bajar2==0)
                {
                    if(ledbarra2!=0x02)
                    {
                        ledbarra2=ledbarra2/2;
                        barras(ledbarra1,ledbarra2);
                        ledball=ledbarra2;
                        enviardato2(0x07,ledbarra2);
                    }
                        
                }
            }
        }
    }
   
}

void Juego () // arreglar rebote de la barra
{
    
    while(Trayectoria==1 && banderagol==0) // trayectoria 1
    {
       // Marcador();

        if(filaball<0x08)//estoy en matriz 1
        {
            if(ledball!=0x80) // si no toca techo
            {
                barras(ledbarra1,ledbarra2);
                enviardato(filaball,0); //avanza
                ledball=ledball*2;
                filaball++;
                enviardato(filaball,ledball);
                __delay_ms(tiempo);
            }
            else // toca techo
            {
                Trayectoria=4;
                barras(ledbarra1,ledbarra2);
            }
        }
        else if (filaball==0x08){ // COLUMNA CRITICA
            if(ledball!=0x80) // si no toca techo
            {
                enviardato(filaball,0); //avanza
                ledball=ledball*2;
                filaball++;
                enviardato2(0x01,ledball);
                __delay_ms(tiempo);
            }
            else // toca techo
            {
                Trayectoria=4;
                barras(ledbarra1,ledbarra2);
            }
        }
        else // estoy en matriz 2
        {
            if(filaball!=0x10) //pregunto si no es gol
            {
                if(ledball!=0x80) // pregunto si es distinto del techo
                {
                    barras(ledbarra1,ledbarra2);
                enviardato(0x08,0); //avanza
                enviardato2((filaball-0x08),0);
                ledball=ledball*2;
                filaball++;
                enviardato2((filaball-0x08),ledball);
                __delay_ms(tiempo);
                }
                else
                {
                    Trayectoria=4;
                    barras(ledbarra1,ledbarra2);
                }
            }
            else
            {
                if(ledball==ledbarra2 || ledball==ledbarra2*2)
                {
                    Trayectoria=2;
                    barras(ledbarra1,ledbarra2);
                }
                else if (ledball==ledbarra2/2)
                {
                    Trayectoria=3;
                    barras(ledbarra1,ledbarra2);
                }
                else
                {
                    gol2=1;
                banderagol=1;
                puntaje1++;
                Gol();
                Marcador();
                }
            }
            
        }
    
    }
    while(Trayectoria==2&& banderagol==0) // trayectoria 2
    {
        // Marcador();

        if(filaball<=0x08)//estoy en matriz 1
        {
            if(filaball!=0x01) //pregunto si no es gol
            {
                if(ledball!=0x80) //pregunto si no es el techo
                {  
                    barras(ledbarra1,ledbarra2);
                enviardato2(0x01,0); // NO SE SI ESTO ESTA AL PEDO
                enviardato(filaball,0); //avanza
                ledball=ledball*2;
                filaball--;
                enviardato(filaball,ledball);
                __delay_ms(tiempo);
                }
                else
                {
                    Trayectoria=3;
                    barras(ledbarra1,ledbarra2);
                }
            }
            else // toco fila limite
            {
                if(ledball==ledbarra1 || ledball==ledbarra1*2)
                {
                    Trayectoria=1;
                    barras(ledbarra1,ledbarra2);
                }
                else if (ledball==ledbarra1/2)
                {
                    Trayectoria=4;
                    barras(ledbarra1,ledbarra2);
                }
                else
                {
                gol1=1;
                banderagol=1;
                puntaje2++;
                Gol();
                Marcador();
                }
            }
            
        }
        else if (filaball==0x09){ //COLUMNA CRITICA
            if(ledball!=0x80) //pregunto si no es el techo
                {  
                barras(ledbarra1,ledbarra2);
                enviardato2(0x01,0); //avanza
                enviardato(filaball,0); // NO SE SI ESTO ESTA AL PEDO
                ledball=ledball*2;
                filaball--;
                enviardato(0x08,ledball);
                __delay_ms(tiempo);
                }
                else
                {
                    Trayectoria=3;
                    barras(ledbarra1,ledbarra2);
                }
        }
        else // estoy en matriz 2
        {
            if(ledball!=0x80) // si no toca techo
            {
                barras(ledbarra1,ledbarra2);
                enviardato2(0x07,0);
                enviardato2(filaball-0x08,0);
                ledball=ledball*2;
                filaball--;
                enviardato2(filaball-0x08,ledball);
                __delay_ms(tiempo);
            }
            else // toca techo
            {
                Trayectoria=3;
                barras(ledbarra1,ledbarra2);
            }
        }
    }
    while(Trayectoria==3 && banderagol==0) // trayectoria 3
    {
       // Marcador();
        if(filaball<=0x08)//estoy en matriz 1
        {
            if(filaball!=0x01) // no toco fila limite
            {
                if(ledball!=0x01) // no toco piso
                {
                    barras(ledbarra1,ledbarra2);
                enviardato2(0x01,0); // NO SE SI ESTO ESTA AL PEDO
                enviardato(filaball,0);
                ledball=ledball/2;
                filaball--;
                enviardato(filaball,ledball);
                __delay_ms(tiempo);
                }
                else //toco piso
                {
                    Trayectoria=2;
                    barras(ledbarra1,ledbarra2);
                }
            }
            else // toco fila limite
            {
                if(ledball==ledbarra1 || ledball==ledbarra1/2)
                {
                    Trayectoria=4;
                    barras(ledbarra1,ledbarra2);
                }
                else if (ledball==ledbarra1*2)
                {
                    Trayectoria=1;
                    barras(ledbarra1,ledbarra2);
                }
                else
                {
                    gol1=1;
                banderagol=1;
                puntaje2++;
                Gol();
                Marcador();
                }
            }//
            
        }
        else if(filaball==0x09){
            if(ledball!=0x01) // no toco piso
                {
                barras(ledbarra1,ledbarra2);
                enviardato2(0x01,0); //avanza
                enviardato(filaball,0); // NO SE SI ESTO ESTA AL PEDO
                ledball=ledball/2;
                filaball--;
                enviardato(0x08,ledball);
                __delay_ms(tiempo);
                }
                else //toco piso
                {
                    Trayectoria=2;
                    barras(ledbarra1,ledbarra2);
                }
        }
        else // estoy en matriz 2
        {
            if(ledball!=0x01) // si no toca piso
            {
                barras(ledbarra1,ledbarra2);
                enviardato2(filaball-0x08,0); //avanza
                ledball=ledball/2;
                filaball--;
                enviardato2(filaball-0x08,ledball);
                __delay_ms(tiempo);
            }
            else // toca piso
            {
                Trayectoria=2;
                barras(ledbarra1,ledbarra2);
            }
        }
    }
    while(Trayectoria==4 && banderagol==0 ) // trayectoria 4
    {
       // Marcador();
        if(filaball<0x08)//estoy en matriz 1
        {
            if(ledball!=0x01) // si no toca piso
            {
                barras(ledbarra1,ledbarra2);
                enviardato(filaball,0); //avanza
                ledball=ledball/2;
                filaball++;
                enviardato(filaball,ledball);
                __delay_ms(tiempo);
            }
            else // toca techo
            {
                Trayectoria=1;
                barras(ledbarra1,ledbarra2);
            }
        }
        else if(filaball==0x08){
            if(ledball!=0x01) // si no toca piso
            {
                barras(ledbarra1,ledbarra2);
                enviardato(filaball,0); //avanza
                ledball=ledball/2;
                filaball++;
                enviardato2(0x01,ledball);
                __delay_ms(tiempo);
            }
            else // toca techo
            {
                Trayectoria=1;
                barras(ledbarra1,ledbarra2);
            }
        }
        else // estoy en matriz 2
        {
            if(filaball!=0x10)
            {
                if(ledball!=0x01)
                {   
                    barras(ledbarra1,ledbarra2);
                enviardato(0x08,0); //avanza
                enviardato2((filaball-0x08),0);
                ledball=ledball/2;
                filaball++;
                enviardato2((filaball-0x08),ledball);
                __delay_ms(tiempo);
                }
                else
                {
                    Trayectoria=1;
                    barras(ledbarra1,ledbarra2);
                }
            }
            else
            {
                if(ledball==ledbarra2 || ledball==ledbarra2*2)
                {
                    Trayectoria=2;
                    barras(ledbarra1,ledbarra2);
                }
                else if (ledball==ledbarra2/2)
                {
                    Trayectoria=3;
                    barras(ledbarra1,ledbarra2);
                }
                else
                {
                    gol2=1;
                banderagol=1;
                puntaje1++;
                Gol();
                Marcador();
                }
            }
            
        }
    
    }
    if(banderagol==1)
    {
        banderagol=0;
        Saque();
        saque=1;
    }
}

void Gol()
{
    INTCONbits.RBIE=0;
    
    enviardato0(0x01,0); //limpio pantallas
    enviardato0(0x02,0);
    enviardato0(0x03,0);
    enviardato0(0x04,0);
    enviardato0(0x05,0);
    enviardato0(0x06,0);
    enviardato0(0x07,0);
    enviardato0(0x08,0);
    
    //letra G
    enviardato2(0x07,0b01111110);
    enviardato2(0x06,0b01000010);
    enviardato2(0x05,0b01001010);
    enviardato2(0x04,0b01001110);
    enviardato2(0x03,0);
    
    //letra O
    enviardato2(0x02,0b01111110);
    enviardato2(0x01,0b01000010);
    enviardato(0x08,0b01000010);
    enviardato(0x07,0b01111110);
    
    //letra L
    enviardato(0x05,0b01111110);
    enviardato(0x04,0x02);
    enviardato(0x03,0x02);
    enviardato(0x02,0x02);
    
    __delay_ms(1500);
    INTCONbits.RBIE=0;
    // mostrar la frase gol, con display LCD, o en el led, y que se mueva
}

void Saque()
{
    ledbarra1=0x08;  //barras por defecto
    ledbarra2=0x08;
    enviardato0(0x01,0); //limpio pantallas
    enviardato0(0x02,0);
    enviardato0(0x03,0);
    enviardato0(0x04,0);
    enviardato0(0x05,0);
    enviardato0(0x06,0);
    enviardato0(0x07,0);
    enviardato0(0x08,0);
    if(gol1==1 && gol2==0)
    {
        filaball=0x02;
        ledball=0x08;
        Trayectoria=1;
        enviardato(filaball,ledball);
    }
    else if(gol1==0 && gol2==1)
    {
        filaball=0x0E;
        ledball=0x08;
        Trayectoria=2;
        
        enviardato2(0x07,ledball);
    }
    barras(ledbarra1,ledbarra2);
}

void Marcador()
{
    INTCONbits.RBIE=0;
    //muestra el marcador y el tiempo constantemente
//    sprintf(linea1,"     PONG     ");
//    sprintf(linea2,"%1u : %1u",puntaje1,puntaje2);
//    LCD(linea1);
//    LCD2(linea2);
    
    
    //DISPLAY 7 SEGMENTOS
            
//    PORTD=puntaje2 | 0b10000000;
//    __delay_ms(2);
//    
//    PORTD=puntaje1 | 0b00010000;
//    __delay_ms(2);
    
    enviardato0(0x01,0); //limpio pantallas
    enviardato0(0x02,0);
    enviardato0(0x03,0);
    enviardato0(0x04,0);
    enviardato0(0x05,0);
    enviardato0(0x06,0);
    enviardato0(0x07,0);
    enviardato0(0x08,0);
    enviardato(0x08,0x08);//escribo el guion
    enviardato2(0x01,0x08);//escribo el guion
    switch (puntaje1)
    {
        case 1:
            enviardato(0x05,0b01111110);
            break;
        case 2:
            enviardato(0x06,0b01000110);
            enviardato(0x05,0b01001010);
            enviardato(0x04,0b01010010);
            enviardato(0x03,0b01100010);
            break;
        case 3:
            enviardato(0x06,0b00101010);
            enviardato(0x05,0b00101010);
            enviardato(0x04,0b00101010);
            enviardato(0x03,0b01111110);
            break;
        case 4:
            enviardato(0x06,0b01111000);
            enviardato(0x05,0x08);
            enviardato(0x04,0x08);
            enviardato(0x03,0b01111110);
            break;
        case 5:
            enviardato(0x06,0b01110010);
            enviardato(0x05,0b01010010);
            enviardato(0x04,0b01010010);
            enviardato(0x03,0b01011110);
            break;
        case 6:
            enviardato(0x06,0b01111110);
            enviardato(0x05,0b01010010);
            enviardato(0x04,0b01010010);
            enviardato(0x03,0b01011110);
            break;
        case 7:
            enviardato(0x06,0x40);
            enviardato(0x05,0x40);
            enviardato(0x04,0x40);
            enviardato(0x03,0b01111110);
            break;
        case 8:
            enviardato(0x06,0b01111110);
            enviardato(0x05,0b01010010);
            enviardato(0x04,0b01010010);
            enviardato(0x03,0b01111110);
            break;
        case 9:
            enviardato(0x06,0b01110000);
            enviardato(0x05,0b01010000);
            enviardato(0x04,0b01010000);
            enviardato(0x03,0b01111110);
            break;
        case 10:
            enviardato(0x06,0b01111110);
            enviardato(0x04,0b01111110);
            enviardato(0x03,0b01000010);
            enviardato(0x02,0b01111110);
            break;
        default:;
            }
    switch (puntaje2)
    {
        case 1:
            enviardato2(0x05,0b01111110);
            break;
        case 2:
            enviardato2(0x06,0b01000110);
            enviardato2(0x05,0b01001010);
            enviardato2(0x04,0b01010010);
            enviardato2(0x03,0b01100010);
            break;
        case 3:
            enviardato2(0x06,0b00101010);
            enviardato2(0x05,0b00101010);
            enviardato2(0x04,0b00101010);
            enviardato2(0x03,0b01111110);
            break;
        case 4:
            enviardato2(0x06,0b01111000);
            enviardato2(0x05,0x08);
            enviardato2(0x04,0x08);
            enviardato2(0x03,0b01111110);
            break;
        case 5:
            enviardato2(0x06,0b01110010);
            enviardato2(0x05,0b01010010);
            enviardato2(0x04,0b01010010);
            enviardato2(0x03,0b01011110);
            break;
        case 6:
            enviardato2(0x06,0b01111110);
            enviardato2(0x05,0b01010010);
            enviardato2(0x04,0b01010010);
            enviardato2(0x03,0b01011110);
            break;
        case 7:
            enviardato2(0x06,0x40);
            enviardato2(0x05,0x40);
            enviardato2(0x04,0x40);
            enviardato2(0x03,0b01111110);
            break;
        case 8:
            enviardato2(0x06,0b01111110);
            enviardato2(0x05,0b01010010);
            enviardato2(0x04,0b01010010);
            enviardato2(0x03,0b01111110);
            break;
        case 9:
            enviardato2(0x06,0b01110000);
            enviardato2(0x05,0b01010000);
            enviardato2(0x04,0b01010000);
            enviardato2(0x03,0b01111110);
            break;
        case 10:
            enviardato2(0x07,0b01111110);
            enviardato2(0x05,0b01111110);
            enviardato2(0x04,0b01000010);
            enviardato2(0x03,0b01111110);
            break;
        default:;
            }
    __delay_ms(1500);
            
    
    
}

void barras(unsigned char posicionbarra1,unsigned char posicionbarra2)
{
    //barra 1
    unsigned char barrafinal1=0;
    barrafinal1=posicionbarra1+(2*posicionbarra1)+(posicionbarra1/2);
    enviardato(0x01,barrafinal1);
    //barra 2
    unsigned char barrafinal2=0;
     barrafinal2=posicionbarra2+(2*posicionbarra2)+(posicionbarra2/2);
    enviardato2(0x08,barrafinal2);
}

void moverbarra(){
    if(bandera1==1){
              bandera1=0;
              IOCB=0b11000001;
              while(subir1==1);
              ledbarra1=ledbarra1*2;
              barras(ledbarra1,ledbarra2);
              IOCB=0b11000011;;
          }
          else if(bandera2==1){
              bandera2=0;
              IOCB=0b11000010;
              while(bajar1==1);
              ledbarra1=ledbarra1/2;
              barras(ledbarra1,ledbarra2);
              IOCB=0b11000011;
          }
          else if(bandera3==1){
              bandera3=0;
              IOCB=0b01000011;
              while(subir2==1);
              ledbarra2=ledbarra2/2;
              barras(ledbarra1,ledbarra2);
              IOCB=0b11000011;
          }
          else{
              bandera4=0;
              IOCB=0b10000011;
              while(bajar2==1);
              ledbarra2=ledbarra2/2;
              barras(ledbarra1,ledbarra2);
              IOCB=0b11000011;
          }
}
