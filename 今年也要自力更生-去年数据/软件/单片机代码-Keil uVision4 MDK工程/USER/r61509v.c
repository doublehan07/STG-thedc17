#include "r61509v.h"

#include "8080.h"
#include "public.h"


void R61509V_Init(void)
{
	uint16_t x,y;
	Port8080_Init();
	
	Port8080_SetOut();
	Port8080_Select();
	Port8080_RST();
	
	Port8080_WriteCommand(0x0000);
	Port8080_WriteCommand(0x0000);
	Port8080_WriteCommand(0x0000);
	Port8080_WriteCommand(0x0000);

	Port8080_WriteCommand(0x0600);Port8080_WriteData(0x0001);//soft reset
	Delay_mS(20);
	Port8080_WriteCommand(0x0600);Port8080_WriteData(0x0000);//soft reset
	Delay_mS(50);

	Port8080_WriteCommand(0x0001);Port8080_WriteData(0x0100);//Driver Output Control
	Port8080_WriteCommand(0x0002);Port8080_WriteData(0x0100);//LCD Driving Wave Control
	Port8080_WriteCommand(0x0003);Port8080_WriteData(0x1030);//Entry Mode
	Port8080_WriteCommand(0x0008);Port8080_WriteData(0x0808);//
	/*gamma set 20090423 BY LYJ*/

	Port8080_WriteCommand(0x0300);Port8080_WriteData(0x000c); 
	Port8080_WriteCommand(0x0301);Port8080_WriteData(0x5A0B); 
	Port8080_WriteCommand(0x0302);Port8080_WriteData(0x0906); 
	Port8080_WriteCommand(0x0303);Port8080_WriteData(0x1017); 
	Port8080_WriteCommand(0x0304);Port8080_WriteData(0x2300); 
	Port8080_WriteCommand(0x0305);Port8080_WriteData(0x1700); 
	Port8080_WriteCommand(0x0306);Port8080_WriteData(0x6309); 
	Port8080_WriteCommand(0x0307);Port8080_WriteData(0x0c09); 
	Port8080_WriteCommand(0x0308);Port8080_WriteData(0x100c); 
	Port8080_WriteCommand(0x0309);Port8080_WriteData(0x2232); 
	/*power set*/
	Port8080_WriteCommand(0x0010);Port8080_WriteData(0x0014);
	Port8080_WriteCommand(0x0011);Port8080_WriteData(0x0101);  
	Port8080_WriteCommand(0x0100);Port8080_WriteData(0x0230);//
	Delay_mS(20);
	Port8080_WriteCommand(0x0101);Port8080_WriteData(0x0247);//
	Delay_mS(20);
	Port8080_WriteCommand(0x0103);Port8080_WriteData(0x0a00);//Starts VLOUT3,Sets the VREG1OUT.
	Delay_mS(20);
	Port8080_WriteCommand(0x0280);Port8080_WriteData(0xf0ff);//VCOMH voltage   //0xcEff
	Delay_mS(10);
	Port8080_WriteCommand(0x0102);Port8080_WriteData(0xB1b0);//Starts VLOUT3,Sets the VREG1OUT.
	Delay_mS(100);  
	/*window set*/
	Port8080_WriteCommand(0x0210);Port8080_WriteData(0x0000);//Window Horizontal RAM Address Start
	Port8080_WriteCommand(0x0211);Port8080_WriteData(0x00ef);//Window Horizontal RAM Address End
	Port8080_WriteCommand(0x0212);Port8080_WriteData(0x0000);//Window Vertical RAM Address Start
	Port8080_WriteCommand(0x0213);Port8080_WriteData(0x018f);//Window Vertical RAM Address End
	Port8080_WriteCommand(0x0200);Port8080_WriteData(0x0000);//RAM Address Set (Horizontal Address)
	Port8080_WriteCommand(0x0201);Port8080_WriteData(0x0000);//RAM Address Set (Vertical Address)
	Port8080_WriteCommand(0x0401);Port8080_WriteData(0x0001);//Base Image Display
	Port8080_WriteCommand(0x0007);Port8080_WriteData(0x0100);//Display Control 1
	Delay_mS(50);
	Port8080_WriteCommand(0x0202);

	Port8080_Select();
	for(x=0;x!=240;x++)
	{
		for(y=0;y!=400;y++)
		  Port8080_WriteData(0x0000);
	}
  Port8080_Deselect();
}


void R61509V_WindowSet(uint16_t x1,uint16_t x2,uint16_t y1, uint16_t y2)
{
	Port8080_WriteCommand(0x0210);Port8080_WriteData(x1);//Window Horizontal RAM Address Start
	Port8080_WriteCommand(0x0211);Port8080_WriteData(x2);//Window Horizontal RAM Address End
	Port8080_WriteCommand(0x0212);Port8080_WriteData(y1);//Window Vertical RAM Address Start
	Port8080_WriteCommand(0x0213);Port8080_WriteData(y2);//Window Vertical RAM Address End
	Port8080_WriteCommand(0x0200);Port8080_WriteData(x1);//RAM Address Set (Horizontal Address)
	Port8080_WriteCommand(0x0201);Port8080_WriteData(y1);//RAM Address Set (Vertical Address)
		
}

void R61509V_DrawRectangle(uint16_t color, uint16_t x1,uint16_t x2, uint16_t y1,uint16_t y2)
{
	uint32_t dotcount=(x2-x1+1)*(y2-y1+1);
	Port8080_Select();
	R61509V_WindowSet(x1,x2,y1,y2);
	Port8080_WriteCommand(0x0202);
	Port8080_DataTransfer();
	while(dotcount--)
	{
		Port8080_WriteData(color);
	}
	Port8080_Deselect();
}

void R61509V_PutData(uint16_t *color, uint16_t x1,uint16_t x2, uint16_t y1,uint16_t y2)
{
	uint32_t dotcount=(x2-x1+1)*(y2-y1+1);
	Port8080_Select();
	R61509V_WindowSet(x1,x2,y1,y2);
	Port8080_WriteCommand(0x0202);
	Port8080_DataTransfer();
	while(dotcount--)
	{
		Port8080_WriteData(*(color++));
	}
	Port8080_Deselect();
}




