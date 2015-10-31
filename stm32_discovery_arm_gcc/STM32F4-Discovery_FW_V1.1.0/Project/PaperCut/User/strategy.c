#include "strategy.h"

struct RECEIVE ReceiveAI;
struct SEND SendAI;
char ToBeSent[3];

void parseReceivedPack(char *ReceivePacket)
{
	ReceiveAI = *(struct RECEIVE *)ReceivePacket;
}