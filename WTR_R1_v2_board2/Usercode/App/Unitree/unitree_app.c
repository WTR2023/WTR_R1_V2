#include "unitree_app.h"

/**
 * @brief   左右侧取苗夹取准备动作
 */
void Unitree_Grip_Ready(void)
{
    Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, (unitree_offset_right - _PI - 0.6), 0.08, 0.06);
    osDelay(100);
    Unitree_UART_tranANDrev(unitree_motor_left, 1, 1, 0, 0, (unitree_offset_left + _PI + 0.5), 0.08, 0.06);
    osDelay(100);
    Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, (unitree_offset_right - _PI - 0.6), 0.08, 0.06);
    osDelay(100);
    Unitree_UART_tranANDrev(unitree_motor_left, 1, 1, 0, 0, (unitree_offset_left + _PI + 0.5), 0.08, 0.06);
    osDelay(100);
}

/**
 * @brief   左右侧取苗夹取动作
 */
void Unitree_Grip(void)
{
    Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, (unitree_offset_right - _PI + 0.08), 0.15, 0.03);
    osDelay(100);
    Unitree_UART_tranANDrev(unitree_motor_left, 1, 1, 0, 0, (unitree_offset_left + _PI - 0.08), 0.15, 0.03);
    osDelay(100);
    Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, (unitree_offset_right - _PI + 0.08), 0.15, 0.03);
    osDelay(100);
    Unitree_UART_tranANDrev(unitree_motor_left, 1, 1, 0, 0, (unitree_offset_left + _PI - 0.08), 0.15, 0.03);
    osDelay(100);
}

/**
 * @brief   左右侧取苗夹取至存储区动作
 */
void Unitree_Grip_toDeposit(void)
{
    Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, (unitree_offset_right + 0.3), 0.09, 0.06);
    osDelay(100);
    Unitree_UART_tranANDrev(unitree_motor_left, 1, 1, 0, 0, (unitree_offset_left - 0.3), 0.09, 0.06);
    osDelay(100);
    Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, (unitree_offset_right + 0.3), 0.09, 0.06);
    osDelay(100);
    Unitree_UART_tranANDrev(unitree_motor_left, 1, 1, 0, 0, (unitree_offset_left - 0.3), 0.09, 0.06);
    osDelay(100);
}

/**
 * @brief   左右侧取苗默认动作
 */
void Unitree_Grip_Default(void)
{
    Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, (unitree_offset_right - _PI / 6 - 0.5), 0.09, 0.05);
    osDelay(100);
    Unitree_UART_tranANDrev(unitree_motor_left, 1, 1, 0, 0, (unitree_offset_left + _PI / 6 + 0.5), 0.09, 0.05);
    osDelay(100);
    Unitree_UART_tranANDrev(unitree_motor_right, 0, 1, 0, 0, (unitree_offset_right - _PI / 6 - 0.5), 0.09, 0.05);
    osDelay(100);
    Unitree_UART_tranANDrev(unitree_motor_left, 1, 1, 0, 0, (unitree_offset_left + _PI / 6 + 0.5), 0.09, 0.05);
    osDelay(100);
}