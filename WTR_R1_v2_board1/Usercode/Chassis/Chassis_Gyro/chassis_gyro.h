#ifndef __CHASSIS_GYRO_H
#define __CHASSIS_GYRO_H

#include "chassis_start.h"

extern uint8_t chassis_gyro_state;
extern float chassis_offset;

void Chassis_Gyro_TaskStart(void);

void Chassis_Gyro_Task(void *argument);

#endif