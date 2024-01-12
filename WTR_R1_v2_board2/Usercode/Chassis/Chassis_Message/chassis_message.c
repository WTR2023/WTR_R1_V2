#include "chassis_message.h"

uint8_t board_message_buffer[7]; // 6字节接收数据
Chassis_Message chassis_message = {0};

int Board_Message_Decode(void)
{
    if (board_message_buffer[0] != 0x0B || board_message_buffer[1] != 0x0C || board_message_buffer[5] != 0x0C || board_message_buffer[6] != 0x0B) {
        for (int i = 0; i < 6; i++) {
            board_message_buffer[i] = 0;
        }
        return 1;
    }
    chassis_message.grip_message         = board_message_buffer[2];
    chassis_message.direction_message    = board_message_buffer[3];
    chassis_message.grip_resolve_message = board_message_buffer[4];
    return 0;
}

/**
 * @brief   板间通讯初始化
 */
void Board_Message_Init(void)
{
    HAL_UART_Receive_DMA(&BOARD_MESSAGE_UART_HANDLE, board_message_buffer, 7);
    __HAL_UART_ENABLE_IT(&BOARD_MESSAGE_UART_HANDLE, UART_IT_IDLE);
}

/**
 * @brief   串口回调函数
 */
__weak void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == BOARD_MESSAGE_UART) {
        Board_Message_Decode();
    }
}