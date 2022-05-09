/*
 * Reloj.c
 *
 * Created: 25/4/2022 14:54:59
 * Author : Barcala
 */  

/* Inclusión de cabeceras de bibliotecas de código */

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


