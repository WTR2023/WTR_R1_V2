#ifndef __CHASSIS_STATEMECHINE_H
#define __CHASSIS_STATEMECHINE_H

#include "chassis.h"

enum Grip_Seed_State {
    Grip_Ready,     // 准备取苗状态，两臂内收并张开夹爪
    Grip_Progress,  // 取苗状态，两臂竖直，收紧夹爪并抬升
    Grip_toDeposit, // 运送至存储区状态，两臂收回
    Grip_Defalut,   // 射球时的默认状态
};

enum Plant_Seed_State {
    Plant_Deposit,
    Plant,
    Plant_Ready,
};

enum Chassis_State {
    Seed_Mode,
    Ball_Mode,
};

extern enum Chassis_State chassis_mode;
extern int seed_count;
extern int land_angle;
extern int right_deposit_angle;
extern int left_deposit_angle;

void Chassis_State_Mechine_Start(void);

void Grip_Seed_Task(void *argument);

#endif