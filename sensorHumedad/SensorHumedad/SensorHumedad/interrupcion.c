/*
 * interrupcion.c
 *
 * Created: 6/20/2022 8:23:36 PM
 *  Author: Hernan
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "main.h"
extern volatile int checkSensorHumidity;

void setupTimer(){
	TCCR0B=(1<<CS02)|(1<<CS00); //configurar el registro del timer0 como temporizador con prescalador de 1024
	TCNT0=99;					//el registro empieza con valor 99
	TIMSK0|= (1<<TOIE0);	    //habilita la interrupcion por desbordamiento del timer0
	sei();				        //habilita interrupciones globales
}

ISR(TIMER0_OVF_vect)
{
	uint8_t static cont=0;
	cont++;
	TCNT0=99;//reinicio contador del timer0
	if(cont==100){
		checkSensorHumidity = false;
		cont=0;//reinicio contador
	}
	
}