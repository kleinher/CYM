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
void iniciar_Referencia();

typedef enum{S0,S1,S2,S3,S4,S5,S6} state;
state estado;

volatile int pote=0;
char data[5];
char BufferRX[32];
char BufferTX[32];
volatile int ProcesarInstruccion;
int RGB[3] = {0,0,0};
int cont = 0;
volatile int OCR0_PB5=0;
volatile int PWM_PB5=0;
volatile int pwmPB5=1;
volatile int flagPote=0;
volatile int referencia=0;
volatile int flagDebug=0;

int main(void)
{
	setupPrescaler();
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
void setupPrescaler(){
	TCCR1B |= (1<<WGM12)|(1<<CS12)|(1<<CS10); 
}
void pwm(int pin,int num){

	switch(pin){
		case 'R':
			if(num<8){
				TCCR1A &= ~(1<<COM1A1);
				TCCR1A &= ~(1<<COM1A0);
				PORTB |= (1<<1);
			}
			else{
				TCCR1A |= (1<<WGM10)|(1<<COM1A1)|(1<<COM1A0);//fast pwm, inverted
				OCR1A = num;
			}
		break;
		case 'G':
			if(num<8){
				TCCR1A &= ~(1<<COM1B1);
				TCCR1A &= ~(1<<COM1B0);
				PORTB |= (1<<2);
			}
			else{
				TCCR1A |= (1<<WGM10)|(1<<COM1B1)|(1<<COM1B0);//fast pwm, inverted
				OCR1B = num;
			}
		break;
		case 'B':
			PWM_PB5 = 1;
			OCR0_PB5 = num;
		break;
	}
}

void iniciar_MEF(){
	estado = S0;
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
				procesarEntrada(&condicion,0);
				ProcesarInstruccion = 0;
				if(condicion){
					estado = S2;
				}
				else{
					estado = S0;
				}
			}
		break;
		case S2: SerialPort_Send_String("Ingrese un valor entre 0 y 255 para G:\n\r"); estado=S3;
		break;
		case S3:
			if(ProcesarInstruccion){
				procesarEntrada(&condicion,1);
				ProcesarInstruccion=0;
				if(condicion){
					estado = S4;
				}
				else{
					estado = S2;
				}
			}
		break;
		case S4: SerialPort_Send_String("Ingrese un valor entre 0 y 255 para B:\n\r"); estado=S5;
		break;
		case S5:
			if(ProcesarInstruccion){
				procesarEntrada(&condicion,2);
				ProcesarInstruccion=0;
				if(condicion){
					estado = S6;
				}
				else{
					estado = S4;
				}
			}
		break;
		case S6: pwm('R',RGB[0]); pwm('G',RGB[1]); pwm('B',RGB[2]); estado = S0;
		break;
	}
}

void intensidad(){
	int potenciometro = (pote * 100)/(255);     // Valor porcentual potenciometro
	int finalR=(int)(RGB[0]*potenciometro/100); // Valor de intensidad led Rojo, acorde a la intencidad del potenciometro
	int finalG=(int)(RGB[1]*potenciometro/100); // Valor de intensidad led Verde, acorde a la intencidad del potenciometro
	int finalB=(int)(RGB[2]*potenciometro/100); // Valor de intensidad led Azul, acorde a la intencidad del potenciometro
	
	pwm('R',finalR); //Seteo de los valores PWM para el led rojo
	pwm('G',finalG); //Seteo de los valores PWM para el led verde
	pwm('B',finalB); //Seteo de los valores PWM para el led azul
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
