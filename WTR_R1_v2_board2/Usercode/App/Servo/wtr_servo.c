#include "wtr_servo.h"

/**
 * @brief   舵机初始化
 */
void Servo_Init(void)
{
    HAL_TIM_PWM_Start(Right_Left_Gripper_Servo_Timer_Handle, Right_Left_Gripper_Servo_Timer_Channel);
    HAL_TIM_PWM_Start(Right_Baffle_Servo_Timer_Handle, Right_Baffle_Servo_Timer_Channel);
    HAL_TIM_PWM_Start(Right_Deposit_Servo_Timer_Handle, Right_Deposit_Servo_Timer_Channel);
    HAL_TIM_PWM_Start(Left_Left_Gripper_Servo_Timer_Handle, Left_Left_Gripper_Servo_Timer_Channel);
    HAL_TIM_PWM_Start(Left_Baffle_Servo_Timer_Handle, Left_Baffle_Servo_Timer_Channel);
    HAL_TIM_PWM_Start(Left_Deposit_Servo_Timer_Handle, Left_Deposit_Servo_Timer_Channel);
    Servo_Open();
    Servo_Deposit_Close();
    Servo_Buffle_Open();
}

/**
 * @brief   夹爪张开
 */
void Servo_Open(void)
{
    __HAL_TIM_SET_COMPARE(Left_Left_Gripper_Servo_Timer_Handle, Left_Left_Gripper_Servo_Timer_Channel, 1150);
    __HAL_TIM_SET_COMPARE(Right_Left_Gripper_Servo_Timer_Handle, Right_Left_Gripper_Servo_Timer_Channel, 550);
}

/**
 * @brief   夹爪抓取
 */
void Servo_Grip(void)
{
    __HAL_TIM_SET_COMPARE(Right_Left_Gripper_Servo_Timer_Handle, Right_Left_Gripper_Servo_Timer_Channel, 750);
    __HAL_TIM_SET_COMPARE(Left_Left_Gripper_Servo_Timer_Handle, Left_Left_Gripper_Servo_Timer_Channel, 750);
}

/**
 * @brief   挡板升高
 */
void Servo_Buffle_Open(void)
{
    __HAL_TIM_SET_COMPARE(Right_Baffle_Servo_Timer_Handle, Right_Baffle_Servo_Timer_Channel, 1100);
    __HAL_TIM_SET_COMPARE(Left_Baffle_Servo_Timer_Handle, Left_Baffle_Servo_Timer_Channel, 250);
}

/**
 * @brief   挡板关闭
 */
void Servo_Buffle_Close(void)
{
    __HAL_TIM_SET_COMPARE(Right_Baffle_Servo_Timer_Handle, Right_Baffle_Servo_Timer_Channel, 600);
    __HAL_TIM_SET_COMPARE(Left_Baffle_Servo_Timer_Handle, Left_Baffle_Servo_Timer_Channel, 750);
}

/**
 * @brief   放苗板打开
 */
void Servo_Deposit_Open(void)
{
    __HAL_TIM_SET_COMPARE(Right_Deposit_Servo_Timer_Handle, Right_Deposit_Servo_Timer_Channel, 520);
    __HAL_TIM_SET_COMPARE(Left_Deposit_Servo_Timer_Handle, Left_Deposit_Servo_Timer_Channel, 948);
}

/**
 * @brief   放苗板关闭
 */
void Servo_Deposit_Close(void)
{
    __HAL_TIM_SET_COMPARE(Left_Deposit_Servo_Timer_Handle, Left_Deposit_Servo_Timer_Channel, 770);
    __HAL_TIM_SET_COMPARE(Right_Deposit_Servo_Timer_Handle, Right_Deposit_Servo_Timer_Channel, 750);
}

