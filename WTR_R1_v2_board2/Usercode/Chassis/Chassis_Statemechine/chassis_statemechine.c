#include "chassis_statemechine.h"

/*********************************************************************/

osThreadId_t Right_Grip_Seed_TaskHandle;
const osThreadAttr_t Right_Grip_Seed_Task_attributes = {
    .name       = "Right_Grip_Seed_Task",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};

enum Grip_Seed_State right_grip_seed_state;
int land_angle;

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
                Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, unitree_offset_right - _PI - 0.07, 0.11, 0.05);
                land_angle = -2;
                while (hDJI[0].AxisData.AxisAngle_inDegree < -3.0f) {
                    osDelay(1);
                }
                Right_Servo_Open();
                osDelay(2000);
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
                break;
            case Grip: // 2. 取苗并准备存储
                Right_Servo_Grip();
                land_angle = -685;
                while (hDJI[0].AxisData.AxisAngle_inDegree > -684.0f) {
                    osDelay(1);
                }
                osDelay(300);
                right_grip_seed_state = Grip_Deposit;
                break;
            case Grip_Deposit: // 3. 苗的存储
                Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, unitree_offset_right + 0.23, 0.11, 0.05);
                osDelay(2000);
                Right_Servo_Open();
                osDelay(100);
                right_grip_seed_state = Grip_Ready;
                break;
            default:
                break;
        }
        osDelay(2);
    }
}