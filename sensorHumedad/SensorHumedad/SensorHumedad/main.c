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
  int ON = false;
  int RST = false;
int main(void)
{
	checkSensorHumidity = true;
	setupTimer();
	SerialPort_Init(103);
	SerialPort_TX_Enable();
	SerialPort_RX_Enable();
	SerialPort_RX_Interrupt_Enable();
	sei();
	displayMenu();
	while(1)
	{
		
		if(RST){
			RST = false;
			ON = false;
			displayMenu();
		}
		if(ProcesarInstruccion){
			ProcesarInstruccion = false;
			procesarEntrada();
			
		}
		if(checkSensorHumidity && ON){
			updateHumidity();
			checkSensorHumidity = false;
		}
	}

}
void procesarEntrada(){
	if(strcmp((char *) BufferRX,"ON") ==0){
		ON = true;
	}
	else if(strcmp((char *) BufferRX,"OFF") ==0){
		ON = false;
	}
	else if(strcmp((char *) BufferRX,"RST") ==0){
		RST = true;
	}
	else {
		sprintf((char *) BufferTX,"%s","Comando no valido\n\r");
		SerialPort_Send_String(BufferTX);
	}
}

