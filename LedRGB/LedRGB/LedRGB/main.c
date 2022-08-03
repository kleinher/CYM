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
void setupTimer();
void setupPines();
void setupADC();
void iniciar_MEF();
void actualizar_MEF();
void procesarEntrada(int*,int);
void pwm(int pin,int num);
void intensidad();

typedef enum{S0,S1,S2,S3,S4,S5,S6} state;
state estado;

volatile int ProcesarInstruccion=0;
volatile uint16_t newData=0;
char data[5];
char BufferRX[32];
char BufferTX[32];
volatile int ProcesarInstruccion;
int RGB[3] = {0,0,1};
int cont = 0;
volatile int OCR0_PB5=0;
volatile int PWM_PB5=0;
volatile int pwmPB5=1;

int main(void)
{
	setupTimer();
	setupPines();
	setupSerialPort(103);
	setupADC();
	sei();
    /* Replace with your application code */
	iniciar_MEF();
	while (1) 
    {
		actualizar_MEF();
		intensidad();
    }
}

void pwm(int pin,int num){
	TCCR1B |= (1<<WGM12)|(1<<CS12)|(1<<CS10);//prescalar /1024
	switch(pin){
		case 'R':
			if(num>250)
				PORTB|=(1<<5);
			else {TCCR1A |= (1<<WGM10)|(1<<COM1A1)|(1<<COM1A0);//fast pwm, inverted
			OCR1A=num;}
		break;
		case 'G':
			if(num>250)
				PORTB|=(1<<5);
			else {TCCR1A |= (1<<WGM10)|(1<<COM1B1)|(1<<COM1B0);//fast pwm, inverted
			OCR1B=num;}
		break;
		case 'B':
			if(num < 8){
				DDRB &= ~(1<<5);
			}
			else{
				if (num > 244)
				{
					DDRB |= (1<<5);
					PORTB |= (1<<5);
				}
				DDRB |= (1<<5);
				PWM_PB5=1;
				OCR0_PB5=num;
			}
			
		break;
	}
}

void iniciar_MEF(){
	estado=S0;
	//PWM manual entre 7 y 248 anda joya (simulador)
	pwm('R',0);
	pwm('G',0);
	pwm('B',0);
}

void actualizar_MEF(){
	static int condicion;
	switch (estado){
		case S0: SerialPort_Send_String("Ingrese un valor entre 0 y 255 para R:\n\r"); estado=S1;
		break;
		case S1:
			if(ProcesarInstruccion){
				ProcesarInstruccion=0;
				procesarEntrada(&condicion,0);
				if(condicion){
					estado=S2;
				}
				else{
					estado=S0;
				}
			}
		break;
		case S2: SerialPort_Send_String("Ingrese un valor entre 0 y 255 para G:\n\r"); estado=S3;
		break;
		case S3:
			if(ProcesarInstruccion){
				ProcesarInstruccion=0;
				procesarEntrada(&condicion,1);
				if(condicion){
					estado=S4;
				}
				else{
					estado=S2;
				}
			}
		break;
		case S4: SerialPort_Send_String("Ingrese un valor entre 0 y 255 para B:\n\r"); estado=S5;
		break;
		case S5:
			if(ProcesarInstruccion){
				ProcesarInstruccion=0;
				procesarEntrada(&condicion,2);
				if(condicion){
					estado=S6;
				}
				else{
					estado=S4;
				}
			}
		break;
		case S6: pwm('R',RGB[0]); pwm('G',RGB[1]); pwm('B',RGB[2]); estado=S0;
		break;
	}
}

void intensidad(){


	int test = (newData * 100)/(255);
	/*
	int ocrR= -1*(((RGB[0]/100)*256)-255);
	int ocrG= -1*(((RGB[1]/100)*256)-255);
	int ocrB= -1*(((RGB[2]/100)*256)-255);
	int porcentajeR = test * ocrR;
	int porcentajeG = test * ocrG;
	int porcentajeB = test * ocrB;
	int finalR = (porcentajeR*255)/100;
	int finalG = (porcentajeG*255)/100;
	int finalB = (porcentajeB*255)/100;
	*/
	int finalR=RGB[0]*test/100;
	int finalG=RGB[1]*test/100;
	int finalB=RGB[2]*test/100;
	
	if(finalR>=255 || finalG>=255 || finalB>=255)
		return 0;
	pwm('R',finalR);
	pwm('G',finalG);
	pwm('B',finalB);
}

void procesarEntrada(int *condicion, int cont){

	int num = atoi((char *) BufferRX);
	SerialPort_Send_String(BufferRX);
	SerialPort_Send_String("\n\r");
	if (num>=0 && num <= 255){    //Verifica el rango valido
		RGB[cont] = num;
		*condicion=true;
	}
	else {						//En el caso de que no es ninguno de las opciones validas, devuelve 1
		SerialPort_Send_String("El numero debe estar entre 0 y 255\n\r");
		*condicion=false;
	}
}