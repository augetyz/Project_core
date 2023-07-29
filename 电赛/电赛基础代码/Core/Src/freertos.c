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
#include "HI229.h"

#include "usbd_cdc_if.h"
#include "usb_user.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Led_Toggle GPIOC->ODR ^= GPIO_PIN_13 // LED_Toggle函数宏定义
#define IMU_speed 80                         // IMU数据采集处理频率控制参数
#define JY61

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


extern char s_cDataUpdate, s_cCmd; // MPU6050移植参数，不用管

extern servo_status servo_motion[];

uint8_t OS_status = 0;             // 任务初始化标志位，为1则初始化OK，为0则还没有嘞

uint8_t debug_date[600] = {0};     // debug数据缓存区，用于缓存需要用来上传的debug数据。

uint8_t IMU_date[IMU_speed] = {0}; // MPU6050数据缓冲区、采用DMA+串口方式接收

uint8_t usart_3_date[25] = {0};
uint8_t usart_6_date[25] = {0};

raw_t raw = {0};             /* IMU stram read/control struct */
uint8_t decode_succ;         /* 0: no new frame arrived, 1: new frame arrived */


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
SemaphoreHandle_t IMUdate_RX_Sem_Handle = NULL; // 信号量初始化，用于作为IMU数据接收完成后的回响，开启IMU_TASK函数的执行。

SemaphoreHandle_t usart3_RX_Sem_Handle = NULL;

SemaphoreHandle_t usart6_RX_Sem_Handle = NULL;

SemaphoreHandle_t Servo_Sem_Handle = NULL;

QueueHandle_t   Speed_Queue =NULL;
QueueHandle_t   goal_Queue =NULL;
QueueHandle_t   IMU_Queue =NULL;
QueueHandle_t   debug_Queue =NULL;
QueueHandle_t   angle_Queue =NULL;
QueueHandle_t   servo_Queue =NULL;
QueueHandle_t   distance_Queue =NULL;
QueueHandle_t   color_Queue =NULL;
QueueHandle_t   black_line_Queue =NULL;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTask_keyHandle;
osThreadId myTask_DisplayHandle;
osThreadId myTask_IMUHandle;
osThreadId myTask_debugHandle;
osThreadId myTask_motorHandle;
osThreadId myTask_usartHandle;
osThreadId myTask_servoHandle;
osThreadId myTask_speedHandle;
osThreadId myTask_pidHandle;
osThreadId myTask_doingHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void key_Task(void const * argument);
void Display_Task(void const * argument);
void IMU_Task(void const * argument);
void deubg_Task(void const * argument);
void motor_Task(void const * argument);
void usart_Task(void const * argument);
void servo_Task(void const * argument);
void speed_Task(void const * argument);
void pid_Task(void const * argument);
void doing_Task(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

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
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  // IMUdate_RX_Sem_Handle二值信号量创建
  IMUdate_RX_Sem_Handle = xSemaphoreCreateBinary();
  
  usart3_RX_Sem_Handle= xSemaphoreCreateBinary();
    
  usart6_RX_Sem_Handle= xSemaphoreCreateBinary();  
    
  Servo_Sem_Handle       = xSemaphoreCreateBinary();
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  Speed_Queue = xQueueCreate((UBaseType_t ) 1,/* 消息队列的长度 *///实时速度参数传递消息队列
                            (UBaseType_t ) sizeof(biu));/* 消息的大小 */
  goal_Queue  = xQueueCreate((UBaseType_t ) 2,/* 消息队列的长度 *///目标速度传递消息队列
                            (UBaseType_t ) sizeof(biu));/* 消息的大小 */
  IMU_Queue   = xQueueCreate((UBaseType_t ) 1,/* 消息队列的长度 */
                            (UBaseType_t ) sizeof(imu));/* 消息的大小 */
  debug_Queue = xQueueCreate((UBaseType_t ) 1,/* 消息队列的长度 */
                            (UBaseType_t ) sizeof(biu));/* 消息的大小 */
  angle_Queue = xQueueCreate((UBaseType_t ) 1,/* 消息队列的长度 */
                            (UBaseType_t ) sizeof(imu));/* 消息的大小 */
  servo_Queue = xQueueCreate((UBaseType_t ) 1,/* 消息队列的长度 */
                            (UBaseType_t ) sizeof(servo_status));/* 消息的大小 */
  distance_Queue = xQueueCreate((UBaseType_t ) 1,/* 消息队列的长度 */
                            (UBaseType_t ) sizeof(biu));/* 消息的大小 */
  color_Queue    = xQueueCreate((UBaseType_t ) 1,/* 消息队列的长度 */
                            (UBaseType_t ) sizeof(biu_int_16));/* 消息的大小 */    
  black_line_Queue=xQueueCreate((UBaseType_t ) 1,/* 消息队列的长度 */
                            (UBaseType_t ) sizeof(biu_int_16));/* 消息的大小 */                              
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityIdle, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask_key */
  osThreadDef(myTask_key, key_Task, osPriorityAboveNormal, 0, 128);
  myTask_keyHandle = osThreadCreate(osThread(myTask_key), NULL);

  /* definition and creation of myTask_Display */
  osThreadDef(myTask_Display, Display_Task, osPriorityIdle, 0, 256);
  myTask_DisplayHandle = osThreadCreate(osThread(myTask_Display), NULL);

  /* definition and creation of myTask_IMU */
  osThreadDef(myTask_IMU, IMU_Task, osPriorityNormal, 0, 256);
  myTask_IMUHandle = osThreadCreate(osThread(myTask_IMU), NULL);

  /* definition and creation of myTask_debug */
  osThreadDef(myTask_debug, deubg_Task, osPriorityLow, 0, 512);
  myTask_debugHandle = osThreadCreate(osThread(myTask_debug), NULL);

  /* definition and creation of myTask_motor */
  osThreadDef(myTask_motor, motor_Task, osPriorityAboveNormal, 0, 256);
  myTask_motorHandle = osThreadCreate(osThread(myTask_motor), NULL);

  /* definition and creation of myTask_usart */
  osThreadDef(myTask_usart, usart_Task, osPriorityNormal, 0, 512);
  myTask_usartHandle = osThreadCreate(osThread(myTask_usart), NULL);

  /* definition and creation of myTask_servo */
  osThreadDef(myTask_servo, servo_Task, osPriorityAboveNormal, 0, 256);
  myTask_servoHandle = osThreadCreate(osThread(myTask_servo), NULL);

  /* definition and creation of myTask_speed */
  osThreadDef(myTask_speed, speed_Task, osPriorityHigh, 0, 256);
  myTask_speedHandle = osThreadCreate(osThread(myTask_speed), NULL);

  /* definition and creation of myTask_pid */
  osThreadDef(myTask_pid, pid_Task, osPriorityHigh, 0, 512);
  myTask_pidHandle = osThreadCreate(osThread(myTask_pid), NULL);

  /* definition and creation of myTask_doing */
  osThreadDef(myTask_doing, doing_Task, osPriorityHigh, 0, 512);
  myTask_doingHandle = osThreadCreate(osThread(myTask_doing), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */                                                                                                                                               
    


  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  优先级最低的任务，点个灯在里面，可以通过观察led闪烁情况观察任务运行情况，
 *          如果不稳定，则任务优先级、缓存分配存在问题
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
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
 * @brief 按键任务，用于检测按键触发，没有使用中断方式，不稳定而且占用资源，这里使用轮询方式依然很nice
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_key_Task */
void key_Task(void const * argument)
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
        HAL_UART_Transmit_DMA(&huart1, usart_3_date, 22);   
        
      }
    }
    if ((GPIOE->IDR & (1 << 1)) == 0)
    {
      osDelay(10);
      if ((GPIOE->IDR & (1 << 1)) == 0)
      {
        while((GPIOE->IDR & (1 << 1)) == 0)
        {
            osDelay(1);
        }
        /*do something*/
//        vTaskSuspend(myTask_doingHandle);
        servo_all_move(servo_motion[0]);
         
      }
    }
    osDelay(1);
  }
  /* USER CODE END key_Task */
}

/* USER CODE BEGIN Header_Display_Task */
/**
* @brief Function implementing the myTask_Display thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Display_Task */
void Display_Task(void const * argument)
{
  /* USER CODE BEGIN Display_Task */
  /* Infinite loop */
    uint8_t RX_Buffer[20];
  for(;;)
  {
      if(usb_vbc_Receive(RX_Buffer,16,HAL_MAX_DELAY))
      {
          CDC_Transmit_FS(RX_Buffer,16);
      }
    osDelay(1);
  }
  /* USER CODE END Display_Task */
}

/* USER CODE BEGIN Header_IMU_Task */
/**
 * @brief IMU数据处理任务、优先级较高，但收到二值信息量控制，当二值信息量被释放后，此任务才可以执行，
 * 信息量释放速度受宏定义IMU_speed控制。
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_IMU_Task */
void IMU_Task(void const * argument)
{
  /* USER CODE BEGIN IMU_Task */
  /* Infinite loop */
  int i;
  imu imu_car;
  OS_status = 1;
  HAL_UART_Receive_DMA(&huart2, IMU_date, IMU_speed);
  for (;;)
  {
    // 获取二值信号量 xSemaphore,没获取到则一直等待
    xSemaphoreTake(IMUdate_RX_Sem_Handle, portMAX_DELAY); /* 等待时间 */
    taskENTER_CRITICAL();
#ifdef JY61
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
#else
    for (i = 0; i < IMU_speed; i++)
    {
      decode_succ = ch_serial_input(&raw, IMU_date[i]);
        if(decode_succ==1)
            break;
    } 

    if (decode_succ)
    {
      for (i = 0; i < 3; i++)
      {
        imu_car.IMU[i] = raw.imu[0].eul[i];
      }
    }  
#endif    
    HAL_UART_Receive_DMA(&huart2, IMU_date, IMU_speed);
    taskEXIT_CRITICAL();
    xQueueSend( IMU_Queue,&imu_car,0 ); 
    xQueueSend( angle_Queue,&imu_car,0 ); 
  }
  /* USER CODE END IMU_Task */
}

/* USER CODE BEGIN Header_deubg_Task */
/**
 * @brief 小车状态上传任务，使用串口一DMA发送，运行周期1000ms
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_deubg_Task */
void deubg_Task(void const * argument)
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

//    HAL_UART_Transmit_DMA(&huart1, debug_date, strlen((char *)debug_date));
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
void motor_Task(void const * argument)
{
  /* USER CODE BEGIN motor_Task */
  /* Infinite loop */
  for (;;)
  {
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
void usart_Task(void const * argument)
{
  /* USER CODE BEGIN usart_Task */
  /* Infinite loop */

    taskENTER_CRITICAL();


        int i=0,num,sign=0;
        int16_t x=0;
        int16_t y=0;
        int16_t wide=0;
        int16_t height=0;
        char* f=(char*)&x;
        char *h=(char*)&y;
        char* u=(char*)&wide;
        char *q=(char*)&height;
        biu_int_16 color_date;
        biu_int_16 black_line_date;    
        HAL_UART_Receive_DMA(&huart3, usart_3_date, 22);   
        HAL_UART_Receive_DMA(&huart6, usart_6_date, 22); 
    
    taskEXIT_CRITICAL();

  for (;;)
  {
      //V831一号
      if(xSemaphoreTake(usart3_RX_Sem_Handle, 10)==pdTRUE) /* 等待时间 */
      {
          vTaskSuspendAll();
          for(i=0;i<8;i++)
          {
              if(usart_3_date[i]==0X2C&&usart_3_date[i+10]==0X5B)
              {
                  num=i;
                  if(usart_3_date[num+1]==0X12)
                    sign=1;
                  else if(usart_3_date[num+1]==0X21)
                    sign=2;
                  else
                    sign=0;
                  break;              
              }
              sign=0;
          }         
//          HAL_UART_Transmit_DMA(&huart1, usart_3_date,22);
          if(sign==1)
          {
              
              for(i=0;i<2;i++)
              {
                  *(f+i)=*(usart_3_date+num+i+2);
              }
    //          i=*(f);*(f)=*(f+1);*(f+1)=i;
              
              for(i=0;i<2;i++)
              {
                  *(h+i)=*(usart_3_date+num+i+4);
              }
              for(i=0;i<2;i++)
              {
                  *(u+i)=*(usart_3_date+num+i+6);
              }
              for(i=0;i<2;i++)
              {
                  *(q+i)=*(usart_3_date+num+i+8);
              }
    //          i=*(h);*(h)=*(h+1);*(h+1)=i;
              if((x!=520&&y!=520)&&(y!=9)) 
              {
                  color_date.date[0]=x;
                  color_date.date[1]=y;
                  color_date.date[2]=11;
                  color_date.date[3]=height;
                  xQueueSend(color_Queue,&color_date,0);
              }
              else 
              {
                  //error
                  
              }
          }
          else if(sign==2)
          {
              usart_3_date[num+6]=usart_3_date[num+7];
              usart_3_date[num+7]=usart_3_date[num+8];
              usart_3_date[num+8]=usart_3_date[num+9];
              x=0;
              for(i=0;i<6;i++)   
              {
                  if(usart_3_date[num+3+i]=='1')
                      x|=0X01<<(5-i)*2;
                  else if(usart_3_date[num+3+i]=='2')
                      x|=0X02<<(5-i)*2;
                  else if(usart_3_date[num+3+i]=='3')
                      x|=0X03<<(5-i)*2;
                  else
                  { 
                    x=0;
                    break;
                  }
              }  
              if(x!=0)
              {
                  if( (x&3<<0) != ( (x&3<<2)>>2 ) &&(x&3<<0) != ( (x&3<<4)>>4 )&&(x&3<<2)>>2 != ( (x&3<<4)>>4 ))
                  {
                     
                      color_date.date[3]=x;
                      color_date.date[2]=22;
                      xQueueSend(color_Queue,&color_date,0);
                  } 
                  else
                  {
                      //error
                  }
              }
                         
          }

        HAL_UART_Receive_DMA(&huart3, usart_3_date, 22); 
        xTaskResumeAll();
    }
      
    //V831二号
    if(xSemaphoreTake(usart6_RX_Sem_Handle, 10)==pdTRUE) /* 等待时间 */
    {
        vTaskSuspendAll();
          for(i=0;i<8;i++)
          {
              if(usart_6_date[i]==0X2C&&usart_6_date[i+10]==0X5B)
              {
                  num=i;
                  if(usart_6_date[num+1]==0X12)
                    sign=1;
                  else if(usart_6_date[num+1]==0X21)
                    sign=2;
                  else
                    sign=0;
                  break;              
              }
              sign=0;
          }         
          xTaskResumeAll();
//          HAL_UART_Transmit_DMA(&huart1, usart_6_date,22);
          if(sign==1)
          {
              
              for(i=0;i<2;i++)
              {
                  *(f+i)=*(usart_6_date+num+i+2);
              }

              
              for(i=0;i<2;i++)
              {
                  *(h+i)=*(usart_6_date+num+i+4);
              }             

              if( *(f)==(char)0XFF && *(f+1)==(char)0XFF && *(h)==(char)0XFF && *(h+1)==(char)0XFF ) 
              {
                  black_line_date.date[0]=0XFF;
                  black_line_date.date[1]=0XFF;
                  black_line_date.date[2]=11;
                  black_line_date.date[3]=0;
                  xQueueSend(black_line_Queue,&black_line_date,0);

              }
              else 
              {
                  black_line_date.date[0]=x;
                  black_line_date.date[1]=y;
                  black_line_date.date[2]=11;
                  black_line_date.date[3]=0;
                  xQueueSend(black_line_Queue,&black_line_date,0);
                  
              }
          }    

        HAL_UART_Receive_DMA(&huart6, usart_6_date, 22); 
    }
    osDelay(10);
  }
  /* USER CODE END usart_Task */
}

/* USER CODE BEGIN Header_servo_Task */
/**
 * @brief Function implementing the myTask_servo thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_servo_Task */
void servo_Task(void const * argument)
{
  /* USER CODE BEGIN servo_Task */
  /* Infinite loop */
    uint16_t time = 0,time_use, i = 0;
    int16_t angle[6];
    static servo_status servo;
  for (;;)
  {
    xQueueReceive( servo_Queue,&servo,portMAX_DELAY);
    if (servo.time == 0)
        time = 1000;
    else
        time = servo.time;
    time_use = time / 100;
    if(servo.value_goal[0]!=0)
    {
        vTaskSuspendAll();
        servo.value_goal[0] = servo.value_goal[0]>1520?1520:servo.value_goal[0];
        angle[0] = ((int)((int)servo.value_goal[0] - PWM1) / 100);
        angle[1] = ((int)((int)servo.value_goal[1] - PWM2) / 100);
        angle[2] = ((int)((int)servo.value_goal[2] - PWM3) / 100);
        angle[3] = ((int)((int)servo.value_goal[3] - PWM4) / 100);
        angle[4] = ((int)((int)servo.value_goal[4] - PWM5) / 100);
        angle[5] = ((int)((int)servo.value_goal[5] - PWM6) / 100);
        xTaskResumeAll();  
        for(i=0;i<100;i++)
        {
            vTaskSuspendAll();
            PWM1  += angle[0] ;
            PWM2  += angle[1] ;
            PWM3  += angle[2] ;
            PWM4  += angle[3] ;
            PWM5  += angle[4] ;
            PWM6  += angle[5] ;             
            xTaskResumeAll();        
            osDelay(time_use);        
        }
        vTaskSuspendAll();
        PWM1 = servo.value_goal[0]>1520?1520:servo.value_goal[0];
        PWM2 = servo.value_goal[1];
        PWM3 = servo.value_goal[2];
        PWM4 = servo.value_goal[3];
        PWM5 = servo.value_goal[4];
        PWM6 = servo.value_goal[5];
        xTaskResumeAll();
        xSemaphoreGive(Servo_Sem_Handle);
    }
    osDelay(10);
    
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
void speed_Task(void const * argument)
{
  /* USER CODE BEGIN speed_Task */
  /* Infinite loop */
   biu speed;
   static biu distance={0,0,0,0};
   uint8_t i=0;
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
    for(i=0;i<4;i++)
    {
        distance.date[i]+=speed.date[i];
    }
    xTaskResumeAll();
    xQueueSend( Speed_Queue,&speed,0 );        /* 等待时间 0 */
    xQueueSend( distance_Queue,&distance,0 );  
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
void pid_Task(void const * argument)
{
  /* USER CODE BEGIN pid_Task */
  /* Infinite loop */
    biu speed_goal;
    biu speed_now;

  for (;;)
  {
    xQueueReceive( Speed_Queue,    /* 消息队列的句柄 */
                                 &speed_now,      /* 发送的消息内容 */
                                 5); /* 等待时间 一直等 */  
    xQueueReceive( goal_Queue,    /* 消息队列的句柄 */
                                 &speed_goal,      /* 发送的消息内容 */
                                 5); /* 等待时间 一直等 */ 
    
    
      
    taskENTER_CRITICAL();

    pid_do(speed_goal,speed_now);

    taskEXIT_CRITICAL();

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
void doing_Task(void const * argument)
{
  /* USER CODE BEGIN doing_Task */
  /* Infinite loop */
    imu imu_date;
    biu distance_now;

    static int back_sign=0;
    static int16_t distance_use=0;
    
    static uint8_t Task_select=Start;//执行调度参数
    
    
    
  for (;;)
  {
      xQueueReceive( angle_Queue, &imu_date, portMAX_DELAY); /* 等待时间 一直等 */  
      xQueueReceive( distance_Queue,&distance_now, 0); /* 等待时间 一直等 */ 

      
      
      switch(Task_select)             
      {
          case Start:
              osDelay(10);
          break;
          
          default:
              osDelay(10);
          break;
      }      
      osDelay(10);
  }
  /* USER CODE END doing_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
