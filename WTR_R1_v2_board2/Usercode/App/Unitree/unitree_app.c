#include "unitree_app.h"

/**
 * @brief   左右侧取苗夹取准备动作
 */
void Unitree_Grip_Ready(void)
{
    unitree_move_smooth(0, unitree_offset_right - _PI - 0.5, 0.35, 0.1);
    unitree_move_smooth(1, unitree_offset_left + _PI + 0.5, 0.35, 0.1);
}

/**
 * @brief   左右侧取苗夹取动作
 */
void Unitree_Grip(void)
{
    unitree_move_smooth(0, unitree_offset_right - _PI + 0.1, 0.35, 0.1);
    unitree_move_smooth(1, unitree_offset_left + _PI - 0.1, 0.35, 0.1);
}

/**
 * @brief   左右侧取苗夹取至存储区动作
 */
void Unitree_Grip_toDeposit(void)
{
    unitree_move_smooth(0, unitree_offset_right, 0.35, 0.1);
    unitree_move_smooth(1, unitree_offset_left, 0.35, 0.1);
}

/**
 * @brief   左右侧取苗默认动作
 */
void Unitree_Grip_Default(void)
{
    unitree_move_smooth(0, unitree_offset_right - _PI / 3, 0.35, 0.1);
    unitree_move_smooth(1, unitree_offset_left + _PI / 3, 0.35, 0.1);
}