/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
uint32_t defaultTaskBuffer[256];
osStaticThreadDef_t defaultTaskControlBlock;
osThreadId RoboSequenceHandle;
uint32_t RoboSequenceBuffer[2048];
osStaticThreadDef_t RoboSequenceControlBlock;
osThreadId loggingHandle;
uint32_t loggingBuffer[2048];
osStaticThreadDef_t loggingControlBlock;
osThreadId MotorSequenceHandle;
uint32_t MotorSequenceBuffer[ 2028 ];
osStaticThreadDef_t MotorSequenceControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const *argument);
void RobotThread(void const *argument);
void LoggingThread(void const *argument);
void MotorThread(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
        *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
        *ppxIdleTaskStackBuffer = &xIdleStack[0];
        *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
        /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

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

        /* Create the thread(s) */
        /* definition and creation of defaultTask */
        osThreadStaticDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 256, defaultTaskBuffer, &defaultTaskControlBlock);
        defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

        /* definition and creation of RoboSequence */
        osThreadStaticDef(RoboSequence, RobotThread, osPriorityRealtime, 0, 2048, RoboSequenceBuffer, &RoboSequenceControlBlock);
        RoboSequenceHandle = osThreadCreate(osThread(RoboSequence), NULL);

        /* definition and creation of logging */
        osThreadStaticDef(logging, LoggingThread, osPriorityLow, 0, 2048, loggingBuffer, &loggingControlBlock);
        loggingHandle = osThreadCreate(osThread(logging), NULL);

        /* definition and creation of MotorSequence */
        osThreadStaticDef(MotorSequence, MotorThread, osPriorityRealtime, 0, 2028, MotorSequenceBuffer, &MotorSequenceControlBlock);
        MotorSequenceHandle = osThreadCreate(osThread(MotorSequence), NULL);

        /* USER CODE BEGIN RTOS_THREADS */
        /* add threads, ... */
        /* USER CODE END RTOS_THREADS */

        /* USER CODE BEGIN RTOS_QUEUES */
        /* add queues, ... */
        /* USER CODE END RTOS_QUEUES */
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
