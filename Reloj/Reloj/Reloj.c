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
					if((t.month==2) && (not_leap()))
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

static char not_leap(void)      //check for leap year
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

static char not_leap_Parcial(void)      //check for leap year
{
	if (!(t_Parcial.year%100))
	{
		return (char)(t_Parcial.year%400);
	}
	else
	{
		return (char)(t_Parcial.year%4);
	}
}

void iniciar_MEF(){
	estado=S0;t_Parcial=t;
}

void actualizar_MEF(){
	uint8_t key='#';
	uint8_t static nuevo=0;
	nuevo=KEYPAD_scan (&key);
	efecto_Apagado();
	switch (estado){
		case S0:
		switch (key){
			case 'A': t_Parcial=t; estado=S1; salida(1,1,ANO_SEGUNDO);
			break;
			case 'D': estado=S0; salida(0,1,ANO_SEGUNDO);
			default: actualizarTiempo();
		}
		break;
		case S1:
		switch (key){
			case 'A': estado=S2; salida(1,1,MES_MINUTO);
			break;
			case 'D': estado=S0; salida(0,1,MES_MINUTO);
			break;
			case 'B': actualizarCampo('A',1); imprimir(); LCDGotoXY(ANO_SEGUNDO,1);
			break;
			case 'C': actualizarCampo('A',0); imprimir(); LCDGotoXY(ANO_SEGUNDO,1);
		}
		break;
		case S2:
		switch (key){
			case 'A': estado=S3; salida(1,1,DIA_HORA);
			break;
			case 'D': estado=S0; salida(0,1,DIA_HORA);
			break;
			case 'B': actualizarCampo('M',1); imprimir(); LCDGotoXY(MES_MINUTO,1);
			break;
			case 'C': actualizarCampo('M',0); imprimir(); LCDGotoXY(MES_MINUTO,1);
		}
		break;
		case S3:
		switch (key){
			case 'A': estado=S4; salida(1,0,DIA_HORA);
			break;
			case 'D': estado=S0; salida(0,0,DIA_HORA);
			break;
			case 'B': actualizarCampo('D',1); imprimir(); LCDGotoXY(DIA_HORA,1);
			break;
			case 'C': actualizarCampo('D',0); imprimir(); LCDGotoXY(DIA_HORA,1);
		}
		break;
		case S4:
		switch (key){
			case 'A': estado=S5; salida(1,0,MES_MINUTO);
			break;
			case 'D': estado=S0; salida(0,0,MES_MINUTO);
			break;
			case 'B': actualizarCampo('h',1); imprimir(); LCDGotoXY(DIA_HORA,0);
			break;
			case 'C': actualizarCampo('h',0); imprimir(); LCDGotoXY(DIA_HORA,0);
		}
		break;
		case S5:
		switch (key){
			case 'A': estado=S6; salida(1,0,ANO_SEGUNDO);
			break;
			case 'D': estado=S0; salida(0,0,ANO_SEGUNDO);
			break;
			case 'B': actualizarCampo('n',1); imprimir(); LCDGotoXY(MES_MINUTO,0);
			break;
			case 'C': actualizarCampo('n',0); imprimir(); LCDGotoXY(MES_MINUTO,0);
		}
		break;
		case S6:
		switch (key){
			case 'A': estado=S0; salida(1,0,ANO_SEGUNDO); salida(0,0,ANO_SEGUNDO);
			break;
			case 'D': estado=S0; salida(0,0,ANO_SEGUNDO);
			break;
			case 'B': actualizarCampo('s',1); imprimir(); LCDGotoXY(ANO_SEGUNDO,0);
			break;
			case 'C': actualizarCampo('s',0); imprimir(); LCDGotoXY(ANO_SEGUNDO,0);
		}
		break;
	}
}

void actualizarTiempo(){
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
void imprimir(){
	if(FlagLcd){
		FlagLcd=0;
		LCDGotoXY(4,1);
		LCDescribeDato(t_Parcial.date,2);
		LCDsendChar('/');
		LCDescribeDato(t_Parcial.month,2);
		LCDsendChar('/');
		LCDescribeDato(t_Parcial.year,2);
		LCDGotoXY(4,0);
		LCDescribeDato(t_Parcial.hour,2);
		LCDsendChar(':');
		LCDescribeDato(t_Parcial.minute,2);
		LCDsendChar(':');
		LCDescribeDato(t_Parcial.second,2);
	}
}

void salida(uint8_t z,uint8_t eje_Y,uint8_t eje_X){
	if(z){
		t=t_Parcial;
		imprimir();
		LCDGotoXY(eje_X,eje_Y);
		posicion[0]=eje_X;
		posicion[1]=eje_Y;
		FlagCursor=1;
	}
	else{
		t_Parcial=t;
		LCDGotoXY(eje_X,eje_Y);
		posicion[0]=eje_X;
		posicion[1]=eje_Y;
		FlagCursor=0;
	}
}
void actualizarCampo(char campo,uint8_t estado){
	switch (campo){
		case 'A':
		if(!estado)
		estado = -1;
		t_Parcial.year += 1*estado;
		t_Parcial.year = t_Parcial.year % 100;
		if(t_Parcial.year == 0 && !estado)
		t_Parcial.year = 99;
		break;
		case 'M':
		if(estado){
			if(t_Parcial.month==12)
			t_Parcial.month=1;
			else t_Parcial.month++;
		}
		else{
			if(t_Parcial.month==1)
			t_Parcial.month=12;
			else t_Parcial.month--;
		}
		break;
		case 'D':
		if(estado){
			if ((t_Parcial.month==4) || (t_Parcial.month==6) || (t_Parcial.month==9) || (t_Parcial.month==11)){
				if(t_Parcial.date==30)
				t_Parcial.date=1;
				else t_Parcial.date++;
			}
			else{
				if((t.month==2) && (not_leap_Parcial())){
					if(t_Parcial.date==28)
					t_Parcial.date=1;
					else t_Parcial.date++;
				}
				else{
					if((t.month==2) && (!not_leap_Parcial())){
						if(t_Parcial.date==29)
						t_Parcial.date=1;
						else t_Parcial.date++;
					}
					else{
						if(t_Parcial.date==31)
						t_Parcial.date=1;
						else t_Parcial.date++;
					}
				}
			}
		}
		else{
			if ((t_Parcial.month==4) || (t_Parcial.month==6) || (t_Parcial.month==9) || (t_Parcial.month==11)){
				if(t_Parcial.date==1)
				t_Parcial.date=30;
				else t_Parcial.date--;
			}
			else{
				if((t.month==2) && (not_leap_Parcial())){
					if(t_Parcial.date==1)
					t_Parcial.date=28;
					else t_Parcial.date--;
				}
				else{
					if((t.month==2) && (!not_leap_Parcial())){
						if(t_Parcial.date==1)
						t_Parcial.date=29;
						else t_Parcial.date--;
					}
					else{
						if(t_Parcial.date==1)
						t_Parcial.date=31;
						else t_Parcial.date--;
					}
				}
			}
		}
		break;
		case 'h':
		if(estado){
			if(t_Parcial.hour==23)
			t_Parcial.hour=0;
			else t_Parcial.hour++;
		}
		else{
			if(t_Parcial.hour==0)
			t_Parcial.hour=23;
			else t_Parcial.hour--;
		}
		break;
		case 'n':
		if(estado){
			if(t_Parcial.minute==59)
			t_Parcial.minute=0;
			else t_Parcial.minute++;
		}
		else{
			if(t_Parcial.minute==0)
			t_Parcial.minute=59;
			else t_Parcial.minute--;
		}
		break;
		case 's':
		if(estado){
			if(t_Parcial.second==59)
			t_Parcial.second=0;
			else t_Parcial.second++;
		}
		else{
			if(t_Parcial.second==0)
			t_Parcial.second=59;
			else t_Parcial.second--;
		}
		break;
	}
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