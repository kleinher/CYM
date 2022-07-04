/*
 * ATmega16_DHT11_Project_File.c
 *
 * http://www.electronicwings.com
 */ 
# define F_CPU 16000000UL
#include "sensorHumedad.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
static uint8_t I_RH,D_RH,I_Temp,D_Temp,CheckSum;
static uint8_t c=0;
/*Señal del MCU para el sensor de humedad */
void Request()				
{
	DDRC |= (1<<DHT11_PIN);		// seteo el pin como salida
	PORTC &= ~(1<<DHT11_PIN);	// Pone en 0 la salida del pin
	_delay_ms(25);				// mantiene la salida en 1 por el tiempo necesario para el sensor
	PORTC |= (1<<DHT11_PIN);	// Pone en 1 la salida del pin
	_delay_us(40);				// Mantiene la salida en 1 por el tiempo necesario para el sensor 
}
/* Captura de respuesta del sensor de humedad */
void Response()				
{
	DDRC &= ~(1<<DHT11_PIN);		   //Seteo el pin como entrada
	while(PINC & (1<<DHT11_PIN));      //Captura el 1 del pin de entrada
	while((PINC & (1<<DHT11_PIN))==0); //Captura el 0 del pin de entrada que envía el sensor
	while(PINC & (1<<DHT11_PIN));	   //Captura el 1 del pin de entrada que envía el sensor
}
/* Recepción de información */
uint8_t Receive_data()			
{	
	for (int q=0; q<8; q++)
	{
		while((PINC & (1<<DHT11_PIN)) == 0);  //Espero a recibir 1
		_delay_us(30);						  //Espero 30us
		if(PINC & (1<<DHT11_PIN))			//En el caso que el terminal siga en 1
			c = (c<<1)|(0x01);				//Entonces la información es 1 
		else								//En el caso que tengo un 0 en el terminal
			c = (c<<1);						//Entonces la información es 0
		while(PINC & (1<<DHT11_PIN));		//Espero que la salida del terminal vuelva a ser 0 
	}
	return c;
}
/*
*Actualización de los valores de humedad recibidos por el sensor
*/
void updateHumidity(){
	char data[5];
	cli();					//Desactivo interrupciones para evitar perder información del sensor
	Request();				//Aviso al sensor que quiero recibir información
	Response();				//Capturo la respuesta del sensor
	I_RH=Receive_data();	// Almaceno los primeros 8 bits de humedad
	D_RH=Receive_data();	// Almaceno los segundos 8 bits de humedad
	I_Temp=Receive_data();	// Almaceno los primeros 8 bits de temperatura
	D_Temp=Receive_data();	// Almaceno los segundos 8 bits de temperatura
	CheckSum=Receive_data();// Alamaceno los 8 bits de checksum
	sei();					//Vuelvo a activar interrupciónes luego de procesar toda la información del sensor
	if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum)		//Suma de los valores de entrada para comprobar el valor del checksum
	{
		SerialPort_Send_String("ERROR EN EL SENSOR DE HUMEDAD!!!!");	//En el caso de error en la recepción de información informo el error por la termina serie
	}
	else
	{
		itoa(I_RH,data,10);						//convierte la parte alta de la humedad(parte entera) en string
		SerialPort_Send_String("Humedad: ");	//Imprime Humedad
		SerialPort_Send_String(data);			//Imprime la parte entera convertida a string
		SerialPort_Send_String(".");
		itoa(D_RH,data,10);						//Convierte la parte baja de la humedad(parte decimal)en string
		SerialPort_Send_String(data);			//imprime la parte decimal convertida a string
		SerialPort_Send_String("\r\n");				
		SerialPort_Send_String("Temperatura: ");
		itoa(I_Temp,data,10);					//Convierte la parte alta de la temperatura(parte entera)en string	
		SerialPort_Send_String(data);			//imprime la parte entera convertida a string
		SerialPort_Send_String(".");
		itoa(D_Temp,data,10);					//Convierte la parte baja de la temperatura(parte decimal)en string
		SerialPort_Send_String(data);			//imprime la parte decimal convertida a string
		SerialPort_Send_String("\r\n");
		itoa(CheckSum,data,10);					//Convierte la parte alta(parte decimal)en string
	}
}
void procesarEntrada(){
	if(strcmp((char *) BufferRX,"ON") ==0){    //Verifica si el comando es "ON"
		ON = true;
	}
	else if(strcmp((char *) BufferRX,"OFF") ==0){ //Verifica si el comando es "OFF"
		ON = false;
	}
	else if(strcmp((char *) BufferRX,"RST") ==0){	//Verifica si el comando es "RST"
		RST = true;
	}
	else {										//En el caso de que no es ninguno de las opciones validas, imprime comando invalido
		sprintf((char *) BufferTX,"%s","Comando no valido\n\r");
		SerialPort_Send_String(BufferTX);
	}
}