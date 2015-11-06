#include "strategy.h"

struct RECEIVE ReceiveAI;
struct SEND SendAI;

uint8_t isFinished = 0 , toBeReceive = 0 , isParsed = 0;

void parseReceivedPack(char *ReceivePacket)
{
	ReceiveAI = *(struct RECEIVE *)ReceivePacket;
	isParsed = 1 ;
}

void parseSendPack(char OurID, char TargetID, char Prop)
{
	SendAI.OurID = OurID;
	SendAI.TargetID = TargetID;
	SendAI.PropID = Prop;
	SendAI.checkD = 0x0D;
	SendAI.checkA = 0x0A;
}