/*
 * Reloj.c
 *
 * Created: 25/4/2022 14:54:59
 * Author : Barcala
 */  

/* Inclusión de cabeceras de bibliotecas de código */

#include "main.h"

int main(void)
{
    /* Replace with your application code */
	setupTimer();
	setupLCD();
	iniciar_MEF();
	while (1) 
    {
		actualizar_MEF();
    }
}
/*SETUP DE LA PANTALLA LCD*/
void setupLCD(){
	LCDinit();
	LCDclr();
	LCDcursorOFF();

}

void setupTimer(){
	DDRB=0b00100000;//PB5 como salida
	TCCR0B=(1<<CS02)|(1<<CS00);//configurar el registro del timer0 como temporizador con prescalador de 1024
	TCNT0=99;//el registro empieza con valor 99
	TIMSK0|= (1<<TOIE0);//habilita la interrupcion por desbordamiento del timer0
	sei();//habilita interrupciones globales
}

ISR(TIMER0_OVF_vect)
{
	uint8_t static cont=0;
	cont++;
	if(cont==50){
		relojFuncion();
		TCNT2=99;//reinicio contador del timer0
		cont=0;//reinicio contador
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
	PORTD=0b00111100;
	
	for(int c=0;c<4;c++){
		//poner en cero las filas
		DDRD &= ~(0b10000000);
		DDRB &= ~(0b00011001);
		
		if(c==3) DDRD |= filas[c];
		else     DDRB |= filas[c];
		
		for(int r=0; r<4; r++){
			if(!(PIND & columna[r])){
				*key = codChar[c][r];
				_delay_ms(500);
				return(1);
			}
		}
	}
	return(0);
}