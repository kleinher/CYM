/*
 * Reloj.c
 *
 * Created: 5/8/2022 11:47:28 PM
 *  Author: Hernan
 */ 
#include "Reloj.h"
void relojFuncion()
{
	FlagLcd=1;
	FlagCambio=!FlagCambio;
	
	if (++t.second==60)        //keep track of time, date, month, and year
	{
		t.second=0;
		if (++t.minute==60)
		{
			t.minute=0;
			if (++t.hour==24)
			{
				t.hour=0;
				if (++t.date==32)
				{
					t.month++;
					t.date=1;
				}
				else if (t.date==31)
				{
					if ((t.month==4) || (t.month==6) || (t.month==9) || (t.month==11))
					{
						t.month++;
						t.date=1;
					}
				}
				else if (t.date==30)
				{
					if(t.month==2)
					{
						t.month++;
						t.date=1;
					}
				}
				else if (t.date==29)
				{
					if((t.month==2) && (not_leap(t)))
					{
						t.month++;
						t.date=1;
					}
				}
				if (t.month==13)
				{
					t.month=1;
					t.year++;
				}
			}
		}
	}
}

static char not_leap(time t)      //check for leap year
{
	if (!(t.year%100))
	{
		return (char)(t.year%400);
	}
	else
	{
		return (char)(t.year%4);
	}
}



void iniciar_MEF(){
	estado=S0;t_Parcial=t;
}

void actualizar_MEF(){
	uint8_t key='#';
	KEYPAD_scan (&key);
	efecto_Apagado();
	switch (estado){
		case S0:
			switch (key){
				case 'A': t_Parcial=t; estado=S1; salidaA(ano);
				break;
				default: actualizarTiempo(t);
			}
		break;
		case S1:
		switch (key){
			case 'A': estado=S2; salidaA(mes);
			break;
			case 'D': estado=S0; salidaD(mes);
			break;
			case 'B': actualizarCampo2(&t_Parcial.year,1,100,-1,1); actualizarTiempo(t_Parcial); LCDGotoXY(ANO_SEGUNDO,1);
			break;
			case 'C': actualizarCampo2(&t_Parcial.year,-1,100,-1,1); actualizarTiempo(t_Parcial); LCDGotoXY(ANO_SEGUNDO,1);
		}
		break;
		case S2:
		switch (key){
			case 'A': estado=S3; salidaA(dia);
			break;
			case 'D': estado=S0; salidaD(dia);
			break;
			case 'B': actualizarCampo2(&t_Parcial.month,1,12,0,0); actualizarTiempo(t_Parcial); LCDGotoXY(MES_MINUTO,1);
			break;
			case 'C': actualizarCampo2(&t_Parcial.month,-1,12,0,0); actualizarTiempo(t_Parcial); LCDGotoXY(MES_MINUTO,1);
		}
		break;
		case S3:
		switch (key){
			case 'A': estado=S4; salidaA(hora);
			break;
			case 'D': estado=S0; salidaD(hora);
			break;
			case 'B': actualizarDia(1); actualizarTiempo(t_Parcial); LCDGotoXY(DIA_HORA,1);
			break;
			case 'C': actualizarDia(-1); actualizarTiempo(t_Parcial); LCDGotoXY(DIA_HORA,1);
		}
		break;
		case S4:
		switch (key){
			case 'A': estado=S5; salidaA(minuto);
			break;
			case 'D': estado=S0; salidaD(minuto);
			break;
			case 'B': actualizarCampo2(&t_Parcial.hour,1,24,-1,1); actualizarTiempo(t_Parcial); LCDGotoXY(DIA_HORA,0);
			break;
			case 'C': actualizarCampo2(&t_Parcial.hour,-1,24,-1,1); actualizarTiempo(t_Parcial); LCDGotoXY(DIA_HORA,0);
		}
		break;
		case S5:
		switch (key){
			case 'A': estado=S6; salidaA(segundo);
			break;
			case 'D': estado=S0; salidaD(segundo);
			break;
			case 'B': actualizarCampo2(&t_Parcial.minute,1,60,-1,1); actualizarTiempo(t_Parcial); LCDGotoXY(MES_MINUTO,0);
			break;
			case 'C': actualizarCampo2(&t_Parcial.minute,-1,60,-1,1); actualizarTiempo(t_Parcial); LCDGotoXY(MES_MINUTO,0);
		}
		break;
		case S6:
		switch (key){
			case 'A': estado=S0; salidaA(segundo); salidaD(segundo);
			break;
			case 'D': estado=S0; salidaD(segundo);
			break;
			case 'B': actualizarCampo2(&t_Parcial.second,1,60,-1,1); actualizarTiempo(t_Parcial); LCDGotoXY(ANO_SEGUNDO,0);
			break;
			case 'C': actualizarCampo2(&t_Parcial.second,-1,60,-1,1); actualizarTiempo(t_Parcial); LCDGotoXY(ANO_SEGUNDO,0);
		}
		break;
	}
}

void actualizarTiempo(time t){
	if(FlagLcd){
		FlagLcd=0;
		LCDGotoXY(4,1);
		LCDescribeDato(t.date,2);
		LCDsendChar('/');
		LCDescribeDato(t.month,2);
		LCDsendChar('/');
		LCDescribeDato(t.year,2);
		LCDGotoXY(4,0);
		LCDescribeDato(t.hour,2);
		LCDsendChar(':');
		LCDescribeDato(t.minute,2);
		LCDsendChar(':');
		LCDescribeDato(t.second,2);
	}
}
void salidaA(pos posi){
	t=t_Parcial;
	actualizarTiempo(t_Parcial);
	LCDGotoXY(posi.x,posi.y);
	posicion[0]=posi.x;
	posicion[1]=posi.y;
	FlagCursor=1;
}
void salidaD(pos posi){
	t_Parcial=t;
	LCDGotoXY(posi.x,posi.y);
	posicion[0]=posi.x;
	posicion[1]=posi.y;
	FlagCursor=0;
}


void actualizarDia(int8_t dir){
	switch (t_Parcial.month)
	{
		case 2:
			if(not_leap(t_Parcial)){
				t_Parcial.date = (t_Parcial.date+dir) % 28;
				if(t_Parcial.date == 0)
					t_Parcial.date = 28;
			}
			else{
				t_Parcial.date = (t_Parcial.date+dir) % 29;
				if(t_Parcial.date == 0)
					t_Parcial.date = 29;
			}
		break;
		case 4:
		case 6:
		case 9:
		case 11:
			t_Parcial.date = (t_Parcial.date+dir) % 30;
			if(t_Parcial.date == 0)
				t_Parcial.date = 30;
		break;
		default:
			t_Parcial.date = (t_Parcial.date+dir) % 31;
			if(t_Parcial.date == 0)
				t_Parcial.date = 31;
		break;
		
	}


}
 void actualizarCampo2(volatile signed char *asdf,int8_t dir,uint8_t maximo,int8_t minimo,int8_t is_month){
	*asdf = (*asdf+dir) % maximo;
	if(*asdf == minimo)
		*asdf = maximo-is_month;
		return;
}

void efecto_Apagado(){
	if(FlagCursor){
		if(FlagCambio){
			switch (posicion[0]){
				case ANO_SEGUNDO:
				if(!posicion[1])
				LCDescribeDato(t_Parcial.second,2);
				else LCDescribeDato(t_Parcial.year,2);
				break;
				case MES_MINUTO:
				if(!posicion[1])
				LCDescribeDato(t_Parcial.minute,2);
				else LCDescribeDato(t_Parcial.month,2);
				break;
				case DIA_HORA:
				if(!posicion[1])
				LCDescribeDato(t_Parcial.hour,2);
				else LCDescribeDato(t_Parcial.date,2);
				break;
			}
		}
		else LCDstring((uint8_t*)"  ",2);
		
		LCDGotoXY(posicion[0],posicion[1]);
	}
}