#ifndef __CHASSIS_STATEMECHINE_H
#define __CHASSIS_STATEMECHINE_H

#include "chassis.h"

enum Grip_Seed_State {
    Grip_Ready,
    Grip,
    Grip_Deposit,
};

enum Plant_Seed_State {
    Plant_Deposit,
    Plant,
    Plant_Ready,
};

extern int right_land_angle;
extern int right_deposit_angle;

void Grip_Seed_Task_Start(void);
void Chassis_State_Mechine_Start(void);

void Right_Grip_Seed_Task(void *argument);
void Right_Plant_Seed_Task(void *argument);

#endif