#include "map.h"

char map[256][256] ;
char OurLocation[2] , lastOurLocation[2];

void saveMap(void){
	int i = 0 ,j = 0;
	savePeoplePosition(ReceiveAI.ID);
	//printf("%c",map[255][255]);
//	printf("%c",map[4][6]);
//	for(;i<256;i++){
//		for(;j<256;j++){
//			printf("%d",map[i][j]);
//		}
//	}
//	printf("%c%c",ReceiveAI.PeopleA[0],ReceiveAI.PeopleA[1]);
}

void savePeoplePosition(char id){
	switch(id){
		case 0:
			if(map[(int)ReceiveAI.PeopleA[0]][(int)ReceiveAI.PeopleA[1]] != 1){
				//printf("%c" , map[ReceiveAI.PeopleA[0]][ReceiveAI.PeopleA[1]]);
				map[(int)ReceiveAI.PeopleA[0]][(int)ReceiveAI.PeopleA[1]] = 1;
				//printf("%c" , map[ReceiveAI.PeopleA[0]][ReceiveAI.PeopleA[1]]);	
				lastOurLocation[0] = OurLocation[0];
				lastOurLocation[1] = OurLocation[1];
				OurLocation[0] = ReceiveAI.PeopleA[0];
				OurLocation[1] = ReceiveAI.PeopleA[1];
			}
			break;
		case 1:
			if(map[(int)ReceiveAI.PeopleB[0]][(int)ReceiveAI.PeopleB[1]] != 1) {
				map[(int)ReceiveAI.PeopleB[0]][(int)ReceiveAI.PeopleB[1]] = 1;
				lastOurLocation[0] = OurLocation[0];
				lastOurLocation[1] = OurLocation[1];
				OurLocation[0] = ReceiveAI.PeopleB[0];
				OurLocation[1] = ReceiveAI.PeopleB[1];
			}
			break;
		default:
			break;
	}
}

