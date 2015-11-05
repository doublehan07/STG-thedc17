#include "public.h"

#include "sysconfig.h"
#include "comunication.h"

void Delay_mS(uint16_t delay)
{
	uint16_t timeset;
	timeset=TimeTicket+delay;
	while(timeset!=(TimeTicket&0xffff));
}

void Delay_Simple(uint32_t delay)
{
	while(delay--);
}

void Array16_SmallToBig(uint16_t *array, uint8_t size)
{
	uint16_t temp;
	uint8_t i,j;
	for(i = 0; i < size - 1; i++)
	{
		for(j = i + 1; j < size; j++)
		{
			if(array[i] > array[j])
			{
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
		}
	}	
}


void FigToString(uint32_t fig, uint8_t fignum, uint8_t figdot, uint8_t *s)
{
	uint32_t div=1000000000;
	uint8_t sp=0;
	for(;fignum!=9;fignum++)
	{
		div=div/10;
	}

	while(div!=1)
	{
		*s='0'+(fig%div)/(div/10);
		s++;
		sp++;
		div=div/10;
		if(sp==figdot)
		{
			*s='.';
			s++;
		}
	}
	*s=0;
}


void FigToStringAuto(uint32_t fig, uint8_t fignum, uint8_t figdot, uint8_t *s)
{
	uint32_t div=1000000000;
	uint8_t temp0=0;
	uint8_t sp=0;
	uint8_t zerocut=0;
	
	for(;fignum!=9;fignum++)
	{
		div=div/10;
	}

	while(div!=1)
	{
		
		if(((temp0=(fig%div)/(div/10))!=0)||(zerocut==1)||(sp+1==figdot))
		{
			*s='0'+(fig%div)/(div/10);
			zerocut=1;
			s++;
		}
		sp++;
		div=div/10;
		if(sp==figdot)
		{
			*s='.';
			s++;
		}
	}
	*s=0;
	
}


void DoubleFigToStringAuto(double fig, uint8_t fignum, uint8_t figdot, uint8_t *s)
{
	uint8_t temp;
	temp=fignum-figdot;
	for(;temp!=0;temp--)
		fig=fig*10;
	FigToStringAuto(fig,fignum, figdot, s);
}


void FigToStringHex(uint32_t fig, uint8_t num, uint8_t *s)
{
	uint8_t *stemp;
	uint8_t divtemp;
	stemp=s+num+2;
	*stemp=0;
	stemp--;
	while(stemp!=s+1)
	{
		divtemp=fig%16;
		if(fig<10)
		{
			*stemp=divtemp+'0';
		}
		else
		{
			*stemp=divtemp-10+'a';
		}
	  fig=fig/16;
		stemp--;
	}
	*(stemp--)='x';
	*stemp='0';
	
}


uint8_t StringAdd(uint8_t *dirs, uint8_t *dirsadd, uint8_t size)
{
	for(;size!=0;size--)
	{
		if(*(dirs++)==0)
		{
			dirs--;
			for(;size!=0;size--)
			{
				
				if((*(dirs++)=*(dirsadd++))==0)
					return 0;
			}
			*(dirs-1)=0;
			USARTSendString("\n space not enough");
			USARTSendString("\n ");
			USARTSendString(dirs);
			USARTSendString("\n ");
			USARTSendString(dirsadd);
			return 1;
		}
	}
	USARTSendString("\n not a string");
	return 2;
}

	
uint8_t CompareString(uint8_t *s1,uint8_t *s2)
{
	while((*(s1++)!=0)&&(*(s2++)!=0))
	{
		if(*s1!=*s2)
			return 1;
	}
  if((*s1==0)&&(*s2==0))
		return 0;
	else
		return 1;
}

uint8_t CopyString(uint8_t *s, uint8_t *s1, uint8_t s1size)
{
	for(;s1size!=0;s1size--)
	{
		if((*s)==0)
		{
			*s1=0;
			return 0;
		}
		*(s1++)=*(s++);
	}
	USARTSendString("copy string fail, space not enough ");
	USARTSendFigDec(s1size,3);
	USARTSendString(s);
	return 1;	
}



