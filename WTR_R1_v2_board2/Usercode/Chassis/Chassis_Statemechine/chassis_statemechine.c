#include "chassis_statemechine.h"

/*********************************************************************/
osThreadId_t Grip_Seed_TaskHandle; // 取苗控制线程
const osThreadAttr_t Grip_Seed_Task_attributes = {
    .name       = "Grip_Seed_Task",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};

osThreadId_t Plant_Seed_TaskHandle; // 放苗控制线程
const osThreadAttr_t Plant_Seed_Task_attributes = {
    .name       = "Plant_Seed_Task",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};

/********************************************************************************************/
enum Chassis_State chassis_mode;      // 底盘状态
enum Grip_Seed_State grip_seed_state; // 取苗状态

osThreadState_t grip_seed_task_state; // 线程挂起状态定义

int seed_count          = 0; // 取苗数量
int land_angle          = 0; // 升降电机
int right_deposit_angle = 0; // 右侧挡板电机
int left_deposit_angle  = 0; // 左侧挡板电机

int deposit_point[5] = {
    0,
    370,
    640,
    910,
    1120,
}; // 挡板标记点

/********************************************************************************************/

/**
 * @brief   总状态机线程开启
 */
void Chassis_State_Mechine_Start(void)
{
    chassis_mode         = Seed_Mode; // 默认为苗模式
    grip_seed_state      = Grip_Ready;
    seed_count           = 0; // 取苗架默认为空
    Grip_Seed_TaskHandle = osThreadNew(Grip_Seed_Task, NULL, &Grip_Seed_Task_attributes);
}

/**
 * @brief   取苗状态机
 */
void Grip_Seed_Task(void *argument)
{
    for (;;) {
        switch (grip_seed_state) {
            case Grip_Defalut:
                // 状态机动作执行
                Unitree_Grip_Default();
                Servo_Open();
                land_angle = -2;
                while (((hDJI[0].AxisData.AxisAngle_inDegree - land_angle) > 2.0f) ||
                       ((hDJI[0].AxisData.AxisAngle_inDegree - land_angle) < -2.0f) ||
                       ((hDJI[2].AxisData.AxisAngle_inDegree - land_angle) > 2.0f) ||
                       ((hDJI[2].AxisData.AxisAngle_inDegree - land_angle) < -2.0f)) {
                    osDelay(1);
                }
                // 状态机状态转换
                if (chassis_mode == Seed_Mode) {
                    grip_seed_state = Grip_Ready;
                } else {
                    grip_seed_state = Grip_Defalut;
                }
                break;
            case Grip_Ready:
                // 状态机动作执行
                Unitree_Grip_Ready();
                Servo_Open();
                land_angle = -2;
                while (((hDJI[0].AxisData.AxisAngle_inDegree - land_angle) > 2.0f) ||
                       ((hDJI[0].AxisData.AxisAngle_inDegree - land_angle) < -2.0f) ||
                       ((hDJI[2].AxisData.AxisAngle_inDegree - land_angle) > 2.0f) ||
                       ((hDJI[2].AxisData.AxisAngle_inDegree - land_angle) < -2.0f)) {
                    osDelay(1);
                }
                // 状态机状态转换
                if (chassis_mode == Ball_Mode) {
                    grip_seed_state = Grip_Defalut;
                } else {
                    switch (chassis_message.grip_message) {
                        case 1:
                            grip_seed_state = Grip_Progress;
                            break;
                        case 2:
                            grip_seed_state = Grip_Ready;
                            break;
                        case 3:
                            grip_seed_state = Grip_Ready;
                            break;
                        default:
                            break;
                    }
                }
                break;
            case Grip_Progress:
                // 状态机动作执行
                Unitree_Grip();
                osDelay(500);
                Servo_Grip();
                osDelay(100);
                land_angle = -715;
                while (((hDJI[0].AxisData.AxisAngle_inDegree - land_angle) > 2.0f) ||
                       ((hDJI[0].AxisData.AxisAngle_inDegree - land_angle) < -2.0f) ||
                       ((hDJI[2].AxisData.AxisAngle_inDegree - land_angle) > 2.0f) ||
                       ((hDJI[2].AxisData.AxisAngle_inDegree - land_angle) < -2.0f)) {
                    osDelay(1);
                }
                osDelay(100);
                // 状态机状态转换
                grip_seed_state = Grip_toDeposit;
                break;
            case Grip_toDeposit:
                // 状态机动作执行
                Unitree_Grip_toDeposit();
                osDelay(2000);
                Servo_Open();
                osDelay(1000);
                Unitree_Grip_Ready();
                Servo_Buffle_Close();
                land_angle          = -2;
                right_deposit_angle = deposit_point[1];
                left_deposit_angle  = -deposit_point[1];
                while (((hDJI[0].AxisData.AxisAngle_inDegree - land_angle) > 2.0f) ||
                       ((hDJI[0].AxisData.AxisAngle_inDegree - land_angle) < -2.0f) ||
                       ((hDJI[1].AxisData.AxisAngle_inDegree - right_deposit_angle) > 2.0f) ||
                       ((hDJI[1].AxisData.AxisAngle_inDegree - right_deposit_angle) < -2.0f) ||
                       ((hDJI[2].AxisData.AxisAngle_inDegree - land_angle) > 2.0f) ||
                       ((hDJI[2].AxisData.AxisAngle_inDegree - land_angle) < -2.0f) ||
                       ((hDJI[3].AxisData.AxisAngle_inDegree - left_deposit_angle) > 2.0f) ||
                       ((hDJI[3].AxisData.AxisAngle_inDegree - left_deposit_angle) < -2.0f)) {
                    osDelay(1);
                }
                right_deposit_angle = deposit_point[0];
                left_deposit_angle  = deposit_point[0];
                while (((hDJI[1].AxisData.AxisAngle_inDegree - right_deposit_angle) > 2.0f) ||
                       ((hDJI[1].AxisData.AxisAngle_inDegree - right_deposit_angle) < -2.0f) ||
                       ((hDJI[3].AxisData.AxisAngle_inDegree - left_deposit_angle) > 2.0f) ||
                       ((hDJI[3].AxisData.AxisAngle_inDegree - left_deposit_angle) < -2.0f)) {
                    osDelay(1);
                }
                seed_count++;
                Servo_Buffle_Open();
                // 状态机状态转换
                grip_seed_state = Grip_Ready;
                break;
            default:
                break;
        }
        osDelay(2);
    }
}