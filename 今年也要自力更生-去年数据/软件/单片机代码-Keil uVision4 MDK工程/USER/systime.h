#ifndef _SYSTIME_H
#define _SYSTIME_H

#include "stm32f10x_conf.h"
#include "stm32f10x.h"





u8 RTC_Init(void);

typedef struct
{
	uint8_t month, day, hour, minute,second; 
	uint16_t year;uint8_t date[20],time[12];
} datestruct;//????????

#define yearadd 0
#define yearsub 1
#define monthadd 2
#define monthsub 3
#define dayadd 4
#define daysub 5
#define houradd 6
#define hoursub 7
#define minuteadd 8
#define minutesub 9
#define secadd 10
#define secsub 11

void totalsec_to_date(uint32_t totalsec, volatile datestruct *p);
uint32_t setdate(uint32_t totalsec, uint8_t setnum);


#endif



