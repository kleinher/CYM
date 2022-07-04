/*
* main.c
*
* Created: 6/20/2022 5:43:35 PM
*  Author: Hernan
*/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include "main.h"
volatile int checkSensorHumidity;
volatile int ProcesarInstruccion;
char BufferRX[32];
char BufferTX[32];
static int ON = false;
static int RST = false;
int main(void)
{
	setupTimer();
	setupSerialPort(103);
	displayMenu(); //Rutina que imprime el menu en la terminal serie
	while(1)
	{
		if(RST){			// Lógica de reset
			RST = false;
			ON = false;
			displayMenu();
		}
		if(ProcesarInstruccion){ //Lógica de proceso de comando del usuario
			ProcesarInstruccion = false;
			procesarEntrada();
			
		}
		if(checkSensorHumidity && ON){ //Lógica de registro de temperatura/humedad
			updateHumidity();
			checkSensorHumidity = false;
		}
	}

}


