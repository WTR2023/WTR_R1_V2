#include "wtr_ops.h"
#include <stdio.h>

OPS_t OPS_Data = {0};
uint8_t ops_buffer[28]; // 接收缓冲区

/****************************************************************/
/**
 * @brief   字符串拼接函数
 */
static void Stract(char strDestination[], char strSource[], int num)
{
    int i = 0, j = 0;
    while (strDestination[i] != '\0') {
        i++;
    }
    for (j = 0; j < num; j++) {
        strDestination[i++] = strSource[j];
    }
}

/**
 * @brief   x坐标更新函数
 * @param   x   新的坐标值
 */
void OPS_Update_X(float x)
{
    int i            = 0;
    char update_x[8] = "ACTX";
    static union {
        float x;
        char data[4];
    } new_set;
    new_set.x = x;
    Stract(update_x, new_set.data, 4);
    for (i = 0; i < 8; i++) {
        HAL_UART_Transmit(&OPS_UART_HANDLE, update_x, 8, 0xFF);
    }
}

/**
 * @brief   y坐标更新函数
 * @param   y   新的坐标值
 */
void OPS_Update_Y(float y)
{
    int i            = 0;
    char update_y[8] = "ACTY";
    static union {
        float y;
        char data[4];
    } new_set;
    new_set.y = y;
    Stract(update_y, new_set.data, 4);
    for (i = 0; i < 8; i++) {
        HAL_UART_Transmit(&OPS_UART_HANDLE, update_y, 8, 0xFF);
    }
}

/**
 * @brief   航向角更新函数
 * @param   angle   新的航向角值
 */
void OPS_Update_A(float angle)
{
    int i            = 0;
    char update_a[8] = "ACTJ";
    static union {
        float angle;
        char data[4];
    } new_set;
    new_set.angle = angle;
    Stract(update_a, new_set.data, 4);
    for (i = 0; i < 8; i++) {
        HAL_UART_Transmit(&OPS_UART_HANDLE, update_a, 8, 0xFF);
    }
}

/**
 * @brief   码盘解码函数
 */
uint8_t OPS_Decode(void)
{
    static union {
        uint8_t data[24];
        float ActVal[6];
    } posture;
    if (ops_buffer[0] != 0x0D || ops_buffer[1] != 0x0A || ops_buffer[26] != 0x0A || ops_buffer[27] != 0x0D) {
        for (int i = 0; i < 28; i++) {
            ops_buffer[i] = 0;
        }
        return 1;
    }
    for (int i = 0; i < 24; i++) {
        posture.data[i] = ops_buffer[i + 2];
    }
    OPS_Data.z_angle = posture.ActVal[0];
    OPS_Data.x_angle = posture.ActVal[1];
    OPS_Data.y_angle = posture.ActVal[2];
    OPS_Data.pos_x   = posture.ActVal[3];
    OPS_Data.pos_y   = posture.ActVal[4];
    OPS_Data.w_z     = posture.ActVal[5];
    return 0;
}

/**
 * @brief   码盘初始化函数
 */
void OPS_Init(void)
{
    HAL_UART_Receive_DMA(&OPS_UART_HANDLE, ops_buffer, 28);
    __HAL_UART_ENABLE_IT(&OPS_UART_HANDLE, UART_IT_IDLE);
}

/**
 * @brief   串口回调函数
 */
__weak void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == OPS_UART) {
        OPS_Decode();
    }
}
