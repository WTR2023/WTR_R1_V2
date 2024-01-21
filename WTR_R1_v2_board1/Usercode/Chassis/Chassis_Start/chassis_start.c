/**
 * @brief   底盘启动线程
 */
#include "chassis_start.h"

/**
 * @brief   底盘总初始化段
 */
void StartDefaultTask(void *argument)
{
    static int cnt = 0;
    /**********************外设及硬件初始化***********************/
    Chassis_Init();         // DJI电机初始化
    Chassis_Joytick_Init(); // 遥控器初始化
    HW101_Init();           // 陀螺仪初始化
    Servo_Init();           // 舵机初始化
    while (cnt <= 5000) {
        cnt++;
        if (cnt % 1000 == 0) {
            HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_1);
        }
        osDelay(1);
    }
    Chassis_Perception_Init(); // 码盘初始化
    /***************************线程开启*************************/
    Chassis_Gyro_TaskStart(); // 陀螺仪线程
    while (chassis_gyro_state != 1) {
        osDelay(1);
    }
    Chassis_CAN_Message_TaskStart(); // CAN发送线程
    Chassis_Remotectl_TaskStart();   // 遥控器线程
    Chassis_State_Mechine_Start();   // 状态机线程
    Chassis_Message_TaskStart();     // 板间通讯线程
    Debug_TaskStart();               // 调试线程
    for (;;) {
        osDelay(1000);
    }
}