/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "wtr_servo.h"
#include "wtr_can.h"
#include "DJI.h"
#include "Caculate.h"
#include "Unitree_user.h"
#include "wtr_action.h"
#include <stdio.h>
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
osThreadId_t can_message_TaskHandle;
const osThreadAttr_t can_message_Task_attributes = {
    .name       = "can_message_Task",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityHigh1,
};

osThreadId_t seeding_TaskHandle;
const osThreadAttr_t seeding_Task_attributes = {
    .name       = "seeding_Task",
    .stack_size = 128 * 4,
    .priority   = (osPriority_t)osPriorityNormal,
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
void CAN_Message_Task(void *argument);
void Seeding_Task(void *argument);
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
    osDelay(5000);
    // 1. Servo init
    Servo_Init();
    Servo_Open();
    // 2. DJI Init
    can1.CAN_Rx_Filter_Init();
    can1.CAN_Start();
    can1.CAN_Interrupt_Init();
    hDJI[0].motorType = M2006;
    DJI_Init();
    // 3. Unitree init
    unitree_motor_right = Unitree_Create_Motor();
    Unitree_init(unitree_motor_right, &huart6);
    osDelay(2000);
    Unitree_UART_tranANDrev(unitree_motor_right, 0, 0, 0, 0, 0, 0, 0);
    osDelay(2000);
    Unitree_UART_tranANDrev(unitree_motor_right, 0, 0, 0, 0, 0, 0, 0);
    osDelay(2000);
    unitree_offset_right = unitree_motor_right->data.Pos;
    // 4. start task
    can_message_TaskHandle = osThreadNew(CAN_Message_Task, NULL, &can_message_Task_attributes);
    seeding_TaskHandle     = osThreadNew(Seeding_Task, NULL, &seeding_Task_attributes);
    for (;;) {
        printf("%d\r\n", (int)(unitree_motor_right->data.Pos * 100));
        osDelay(2);
    }
    /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/**
 * @brief  CAN send thread
 * @param  argument: Not used
 * @retval None
 */
void CAN_Message_Task(void *argument)
{
    for (;;) {
        positionServo(land_angle, &hDJI[0]);
        CanTransmit_DJI_1234(hDJI[0].speedPID.output, 0, 0, 0);
        osDelay(5);
    }
}

/**
 * @brief
 * @param  argument: Not used
 * @retval None
 */
void Seeding_Task(void *argument)
{
    osDelay(500);
    Right_Grip_Seedling();
    for (;;) {
        osDelay(5);
    }
}
/* USER CODE END Application */
