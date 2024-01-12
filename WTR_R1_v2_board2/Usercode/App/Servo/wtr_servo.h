#ifndef __WTR_SERVO_H
#define __WTR_SERVO_H

#include "stm32f4xx.h"
#include "tim.h"

/**
 * 夹爪定义：
 * 右侧夹取臂左夹爪：PA0 ----- TIM2_CH1
 * 右侧放苗板：     PA1 ----- TIM2_CH2
 * 右侧挡板:        PA2 ----- TIM2_CH3
 */

#define Right_Left_Gripper_Servo_Timer_Handle  &htim2
#define Right_Left_Gripper_Servo_Timer_Channel TIM_CHANNEL_1

#define Right_Deposit_Servo_Timer_Handle       &htim2
#define Right_Deposit_Servo_Timer_Channel      TIM_CHANNEL_2

#define Right_Baffle_Servo_Timer_Handle        &htim2
#define Right_Baffle_Servo_Timer_Channel       TIM_CHANNEL_3

void Servo_Init(void);
void Right_Servo_Open(void);
void Right_Servo_Grip(void);
void Right_Servo_Buffle_Open(void);
void Right_Servo_Buffle_Close(void);
void Right_Servo_Deposit_Open(void);
void Right_Servo_Deposit_Close(void);

#endif