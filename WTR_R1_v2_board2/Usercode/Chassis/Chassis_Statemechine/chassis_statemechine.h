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

extern float unitree_motor_left_pos;
extern float unitree_motor_left_kp;
extern float unitree_motor_left_kw;
extern float unitree_motor_right_pos;
extern float unitree_motor_right_kp;
extern float unitree_motor_right_kw;

void unitree_move_smooth(int id, float move_angle, float KP, float KW);

void Chassis_State_Mechine_Start(void);

void Grip_Seed_Task(void *argument);

#endif