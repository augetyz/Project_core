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
#include "servo.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "motor_ctrl.h"
#include "pid.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Led_Toggle GPIOC->ODR ^= GPIO_PIN_13 // LED_Toggle�����궨��
#define IMU_speed 80                         // IMU���ݲɼ�����Ƶ�ʿ��Ʋ���

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart6_rx;

extern char s_cDataUpdate, s_cCmd; // MPU6050��ֲ���������ù�

uint8_t OS_status = 0;             // �����ʼ����־λ��Ϊ1���ʼ��OK��Ϊ0��û����

uint8_t debug_date[600] = {0};     // debug���ݻ����������ڻ�����Ҫ�����ϴ���debug���ݡ�

uint8_t IMU_date[IMU_speed] = {0}; // MPU6050���ݻ�����������DMA+���ڷ�ʽ����



Car_status car_status; // С��״̬�ṹ���ʼ��

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
SemaphoreHandle_t IMUdate_RX_Sem_Handle = NULL; // �ź�����ʼ����������ΪIMU���ݽ�����ɺ�Ļ��죬����IMU_TASK������ִ�С�
QueueHandle_t   Speed_Queue =NULL;
QueueHandle_t   goal_Queue =NULL;
QueueHandle_t   IMU_Queue =NULL;
QueueHandle_t   debug_Queue =NULL;
QueueHandle_t   angle_Queue =NULL;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTask_keyHandle;
osThreadId myTask_ledHandle;
osThreadId myTask_IMUHandle;
osThreadId myTask_debugHandle;
osThreadId myTask_motorHandle;
osThreadId myTask_usartHandle;
osThreadId myTask_oledHandle;
osThreadId myTask_servoHandle;
osThreadId myTask_speedHandle;
osThreadId myTask_pidHandle;
osThreadId myTask_doingHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const *argument);
void key_Task(void const *argument);
void led_Task(void const *argument);
void IMU_Task(void const *argument);
void deubg_Task(void const *argument);
void motor_Task(void const *argument);
void usart_Task(void const *argument);
void oled_Task(void const *argument);
void servo_Task(void const *argument);
void speed_Task(void const *argument);
void pid_Task(void const *argument);
void doing_Task(void const *argument);

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
  // IMUdate_RX_Sem_Handle��ֵ�ź�������
  IMUdate_RX_Sem_Handle = xSemaphoreCreateBinary();
  if (NULL != IMUdate_RX_Sem_Handle)
    printf("IMUdate_RX_Sem_Handle��ֵ�ź��������ɹ�!\r\n");
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  Speed_Queue = xQueueCreate((UBaseType_t ) 1,/* ��Ϣ���еĳ��� */
                            (UBaseType_t ) sizeof(biu));/* ��Ϣ�Ĵ�С */
  goal_Queue  = xQueueCreate((UBaseType_t ) 2,/* ��Ϣ���еĳ��� */
                            (UBaseType_t ) sizeof(biu));/* ��Ϣ�Ĵ�С */
  IMU_Queue   = xQueueCreate((UBaseType_t ) 1,/* ��Ϣ���еĳ��� */
                            (UBaseType_t ) sizeof(imu));/* ��Ϣ�Ĵ�С */
  debug_Queue = xQueueCreate((UBaseType_t ) 1,/* ��Ϣ���еĳ��� */
                            (UBaseType_t ) sizeof(biu));/* ��Ϣ�Ĵ�С */
  angle_Queue = xQueueCreate((UBaseType_t ) 1,/* ��Ϣ���еĳ��� */
                            (UBaseType_t ) sizeof(imu));/* ��Ϣ�Ĵ�С */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityIdle, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask_key */
  osThreadDef(myTask_key, key_Task, osPriorityAboveNormal, 0, 128);
  myTask_keyHandle = osThreadCreate(osThread(myTask_key), NULL);

  /* definition and creation of myTask_led */
//  osThreadDef(myTask_led, led_Task, osPriorityIdle, 0, 128);
//  myTask_ledHandle = osThreadCreate(osThread(myTask_led), NULL);

  /* definition and creation of myTask_IMU */
  osThreadDef(myTask_IMU, IMU_Task, osPriorityNormal, 0, 512);
  myTask_IMUHandle = osThreadCreate(osThread(myTask_IMU), NULL);

  /* definition and creation of myTask_debug */
  osThreadDef(myTask_debug, deubg_Task, osPriorityNormal, 0, 512);
  myTask_debugHandle = osThreadCreate(osThread(myTask_debug), NULL);

  /* definition and creation of myTask_motor */
//  osThreadDef(myTask_motor, motor_Task, osPriorityAboveNormal, 0, 512);
//  myTask_motorHandle = osThreadCreate(osThread(myTask_motor), NULL);

  /* definition and creation of myTask_usart */
//  osThreadDef(myTask_usart, usart_Task, osPriorityNormal, 0, 512);
//  myTask_usartHandle = osThreadCreate(osThread(myTask_usart), NULL);

  /* definition and creation of myTask_oled */
//  osThreadDef(myTask_oled, oled_Task, osPriorityLow, 0, 128);
//  myTask_oledHandle = osThreadCreate(osThread(myTask_oled), NULL);

  /* definition and creation of myTask_servo */
  osThreadDef(myTask_servo, servo_Task, osPriorityAboveNormal, 0, 256);
  myTask_servoHandle = osThreadCreate(osThread(myTask_servo), NULL);

  /* definition and creation of myTask_speed */
  osThreadDef(myTask_speed, speed_Task, osPriorityHigh, 0, 512);
  myTask_speedHandle = osThreadCreate(osThread(myTask_speed), NULL);

  /* definition and creation of myTask_pid */
  osThreadDef(myTask_pid, pid_Task, osPriorityHigh, 0, 512);
  myTask_pidHandle = osThreadCreate(osThread(myTask_pid), NULL);

  /* definition and creation of myTask_doing */
  osThreadDef(myTask_doing, doing_Task, osPriorityRealtime, 0, 512);
  myTask_doingHandle = osThreadCreate(osThread(myTask_doing), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
    


  /* USER CODE END RTOS_THREADS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  ���ȼ���͵����񣬵���������棬����ͨ���۲�led��˸����۲��������������
 *          ������ȶ������������ȼ�����������������
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
    
  for (;;)
  {
    Led_Toggle;
    
    osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_key_Task */
/**
 * @brief �����������ڼ�ⰴ��������û��ʹ���жϷ�ʽ�����ȶ�����ռ����Դ������ʹ����ѯ��ʽ��Ȼ��nice
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_key_Task */
void key_Task(void const *argument)
{
  /* USER CODE BEGIN key_Task */
  /* Infinite loop */
    
  for (;;)
  {
    if ((GPIOA->IDR & (1 << 15)) == 0)
    {
      osDelay(10);
      if ((GPIOA->IDR & (1 << 15)) == 0)
      {
        while((GPIOA->IDR & (1 << 15)) == 0)
        {
            osDelay(1);
        }
        /*do something*/
        
        
      }
    }
    osDelay(1);
  }
  /* USER CODE END key_Task */
}

/* USER CODE BEGIN Header_led_Task */
/**
 * @brief RGB led����������ʾ��ǰ״̬
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_led_Task */
void led_Task(void const *argument)
{
  /* USER CODE BEGIN led_Task */
  /* Infinite loop */
  for (;;)
  {

    osDelay(1000);
  }
  /* USER CODE END led_Task */
}

/* USER CODE BEGIN Header_IMU_Task */
/**
 * @brief IMU���ݴ����������ȼ��ϸߣ����յ���ֵ��Ϣ�����ƣ�����ֵ��Ϣ�����ͷź󣬴�����ſ���ִ�У�
 * ��Ϣ���ͷ��ٶ��ܺ궨��IMU_speed���ơ�
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_IMU_Task */
void IMU_Task(void const *argument)
{
  /* USER CODE BEGIN IMU_Task */
  /* Infinite loop */
  int i;
  imu imu_car;
  OS_status = 1;
  HAL_UART_Receive_DMA(&huart2, IMU_date, IMU_speed);
  for (;;)
  {
    // ��ȡ��ֵ�ź��� xSemaphore,û��ȡ����һֱ�ȴ�
    xSemaphoreTake(IMUdate_RX_Sem_Handle, portMAX_DELAY); /* �ȴ�ʱ�� */
    taskENTER_CRITICAL();
    for (i = 0; i < IMU_speed; i++)
    {
      WitSerialDataIn(IMU_date[i]);
    }
    if (s_cDataUpdate)
    {
      for (i = 0; i < 3; i++)
      {
        imu_car.IMU[i] = sReg[Roll + i] / 32768.0f * 180.0f;
      }
    }
    HAL_UART_Receive_DMA(&huart2, IMU_date, IMU_speed);
    taskEXIT_CRITICAL();
    xQueueSend( IMU_Queue,&imu_car,0 ); 
    xQueueSend( angle_Queue,&imu_car,0 ); 
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
void deubg_Task(void const *argument)
{
  /* USER CODE BEGIN deubg_Task */
  /* Infinite loop */
    imu imu_car;
    biu speed_now;
    biu speed_goal;
  for (;;)
  {
    

    xQueueReceive(IMU_Queue,&imu_car,10);
    xQueueReceive(debug_Queue,&speed_goal,10);
    xQueuePeek(Speed_Queue,&speed_now,10);
    
    taskENTER_CRITICAL();
    sprintf((char *)debug_date,"%.2f,%.2f,%.2f,%d,%d,%d,%d,%d,%d,%d,%d\n",\
    imu_car.IMU[0], imu_car.IMU[1], imu_car.IMU[2],\
    speed_now.date[0], speed_goal.date[0],speed_now.date[1], speed_goal.date[1],\
    speed_now.date[2], speed_goal.date[2],speed_now.date[3], speed_goal.date[3]);

    HAL_UART_Transmit_DMA(&huart1, debug_date, strlen((char *)debug_date));
    taskEXIT_CRITICAL();
    
    osDelay(70);
  }
  /* USER CODE END deubg_Task */
}

/* USER CODE BEGIN Header_motor_Task */
/**
 * @brief Function implementing the myTask_motor thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_motor_Task */
void motor_Task(void const *argument)
{
  /* USER CODE BEGIN motor_Task */
  /* Infinite loop */
  for (;;)
  {
//      speed_CTRL(1000,-1000,-1000,1000);
//      osDelay(1000); 
//      speed_CTRL(-1000,1000,-1000,1000);
//      speed_CTRL(0,0,0,0);
      vTaskSuspend(NULL);
      osDelay(5000); 
    
  }
  /* USER CODE END motor_Task */
}

/* USER CODE BEGIN Header_usart_Task */
/**
 * @brief Function implementing the myTask_usart thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_usart_Task */
void usart_Task(void const *argument)
{
  /* USER CODE BEGIN usart_Task */
  /* Infinite loop */
  for (;;)
  {
    
    osDelay(100);
  }
  /* USER CODE END usart_Task */
}

/* USER CODE BEGIN Header_oled_Task */
/**
 * @brief Function implementing the myTask_oled thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_oled_Task */
void oled_Task(void const *argument)
{
  /* USER CODE BEGIN oled_Task */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END oled_Task */
}

/* USER CODE BEGIN Header_servo_Task */
/**
 * @brief Function implementing the myTask_servo thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_servo_Task */
void servo_Task(void const *argument)
{
  /* USER CODE BEGIN servo_Task */
  /* Infinite loop */
  for (;;)
  {

    osDelay(50);
  }
  /* USER CODE END servo_Task */
}

/* USER CODE BEGIN Header_speed_Task */
/**
 * @brief Function implementing the myTask_speed thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_speed_Task */
void speed_Task(void const *argument)
{
  /* USER CODE BEGIN speed_Task */
  /* Infinite loop */
   biu speed;
  for (;;)
  {

    vTaskSuspendAll();
    speed.date[0] =  6720 - TIM2->CNT;
    TIM2->CNT = 6720;

    speed.date[1] = TIM3->CNT - 6720;
    TIM3->CNT = 6720;

    speed.date[2] =  6720 - TIM4->CNT;
    TIM4->CNT = 6720;

    speed.date[3] = TIM5->CNT - 6720;
    TIM5->CNT = 6720;
    xTaskResumeAll();
    xQueueSend( Speed_Queue,&speed,0 );        /* �ȴ�ʱ�� 0 */
      
    osDelay(10);
  }
  /* USER CODE END speed_Task */
}

/* USER CODE BEGIN Header_pid_Task */
/**
 * @brief Function implementing the myTask_pid thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_pid_Task */
void pid_Task(void const *argument)
{
  /* USER CODE BEGIN pid_Task */
  /* Infinite loop */
    biu speed_goal;
    biu speed_now;
  for (;;)
  {
    xQueueReceive( Speed_Queue,    /* ��Ϣ���еľ�� */
                                 &speed_now,      /* ���͵���Ϣ���� */
                                 5); /* �ȴ�ʱ�� һֱ�� */  
    xQueueReceive( goal_Queue,    /* ��Ϣ���еľ�� */
                                 &speed_goal,      /* ���͵���Ϣ���� */
                                 5); /* �ȴ�ʱ�� һֱ�� */ 
//    printf("%d,%d\n",speed_now.date[0],speed_now.date[1]);
    
    taskENTER_CRITICAL();
//    printf("%d,%d,%d,%d,%d,%d,%d,%d\n",speed_now.date[0],speed_now.date[1],speed_now.date[2],speed_now.date[3],speed_goal.date[0],speed_goal.date[1],speed_goal.date[2],speed_goal.date[3]);
    pid_do(speed_goal,speed_now);
//    speed_get(); 
//    pid_doing(car_status);
    taskEXIT_CRITICAL();
//      pid_doing(car_status);
    osDelay(10);
  }
  /* USER CODE END pid_Task */
}

/* USER CODE BEGIN Header_doing_Task */
/**
 * @brief Function implementing the myTask_doing thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_doing_Task */
void doing_Task(void const *argument)
{
  /* USER CODE BEGIN doing_Task */
  /* Infinite loop */
    imu imu_date;
    uint8_t num=0;
  for (;;)
  {
      xQueueReceive( angle_Queue, &imu_date, portMAX_DELAY); /* �ȴ�ʱ�� һֱ�� */  
//      if(direction_Set(10,imu_date))
//      {         
//          num++;
//          if(num>10)
//          {
//              speed_CTRL(0,0,0,0);
//              vTaskSuspend(NULL);//���ת��ʱ����
//              num=0;
//          }                                                                                                
//      }
      advance_angle(0,imu_date,1000);
      osDelay(10);
  }
  /* USER CODE END doing_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
