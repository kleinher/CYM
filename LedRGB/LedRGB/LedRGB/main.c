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
#include "main.h"

void pantalla();
void setupPines();
void setupADC();
void pwm(int pin,int num);

volatile int ProcesarInstruccion=0;
volatile uint8_t newData=0;
char data[5];
char BufferRX[32];
char BufferTX[32];
volatile int ProcesarInstruccion;
int val[3] = {0,0,0};
int cont = 0;
int imprimir = true;
int main(void)
{
	setupTimer();
	setupPines();
	setupSerialPort(103);
	
	setupADC();
	sei();
    /* Replace with your application code */
	

	pantalla();
	while (1) 
    {
		if(imprimir){
		switch (cont){
			case 0:
			SerialPort_Send_String("Ingrese el valor R\n\r");
			imprimir = false;
			break;
			case 1:
			SerialPort_Send_String("Ingrese el valor G\n\r");
			imprimir = false;
			break;
			case 2:
			SerialPort_Send_String("Ingrese el valor B\n\r");
			imprimir = false;
			break;
		}
		}
		
		if(newData<255){
			if(!(val[0] == 0))
				pwm('R',val[0]+newData);
			if(!(val[1] == 0))
				pwm('G',val[1]);
			if(!(val[2] == 0))
				pwm('B',val[2]);
			//SerialPort_Send_String(itoa(R,data,10));
		}
		
		if(ProcesarInstruccion){ //Lógica de proceso de comando del usuario
			ProcesarInstruccion = false;
			procesarEntrada();
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


ISR(ADC_vect) {//when new ADC value ready 
	newData = ADCH;//get value from A0 
}

void procesarEntrada(int *aux){

	int num = atoi((char *) BufferRX);
	
	if (num <= 255){    //Verifica si el comando es "ON"
		val[cont] = num;
		imprimir =  true;
		cont = (cont + 1) % 3;
	}
	else {										//En el caso de que no es ninguno de las opciones validas, imprime comando invalido
		sprintf((char *) BufferTX,"%s","El numero debe estar entre 0 y 255\n\r");
		SerialPort_Send_String(BufferTX);
		imprimir =  true;
	}
}