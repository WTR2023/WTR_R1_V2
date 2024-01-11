/**
 * @brief   底盘启动线程
 */
#include "chassis_start.h"

/**
 * @brief   底盘总初始化段
 */
void StartDefaultTask(void *argument)
{
    /**********************外设及硬件初始化***********************/
    Unitree_User_Init(); // Unitree电机初始化
    Servo_Init();        // 舵机初始化
    Chassis_Init();      // DJI 电机初始化
    /***************************线程开启*************************/

    Chassis_CAN_Message_TaskStart();    // CAN消息发送线程
    for (;;) {
        osDelay(1000);
    }
}