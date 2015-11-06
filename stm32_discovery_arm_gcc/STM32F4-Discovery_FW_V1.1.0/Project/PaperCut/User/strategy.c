#include "strategy.h"

struct RECEIVE ReceiveAI;
struct SEND SendAI;

uint8_t isFinished = 0 , toBeReceive = 0 , isParsed = 0 , requestToBeSent = 0;
char ReceivedPacket[23] , toBeParsedPack[23];
char receive_data[32];

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
	
void useProp(char propID){}
	
void listenToMaster(void){
	
}
	
	
