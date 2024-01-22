#ifndef __CHASSIS_H
#define __CHASSIS_H

#include "chassis_start.h"

void Chassis_Init(void);
void Chassis_CAN_Message_TaskStart(void);
void Chassis_Unitree_UART_Message_TaskStart(void);

void CAN_Message_Task(void *argument);
void Unitree_UART_Message_Task(void *argument);

#endif