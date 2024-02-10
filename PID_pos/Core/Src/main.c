/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define pi 3.141592654
#define p2r 2*pi/1848
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
unsigned char previous_state;
int count_value, PosCnt;
double dgree;
float Kp, Ki, Kd, Kb;
int Low_lim, High_lim;
float Ts = 0.05, alpha = 0.6;
char tick = 0;
uint8_t pwm;
double set_point, real;
char str[30];
char RX_data[50], send[50];
int count_data;
double setpoint;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void send_char(char data)
{
	while((USART1->SR & (1 << 6)) == 0);
	USART1->DR = data;
}
void send_string(char *str)
{
	while(*str) send_char(*str++);
}
void dir(double data)
{
	if (data > 0)
	{
		GPIOB->ODR |= (1 << 0);
		GPIOB->ODR &= ~(1 << 1);
	}
	else if (data < 0)
	{
		GPIOB->ODR &= ~(1 << 0);
		GPIOB->ODR |= (1 << 1);
	}
	else
	{
		GPIOB->ODR &= ~(1 << 0);
		GPIOB->ODR &= ~(1 << 1);
	}
}
void PID(double set_point, double real)
{
	double pid, out;
	
	High_lim = 400, Low_lim = 0;
	static double e_pre = 0;
	static double Ui_pre = 0;
	static double e_reset = 0;
	static double Ud_f_pre = 0;
	
	double e;
	double Up, Ui, Ud, Ud_f;
	
	Kp = 0.8746591948;
	Ki = 1.807511699;
	Kd = 0.09015969137;
	
	Kb = 4.477487171;
	
	e = set_point - real; // error
	
	Up = Kp*e; //
	
	Ui = Ui_pre + Ki*Ts*e + Kb*Ts*e_reset; // antiwindup
	
	Ud = Kd*(e - e_pre)/Ts;//
	Ud_f = (1 - alpha)*Ud_f_pre + alpha*Ud; // low pass fillter

	// Luu lai cac gia tri
	e_pre = e;
	Ui_pre = Ui;
	Ud_f_pre = Ud_f;
	
	pid = (Up + Ui + Ud_f);
	
	// set rotations
	dir(pid);
	out = abs((int) pid);
	// Saturated
	if (out > High_lim) pwm = High_lim;
	else if(out < Low_lim) pwm = Low_lim;
	else pwm = pid;
	e_reset = pwm - out;
	// Set value PWM
	TIM3->CCR1 = pwm;
}


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void TIM2_IRQHandler(void) // 50ms
{
	TIM2->SR &= ~TIM_SR_UIF;
	dgree = (PosCnt*2*pi + count_value*p2r)*(360/(2*pi));
	setpoint = atof(send);
	PID(setpoint, dgree);
	
	tick++;
	if(tick == 10)
	{
		tick = 0;
		sprintf(str, "%f\n", dgree);
		send_string(str);	
	}
}
void EXTI3_IRQHandler(void)
{
	EXTI->PR |= (1 << 3);//clear interupt flag EXTI3
	unsigned char state1;
	state1 = (state1 << 1) | HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
	state1 = (state1 << 1) | HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
	
	state1 = state1 & 0x03;
	
	switch(state1)
	{
		case 0:
			if(previous_state == 1) count_value++;
			else count_value--;
			break;
		case 1:
			if(previous_state == 3) count_value++;
			else count_value--;
			break;
		case 2:
			if(previous_state == 0) count_value++;
			else count_value--;
			break;
		case 3:
			if(previous_state == 2) count_value++;
			else count_value--;
			break;
	}
	previous_state = state1;
	if(count_value >= 1848)
	{
		count_value = 0;
		PosCnt++;
	}
	else if(count_value <= -1848)
	{
		count_value = 0;
		PosCnt--;
	}
}
void EXTI4_IRQHandler(void)
{
	EXTI->PR |= (1 << 4);//clear interupt flag EXTI3
	unsigned char state0;
	state0 = (state0 << 1) | HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
	state0 = (state0 << 1) | HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
	
	state0 = state0 & 0x03;
	
	switch(state0)
	{
		case 0:
			if(previous_state == 1) count_value++;
			else count_value--;
			break;
		case 1:
			if(previous_state == 3) count_value++;
			else count_value--;
			break;
		case 2:
			if(previous_state == 0) count_value++;
			else count_value--;
			break;
		case 3:
			if(previous_state == 2) count_value++;
			else count_value--;
			break;
	}
	previous_state = state0;
	if(count_value >= 1848)
	{
		count_value = 0;
		PosCnt++;
	}
	else if(count_value <= -1848)
	{
		count_value = 0;
		PosCnt--;
	}
}
void USART1_IRQHandler(void)
{
	
	if((USART1->DR != '\n')) RX_data[count_data++] = USART1->DR;
	else{
		char size = sizeof(send) / sizeof(send[0]);
		for (int i = 0; i < size; i++) {
        send[i] = 0;  // clear array send
    }
		
		for (int i = 0; i < count_data; i++){
			send[i] = RX_data[i];
		}
		send_string(send);
		count_data = 0;
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
	RCC->APB2ENR |= (1 << 0) | (1 << 2) | (1 << 3); // clock for AFIO, GPIOA, GPIOB
	GPIOA->CRL |= 0x00000000;
	GPIOA->CRH |= 0x00000000;
	GPIOB->CRL |= 0x00000000;
	GPIOB->CRH |= 0x00000000;
	
	// Init USART1_115200
	RCC->APB2ENR |= (1 << 14); // clock for USART1
	GPIOA->CRH |= (9 << (4*1)); // PA9->TX
	GPIOA->CRH |= (4 << (4*2)); // PA10->RX
	USART1->BRR |= (4 << 4) | (5 << 0); // Baudrate 115200
	USART1->CR1 |= (1 << 2) | (1 << 3) | (1 << 13);
	USART1->CR1 |= (1 << 5);
	NVIC->ISER[1] |= (1 << 5);
	
	// Init timer 2 alternal clock (50ms)
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = (80 - 1);
	TIM2->ARR = (5000 - 1);
	TIM2->EGR |= TIM_EGR_UG;
	TIM2->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM2->CR1 |= TIM_CR1_CEN;

	// Init TIM3 PWM 20kHz
	GPIOA->CRL |= (0x09 << (4*6)); // PA6
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->PSC = 0;
	TIM3->ARR = 400;
	TIM3->CCMR1 |= (0x06 << 4);
	TIM3->CCER |= (1 << 0);
	TIM3->CR1 |= TIM_CR1_CEN;

	// Init exit EXTI3->(PA3), EXTI4->(PA4)
	GPIOA->CRL |= (8 << (4*3)) | (8 << (4*4));
	GPIOA->ODR &= ~((1 << 3) | (1 << 4));
	
	AFIO->EXTICR[0] &= ~(15 << (4*3));
	AFIO->EXTICR[1] &= ~(15 << (4*0));
	
	EXTI->IMR |= (1 << 3) | (1 << 4);
	EXTI->RTSR |= (1 << 3) | (1 << 4);
	EXTI->FTSR |= (1 << 3) | (1 << 4);
	
	NVIC->ISER[0] |= (1 << 9) | (1 << 10);

	// Init PB0, PB1
	GPIOB->CRL |= (1 << (4*0)) | (1 << (4*1));
	GPIOB->ODR |= (1 << 0);
	GPIOB->ODR |= (1 << 1);
	
//	HAL_Delay(2000);
//	TIM3->CCR1 = 400;
//	HAL_Delay(100);
//	TIM3->CCR1 = 0;
 	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PC13 PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 PA7
                           PA8 PA9 PA10 PA11
                           PA12 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB10
                           PB11 PB12 PB13 PB14
                           PB15 PB3 PB4 PB5
                           PB6 PB7 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure peripheral I/O remapping */
  __HAL_AFIO_REMAP_PD01_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
