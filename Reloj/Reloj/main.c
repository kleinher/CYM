/*
 * Reloj.c
 */  

/* Inclusi�n de cabeceras de bibliotecas de c�digo */

#include "main.h"

int main(void)
{
    /* Replace with your application code */
	setupTimer();
	setupLCD();
	iniciar_MEF();
	while (1) 
    {
		actualizar_MEF();
    }
}
/*SETUP DE LA PANTALLA LCD*/
void setupLCD(){
	LCDinit();
	LCDclr();
	LCDcursorOFF();
}


