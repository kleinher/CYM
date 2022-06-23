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
int main(void)
{
	checkSensorHumidity = true;
	setupTimer();
	SerialPort_Init(103);
	SerialPort_TX_Enable();
	SerialPort_RX_Enable();
	displayMenu();
	while(1)
	{
		char arreglo[4] ="";
		int i = 0;
		int cont = 1;
		char test = SerialPort_Recive_Data();
		while(test != '\r' && i < 4){
			arreglo[i] = test;
			if(cont == 1 ){
					SerialPort_Send_Data(test);
					cont = 0;
			}
			
			 i++;
			 SerialPort_Wait_Until_New_Data();
			 test = SerialPort_Recive_Data();
			 
		}
		
		
		if(checkSensorHumidity){
			updateHumidity();
			checkSensorHumidity = false;
		}
	}

}

