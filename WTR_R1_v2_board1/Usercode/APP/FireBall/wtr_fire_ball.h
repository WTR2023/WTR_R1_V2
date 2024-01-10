#ifndef __WTR_FIRE_BALL_H
#define __WTR_FIRE_BALL_H

#include "wtr_servo.h"
#include "stm32f4xx.h"
#include "tim.h"
#include "cmsis_os.h"

enum Fire_State {
    Fire_Ready, // 发射准备状态
    Fire        // 发射状态
};

extern int arm_angle;              // 机械臂角度
extern int rail_angle;             // 供弹轨道角度
extern enum Fire_State fire_state; // 发射状态

void Arm_Grip_Action(void);

#endif