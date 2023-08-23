/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "soft_user.h"
#include "string.h"
#include "JY61.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct Car_status
{
    uint16_t Car_speed[4];
    float IMU[3];
    uint16_t ditance_x;
    uint16_t ditance_y;
    uint8_t task;
}Car_status;

char task_test_text[]="�򵥵Ĳ�������";
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Led_Toggle GPIOC->ODR^=GPIO_PIN_13
#define IMU_speed 80
#define task_test 0X00
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern char s_cDataUpdate, s_cCmd;

uint8_t OS_status=0;
uint8_t debug_date[400]={0};
uint8_t IMU_date[IMU_speed]={0};

extern int f_sprintf(char *out, const char *format, ...);

Car_status car_status;

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
SemaphoreHandle_t IMUdate_RX_Sem_Handle =NULL;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTask_keyHandle;
osThreadId myTask_ledHandle;
osThreadId myTask_IMUHandle;
osThreadId myTask_debugHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void key_Task(void const * argument);
void led_Task(void const * argument);
void IMU_Task(void const * argument);
void deubg_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
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
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
    IMUdate_RX_Sem_Handle = xSemaphoreCreateBinary();	 
  if(NULL != IMUdate_RX_Sem_Handle)
    printf("IMUdate_RX_Sem_Handle��ֵ�ź��������ɹ�!\r\n");
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityIdle, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask_key */
  osThreadDef(myTask_key, key_Task, osPriorityAboveNormal, 0, 128);
  myTask_keyHandle = osThreadCreate(osThread(myTask_key), NULL);

  /* definition and creation of myTask_led */
  osThreadDef(myTask_led, led_Task, osPriorityIdle, 0, 128);
  myTask_ledHandle = osThreadCreate(osThread(myTask_led), NULL);

  /* definition and creation of myTask_IMU */
  osThreadDef(myTask_IMU, IMU_Task, osPriorityAboveNormal, 0, 128);
  myTask_IMUHandle = osThreadCreate(osThread(myTask_IMU), NULL);

  /* definition and creation of myTask_debug */
  osThreadDef(myTask_debug, deubg_Task, osPriorityLow, 0, 128);
  myTask_debugHandle = osThreadCreate(osThread(myTask_debug), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

    OS_status=1;
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
      Led_Toggle;
    osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_key_Task */
/**
* @brief Function implementing the myTask_key thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_key_Task */
void key_Task(void const * argument)
{
  /* USER CODE BEGIN key_Task */
  /* Infinite loop */
  for(;;)
  {
      if((GPIOA->IDR&(1<<15))==0)
      {
          osDelay(10);
          if((GPIOA->IDR&(1<<15))==0)
          {
              while((GPIOA->IDR&(1<<15))==0);
              /*do something*/
              printf("��������\n");
              
              
              
          }     
      }
    osDelay(1);
  }
  /* USER CODE END key_Task */
}

/* USER CODE BEGIN Header_led_Task */
/**
* @brief Function implementing the myTask_led thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_led_Task */
void led_Task(void const * argument)
{
  /* USER CODE BEGIN led_Task */
  /* Infinite loop */
  for(;;)
  {
      GPIOC->ODR^=GPIO_PIN_12;
    osDelay(100);
  }
  /* USER CODE END led_Task */
}

/* USER CODE BEGIN Header_IMU_Task */
/**
* @brief Function implementing the myTask_IMU thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_IMU_Task */
void IMU_Task(void const * argument)
{
  /* USER CODE BEGIN IMU_Task */
  /* Infinite loop */
    BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    int i;
  for(;;)
  {
      //��ȡ��ֵ�ź��� xSemaphore,û��ȡ����һֱ�ȴ�
	xReturn = xSemaphoreTake(IMUdate_RX_Sem_Handle,/* ��ֵ�ź������ */
                              portMAX_DELAY); /* �ȴ�ʱ�� */

    for(i=0;i<IMU_speed;i++)
      {
          WitSerialDataIn(IMU_date[i]);
      }
    if(s_cDataUpdate)
		{
			for(i = 0; i < 3; i++)
			{				
				car_status.IMU[i] = sReg[Roll+i] / 32768.0f * 180.0f;                
			}
            
		}
    HAL_UART_Receive_DMA(&huart2,IMU_date,IMU_speed);
    
  }
  /* USER CODE END IMU_Task */
}

/* USER CODE BEGIN Header_deubg_Task */
/**
* @brief С��״̬�ϴ�����ʹ�ô���һDMA���ͣ���������1000ms
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_deubg_Task */
void deubg_Task(void const * argument)
{
  /* USER CODE BEGIN deubg_Task */
  /* Infinite loop */
  for(;;)
  {
      char task_description[20];

      switch(car_status.task)
      {
          case task_test:
              strcpy(task_description, task_test_text); 
          break;
          default:
              /*error*/
          break;
      }
      
      sprintf((char*)debug_date,"����λ�ã�X��%dcm  Y:%dcm\n\
                               \n����״̬�� Pitch:%.3f   Roll:%.3f    Yaw:%.3f\n\
                               \n����ת�٣�\
                               \nMotor1:%d     Motor1:%d \
                               \nMotor3:%d     Motor4:%d\n\
                               \n��ǰ����: %s\n\n\n",\
                                car_status.ditance_x,car_status.ditance_y,car_status.IMU[0],car_status.IMU[1],car_status.IMU[2],\
                                car_status.Car_speed[0],car_status.Car_speed[1],\
                                car_status.Car_speed[2],car_status.Car_speed[3],task_description);
      HAL_UART_Transmit_DMA(&huart1,debug_date,strlen((char*)debug_date));
      osDelay(1000);
  }
  /* USER CODE END deubg_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
//      sprintf((char*)debug_date,"����λ�ã�X��%dcm  Y:%dcm\n\
//                               \n����״̬�� Pitch:%.3f   Roll:%.3f    Yaw:%.3f\n\
//                               \n����ת�٣�\
//                               \nMotor1:%d     Motor1:%d \
//                               \nMotor3:%d     Motor4:%d\n\
//                               \n��ǰ����: %s\n\n\n",\
//                                car_status.ditance_x,car_status.ditance_y,car_status.IMU[0],car_status.IMU[1],car_status.IMU[2],\
//                                car_status.Car_speed[0],car_status.Car_speed[1],car_status.Car_speed[2],car_status.Car_speed[3],task_description);
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
