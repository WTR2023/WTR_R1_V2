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
    Chassis_Init();            // DJI电机初始化
    Chassis_Joytick_Init();    // 遥控器初始化
    Chassis_Perception_Init(); // 码盘初始化
    Servo_Init();              // 舵机初始化
    osDelay(3000);             // 等待外设初始化结束
    /***************************线程开启*************************/
    Chassis_CAN_Message_TaskStart(); // CAN发送线程
    Chassis_Remotectl_TaskStart();   // 遥控器线程
    Debug_TaskStart();               // 调试线程
    for (;;) {
        osDelay(1000);
    }
}