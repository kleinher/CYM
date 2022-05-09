/*
 * Reloj.h
 *
 * Created: 5/8/2022 11:54:47 PM
 *  Author: Hernan
 */ #include <avr/io.h>
#include "lcd.h"

typedef struct{
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char date;
	unsigned char month;
	unsigned char year;
}time;
volatile uint8_t FlagLcd=0;
volatile uint8_t FlagCambio=0;
volatile time t={10,29,14,19,4,98};
volatile time t_Parcial={10,29,14,19,4,21};
static char not_leap(void);
static char not_leap_Parcial(void);
#define DIA_HORA 4
#define MES_MINUTO 7
#define ANO_SEGUNDO 10
typedef enum{S0,S1,S2,S3,S4,S5,S6} state;
state estado;
volatile uint8_t posicion[2]={0,0};
volatile uint8_t FlagCursor=0;
void salidaA(uint8_t eje_Y,uint8_t eje_X);
	void salidaD(uint8_t eje_Y,uint8_t eje_X);
	void actualizarCampo(char campo,uint8_t estado);
	void efecto_Apagado();
	void actualizarTiempo();
	void imprimir();