#include "hardware.h"


#include "pwm.h"
#include "adc.h"
#include "spi_memory.h"
#include "motor_ctr.h"
#include "IIC_6050.h"


void HardwareDriver_Init(void)
{
	//RTC_Init();
	ADC_Init_User();
	SPIFlash_Init();
	IIC_Init();
	PWM_Init();
	InitMPU6050();
	MOTOR_Init();
}


