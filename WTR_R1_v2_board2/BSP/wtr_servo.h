#ifndef __WTR_SERVO_H
#define __WTR_SERVO_H

#include "stm32f4xx.h"
#include "tim.h"

/**
 * 夹爪定义：
 * 右侧夹取臂左夹爪：PA0 ----- TIM2_CH1
 */

#define Right_Left_Gripper_Servo_Timer_Handle   &htim2
#define Right_Left_Gripper_Servo_Timer_Channel  TIM_CHANNEL_1

void Servo_Init(void);
void Servo_Open(void);
void Servo_Grip(void);

#endif