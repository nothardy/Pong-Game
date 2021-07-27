#define RS PORTBbits.RB6
#define RW PORTBbits.RB5
#define E PORTBbits.RB4
#define DB4 PORTBbits.RB3
#define DB5 PORTBbits.RB2
#define DB6 PORTBbits.RB1
#define DB7 PORTBbits.RB0

void invertir(unsigned char entrada)
{
    DB4= entrada & 0b00000001;
    DB5= (entrada & 0b00000010)>>1;
    DB6= (entrada & 0b00000100)>>2;
    DB7= (entrada & 0b00001000)>>3;      
}

void envio(void)
{
    RS=1;
    RW=0;
    __delay_ms(1);
    E=1;
    __delay_us(2);
    E=0;
}

void inicializacion(void)
{    __delay_ms(30);
    RS=0;
    RW=0;
    DB7=0;
    DB6=0;
    DB5=1;
    DB4=1;
    E=1;
    __delay_ms(5);
    E=0;
    __delay_ms(5); // 4.1us o más
    RS=0;
    RW=0;
    DB7=0;
    DB6=0;
    DB5=1;
    DB4=1;
    E=1;
    __delay_ms(5);
    E=0;
    __delay_ms(5); // 100us o más
    RS=0;
    RW=0;
    DB7=0;
    DB6=0;
    DB5=1;
    DB4=0;
    E=1;
    __delay_ms(5);
    E=0;
    __delay_ms(5); //BF
    RS=0;
    RW=0;
    DB7=0;
    DB6=0;
    DB5=1;
    DB4=0;
    E=1;
    __delay_ms(5);
    E=0;
    RS=0;
    RW=0;
    DB7=1;      //fijar numero de lineas usamos una linea
    DB6=1;       //fijar numero de Caracteres usamos tamaño 5X10 puntos
    E=1;
    __delay_ms(5);
    E=0;
    __delay_ms(5);
    RS=0;
    RW=0;
    DB7=0;
    DB6=0;
    DB5=0;
    DB4=0;
    E=1;
    __delay_ms(5);
    E=0;
    RS=0;
    RW=0;
    DB7=1;      
    DB6=0;
    DB5=0;
    DB4=0;
    E=1;
    __delay_ms(5);
    E=0;
    __delay_ms(5); //BF
    RS=0;
    RW=0;
    DB7=0;
    DB6=0;
    DB5=0;
    DB4=0;
    E=1;
    __delay_ms(5);
    E=0;
    RS=0;
    RW=0;
    DB7=0;      
    DB6=0;
    DB5=0;
    DB4=1;
    E=1;
    __delay_ms(5);
    E=0;
    __delay_ms(5); //BF
    RS=0;
    RW=0;
    DB7=0;
    DB6=0;
    DB5=0;
    DB4=0;
    E=1;
    __delay_ms(5);
    E=0;
    RS=0;
    RW=0;
    DB7=1;      
    DB6=1;
    DB5=1;          //incremento luego de escritura
    DB4=0;          //corre cursor luego del ingreso de datos
    E=1;
    __delay_ms(5);
    E=0;
}


void bajar(void)
{
    RW=0;
    RS=0;
    DB7=1;
    DB6=1;
    DB5=0;
    DB4=0;
    E=1;
    __delay_ms(5);
    E=0;
    RW=0;
    RS=0;
    DB7=0;
    DB6=0;
    DB5=0;
    DB4=0;
    E=1;
    __delay_ms(5);
    E=0;
}

void subir(void)
{
    RW=0;
    RS=0;
    DB7=1;
    DB6=0;
    DB5=0;
    DB4=0;
    E=1;
    __delay_ms(5);
    E=0;
    RW=0;
    RS=0;
    DB7=0;
    DB6=0;
    DB5=0;
    DB4=0;
    E=1;
    __delay_ms(5);
    E=0;
}

void LCD(unsigned char *p1)
{
    unsigned char letraenvio=0;
    while(*p1!=0)
    {
        letraenvio = *p1;
        letraenvio= letraenvio>>4;  //corro el vector 4 lugares a la izquierda 
        invertir(letraenvio); //muestro en puerto B
        envio();
        letraenvio=*p1 & 0x0F;
        invertir(letraenvio); //muestro en puerto B
        envio();
        p1++;
    }
}

void LCD2(unsigned char *p2)
{
    unsigned char letraenvio=0;
    bajar();
    while(*p2!=0)
    {
        letraenvio = *p2;
        letraenvio= letraenvio>>4;  //corro el vector 4 lugares a la izquierda 
        invertir(letraenvio); //muestro en puerto B
        envio();
        letraenvio= *p2 & 0x0F;
        invertir(letraenvio); //muestro en puerto B
        envio();
        p2++;
    }
    subir();
}


void Limpiardisplay(void)
{
    RS=0;
    RW=0;
    DB7=0;
    DB6=0;
    DB5=0;
    DB4=0;
    E=1;
    __delay_ms(5);
    E=0;
    RS=0;
    RW=0;
    DB7=0;
    DB6=0;
    DB5=0;
    DB4=1;
    E=1;
    __delay_ms(5);
    E=0;
}

void backspace(void)
{
    //muevo cursor hacia la izq
    RW=0;
    RS=0;
    DB7=0;
    DB6=0;
    DB5=0;
    DB4=1;
    E=1;
    __delay_ms(5);
    E=0;
    RW=0;
    RS=0;
    DB7=0;
    DB6=0;
    DB5=0;
    DB4=0;
    E=1;
    __delay_ms(5);
    E=0;
    // escribo espacio en blanco
    RS=1;
    RW=0;
    DB7=1;
    DB6=0;
    DB5=1;
    DB4=0;
    E=1;
    __delay_ms(5);
    E=0;
    RS=1;
    RW=0;
    DB7=0;
    DB6=0;
    DB5=0;
    DB4=0;
    E=1;
    __delay_ms(5);
    E=0;
    //muevo cursor hacia la izq
    RW=0;
    RS=0;
    DB7=0;
    DB6=0;
    DB5=0;
    DB4=1;
    E=1;
    __delay_ms(5);
    E=0;
    RW=0;
    RS=0;
    DB7=0;
    DB6=0;
    DB5=0;
    DB4=0;
    E=1;
    __delay_ms(5);
    E=0;
}