/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "wtr_can.h"
#include "Caculate.h"
#include "DJI.h"
#include <stdio.h>
#include "wtr_fire_ball.h"
#include "wtr_chassis.h"
#include "wtr_as69.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
osThreadId_t fireTaskHandle;
const osThreadAttr_t fireTask_attributes = {
    .name       = "fireTask",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};

osThreadId_t chassisTaskHandle;
const osThreadAttr_t chassisTask_attributes = {
    .name       = "chassisTask",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};

osThreadId_t can_message_TaskHandle;
const osThreadAttr_t can_message_Task_attributes = {
    .name       = "can_message_Task",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityHigh1,
};

osThreadId_t as69_TaskHandle;
const osThreadAttr_t as69_Task_attributes = {
    .name       = "as69_Task",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityHigh1,
};
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name       = "defaultTask",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void FireTask(void *argument);
void Chassis_Task(void *argument);
void CAN_Message_Task(void *argument);
void AS69_Task(void *argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* creation of defaultTask */
    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    fireTaskHandle         = osThreadNew(FireTask, NULL, &fireTask_attributes);
    chassisTaskHandle      = osThreadNew(Chassis_Task, NULL, &chassisTask_attributes);
    can_message_TaskHandle = osThreadNew(CAN_Message_Task, NULL, &can_message_Task_attributes);
    as69_TaskHandle        = osThreadNew(AS69_Task, NULL, &as69_Task_attributes);
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    for (;;) {
        osDelay(1);
    }
    /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/**
 * @brief  取球发射电机控制线程
 * @param  argument: Not used
 * @retval None
 */
void CAN_Message_Task(void *argument)
{
    for (;;) {
        speedServo(v_1, &hDJI[0]);
        speedServo(v_2, &hDJI[1]);
        speedServo(v_3, &hDJI[2]);
        speedServo(v_4, &hDJI[3]);
        positionServo(arm_angle, &hDJI[4]);
        positionServo(rail_angle, &hDJI[5]);
        speedServo(5000, &hDJI[6]);
        speedServo(-5000, &hDJI[7]);
        CanTransmit_DJI_1234(hDJI[0].speedPID.output, hDJI[1].speedPID.output, hDJI[2].speedPID.output, hDJI[3].speedPID.output);
        CanTransmit_DJI_5678(hDJI[4].speedPID.output, hDJI[5].speedPID.output, 0, 0);
        osDelay(5);
    }
}

/**
 * @brief  摩擦轮发射线程
 * @param  argument: Not used
 * @retval None
 */
void FireTask(void *argument)
{
    osDelay(500);
    Arm_Grip_Action();
    for (;;) {

        osDelay(5);
    }
}

/**
 * @brief  底盘控制线程
 * @param  argument: Not used
 * @retval None
 */
void Chassis_Task(void *argument)
{
    for (;;) {
        float mvx, mvy, mwc;
        mvx = -(float)((raw_data.ch1 - 1024) * 66) / 4000.0;
        mvy = (float)((raw_data.ch0 - 1024) * 66) / 4000.0;
        mwc = 0;
        Inverse_kinematic_equation(mvx, mvy, mwc, &v_1, &v_2, &v_3, &v_4);
        osDelay(5);
    }
}

/**
 * @brief  遥控器解码线程
 * @param  argument: Not used
 * @retval None
 */
void AS69_Task(void *argument)
{
    HAL_UART_Receive_DMA(&huart1, as69_buffer, 18);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    for (;;) {
        osDelay(1);
    }
}

/* USER CODE END Application */
