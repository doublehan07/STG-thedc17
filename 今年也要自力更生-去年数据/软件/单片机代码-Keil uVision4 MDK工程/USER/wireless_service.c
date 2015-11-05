#include "wireless_service.h"
#include "comunication.h"
#include "analog_signal_service.h"
#include "usart_gps.h"
#include <math.h>
#include "key.h"

WirelessData_Struct WirelessDataRX={0};
WirelessData_Struct WirelessDataTX={0};

//used for latitude and longgitude
double StringToDouble0(uint8_t* DoubleString)
{
	double Du=0.0,Bignum = 0.0, smallnum = 0.0,total=0.0,temp=0.0,m=1.0;
	uint8_t i = 1,size=0,*stringtemp;
	stringtemp = DoubleString;

	while (i)
	{
		if (*stringtemp != '.'){ size++, stringtemp++; }
		if (*stringtemp == '.')break;
	}

	while (size-2)
	{
		temp = *DoubleString - 48;
		Du *= 10;
		Du += temp;
		DoubleString++;
		size--;
	}

	while (m)
	{
		if (*DoubleString == '.')
		{
			i = 0; 
			DoubleString++; 
			continue;
		}

		if (*DoubleString == '\0')break;

		if (i == 1)
		{
			temp = *DoubleString-48;
			Bignum *= 10;
			Bignum += temp;
			DoubleString++;
		}

		if (i == 0)
		{
			m *= 10.0;
			temp = *DoubleString-48;
			temp /= m;
			smallnum += temp;

			DoubleString++;
		}
	}
	temp=(Bignum + smallnum)/60.0;
	total = temp + Du;
	return total;
}


//used for others
double StringToDouble1(uint8_t* DoubleString)
{
	double Bignum = 0.0, smallnum = 0.0,total=0.0,temp=0.0,m=1.0;
	uint8_t i = 1;

	while (m)
	{
		if (*DoubleString == '.')
		{
			i = 0; DoubleString++; continue;
		}

		if (*DoubleString == '\0')break;

		if (i == 1)
		{
			temp = *DoubleString-48;
			Bignum *= 10;
			Bignum += temp;
			DoubleString++;
		}

		if (i == 0)
		{
			m *= 10.0;
			temp = *DoubleString-48;
			temp /= m;
			smallnum += temp;

			DoubleString++;
		}
	}
	total=Bignum + smallnum;
	return total;
}

const double EARTH_RADIUS = 6371.393;


double rad(double d) //?????
{
   return d * 3.1415926 / 180.0;
}

double GetDistance(double lat1, double lng1, double lat2, double lng2)//lat?? lng?? ???????
{
   double radLat1 = rad(lat1);
   double radLat2 = rad(lat2);
   double a = radLat1 - radLat2;
   double b = rad(lng1) - rad(lng2);
   double s=2*asin(sqrt(pow(sin(a/2),2)+cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));//????Google map??
   s=s*EARTH_RADIUS;
   return s*1000;
}
//????????“??????????111km,??????????111*????”
void myFellow(double lat1,double lng1,double lat2,double lng2,fellow *fellow1)//1???2???
{
   fellow1->xlabel=(lat2-lat1)*111*1000;
   fellow1->ylabel=(lng2-lng1)*111*1000*cos((lat1+lat2)/2);
   fellow1->distance=GetDistance(lat1, lng1, lat2, lng2);
   if(fellow1->ylabel==0)
      fellow1->angle=90;
   else
      fellow1->angle=atan(fellow1->xlabel/fellow1->ylabel);
   if(fellow1->xlabel<0)
     fellow1->angle += 180;
}


void Wireless_Service_Init(void)
{
}


WirelessProc_Struct WirelessDataProc;

void RefreshMyData(void)
{
	if((GPS_Data.latitude[0])&&(GPS_Data.longitude[0]))
	{
		WirelessDataTX.latitude=StringToDouble0(GPS_Data.latitude);
		WirelessDataTX.longitude=StringToDouble0(GPS_Data.longitude);
		WirelessDataTX.status=0;
	}
	else
		WirelessDataTX.status=3;
	if(GPS_Data.speed[0])
	{
		WirelessDataTX.speed=StringToDouble1(GPS_Data.speed);
	}
	if(KeyState[0]==0)
	{
		WirelessDataTX.status=1;
	}
	WirelessDataTX.heartrate=HeartRate;
}

void RefreshHostProcData(void)
{
	fellow fellowdata;
	myFellow(WirelessDataTX.latitude, WirelessDataTX.longitude,WirelessDataRX.latitude, WirelessDataRX.longitude, &fellowdata);
	WirelessDataProc.distance=fellowdata.distance;
	WirelessDataProc.course=fellowdata.angle;
	WirelessDataProc.x=fellowdata.xlabel;
	WirelessDataProc.y=fellowdata.ylabel;
}


OS_STK WIRELESS_TASK_STK[WIRELESS_STK_SIZE];
void Wireless_task(void *pdata)
{
	WirelessData_Struct *WirelessDataPoint;
	uint16_t time=1000;
	while(1)
	{
		WirelessDataPoint=&WirelessDataRX;
		NRF24_SetRXMode();
		while(NRF24_GetData((uint8_t *)(WirelessDataPoint)))
		{
			OSTimeDly(2);
			time--;
			if(time==0)
			{
				time++;
				WirelessDataRX.status=4;
			}
		}
		if(WirelessDataRX.status!=3)
		{
			RefreshMyData();
			RefreshHostProcData();
		}
		time=1000;
		WirelessDataPoint=&WirelessDataTX;
		RefreshMyData();
		NRF24_SetTxMode((uint8_t *)(WirelessDataPoint));
		NRF24_CheckACK();
		datacount++;
		
		OSTimeDly(750);

  }
}




