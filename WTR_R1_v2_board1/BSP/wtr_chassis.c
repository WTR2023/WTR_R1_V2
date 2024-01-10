#include "wtr_chassis.h"
#include "math.h"

#define R               0.076 // 全向轮半径
#define r               0.297 // 底盘半径
#define PI              3.1415926
#define Reduction_ratio (3591.0f / 187.0f) // M3508减速比

int32_t v_1, v_2, v_3, v_4; // 底盘速度

/**
 * @brief   逆运动学方程，输入速度求解电机转速
 * @param   vx          底盘前向速度，单位m/s
 * @param   vy          底盘左横向速度，单位m/s
 * @param   wc          转向角速度，使用弧度，单位rad/s
 */
void Inverse_kinematic_equation(float vx, float vy, float wc, int32_t *_v_1, int32_t *_v_2, int32_t *_v_3, int32_t *_v_4)
{
    float v1, v2, v3, v4;
    v1 = (float)((sqrt(2) * vx / 2 - sqrt(2) * vy / 2 - wc * r) / R);
    v2 = (float)((-sqrt(2) * vx / 2 - sqrt(2) * vy / 2 - wc * r) / R);
    v3 = (float)((-sqrt(2) * vx / 2 + sqrt(2) * vy / 2 - wc * r) / R);
    v4 = (float)((sqrt(2) * vx / 2 + sqrt(2) * vy / 2 - wc * r) / R);

    *_v_1 = (int32_t)(v1 * 60.0) / (2 * PI);
    *_v_2 = (int32_t)(v2 * 60.0) / (2 * PI);
    *_v_3 = (int32_t)(v3 * 60.0) / (2 * PI);
    *_v_4 = (int32_t)(v4 * 60.0) / (2 * PI);
}
/**
 * @brief   正运动学方程，返回轮式里程计的速度数据
 * @param   vx          底盘前向速度，单位m/s
 * @param   vy          底盘左横向速度，单位m/s
 * @param   wc          转向角速度，使用弧度，单位rad/s
 */
void Forward_kinematic_equation(float *vx, float *vy, float *wc)
{
}