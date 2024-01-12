#include "chassis_statemechine.h"

/*********************************************************************/

osThreadId_t Right_Grip_Seed_TaskHandle;
const osThreadAttr_t Right_Grip_Seed_Task_attributes = {
    .name       = "Right_Grip_Seed_Task",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};

enum Grip_Seed_State right_grip_seed_state;
const int deposit_point[2] = {0, 540}; // 右侧挡板标记点
int right_land_angle;                  // 右侧升降电机
int right_deposit_angle;

/*********************************************************************/

/**
 * @brief   取苗状态机线程开启
 */
void Grip_Seed_Task_Start(void)
{
    Right_Grip_Seed_TaskHandle = osThreadNew(Right_Grip_Seed_Task, NULL, &Right_Grip_Seed_Task_attributes);
}

/**
 * @brief   总状态机线程开启
 */
void Chassis_State_Mechine_Start(void)
{
    Grip_Seed_Task_Start();
}

/**
 * @brief   取苗状态机
 */
void Right_Grip_Seed_Task(void *argument)
{
    for (;;) {
        switch (right_grip_seed_state) {
            case Grip_Ready: // 1. 取苗准备
                Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, unitree_offset_right - _PI - 0.2, 0.11, 0.05);
                right_land_angle = -2;
                while (hDJI[0].AxisData.AxisAngle_inDegree < -3.0f) {
                    osDelay(1);
                }
                Right_Servo_Open();
                osDelay(2000);
                if (chassis_message.direction_message == 0xAA && chassis_message.grip_resolve_message == 0xAA) {
                    switch (chassis_message.grip_message) {
                        case 1:
                            right_grip_seed_state = Grip;
                            break;
                        case 2:
                            right_grip_seed_state = Grip_Ready;
                            break;
                        case 3:
                            right_grip_seed_state = Grip_Ready;
                            break;
                        default:
                            break;
                    }
                }
                break;
            case Grip: // 2. 取苗并准备存储
                Right_Servo_Grip();
                osDelay(100);
                right_land_angle = -685;
                while (hDJI[0].AxisData.AxisAngle_inDegree > -684.0f) {
                    osDelay(1);
                }
                osDelay(300);
                if (chassis_message.direction_message == 0xAA && chassis_message.grip_resolve_message == 0xAA) {
                    switch (chassis_message.grip_message) {
                        case 2:
                            right_grip_seed_state = Grip_Deposit;
                            break;
                        default:
                            right_grip_seed_state = Grip;
                            break;
                    }
                }
                break;
            case Grip_Deposit: // 3. 苗的存储
                Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, unitree_offset_right + 0.25, 0.11, 0.05);
                osDelay(3000);
                Right_Servo_Open();
                osDelay(1000);
                Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, unitree_offset_right - _PI - 0.2, 0.11, 0.05);
                osDelay(2000);
                right_deposit_angle = deposit_point[1];
                while (hDJI[1].AxisData.AxisAngle_inDegree < (deposit_point[1] - 2.0f)) {
                    osDelay(1);
                }
                right_deposit_angle = deposit_point[0];
                while (hDJI[1].AxisData.AxisAngle_inDegree > (deposit_point[0] + 2.0f)) {
                    osDelay(1);
                }
                right_grip_seed_state = Grip_Ready;
                break;
            default:
                break;
        }

        osDelay(2);
    }
}