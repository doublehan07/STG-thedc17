#include "analog_signal_service.h"

#include "adc.h"
#include "includes.h"
#include "IIC.h"
#include "comunication.h"
#include "systime.h"
#include "includes.h"
#include "sysconfig.h"

volatile uint16_t ADCValueProc[8]={0};

void AnalogProc(void)
{
	uint8_t i=0;
	uint8_t j=0;
	for(i=0;i!=8;i++)
		ADCValueProc[i]=0;
	
	for(j=0; j!=16;j++)
	{
		for(i=0; i!=8; i++)
			ADCValueProc[i]+=ADCValue[i];
		OSTimeDly(5);
	}
	
	for(i=0;i!=8;i++)
		ADCValueProc[i]=ADCValueProc[i]/16;
}



volatile uint16_t Voltage[9]={0};

#define VOLREF 1200
#define VOLREFADC ADCValueProc[ADC_VOLREF]


void ADCValueToVoltage(void)
{
  static uint8_t channel;
  for(channel=0;channel!=8;channel++)
    Voltage[channel]=(uint32_t)(((uint32_t)ADCValueProc[channel]*VOLREF)/VOLREFADC);
	Voltage[8]=VOLREF*4096/((uint32_t)ADCValue[ADC_VOLREF]);
}

volatile uint16_t CPUTemperature=0; //0.01degree

#define AVGSLOPE     43  //0.1mV
#define TEMP25VOL 14300  //0.1mV
#define TEMPVOL   ((int32_t)Voltage[ADC_TEMPERATURE]*10)  //0.1mv
void CalculateCPUTemp(void)
{
	CPUTemperature=((TEMP25VOL-TEMPVOL)*100)/43+2500;
}

volatile datestruct DateTime={0};

volatile uint8_t HeartRate;
#define HEARTRATEVOL Voltage[2]
void HeartRateExec(void)
{
	static uint32_t timeold;
	uint32_t timenew;
	static uint8_t oldstate;
	
	timenew=TimeTicket;
	if((HEARTRATEVOL>2200)&&(oldstate==0))
	{
		oldstate=1;
		if(((timenew-timeold)>(60000/230))&&((timenew-timeold)<(60000/35)))
		{
			HeartRate=60000/(timenew-timeold);
		}
		else
			HeartRate=0;
		timeold=timenew;
	}
	else if(HEARTRATEVOL<2000)
		oldstate=0;
	else
		;
}

#define BATTERYVOLT   (Voltage[0]*2)


volatile uint8_t BatteryPercent;

uint8_t GetBatteryPercent(void)
{
	const uint8_t percent[12]={100,90,80,70,60,50,40,30,20,10,5,0};
  const uint16_t volt[12]={4200,4060,3980,3920,3870,3820,3790,3770,3740,3680,3450,3000};
  uint16_t batteryvolt;
  uint8_t tablepoint;

  batteryvolt=BATTERYVOLT;
  
  if(batteryvolt>4400){
		//USARTSendFigDec(batteryvolt,4);
		return 0xff;}
	else if(batteryvolt>=4200)
		return 100;
	else if(batteryvolt<3000)
		return 0;
	else
	{
		for(tablepoint=0;volt[tablepoint]>batteryvolt;tablepoint++);
		
		return (uint32_t)(((batteryvolt-volt[tablepoint])*(percent[tablepoint-1]-percent[tablepoint]))/(volt[tablepoint-1]-volt[tablepoint])+percent[tablepoint]);

	}
	
}


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
void SetTime(uint8_t command)
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();
	RTC_SetCounter(setdate(RTC_GetCounter(),command));
	OS_EXIT_CRITICAL();
}

OS_STK ANALOG_SIGNAL_TASK_STK[ANALOG_SIGNAL_STK_SIZE];
void Analog_Signal_task(void *pdata)
{
	while(1)
	{
		AnalogProc();
		ADCValueToVoltage();
		CalculateCPUTemp();
		IIC_ReadLM75ATemp();
		//totalsec_to_date(RTC_GetCounter(),&DateTime);
		HeartRateExec();
		BatteryPercent=GetBatteryPercent();
	}
}




