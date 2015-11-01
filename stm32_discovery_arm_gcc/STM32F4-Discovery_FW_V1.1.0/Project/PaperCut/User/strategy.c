#include "strategy.h"

struct RECEIVE ReceiveAI;
struct SEND SendAI;

void parseReceivedPack(char *ReceivePacket)
{
	ReceiveAI = *(struct RECEIVE *)ReceivePacket;
}

void parseSendPack(char OurID, char TargetID, char Prop)
{
	SendAI.OurID = OurID;
	SendAI.TargetID = TargetID;
	SendAI.PropID = Prop;
	SendAI.checkD = 0x0D;
	SendAI.checkA = 0x0A;
}