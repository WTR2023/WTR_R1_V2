#include "chassis_statemechine.h"
/*********************************************/
#define MOVE_STEPLENGTH 0.005

/*********************************************/
osThreadId_t Grip_Seed_TaskHandle; // 取苗控制线程
const osThreadAttr_t Grip_Seed_Task_attributes = {
    .name       = "Grip_Seed_Task",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};

osThreadId_t Plant_Seed_TaskHandle; // 放苗控制线程
const osThreadAttr_t Plant_Seed_Task_attributes = {
    .name       = "Plant_Seed_Task",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};

/*********************************************/

int land_point[2]    = {0, -705};
int deposit_point[5] = {
    0,
    370,
    640,
    910,
    1120,
}; // 挡板标记点

enum Chassis_State chassis_mode;      // 底盘状态
enum Grip_Seed_State grip_seed_state; // 取苗状态

// 变量定义
float unitree_motor_left_pos;
float unitree_motor_left_kp;
float unitree_motor_left_kw;
float unitree_motor_right_pos;
float unitree_motor_right_kp;
float unitree_motor_right_kw;

int seed_count;

int land_angle          = 0; // 升降电机
int right_deposit_angle = 0; // 右侧挡板电机
int left_deposit_angle  = 0; // 左侧挡板电机
/*********************************************/
/**
 * @brief   等待动作完成函数
 */
static int motor_wait_to_finish(float feedback, float setpoint)
{
    if (((feedback - setpoint) < -2.0f) || ((feedback - setpoint) > 2.0f)) {
        return 0;
    } else {
        return 1;
    }
}

/**
 * @brief   Unitree电机平滑运动处理函数（反丢步 + 平滑）
 * @param   id          电机ID
 * @param   move_angle  旋转目标角度
 * @param   KP          力矩系数
 * @param   KW          阻尼系数
 */
void unitree_move_smooth(int id, float move_angle, float KP, float KW)
{
    int dir; // 方向定义
    int step;
    float delta_angle; // 相距角度，正数为逆时针，负数为顺时针
    float abs_angle;   // 绝对值化后的角度
    float start_point; // 起始点
    // 1. 计算距离
    switch (id) {
        case 0:
            // 记录起始点
            start_point = unitree_motor_right_pos;
            break;
        case 1:
            start_point = unitree_motor_left_pos;
            break;
        default:
            break;
    }
    delta_angle = move_angle - start_point;
    // 2. 角度方向处理
    if (delta_angle < 0.0f) // 顺时针
    {
        abs_angle = -delta_angle;
        dir       = 0;
    } else // 逆时针
    {
        abs_angle = delta_angle;
        dir       = 1;
    }
    // 3. 计算步数
    step = (int)(abs_angle / MOVE_STEPLENGTH) + 1;
    // 4. 根据ID进行移动
    switch (id) {
        case 0: // 右侧电机
            // 设定参数值
            unitree_motor_right_kp = KP;
            unitree_motor_right_kw = KW;
            // 前n步进行依次步进
            for (int i = 0; i < (step - 1); i++) {
                if (dir == 0) {
                    unitree_motor_right_pos = unitree_motor_right_pos - MOVE_STEPLENGTH;
                } else if (dir == 1) {
                    unitree_motor_right_pos = unitree_motor_right_pos + MOVE_STEPLENGTH;
                }
                osDelay(1);
            }
            // 最后一步到达目标点
            unitree_motor_right_pos = move_angle;
            break;
        case 1:
            // 设定参数值
            unitree_motor_left_kp = KP;
            unitree_motor_left_kw = KW;
            // 前n步进行依次步进
            for (int i = 0; i < (step - 1); i++) {
                if (dir == 0) {
                    unitree_motor_left_pos = unitree_motor_left_pos - MOVE_STEPLENGTH;
                } else if (dir == 1) {
                    unitree_motor_left_pos = unitree_motor_left_pos + MOVE_STEPLENGTH;
                }
                osDelay(1);
            }
            // 最后一步到达目标点
            unitree_motor_left_pos = move_angle;
            break;
        default:
            break;
    }
}

/**********************************************/
/**
 * @brief   总状态机线程开启
 */
void Chassis_State_Mechine_Start(void)
{
    chassis_mode         = Seed_Mode; // 默认为苗模式
    grip_seed_state      = Grip_Ready;
    seed_count           = 0; // 取苗架默认为空
    Grip_Seed_TaskHandle = osThreadNew(Grip_Seed_Task, NULL, &Grip_Seed_Task_attributes);
}

/**
 * @brief   取苗状态机
 */
void Grip_Seed_Task(void *argument)
{
    for (;;) {
        switch (grip_seed_state) {
            case Grip_Defalut:
                // 状态机动作执行
                Unitree_Grip_Default();
                Servo_Open();
                land_angle = -2;
                while ((motor_wait_to_finish(hDJI[0].AxisData.AxisAngle_inDegree, land_angle) != 1) || (motor_wait_to_finish(hDJI[2].AxisData.AxisAngle_inDegree, land_angle) != 1)) {
                    osDelay(1);
                }
                // 状态机状态转换
                if (chassis_mode == Seed_Mode) {
                    grip_seed_state = Grip_Ready;
                } else {
                    grip_seed_state = Grip_Defalut;
                }
                break;
            case Grip_Ready:
                // 状态机动作执行
                Unitree_Grip_Ready();
                Servo_Open();
                land_angle = -2;
                while ((motor_wait_to_finish(hDJI[0].AxisData.AxisAngle_inDegree, land_angle) != 1) || (motor_wait_to_finish(hDJI[2].AxisData.AxisAngle_inDegree, land_angle) != 1)) {
                    osDelay(1);
                }
                // 状态机状态转换
                if (chassis_mode == Ball_Mode) {
                    grip_seed_state = Grip_Defalut;
                } else {
                    switch (chassis_message.grip_message) {
                        case 1:
                            grip_seed_state = Grip_Progress;
                            break;
                        case 2:
                            grip_seed_state = Grip_Ready;
                            break;
                        case 3:
                            grip_seed_state = Grip_Ready;
                            break;
                        default:
                            break;
                    }
                }
                break;
            case Grip_Progress:
                // 状态机动作执行
                Unitree_Grip();
                osDelay(500);
                Servo_Grip();
                osDelay(100);
                land_angle = -715;
                while ((motor_wait_to_finish(hDJI[0].AxisData.AxisAngle_inDegree, land_angle) != 1) || (motor_wait_to_finish(hDJI[2].AxisData.AxisAngle_inDegree, land_angle) != 1)) {
                    osDelay(1);
                }
                osDelay(100);
                // 状态机状态转换
                grip_seed_state = Grip_toDeposit;
                break;
            case Grip_toDeposit:
                // 状态机动作执行
                Unitree_Grip_toDeposit();
                osDelay(2000);
                Servo_Open();
                osDelay(1000);
                Unitree_Grip_Ready();
                Servo_Buffle_Close();
                land_angle          = -2;
                right_deposit_angle = deposit_point[1];
                left_deposit_angle  = -deposit_point[1];
                while ((motor_wait_to_finish(hDJI[0].AxisData.AxisAngle_inDegree, land_angle) != 1) ||
                       (motor_wait_to_finish(hDJI[2].AxisData.AxisAngle_inDegree, land_angle) != 1) ||
                       (motor_wait_to_finish(hDJI[1].AxisData.AxisAngle_inDegree, right_deposit_angle) != 1) ||
                       (motor_wait_to_finish(hDJI[3].AxisData.AxisAngle_inDegree, left_deposit_angle) != 1)) {
                    osDelay(1);
                }
                right_deposit_angle = deposit_point[0];
                left_deposit_angle  = deposit_point[0];
                while ((motor_wait_to_finish(hDJI[1].AxisData.AxisAngle_inDegree, right_deposit_angle) != 1) ||
                       (motor_wait_to_finish(hDJI[3].AxisData.AxisAngle_inDegree, left_deposit_angle) != 1)) {
                    osDelay(1);
                }
                seed_count++;
                Servo_Buffle_Open();
                // 状态机状态转换
                grip_seed_state = Grip_Ready;
                break;
            default:
                break;
        }
        osDelay(2);
    }
}