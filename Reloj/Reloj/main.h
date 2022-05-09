/*
 * main.h
 *
 * Created: 2/5/2022 15:40:58
 *  Author: Barcala
 */ 
#include <avr/io.h>			// Definición de Registros del microcontrolador
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "lcd.h"


void setupLCD();
void setupTimer();

void actualizar_MEF();
void iniciar_MEF();

void salida(uint8_t,uint8_t,uint8_t);
void actualizarTiempo();
void actualizarCampo(char,uint8_t);
void imprimir();
void efecto_Apagado();
void relojFuncion();





volatile uint8_t actualizar=0;
	



#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 16000000UL // Especifico la frecuencia de reloj del MCU en 8MHz
#include <util/delay.h>


#endif /* MAIN_H_ */