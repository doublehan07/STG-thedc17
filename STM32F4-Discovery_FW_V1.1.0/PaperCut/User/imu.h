#ifndef __IMU_H
#define __IMU_H

#include "stdint.h"

static char rev_buf[64]; /* ���յ���ԭʼ���� */

enum input_status { 
STATUS_IDLE, /* ���� */
STATUS_SOF, /* ���յ�֡ͷ (0x88, 0xAF)*/
STATUS_LEN, /* ���յ������ֽ� */
STATUS_DATA, /*���յ����� */
};

struct imu_data { 
int16_t accl[3];
int16_t gyro[3];
int16_t mag[3];
int16_t yaw;
int16_t pitch;
int16_t roll;
int32_t presure;
};



int imu_rev_get_data(struct imu_data* data);

void imu_rev_process(char ch);

#endif
