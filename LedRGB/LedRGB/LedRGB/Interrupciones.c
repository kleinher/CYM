#define F_CPU 16000000
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>

#include "serialPort.h"

#include "main.h"
extern volatile int ProcesarInstruccion;
extern volatile int newData;
extern char BufferRX[32];
extern volatile int OCR0_PB5;
extern volatile int PWM_PB5;
extern volatile int newData;
/*Configuración del timer 0*/
void setupTimer(){
	TCCR0B=0x02; // modo CTC
	TCCR0A=(1<<CS02)|(1<<CS00); //configurar el registro del timer0 como temporizador con prescalador de 1024
	OCR0A=255;					//el registro empieza con valor 255
	TIMSK0|= (1<<OCIE0A);	    //habilita la interrupcion por desbordamiento del timer0
}

void setupPines(){
	DDRB |= (1<<1)|(1<<2)|(0<<5);
}

void setupADC(){
	ADCSRA = 0;
	ADCSRB = 0;
	ADMUX |= (1 << REFS1); //set reference voltage
	ADMUX |= (1 << ADLAR); //left align the ADC value- so we can read highest 8 bits from ADCH register only //
	ADCSRA |= (1 << ADPS2); //prescalador ADC 8
	ADCSRA |= (1 << ADATE); //enabble auto trigger
	ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
	ADCSRA |= (1 << ADEN); //enable ADC
	ADCSRA |= (1 << ADSC); //start ADC measurements
}

/*
*Rutina de atención de interrupción Timer
*/
ISR(TIMER0_COMPA_vect)
{	
	static int flag=0;
	if(PWM_PB5==1){
		if(flag==0){
			flag=1;
			//PORTB &= ~(1<<5); // invertido
			PORTB |= (1<<5); // invertido
			OCR0A=OCR0_PB5;
		}
		else {
			flag=0;
			//PORTB |= (1<<5); // no invertido
			PORTB &= ~(1<<5); // invertido
			OCR0A=(255-OCR0_PB5);
		}
	}
}

/*
*Rutina de atención de interrupción Terminal serie
*/
ISR(USART_RX_vect){
	volatile char RX_Data = 0;
	static short int Index=0;
	
	RX_Data = UDR0;				//Obtengo la información del terminal
	if(RX_Data != '\r'){		//si no es un salto de linea entonces almaceno caracter en el buffer
		BufferRX[Index++] = RX_Data;
	}
	else{						//Si es un salto de linea entonces
		BufferRX[Index]='\0';	//Determino el final del string
		ProcesarInstruccion = true; //Activo flag para atención de instrucción
		Index=0;				//Vuelvo el indice al principio del vector
	}
}

ISR(ADC_vect) {//when new ADC value ready
	newData = ADCH;//get value from A0
}