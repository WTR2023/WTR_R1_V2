#include "wtr_servo.h"

/**
 * @brief   舵机初始化
*/
void Servo_Init(void)
{
    HAL_TIM_PWM_Start(Right_Left_Gripper_Servo_Timer_Handle, Right_Left_Gripper_Servo_Timer_Channel);
}

/**
 * @brief   右侧夹爪张开
*/
void Right_Servo_Open(void)
{
    __HAL_TIM_SET_COMPARE(Right_Left_Gripper_Servo_Timer_Handle, Right_Left_Gripper_Servo_Timer_Channel, 500);
}

/**
 * @brief   右侧夹爪抓取
*/
void Right_Servo_Grip(void)
{
    __HAL_TIM_SET_COMPARE(Right_Left_Gripper_Servo_Timer_Handle, Right_Left_Gripper_Servo_Timer_Channel, 750);
}