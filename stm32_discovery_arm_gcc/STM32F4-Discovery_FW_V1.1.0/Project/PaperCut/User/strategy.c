#include "strategy.h"

struct RECEIVE ReceiveAI;
struct SEND SendAI;

uint8_t isFinished = 0 , toBeReceive = 0 , isParsed = 0 , requestToBeSent = 0;
char ReceivedPacket[23] , toBeParsedPack[23];
char receive_data[32];
char OurLocation[2];

void parseReceivedPack(char *ReceivePacket)
{
	int i = 0;
	ReceiveAI = *(struct RECEIVE *)ReceivePacket;
		
//	receive_data[i++] = ReceiveAI.ID;
//	receive_data[i++] = ReceiveAI.Status;
//	receive_data[i++] = ReceiveAI.WarningA;
//	receive_data[i++] = ReceiveAI.WarningB;
//	receive_data[i++] = ReceiveAI.WarningC;
//	receive_data[i++] = ReceiveAI.WarningD;
//		
//	receive_data[i++] = ReceiveAI.If_track;
//	receive_data[i++] = ReceiveAI.Prop_num;
//	receive_data[i++] = ReceiveAI.Track_cond;
//		
//	receive_data[i++] = ReceiveAI.PropI;
//	receive_data[i++] = ReceiveAI.PropII;
//		
//	receive_data[i++] = ReceiveAI.PeopleA[0];
//	receive_data[i++] = ReceiveAI.PeopleA[1];
//	receive_data[i++] = ReceiveAI.PeopleB[0];
//	receive_data[i++] = ReceiveAI.PeopleB[1];
//	receive_data[i++] = ReceiveAI.PeopleC[0];
//	receive_data[i++] = ReceiveAI.PeopleC[1];
//	receive_data[i++] = ReceiveAI.PeopleD[0];
//	receive_data[i++] = ReceiveAI.PeopleD[1];
//	receive_data[i++] = ReceiveAI.PropA[0];
//	receive_data[i++] = ReceiveAI.PropA[1];
//	receive_data[i++] = ReceiveAI.PropB[0];
//	receive_data[i++] = ReceiveAI.PropB[1];
//	receive_data[i++] = ReceiveAI.PropC[0];
//	receive_data[i++] = ReceiveAI.PropC[1];
//	receive_data[i++] = ReceiveAI.PropD[0];	
//	receive_data[i++] = ReceiveAI.PropD[1];	
//		
//	receive_data[i++] = ReceiveAI.Source;	
//	receive_data[i++] = ReceiveAI.Target;	
//	receive_data[i++] = ReceiveAI.PropNow;	
//	receive_data[i++] = ReceiveAI.CheckD;	
//	receive_data[i++] = ReceiveAI.CheckA;	
	//printf("h");
	isParsed = 1 ;
}

void parseSendPack(char OurID, char TargetID, char Prop)
{
	SendAI.OurID = OurID;
	SendAI.TargetID = TargetID;
	SendAI.PropID = Prop;
	SendAI.checkD = 0x0D;
	SendAI.checkA = 0x0A;
	requestToBeSent = 1;
}

char getNearestPropID(void){}
	
void useProp(char propID){
	
}

/*
void setOurValue(void)
{
	if(ReceiveAI.ID == 0) //A
	{
		OurLocation[0] = ReceiveAI.PeopleA[0];
		OurLocation[1] = ReceiveAI.PeopleA[1];
		//return ReceiveAI.PeopleA;
	}
	else if(ReceiveAI.ID == 1) //B
	{
		OurLocation[0] = ReceiveAI.PeopleB[0];
		OurLocation[1] = ReceiveAI.PeopleB[1];
		//return ReceiveAI.PeopleB;
	}
	else if(ReceiveAI.ID == 2) //C
	{
		OurLocation[0] = ReceiveAI.PeopleC[0];
		OurLocation[1] = ReceiveAI.PeopleC[1];
		//return ReceiveAI.PeopleC;
	}
	else //D
	{
		OurLocation[0] = ReceiveAI.PeopleD[0];
		OurLocation[1] = ReceiveAI.PeopleD[1];
		//return ReceiveAI.PeopleD;
	}
}
	
char* locateOneProp(void)
{
	setOurValue();
	//Get the nearest prop
	uint8_t num = ReceiveAI.Prop_num;
	uint8_t ID, i = 0;
	int distance = 0;
	int diffx, diffy;
	for( ; i < num; i++)
	{
		if(i == 0)
		{
			diffx = OurLocation[0] - ReceiveAI.PropA[0];
			diffy = OurLocation[1] - ReceiveAI.PropA[1];
			distance = sqrt(diffx * diffx + diffy * diffy);
			ID = 0;
		}
		else if(i == 1)
		{
			
		}
		else if(i == 2)
		{}
		else
		{}
	}
}
*/
	
