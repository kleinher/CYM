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
uint8_t KEYPAD_scan (uint8_t *);
uint8_t KEYPAD_Update (uint8_t *);
void actualizar_MEF();
void iniciar_MEF();

void salida(uint8_t,uint8_t,uint8_t);
void actualizarTiempo();
void actualizarCampo(char,uint8_t);
void imprimir();
void efecto_Apagado();
void relojFuncion();





volatile uint8_t actualizar=0;
	
	
const uint8_t filas[4] =   {0b00010000,0b00001000,0b00000001,0b10000000};
const uint8_t columna[4] = {0b00001000,0b00100000,0b00010000,0b00000100};
const char codChar[4][4] = {{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'0','*','#','D'}};


#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 16000000UL // Especifico la frecuencia de reloj del MCU en 8MHz
#include <util/delay.h>


#endif /* MAIN_H_ */