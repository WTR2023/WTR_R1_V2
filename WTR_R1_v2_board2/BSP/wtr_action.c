#include "wtr_action.h"

float land_angle = 0;

/**
 * @brief   右机械臂动作流程
 */
void Right_Grip_Seedling(void)
{
    // 1. 机械臂张开
    Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, unitree_offset_right - _PI - 0.07, 0.11, 0.05);
    osDelay(100);
    Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, unitree_offset_right - _PI - 0.07, 0.11, 0.05);
    osDelay(100);
    // 2. 夹爪张开
    Servo_Open();
    osDelay(3000);
    // 3. 抓取
    Servo_Grip();
    land_angle = -685;
    while (hDJI[0].AxisData.AxisAngle_inDegree > -684.0f) {
        osDelay(1);
    }
    // 4. 机械臂回收
    Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, unitree_offset_right + 0.23, 0.11, 0.05);
    osDelay(100);
    Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, unitree_offset_right + 0.23, 0.11, 0.05);
    osDelay(100);
    osDelay(3000);
    Servo_Open();
    osDelay(500);
    Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, unitree_offset_right - _PI - 0.07, 0.11, 0.05);
    osDelay(100);
    Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, unitree_offset_right - _PI - 0.07, 0.11, 0.05);
    osDelay(100);
}