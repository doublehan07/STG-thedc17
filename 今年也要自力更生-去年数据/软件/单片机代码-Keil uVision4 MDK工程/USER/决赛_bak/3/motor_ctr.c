#include "motor_ctr.h"

#include "pwm.h"
#include "adc.h"
#include "public.h"
#include "sysconfig.h"
#include "debug_exec.h"

#define MT_DIR_PORT			GPIOA
#define MT1_DIR_PIN			GPIO_Pin_0
#define MT2_DIR_PIN			GPIO_Pin_1
#define MT3_DIR_PIN			GPIO_Pin_11
#define MT4_DIR_PIN			GPIO_Pin_12

uint8_t PWMInited = 0;
uint16_t FullVoltage[2] = {0};

uint16_t MaxSpeed[2] = {1315, 1300};

int16_t dV[2] = {0};

void MOTOR_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t i;	
	
	//gpio initialize
	GPIO_InitStructure.GPIO_Pin = MT1_DIR_PIN|MT2_DIR_PIN|MT3_DIR_PIN|MT4_DIR_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??????50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	GPIO_Init(GPIOA, &GPIO_InitStructure);//?????GPIOx???

	Delay_mS(10);
	
	//PreTest
	MOTOR_Voltage_Control();//Set PWM To 0
	Delay_mS(10);
	
	for(i=0;i!=4;i++)
	{
		if(ADC_Voltage[ADC_MOTOR1+i] > 1300 && ADC_Voltage[ADC_MOTOR1+i] < 1700)
			MSZV[i] = ADC_Voltage[ADC_MOTOR1+i];
	}
	
	Delay_mS(10);
	
	for(i=0;i!=4;i++)
	{
		if(ADC_Voltage[ADC_MOTOR1+i] > 1300 && ADC_Voltage[ADC_MOTOR1+i] < 1700)
			MSZV[i] = (MSZV[i] + ADC_Voltage[ADC_MOTOR1+i]) / 2;
	}
	
	MotorVoltage[2] = 400;
	MotorVoltage[3] = -400;
	
	MOTOR_Voltage_Control();//Set PWM3 And PWM4 To HALF
	
	Delay_mS(10);
	FullVoltage[0] = MSZV[2] - ADC_Voltage[ADC_MOTOR3]; //MoveForward
	FullVoltage[1] = ADC_Voltage[ADC_MOTOR4] - MSZV[3]; //MoveBack
	
	Delay_mS(10);
	FullVoltage[0] += MSZV[2] - ADC_Voltage[ADC_MOTOR3]; //MoveForward
	FullVoltage[1] += ADC_Voltage[ADC_MOTOR4] - MSZV[3]; //MoveBack

	MotorVoltage[2] = 0;
	MotorVoltage[3] = 0;

/*	for(i=0;i!=2;i++)
	{
		MSMR[i][0] = MSZV[i] > FullVoltage[0] ? FullVoltage[0] : MSZV[i];//Forward
		MSMR[i][1] = MSZV[i] > FullVoltage[1] ? FullVoltage[1] : MSZV[i];//Back
	}
*/
	MotorVoltage[0] = MotorVoltage[1] = (int32_t)100 * 800 / FullVoltage[0];//100mV of FeedBack
	MOTOR_Voltage_Control();//Set PWM1 And PWM2
	Delay_mS(10);
	for(i=0;i!=2;i++)
	{
		dV[i] = MSZV[i] - ADC_Voltage[ADC_MOTOR1+i];//Get deltaV
	}
	
	Delay_mS(10);
	for(i=0;i!=2;i++)
	{
		dV[i] += MSZV[i] - ADC_Voltage[ADC_MOTOR1+i];//Get 2 * deltaV
	}
	
	MotorVoltage[0] = MotorVoltage[1] = 0;
	
	PWMInited = 1;
}


#define MT1_DIR_N		GPIO_SetBits(MT_DIR_PORT, MT1_DIR_PIN)
#define MT1_DIR_P		GPIO_ResetBits(MT_DIR_PORT, MT1_DIR_PIN)
#define MT2_DIR_N		GPIO_SetBits(MT_DIR_PORT, MT2_DIR_PIN)
#define MT2_DIR_P		GPIO_ResetBits(MT_DIR_PORT, MT2_DIR_PIN)
#define MT3_DIR_N		GPIO_SetBits(MT_DIR_PORT, MT3_DIR_PIN)
#define MT3_DIR_P		GPIO_ResetBits(MT_DIR_PORT, MT3_DIR_PIN)
#define MT4_DIR_N		GPIO_SetBits(MT_DIR_PORT, MT4_DIR_PIN)
#define MT4_DIR_P		GPIO_ResetBits(MT_DIR_PORT, MT4_DIR_PIN)

int16_t MotorVoltage[4]={0};

void MOTOR_Voltage_Control(void)
{
	if(MotorVoltage[0]<0)
	{
		MT1_DIR_N;
		PWM_Motor_1=800+MotorVoltage[0];
	}
	else
	{
		MT1_DIR_P;
		PWM_Motor_1=MotorVoltage[0];
	}
	
	
	if(MotorVoltage[1]<0)
	{
		MT2_DIR_N;
		PWM_Motor_2=800+MotorVoltage[1];
	}
	else
	{
		MT2_DIR_P;
		PWM_Motor_2=MotorVoltage[1];
	}
	if(MotorVoltage[2]<0)
	{
		MT3_DIR_N;
		PWM_Motor_3=800+MotorVoltage[2];
	}
	else
	{
		MT3_DIR_P;
		PWM_Motor_3=MotorVoltage[2];
	}
	if(MotorVoltage[3]<0)
	{
		MT4_DIR_N;
		PWM_Motor_4=800+MotorVoltage[3];
	}
	else
	{
		MT4_DIR_P;
		PWM_Motor_4=MotorVoltage[3];
	}
}


volatile int16_t MotorSpeed[2]={0};
volatile int16_t MotorSpeedSet[2]={0};//Percentage

volatile uint16_t MSZV[4]={1487,1487,1487,1487};  //the adc motor voltage when motor is stop
//volatile uint16_t MSMR[2][2]={1487,1487,1487,1487};  //the maxmium motor votage range, must be smaller than MSZV

volatile uint8_t ReTestFV = 0;

void MOTOR_CalculateMotorSpeed(void)
{
	uint8_t i;
	int32_t Vi;
	MotorVoltage[2] = ReTestFV ? 400 : 0;
	MotorVoltage[3] = -MotorVoltage[2];

	if (ReTestFV && MSZV[2] - ADC_Voltage[ADC_MOTOR3] > 500)
	{
		FullVoltage[0] = FullVoltage[0] / 2 + MSZV[2] - ADC_Voltage[ADC_MOTOR3]; //MoveForward
		FullVoltage[1] = FullVoltage[1] / 2 + ADC_Voltage[ADC_MOTOR4] - MSZV[3]; //MoveBack
	}
	
	for(i=0;i!=2;i++)
	{
		Vi = ((int32_t)MSZV[i]-ADC_Voltage[ADC_MOTOR1+i]) * 800;
		Vi /= ((Vi > 0) ? FullVoltage[0] : FullVoltage[1]);//Vi of 800
		if (dV[i] > 2 && dV[i] < 140)
			Vi -= (MotorVoltage[i] - Vi) * dV[i] / 200;
		else if (dV[i] >= 140)
			Vi = 0;
		/*
		if(ADC_Voltage[ADC_MOTOR1+i]>MSZV[i]+MSMR[i][1])
			MotorSpeed[i]=-800;
		else if(ADC_Voltage[ADC_MOTOR1+i]<MSZV[i]-MSMR[i][0])
			MotorSpeed[i]=800;
		else
		{
			int32_t temp = ((int32_t)MSZV[i]-ADC_Voltage[ADC_MOTOR1+i]) * 800;
			MotorSpeed[i] = temp / (temp > 0 ? MSMR[i][0] : MSMR[i][1]);
		}*/
		if (Vi > 800)
			MotorSpeed[i] = 800;
		else if (Vi < -800)
			MotorSpeed[i] = -800;
		else
			MotorSpeed[i] = Vi;
	}
}




#define SCALE 1.1

void MOTOR_Speed_Control(void)
{
	uint8_t i;
	int32_t TargetSpeed;
	for(i=0;i!=2;i++)
	{
		int16_t LastVoltage = MotorVoltage[i];
		if(MotorSpeedSet[i]>100)
			MotorSpeedSet[i]=100;
		else if(MotorSpeedSet[i]<-100)
			MotorSpeedSet[i]=-100;
		TargetSpeed = (int32_t)MotorSpeedSet[i] * MaxSpeed[i] * 8 / (MotorSpeedSet[i] > 0 ? FullVoltage[0] : FullVoltage[1]);
		/*
		if(MotorSpeed[i]>MotorSpeedSet[i]+25)
			MotorVoltage[i]--;
		else if(MotorSpeed[i]<MotorSpeedSet[i]-25)
			MotorVoltage[i]++;
		else if((MotorSpeed[i]>MotorSpeedSet[i]+10)&&(TimeTicket%5==0))
			MotorVoltage[i]--;
		else if((MotorSpeed[i]<MotorSpeedSet[i]-10)&&(TimeTicket%5==0))
			MotorVoltage[i]++;
		else if((MotorSpeed[i]>MotorSpeedSet[i])&&(TimeTicket%20==0))
			MotorVoltage[i]--;
		else if((MotorSpeed[i]<MotorSpeedSet[i])&&(TimeTicket%20==0))
			MotorVoltage[i]++;
		else
			;
		if(MotorVoltage[i]>200)
			MotorVoltage[i]=200;
		else if(MotorVoltage[i]<-200)
			MotorVoltage[i]=-200;
		else
			;*/
		if ((TargetSpeed > 80 || TargetSpeed < -80) && (TargetSpeed < MotorSpeed[i] - 40 || TargetSpeed > MotorSpeed[i] + 40))
		{
			if (TargetSpeed && TargetSpeed * MotorSpeed[i] <= 0)//ÒìºÅ»òÍ£Ö¹
				MotorVoltage[i] = TargetSpeed > 0 ? 800 : -800;
			else
				MotorVoltage[i] = TargetSpeed * TargetSpeed * TargetSpeed * SCALE / MotorSpeed[i] / MotorSpeed[i];
		}
		else if (TargetSpeed < MotorSpeed[i] - 15 || TargetSpeed > MotorSpeed[i] + 15)
		{
			if (TargetSpeed && TargetSpeed * MotorSpeed[i] <= 0)//ÒìºÅ»òÍ£Ö¹
				MotorVoltage[i] = TargetSpeed * SCALE + 100;
			else
				MotorVoltage[i] = TargetSpeed * TargetSpeed * SCALE / MotorSpeed[i];
		}
		else
		{
			if (!TargetSpeed)
				MotorVoltage[i] = 0;
			if (TargetSpeed && TargetSpeed < MotorSpeed[i] - 3)
				MotorVoltage[i]++;
			if (TargetSpeed && TargetSpeed > MotorSpeed[i] - 3)
				MotorVoltage[i]--;
		}
//		if (TargetSpeed == 0)
//			MotorVoltage[i] = 0;
		if (MotorVoltage[i] - LastVoltage > 400)
			MotorVoltage[i] = LastVoltage + 400;
		else if (MotorVoltage[i] - LastVoltage < -400)
			MotorVoltage[i] = LastVoltage - 400;
		if(MotorVoltage[i]>800)
			MotorVoltage[i]=800;
		else if(MotorVoltage[i]<-800)
			MotorVoltage[i]=-800;
	}
}


void Motor_Exec(void)
{
	if (PWMInited)
	{
		MOTOR_CalculateMotorSpeed();
		MOTOR_Speed_Control();
		MOTOR_Voltage_Control();
	}
}

