#ifndef __CHASSIS_MESSAGE_H
#define __CHASSIS_MESSAGE_H

#include "chassis_start.h"

typedef struct Chassis_Message {
    uint8_t grip_message; // 夹取信息
} Chassis_Message;

/************************ 用户定义 ***************************/
#define BOARD_MESSAGE_UART_HANDLE huart8 // 串口选项
#define BOARD_MESSAGE_UART        UART8  // 串口选项

extern uint8_t board_message_buffer[6];
extern Chassis_Message chassis_message;

int Board_Message_Decode(void);

#endif