#include "chassis_statemechine.h"

/***********************************************************/
osThreadId_t State_mechine_TaskHandle;
const osThreadAttr_t State_mechine_Task_attributes = {
    .name       = "State_mechine_Task",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};

osThreadId_t Fire_Ball_TaskHandle;
const osThreadAttr_t Fire_Ball_Task_attributes = {
    .name       = "Fire_Ball_Task",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};

enum Chassis_State chassis_mode; // 底盘总控制线程
enum Fire_State fire_state;

int arm_angle      = 0;
int rail_angle     = 0;
int friction_speed = 0;

/***********************************************************/

/**
 * @brief   射球状态机线程开启
 */
void Fire_Ball_Task_Start(void)
{
    fire_state           = Fire_Ready;
    Fire_Ball_TaskHandle = osThreadNew(Fire_Ball_Task, NULL, &Fire_Ball_Task_attributes);
}

/**
 * @brief   总状态机线程开启
 */
void Chassis_State_Mechine_Start(void)
{
    chassis_mode = Seed_Mode;
    Fire_Ball_Task_Start();
    State_mechine_TaskHandle = osThreadNew(Chassis_State_mechine_Task, NULL, &State_mechine_Task_attributes);
}

/**
 * @brief   总控制状态机
 */
void Chassis_State_mechine_Task(void *argument)
{
    for (;;) {
        if (chassis_mode == Seed_Mode) {
            while (fire_state != Fire_Ready) {
                osDelay(1);
            }
            friction_speed = 0;
            osThreadSuspend(Fire_Ball_TaskHandle); // 挂起射球线程
        } else if (chassis_mode == Ball_Mode) {
            osThreadResume(Fire_Ball_TaskHandle); // 解挂射球线程
            friction_speed = 5000;
        }
        osDelay(2);
    }
}

/**
 * @brief       射球状态机
 * @attention   现在为手动射球，后续考虑加入十二键值进行半自动射球
 */
void Fire_Ball_Task(void *argument)
{
    for (;;) {
        switch (fire_state) {
            case Fire_Ready: // 射球默认状态
                // 状态机状态动作
                HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_SET);
                Servo_Grip();
                Servo_In();
                arm_angle  = 180;
                rail_angle = 2;
                while (((hDJI[4].AxisData.AxisAngle_inDegree - arm_angle) > 2.0f) ||
                       ((hDJI[4].AxisData.AxisAngle_inDegree - arm_angle) < -2.0f) ||
                       ((hDJI[5].AxisData.AxisAngle_inDegree - rail_angle) < -2.0f) ||
                       ((hDJI[5].AxisData.AxisAngle_inDegree - rail_angle) > 2.0f)) {
                    osDelay(1);
                }
                // 状态机状态转换
                switch (as69_data.left) {
                    case 1:
                        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_RESET);
                        fire_state = Fire_ReadytoPick;
                        break;
                    case 2:
                        fire_state = Fire_Ready;
                        break;
                    case 3:
                        fire_state = Fire_Ready;
                        break;
                    default:
                        fire_state = Fire_Ready;
                        break;
                }
                break;
            case Fire_ReadytoPick: // 准备捡球状态
                // 状态机状态动作
                HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_SET);
                Servo_Out();
                Servo_Reset();
                arm_angle = 5;
                while (((hDJI[4].AxisData.AxisAngle_inDegree - arm_angle) > 2.0f) ||
                       ((hDJI[4].AxisData.AxisAngle_inDegree - arm_angle) < -2.0f)) {
                    osDelay(1);
                }
                // 状态机状态转换
                HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_RESET);
                fire_state = Fire_ReadytoFire;
                break;
            case Fire_ReadytoFire: // 准备射球状态
                // 状态机状态动作
                HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, GPIO_PIN_SET);
                Servo_Grip();
                osDelay(500);
                Servo_In();
                arm_angle = 180;
                while (((hDJI[4].AxisData.AxisAngle_inDegree - arm_angle) > 2.0f) ||
                       ((hDJI[4].AxisData.AxisAngle_inDegree - arm_angle) < -2.0f)) {
                    osDelay(1);
                }
                // 状态机状态转换
                HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, GPIO_PIN_RESET);
                fire_state = Fire_Progress;
                break;
            case Fire_Progress:
                // 状态机状态动作
                HAL_GPIO_WritePin(GPIOG, GPIO_PIN_5, GPIO_PIN_SET);
                osDelay(500);
                Servo_Reset();
                osDelay(500);
                rail_angle = -630;
                while (((hDJI[5].AxisData.AxisAngle_inDegree - rail_angle) > 2.0f) ||
                       ((hDJI[5].AxisData.AxisAngle_inDegree - rail_angle) < -2.0f)) {
                    osDelay(1);
                }
                rail_angle = 2;
                while (((hDJI[5].AxisData.AxisAngle_inDegree - rail_angle) > 2.0f) ||
                       ((hDJI[5].AxisData.AxisAngle_inDegree - rail_angle) < -2.0f)) {
                    osDelay(1);
                }
                // 状态机状态转换
                HAL_GPIO_WritePin(GPIOG, GPIO_PIN_5, GPIO_PIN_RESET);
                fire_state = Fire_Ready;
                break;
        }
        osDelay(1);
    }
}