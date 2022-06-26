/*
 * ATmega16_DHT11_Project_File.c
 *
 * http://www.electronicwings.com
 */ 
# define F_CPU 16000000UL
#define DHT11_PIN 6
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
 uint8_t I_RH,D_RH,I_Temp,D_Temp,CheckSum;
 uint8_t c=0;

void Request()				/* Microcontroller send start pulse/request */
{
	DDRD |= (1<<DHT11_PIN);
	_delay_ms(20);	
	PORTD &= ~(1<<DHT11_PIN);	/* set to low pin */
	_delay_ms(20);			/* wait for 20ms */
	PORTD |= (1<<DHT11_PIN);	/* set to high pin */
	_delay_us(40);	
}

void Response()				/* receive response from DHT11 */
{
	DDRD &= ~(1<<DHT11_PIN);
	while(PIND & (1<<DHT11_PIN));//Limpia el 1 de la salida nuestra
	
	while((PIND & (1<<DHT11_PIN))==0); //limpia el 1 del sensor
	
	while(PIND & (1<<DHT11_PIN));//limpia el 0 del sensor
}

uint8_t Receive_data()			/* receive data */
{	
	for (int q=0; q<8; q++)
	{
		while((PIND & (1<<DHT11_PIN)) == 0);  /* check received bit 0 or 1 */
		_delay_us(30);
		if(PIND & (1<<DHT11_PIN))/* if high pulse is greater than 30ms */
		c = (c<<1)|(0x01);	/* then its logic HIGH */
		else			/* otherwise its logic LOW */
		c = (c<<1);
		while(PIND & (1<<DHT11_PIN));
	}
	return c;
}

void updateHumidity(){
	char data[5];
	
	 Request();
	 /* Microcontroller send start pulse/request */
	 Response();		// receive response
	 I_RH=Receive_data();	// store first eight bit in I_RH
	 D_RH=Receive_data();	// store next eight bit in D_RH
	 I_Temp=Receive_data();	// store next eight bit in I_Temp
	 D_Temp=Receive_data();	// store next eight bit in D_Temp
	 CheckSum=Receive_data();// store next eight bit in CheckSum
	 
	 if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum)
	 {
		 SerialPort_Send_String("ERROR EN EL SENSOR DE HUMEDAD!!!!");
	 }
	 else
	 {
		 itoa(I_RH,data,10);
		 SerialPort_Send_String("Humedad: ");
		 SerialPort_Send_String(data);
		 SerialPort_Send_String(".");
		 itoa(D_RH,data,10);
		 SerialPort_Send_String(data);
		 SerialPort_Send_String("\r\n");
		 SerialPort_Send_String("Temperatura: ");
		 itoa(I_Temp,data,10);
		 SerialPort_Send_String(data);
		 SerialPort_Send_String(".");
		 itoa(D_Temp,data,10);
		 SerialPort_Send_String(data);
		 SerialPort_Send_String("\r\n");
		 itoa(CheckSum,data,10);
	
	 }
	 }
