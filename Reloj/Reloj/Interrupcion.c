/*
 * Interrupcion.c
 *
 * Created: 5/9/2022 12:06:11 AM
 *  Author: Hernan
 */ 
#include <avr/io.h>		
#include <avr/interrupt.h>

void setupTimer(){
	DDRB=0b00100000;//PB5 como salida
	TCCR0B=(1<<CS02)|(0<<CS00);//configurar el registro del timer0 como temporizador con prescalador de 1024
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
