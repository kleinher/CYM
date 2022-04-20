/*
 * teclado4x4.c
 *
 * Created: 11/4/2022 19:09:11
 * Author : Administrator
 */ 

/* Inclusión de cabeceras de bibliotecas de código */
#include <avr/io.h> // Definición de Registros del microcontrolador
#define F_CPU 8000000UL // Especifico la frecuencia de reloj del MCU en 8MHz
#include <util/delay.h> // Retardos por software – Macros: depende de F_CPU
#include "lcd.h"
uint8_t KEYPAD_scan (uint8_t *);
uint8_t KEYPAD_Update (uint8_t *);

int main(void)
{
    /* Replace with your application code */
    uint8_t key=0x00;
	uint8_t cod1[12]={' ','C','D','y','M',' ','P','r','u','e','b','a'};
	uint8_t cod2[7]={' ','T','e','c','l','a',':'};
	LCDinit();
	LCDclr();
	//LCDhome();
	LCDstring(cod1,12);
	LCDGotoXY(0,1);
	LCDstring(cod2,7);
	while (1) 
    {
		if(KEYPAD_Update (&key)){
			LCDsendChar(key);//se manda key al lcd
			LCDGotoXY(7,1);
			_delay_ms(50);
		}
		else
		{
			//no se apreto ninguna tecla
		}
    }
}
/********************************************************
FUNCION PARA ESCANEAR UN TECLADO MATRICIAL Y DEVOLVER LA
TECLA PRESIONADA UNA SOLA VEZ. TIENE DOBLE VERIFICACION Y
MEMORIZA LA ULTIMA TECLA PRESIONADA
DEVUELVE:
0 -> NO HAYNUEVA TECLA PRESIONADA
1 -> HAY NUEVA TECLA PRESIONADA Y ES *pkey
********************************************************/
uint8_t KEYPAD_Update (uint8_t *pkey)
{
	static uint8_t Old_key;
	uint8_t Key, Last_valid_key=0xFF; // no hay tecla presionada
	if(!KEYPAD_scan(&Key)) {
		Old_key=0xFF; // no hay tecla presionada
		Last_valid_key=0xFF;
		return 0;
	}
	if(Key==Old_key) { //2da verificación
		if(Key!=Last_valid_key){ //evita múltiple detección
			*pkey=Key;
			Last_valid_key = Key;
			return 1;
		}
	}
	Old_key=Key; //1era verificación
	return 0;
}

uint8_t KEYPAD_scan (uint8_t *key){
	uint8_t c,r;
	PORTD=0b01111111;
	//uint8_t codAscii[4][4] = {{0x31,0x32,0x33,0x41},{0x34,0x35,0x36,0x42},{0x37,0x38,0x39,0x43},{0x30,0x2A,0x23,0x44}};
	uint8_t codAscii[4][4] = {{0b110001,0b110010,0b110011,0b1000001},
							  {0b110100,0b110101,0b110110,0b1000010},
							  {0b110111,0b111000,0b111001,0b1000011},
							  {0b110000,0b101010,0b100011,0b1000100}};
	uint8_t filas[4] = {0b00010000,0b00001000,0b00000001,0b10000000};
	uint8_t columna[4] = {0b00001000,0b00100000,0b00010000,0b00000100};
	for(c=0;c<4;c++){
		//poner en cero las filas
		DDRD &= ~(0b10000000);
		DDRB &= ~(0b00011001);
		if(c==3){
			DDRD |= filas[c];
		}
		else{
			DDRB |= filas[c];
		}
		for(r=0; r<4; r++){
			if(!(PIND & columna[r])){
				*key=(codAscii[c][r]);
				return(1);
			}
		}
	}
	return(0);
}