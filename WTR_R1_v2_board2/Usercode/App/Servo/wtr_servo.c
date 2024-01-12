#include "wtr_servo.h"

/**
 * @brief   舵机初始化
 */
void Servo_Init(void)
{
    HAL_TIM_PWM_Start(Right_Left_Gripper_Servo_Timer_Handle, Right_Left_Gripper_Servo_Timer_Channel);
    HAL_TIM_PWM_Start(Right_Baffle_Servo_Timer_Handle, Right_Baffle_Servo_Timer_Channel);
    HAL_TIM_PWM_Start(Right_Deposit_Servo_Timer_Handle, Right_Deposit_Servo_Timer_Channel);
}

/**
 * @brief   右侧夹爪张开
 */
void Right_Servo_Open(void)
{
    __HAL_TIM_SET_COMPARE(Right_Left_Gripper_Servo_Timer_Handle, Right_Left_Gripper_Servo_Timer_Channel, 600);
}

/**
 * @brief   右侧夹爪抓取
 */
void Right_Servo_Grip(void)
{
    __HAL_TIM_SET_COMPARE(Right_Left_Gripper_Servo_Timer_Handle, Right_Left_Gripper_Servo_Timer_Channel, 750);
}

/**
 * @brief   挡板升高
*/
void Right_Servo_Buffle_Open(void)
{
    __HAL_TIM_SET_COMPARE(Right_Baffle_Servo_Timer_Handle, Right_Baffle_Servo_Timer_Channel, 750);
}

/**
 * @brief   挡板关闭
*/
void Right_Servo_Buffle_Close(void)
{
    __HAL_TIM_SET_COMPARE(Right_Baffle_Servo_Timer_Handle, Right_Baffle_Servo_Timer_Channel, 750);
}

/**
 * @brief   放苗板打开
*/
void Right_Servo_Deposit_Open(void)
{
    __HAL_TIM_SET_COMPARE(Right_Deposit_Servo_Timer_Handle, Right_Deposit_Servo_Timer_Channel, 580);
}

/**
 * @brief   放苗板关闭
*/
void Right_Servo_Deposit_Close(void)
{
    __HAL_TIM_SET_COMPARE(Right_Deposit_Servo_Timer_Handle, Right_Deposit_Servo_Timer_Channel, 750);
}