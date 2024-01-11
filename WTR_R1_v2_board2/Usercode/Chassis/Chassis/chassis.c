#include "chassis.h"

/************************************************************************/
osThreadId_t can_message_TaskHandle;
const osThreadAttr_t can_message_Task_attributes = {
    .name       = "can_message_Task",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityHigh1,
};
/************************************************************************/
/**
 * @brief   底盘初始化
 */
void Chassis_Init(void)
{
    can1.CAN_Rx_Filter_Init();
    can1.CAN_Start();
    can1.CAN_Interrupt_Init();
    hDJI[0].motorType = M2006;
    DJI_Init();
}

/**
 * @brief   底盘电机CAN消息发送线程创建
 */
void Chassis_CAN_Message_TaskStart(void)
{
    can_message_TaskHandle = osThreadNew(CAN_Message_Task, NULL, &can_message_Task_attributes);
}

/**
 * @brief   底盘电机CAN消息发送线程
 */
void CAN_Message_Task(void *argument)
{
    for (;;) {
        positionServo(land_angle, &hDJI[0]);
        CanTransmit_DJI_1234(hDJI[0].speedPID.output, 0, 0, 0);
    }
}