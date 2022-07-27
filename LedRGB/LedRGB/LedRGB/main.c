/*
 * LedRGB.c
 *
 * Created: 25/7/2022 23:38:05
 * Author : Administrator
 */ 
#define F_CPU 16000000
#define RAND_MAX 255
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include "SerialPort.h"

void pantalla();
void setupPines();
void setupADC();
void pwm(int pin,int num);

volatile int ProcesarInstruccion=0;
volatile int newData=0;

char BufferRX[32];
char BufferTX[32];

int main(void)
{
	setupPines();
	setupSerialPort(103);
	setupADC();
	sei();
    /* Replace with your application code */
	
	int R=50;
	int G=50;
	int B=200;
	pantalla();
	while (1) 
    {
		if(newData<256){
			pwm('R',R+newData);
			pwm('G',G+newData);
			pwm('B',B+newData);
		}
    }
}

void pantalla(){
	static int valor=1;
	if(valor==1){
		SerialPort_Send_String("R: ");
		valor=2;
	}
	if(valor==2){
		SerialPort_Send_String("G: ");
		valor=3;
	}
	if(valor==3){
		SerialPort_Send_String("B: ");
		valor=1;
	}
}
 
void setupPines(){
	DDRB |= (1<<1)|(1<<2)|(1<<3);
}

void setupADC(){
	ADCSRA = 0; 
	ADCSRB = 0; 
	ADMUX |= (1 << REFS1); //set reference voltage 
	ADMUX |= (1 << ADLAR); //left align the ADC value- so we can read highest 8 bits from ADCH register only //
	ADCSRA |= (1 << ADPS1); //prescalador ADC 8
	ADCSRA |= (1 << ADATE); //enabble auto trigger 
	ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete 
	ADCSRA |= (1 << ADEN); //enable ADC 
	ADCSRA |= (1 << ADSC); //start ADC measurements
}

void pwm(int pin,int num){
	TCCR1B |= (1<<CS11)|(1<<CS10);//prescalar /64
	TCCR2B |= (1<<CS22)|(1<<CS20);//prescalar /64
	switch(pin){
		case 'B':
			TCCR1A |= (1<<WGM12)|(1<<WGM10)|(1<<COM1A1)|(1<<COM1A0);//fast pwm, inverted
			OCR1A=num;
		break;
		case 'G':
			TCCR1A |= (1<<WGM12)|(1<<WGM10)|(1<<COM1B1)|(1<<COM1B0);//fast pwm, inverted
			OCR1B=num;
		break;
		case 'R':
			TCCR2A |= (1<<WGM21)|(1<<WGM20)|(1<<COM2A1)|(1<<COM2A0);//fast pwm, inverted
			OCR2A=num;
		break;
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
		ProcesarInstruccion = 1; //Activo flag para atención de instrucción
		Index=0;				//Vuelvo el indice al principio del vector
	}
}

ISR(ADC_vect) {//when new ADC value ready 
	newData = ADCH;//get value from A0 
}
