/*
 * robo_tasks.cpp
 * 
 * Created : **
 *  Author : n-is
 *   email : 073bex422.nischal@pcampus.edu.np
 */

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"

#include "robot.h"

/* Export Functions Used in C */
extern "C" void StartDefaultTask(void const *argument);
extern "C" void RobotThread(void const *argument);
extern "C" void LoggingThread(void const *argument);
extern "C" void MotorThread(void const *argument);

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument)
{
        /* Init code for USB_DEVICE */
        // MX_USB_DEVICE_Init();

        /* USER CODE BEGIN StartDefaultTask */
        /* Infinite loop */
        for (;;)
        {
                osDelay(100000);
        }
        /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_RobotThread */
/**
 * @brief Function implementing the RoboSequence thread.
 * @param argument: Not used
 * @retval None
 * 
 * @note This thread is responsible for reading the current state of the robot.
 * 
 *
 * <pre>
 * Tasks performed by this thread :
 * 1) Read orientation given by the OrientationSensor based on the State the
 *    robot is in.
 * 2) Read position given by the PositionSensor based on the State the robot is
 *    in.
 * 3) Filter Orientation data.
 *      3.1) Filter roll
 *      3.2) Filter roll compensated pitch
 *      3.3) Filter tilt compensated pitch
 * 4) Rotate the measured position from body frame to the field frame
 * 5) Filter Position Data
 * 
 * 6) Find the robot's state based on it's current position
 * 7) Calculate the robot's angle of attack to reach the next state
 * 8) Calculate the robot's velocity according to it's state
 * 9) Calculate the correction angular velocity of the robot
 * 10) Calculate the omegas of each wheel
 * 
 * 11) Measure omega of each wheels
 * 12) Compute error
 * 13) Compute PID using available PID_Algorithm
 * 14) set Omega of each wheel
 * 15) update new omegas of all wheels at once
 * </pre>
 */
/* USER CODE END Header_RobotThread */
#include "arduino.h"
#include "usart.h"
#include "error.h"
#include "i2c.h"
#include "devs_config.h"

#include "all_tests.h"


Robot &Khangai_Robot = Robot::get_Instance();
GameField gCurrent_Field = GameField::NONE;

void RobotThread(void const *argument)
{
        /* USER CODE BEGIN RobotThread */
        uint32_t sample_period = 10;
        
        // print_I2C_Addresses(&hi2c1);
        Khangai_Robot.read_Field();
        Khangai_Robot.init(sample_period);

        uint32_t dt = HAL_GetTick();
        uint32_t dt_tmp = HAL_GetTick();
        uint32_t last_run_time = 0;

        HAL_GPIO_WritePin(B_BlueLED_GPIO_Port, B_BlueLED_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(B_RedLED_GPIO_Port, B_RedLED_Pin, GPIO_PIN_RESET);

        HAL_GPIO_WritePin(B_OrangeLED_GPIO_Port, B_OrangeLED_Pin, GPIO_PIN_SET);
        osDelay(sample_period);

        // Khangai_Robot.check_Actuators();

        /* Infinite loop */
        for (;;)
        {
                // Since this is the highest priority task, we can be sure that
                // another task won't start when this task is running
                dt_tmp = HAL_GetTick();
                dt = dt_tmp - dt;

                Khangai_Robot.update(sample_period);

                // for (uint32_t i = 0; i < 100; ++i) {
                //         printf("%ld, %ld\n", dt + last_run_time, i);
                // }
  
                dt = HAL_GetTick();
                dt_tmp = dt - dt_tmp;
                last_run_time = dt_tmp;

                // Check for timing Issues
                if (last_run_time > sample_period / 2) {
                        // Timing Issue Occured since run time is more than half
                        // of sample time
                }

                // Sleep for remaining time of the sampling period if there is
                // time left
                if (dt_tmp < sample_period) {
                        osDelay(sample_period - dt_tmp);
                }
        }
        /* USER CODE END RobotThread */
}

/* USER CODE BEGIN Header_MotorThread */
/**
* @brief Function implementing the MotorSequence thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MotorThread */
void MotorThread(void const *argument)
{
        /* USER CODE BEGIN MotorThread */
        uint32_t sample_period = 10;
        

        uint32_t dt = HAL_GetTick();
        uint32_t dt_tmp = HAL_GetTick();
        uint32_t last_run_time = 0;
        /* Infinite loop */
        for (;;)
        {
                if (Khangai_Robot.is_Initiated()) {
                        // Since this is the highest priority task, we can be sure that
                        // another task won't start when this task is running
                        dt_tmp = HAL_GetTick();
                        dt = dt_tmp - dt;

                        Khangai_Robot.run(sample_period);

                        // for (uint32_t i = 0; i < 100; ++i) {
                        //         printf("%ld, %ld\n", dt + last_run_time, i);
                        // }

                        dt = HAL_GetTick();
                        dt_tmp = dt - dt_tmp;
                        last_run_time = dt_tmp;

                        // Check for timing Issues
                        if (last_run_time > sample_period / 2) {
                                // Timing Issue Occured since run time is more than half
                                // of sample time
                        }

                        // Sleep for remaining time of the sampling period if there is
                        // time left
                        if (dt_tmp < sample_period) {
                                osDelay(sample_period - dt_tmp);
                        }
                }
        }
        /* USER CODE END MotorThread */
}

/* USER CODE BEGIN Header_LoggingThread */
/**
* @brief Function implementing the logging thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LoggingThread */
void log_data();

void LoggingThread(void const *argument)
{
        /* USER CODE BEGIN LoggingThread */
        __HAL_UART_ENABLE_IT(&huart2, UART_IT_TC);
        /* Infinite loop */
        for (;;)
        {
                log_data();
                asm volatile("nop");
        }
        /* USER CODE END LoggingThread */
}
