#include "strategy.h"

struct RECEIVE ReceiveAI;
struct SEND SendAI;

uint8_t isFinished = 0 , toBeReceive = 0 , isParsed = 0 , requestToBeSent = 0;
char ReceivedPacket[23] , toBeParsedPack[23];
char receive_data[32];
extern char OurLocation[2] , lastOurLocation[2];
char destination[2];//x,y,angleErr

char haveDestination = 0;
char IMUinitSuccess = 0;
char KeepForward = 0 , lastKeepForward = 0;
int16_t lastYaw = 0;
int16_t ANGLE_ERR = 0;
int16_t destinationAngle = 0;

extern struct imu_data IMU_Data;
extern char map[256][256] ;

char getRivalID(void){
	return (1-ReceiveAI.ID);
}

void getOurLocation(char* OurLocation){
	switch(ReceiveAI.ID){
		case 0:
			OurLocation[0] = ReceiveAI.PeopleA[0];
			OurLocation[1] = ReceiveAI.PeopleA[1];
			break;
		case 1:
			OurLocation[0] = ReceiveAI.PeopleB[0];
			OurLocation[1] = ReceiveAI.PeopleB[1];
			break;
		default:
			break;
	}
}

void parseReceivedPack(char *ReceivePacket)
{
//	int i = 0;
	ReceiveAI = *(struct RECEIVE *)ReceivePacket;
	isParsed = 1 ;
}

/**
装包函数
装包的时候看一眼，我们使用的道具是道具一还是道具二
如果是道具二，没问题走你
如果是道具一，还要把ReceiveAI里面的道具二变成道具一，以免后面的解析函数用错道具
**/

void parseSendPack(char OurID, char TargetID, char Prop)
{
//	if(Prop==ReceiveAI.PropI){
//		SendAI.OurID = OurID;
//		SendAI.TargetID = TargetID;
//		SendAI.PropID = Prop;
//		SendAI.checkD = 0x0D;
//		SendAI.checkA = 0x0A;
//		ReceiveAI.PropI = ReceiveAI.PropII;
//		ReceiveAI.PropII = 0;
//	}else if(Prop==ReceiveAI.PropII){
//		SendAI.OurID = OurID;
//		SendAI.TargetID = TargetID;
//		SendAI.PropID = Prop;
//		SendAI.checkD = 0x0D;
//		SendAI.checkA = 0x0A;
//		ReceiveAI.PropII = 0;
//	}else{
		SendAI.OurID = OurID;
		SendAI.TargetID = TargetID;
		SendAI.PropID = Prop;
		SendAI.checkD = 0x0D;
		SendAI.checkA = 0x0A;
//}
	
	//requestToBeSent = 1;
}

char getNearestPropID(void){
	return 0;
}
	
void useProp(char propID){
	
}

void sendDefaultPack(void){
		int i = 0 ;
		for(;i<3;i++){
			printf("%c",*((uint8_t *)&SendAI+i));
		}
}

void sendPack(char OurID, char TargetID, char Prop){
	parseSendPack(OurID,TargetID,Prop);
	sendDefaultPack();
}


void checkWhetherToBeAttacked(void){
	if(ReceiveAI.Target&(1<<ReceiveAI.ID) && (ReceiveAI.PropNow&0xF0)!=0){
		switch(checkWhetherPossessDefendProp()){
			case 1:
				sendPack(ReceiveAI.ID,ReceiveAI.ID,ReceiveAI.PropI);break;
			case 2:
				sendPack(ReceiveAI.ID,ReceiveAI.ID,ReceiveAI.PropII);break;
			default:
				break;
		}
	}
}

int checkWhetherPossessDefendProp(void){
	if(ReceiveAI.PropI<=0x04){
		return 1;
	}else if(ReceiveAI.PropII<=0x04){
		return 2;
	}else
		return 0;
}


int checkWhetherPossessAttackProp(void){
	if((ReceiveAI.PropI>0x04)&&(ReceiveAI.PropI<0x09)){
		return 1;
	}else if((ReceiveAI.PropII>0x04)&&(ReceiveAI.PropII<0x09)){
		return 2;
	}else
		return 0;
}

int checkWhetherPossessTrapProp(void){
	if(ReceiveAI.PropI>0x08){
		return 1;
	}else if(ReceiveAI.PropII>0x08){
		return 2;
	}else
		return 0;
}

void useTrapProp(void){
	switch(checkWhetherPossessTrapProp()){
			case 1:
				sendPack(ReceiveAI.ID,getRivalID(),ReceiveAI.PropI);break;
			case 2:
				sendPack(ReceiveAI.ID,getRivalID(),ReceiveAI.PropII);break;
			default:
				break;
		}
}

void useAttackProp(void){
		switch(checkWhetherPossessAttackProp()){
			case 1:
				sendPack(ReceiveAI.ID,getRivalID(),ReceiveAI.PropI);break;
			case 2:
				sendPack(ReceiveAI.ID,getRivalID(),ReceiveAI.PropII);break;
			default:
				break;
		}
}

void checkWhetherToPickProp(){
	int i = ReceiveAI.Prop_num , MinDistanceSquare = 400 , DistanceSquare , toPersue;
	for(;i>0;i--){//轮询四个道具哪个在正方向范围内
		char propi[2] ;
		getPropLocation(propi,i) ;
		if((IMU_Data.yaw/10-culculateAngle(propi)<90)||(IMU_Data.yaw/10-culculateAngle(propi)>-90)){//如果在正方向，取最小距离者
			DistanceSquare = ((int)OurLocation[0]-(int)ReceiveAI.PropA[0])*((int)OurLocation[0]-(int)ReceiveAI.PropA[0]) + ((int)OurLocation[1]-(int)ReceiveAI.PropA[1])*((int)OurLocation[1]-(int)ReceiveAI.PropA[1]);
			if(DistanceSquare < MinDistanceSquare){
				MinDistanceSquare = DistanceSquare;
				toPersue = i;
				//printf("to persue %d", i);
			}
		}
	}
	if(MinDistanceSquare < 3600){
	checkWhetherPassedNearbyProp(toPersue);
	}
}

/**
查看道具附近是否有小车经过的点
有的话headTo该道具
**/
void checkWhetherPassedNearbyProp(int toPersue){
	switch(toPersue){
		case 1: 
			if(checkWhetherPassedNearbyPosition(ReceiveAI.PropA[0],ReceiveAI.PropA[1])){
				//headTo(ReceiveAI.PropA[0],ReceiveAI.PropA[1]);
				destination[0] = ReceiveAI.PropA[0];
				destination[1] = ReceiveAI.PropA[1];
				destinationAngle = culculateAngle(destination);
				haveDestination = 1 ;
			}
			break;
		case 2: 
			if(checkWhetherPassedNearbyPosition(ReceiveAI.PropB[0],ReceiveAI.PropB[1])){
				//headTo(ReceiveAI.PropB[0],ReceiveAI.PropB[1]);
				destination[0] = ReceiveAI.PropB[0];
				destination[1] = ReceiveAI.PropB[1];
				destinationAngle = culculateAngle(destination);
				haveDestination = 1 ;
			}
			break;
		case 3: 
			if(checkWhetherPassedNearbyPosition(ReceiveAI.PropC[0],ReceiveAI.PropC[1])){
				//headTo(ReceiveAI.PropC[0],ReceiveAI.PropC[1]);
				destination[0] = ReceiveAI.PropC[0];
				destination[1] = ReceiveAI.PropC[1];
				destinationAngle = culculateAngle(destination);
				haveDestination = 1 ;
			}
			break;
		case 4: 
			if(checkWhetherPassedNearbyPosition(ReceiveAI.PropD[0],ReceiveAI.PropD[1])){
				//headTo(ReceiveAI.PropD[0],ReceiveAI.PropD[1]);
				destination[0] = ReceiveAI.PropD[0];
				destination[1] = ReceiveAI.PropD[1];
				destinationAngle = culculateAngle(destination);
				haveDestination = 1 ;
			}
	break;
		default: break;
	}
}

int checkWhetherPassedNearbyPosition(char x,char y){//搜索点附近的边框中是否含有经过的点
	int passed = 0 , i = 0 , j = 0;
	for(i = x-searchr ; i < x+searchr ; i++){//搜索上边框
		for(j = y+searchR ; j>y+searchr ; j--){
			if(map[i][j]==1){
				passed = 1;
				return passed;
			}
		}
	}
	for(i = x-searchR ; i<x-searchr ; i++){//搜索左边框
		for(j = y+searchr ; j>y-searchr ; y--){
			if(map[i][j]==1){
				passed = 1;
				return passed;
			}
		}
	}
	for(i = x+searchr ; i<x+searchR ; i++){//搜索右边框
		for(j = y+searchr ; j>y-searchr ; j--){
			if(map[i][j]==1){
				passed = 1;
				return passed;
			}
		}
	}
	for(i = x-searchr ; i<x+searchr ; i++){//搜索下边框
		for(j = y-searchr ; j>y-searchR ; j--){
			if(map[i][j]==1){
				passed = 1;
				return passed;
			}
		}
	}
	return passed;
}


void chechWhetherStillShouldHeadTo(){
	if((culculateAngle(destination)>90)||(culculateAngle(destination)<-90)){
		haveDestination = 0;
	}
}

//char getAngleErr(char* destination){
//	
//}

void checkWhetherInitIMUSuccess(){
	int16_t angleErr = culculateAngleErr(lastYaw/10 , IMU_Data.yaw/10);
	if( angleErr<6 && angleErr>-6 ){
		ANGLE_ERR = culculateAngle(lastOurLocation) - IMU_Data.yaw/10 ;//得到地图坐标方向与IMU坐标方向差值，这个非常重要
		IMUinitSuccess = 1;
	}
	lastYaw = IMU_Data.yaw ;
}

int16_t culculateAngleErr(int16_t myDirection ,int16_t PropDirection){//返回正表示要向右转
	int16_t angleErr = PropDirection - myDirection;
	if(angleErr < -180){
		angleErr += 360;
	}else if(angleErr > 180){
		angleErr -= 360;
	}
	return angleErr;
}

int16_t culculateAngle(char * destination){//根据自身当前坐标和上次记录的坐标计算方向
	float tan = ( OurLocation[1] - destination[1])/(OurLocation[0] - destination[0]);
	//printf("tan our direction in imu = %f",tan);
	return atanf(tan)*180/Pi;
}

int16_t IMUyaw2mapDirection(){
	return (IMU_Data.yaw/10 + ANGLE_ERR);
}

void getPropLocation(char* propi , int i){
	switch(i){
		case 1: 
			propi[0] = ReceiveAI.PropA[0];
			propi[1] = ReceiveAI.PropA[1];
			break;
		case 2:
			propi[0] = ReceiveAI.PropB[0];
			propi[1] = ReceiveAI.PropB[1];
			break;
		case 3: 
			propi[0] = ReceiveAI.PropC[0];
			propi[1] = ReceiveAI.PropC[1];
			break;
		case 4: 
			propi[0] = ReceiveAI.PropD[0];
			propi[1] = ReceiveAI.PropD[1];
			break;
		default: break;
	}
}

void getProp(void){
	sendPack(ReceiveAI.ID, ReceiveAI.ID, 0x0E);
	sendDefaultPack();
}

