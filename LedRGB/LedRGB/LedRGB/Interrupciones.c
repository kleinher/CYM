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
/*Configuración del timer 0*/
void setupTimer(){
	TCCR0B=(1<<CS02)|(1<<CS00); //configurar el registro del timer0 como temporizador con prescalador de 1024
	TCNT0=99;					//el registro empieza con valor 99
	TIMSK0|= (1<<TOIE0);	    //habilita la interrupcion por desbordamiento del timer0
}
/*
*Rutina de atención de interrupción Timer
*/
ISR(TIMER0_OVF_vect)
{
	uint8_t static cont=0;
	char data[5];
	cont++;					//Contador para llegar a interrupciones cada 1 seg
	TCNT0=99;				//Reinicio contador del timer0
	if(cont==100){			//Condición de tiempo requerida (1 seg)
		cont=0;				//reinicio contador
		
		SerialPort_Send_String(itoa(newData,data,10));
		SerialPort_Send_String("\n\r");
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