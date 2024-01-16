#include "chassis_start.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    static uint8_t ucTemp = 0;
    if (huart->Instance == OPS_UART) {
        OPS_Decode();
    }
    if (huart->Instance == USART1) {
        AS69_Decode();
    }
    if (huart->Instance == HUART_CURRENT.Instance) {
        WitSerialDataIn(ucTemp);
        HAL_UART_Receive_IT(huart, &ucTemp, 1);
    }
}