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
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
# define pi 3.141592654
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
char str[50];
char RX_data[50];
char count_data;
char send[50];

int CountValue;
int Cnttmp;

double velocity, velocity_pre;
double v_filt, v_filt_pre;

int tick_2 = 0;

double pwm = 0;
// Value error
double setpoint, real;
int High_lim, Low_lim;

double Kp, Ki, Kd, Kb;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void send_char(char data);
void send_string(char *str);
void dir(double value);
double fabs(double x);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
float Ts = 0.05;
float alpha = 0.6;

void PID(double setpoint, double real){
	double pid;
	
	High_lim = 400, Low_lim = 0;
	static double e_pre = 0;
	static double Ui_pre = 0;
	static double e_reset = 0;
	static double Ud_f_pre = 0;
	
	double e;
	double Up, Ui, Ud, Ud_f;
	
	Kp = 1.222113111;
	Ki = 2.961660972;
	Kd = 0;
	
	Kb = 2.423393502;
	
	e = setpoint - real; // error
	
	Up = Kp*e; //
	
	Ui = Ui_pre + Ki*Ts*e + Kb*Ts*e_reset; // antiwindup
	
	Ud = Kd*(e - e_pre)/Ts;//
	Ud_f = (1 - alpha)*Ud_f_pre + alpha*Ud; // low pass fillter

	// Luu lai cac gia tri
	e_pre = e;
	Ui_pre = Ui;
	Ud_f_pre = Ud_f;
	
	pid = Up + Ui + Ud_f;
	
	// set rotations
	dir(pid);
	// Saturated
	if (pid > High_lim) pwm = High_lim;
	else if(pid < Low_lim) pwm = Low_lim;
	else pwm = pid;
	e_reset = pwm - pid;
	// Set value PWM
	TIM3->CCR1 = pwm;
}

void TIM2_IRQHandler (void){ // 1ms
	if(TIM2->SR & TIM_SR_UIF){
		TIM2->SR &= ~TIM_SR_UIF;   // Clear interrupt flag for Timer 2
		tick_2++;
			
		if (tick_2 == 50) {
			tick_2 = 0;
			// Caculator Velocity
			Cnttmp = CountValue;
			CountValue = 0;
			velocity = Cnttmp * 60.0 / (4 * 11 * 42 * Ts);
//			v_filt = 0.854*v_filt + 0.0728*velocity + 0.0728*velocity_pre;
//			velocity_pre = velocity;
//			velocity = Cnttmp*2*pi / (4 * 11 * 42 * Ts);
			real = velocity;
			setpoint = atof(send);
			
			// PID
			PID(setpoint, real);
		}	
	}
}

int tick_4;
void TIM4_IRQHandler(void) { // 1ms
	if(TIM4->SR & TIM_SR_UIF){
		TIM4->SR &= ~TIM_SR_UIF;   // Clear interrupt flag for Timer 4
		tick_4++;
		
		if (tick_4 == 500) {
			tick_4 = 0;
			// Handle Timer 4 logic here (e.g., update and send velocity)
			sprintf(str, "%f\n", velocity);
//			sprintf(str, "%f\n", v_filt);
			send_string(str);		
		}			
	}
}

void USART1_IRQHandler(void){
	if((USART1->DR != '\n')) RX_data[count_data++] = USART1->DR;
	else{
		char size = sizeof(send) / sizeof(send[0]);
		for (int i = 0; i < size; i++) {
        send[i] = 0;  // clear array send
    }
		
		for (int i = 0; i < count_data; i++){
			send[i] = RX_data[i];
		}
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
  RCC->APB2ENR |= (1 << 0)|(1 << 2)|(1 << 3); //clock for AFIO, GPIOA, GPIOB
  GPIOA->CRL |= 0x00000000; //clear port A CRLow
  GPIOA->CRH |= 0x00000000; //clear port A CRHigh
  GPIOB->CRL |= 0x00000000; //clear port B CRLow
  GPIOB->CRH |= 0x00000000; //clear port B CRHigh
	
// Init USART1
	RCC->APB2ENR |= (1 << 2) | (1 << 14); // clock for GPIOA USART1
	GPIOA->CRH |= 0x00000000;
	GPIOA->CRH |= (9 << (4*1)); // PA9
	GPIOA->CRH |= (4 << (4*2)); // PA10
	USART1->BRR |= (4 << 4) | (5 << 0); // Baudrate 115200
	USART1->CR1 |= (1 << 2)|(1 << 3)|(1 << 13);
	USART1->CR1 |= (1 << 5);
	NVIC->ISER[1] |= (1 << 5);// toa do tuong doi 37-32
  
// TIMER
	// Enable clock for TIM2 and TIM4
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM4EN;

	// TIM2 Configuration (1ms)
	TIM2->PSC = (8 - 1);  // Assuming 8 MHz / (PSC + 1) = 1 MHz
	TIM2->ARR = (1000 - 1);   // 10 MHz / (ARR + 1) = 1 ms
	TIM2->EGR = TIM_EGR_UG;
	TIM2->DIER |= TIM_DIER_UIE; // Enable TIM2 interrupt
	NVIC_EnableIRQ(TIM2_IRQn);

	// TIM4 Configuration (10ms)
	TIM4->PSC = (80 - 1);  // Assuming 8 MHz / (PSC + 1) = 10 kHz
	TIM4->ARR = (100 - 1); // 10 kHz / (ARR + 1) = 10 ms
	TIM4->EGR = TIM_EGR_UG;
	TIM4->DIER |= TIM_DIER_UIE; // Enable TIM4 interrupt
	NVIC_EnableIRQ(TIM4_IRQn);

	// Enable timers
	TIM2->CR1 |= TIM_CR1_CEN;
	TIM4->CR1 |= TIM_CR1_CEN;

// Iinit TIM3 PWM mode 20kHz
  RCC->APB2ENR |= (1 << 2); // clock to GPIOA
  GPIOA->CRL |= (0x09 << (4*6)); // PA6 output AF 
  RCC->APB1ENR |= (1 << 1); //clock to timer 3
  TIM3->PSC = 0; // PSC
  TIM3->ARR = 400; // PWM = 20kHz
//  TIM3->CCR1 = 0; // duty cycle pwm
  TIM3->CCMR1 |= (0x06 << 4); //choose PWM mode 1 at CH1
  TIM3->CCER |= (1 << 0); // enable channel 1 -- TIM 3
  TIM3->CR1 |=(1 << 0); // enable counter
	
//=====Set up EXTI3--PA3 to read encoder======
	GPIOA->CRL |= (8 << (4*3)); // input pull_up/pull_down at PA3
	GPIOA->ODR &= ~(1 << 3);// pull down PA3 PA4 - ENC_A
	AFIO->EXTICR[0] |= (~(15 << (4*3))); // EXTI3 - 0x0000 << (4*3)
	EXTI->IMR |= (1 << 3); // enable EXTI3
	EXTI->RTSR |= (1 << 3); //rising
	EXTI->FTSR |= (1 << 3); //failing
	NVIC->ISER[0] |= (1 << 9); //enable global EXTI3
		
//=====Set up EXTI4--PA4 to read encoder======
	GPIOA->CRL |= (8 << (4*4)); // input pull_up/pull_down at PA4
	GPIOA->ODR &= ~(1 << 4);// pull down PA4 - ENC_B
	AFIO->EXTICR[1] |= (~(15 << (4*0))); // EXTI4 - 0x0000 << (4*4)
	EXTI->IMR |= (1 << 4); // enable EXTI3
	EXTI->RTSR |= (1 << 4); //rising
	EXTI->FTSR |= (1 << 4); //failing
	NVIC->ISER[0] |= (1 << 10); //enable global EXTI4
	
// Init PA4 - PA5	
  GPIOB->CRL |= (1 << (4*0)) | (1 << (4*1)); // Output pin PB0, PB1
  GPIOB->ODR |= (1 << 0); // logic to pin IN1, IN2
  GPIOB->ODR &= ~(1 << 1);
	
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


void dir(double value){
	if (value > 0) {
		GPIOB->ODR |= (1 << 0); // logic to pin IN1, IN2
		GPIOB->ODR &= ~(1 << 1);		
	}
	else if (value < 0) {
		GPIOB->ODR &= ~(1 << 0); // logic to pin IN1, IN2
		GPIOB->ODR |= (1 << 1);
	}
	else {
		GPIOB->ODR &= ~(1 << 0); // logic to pin IN1, IN2
		GPIOB->ODR &= ~(1 << 1);	
	}
}

void EXTI3_IRQHandler(void){
  EXTI->PR |= (1 << 3);//clear interupt flag EXTI3
  CountValue++;    
}

void EXTI4_IRQHandler(void){
  EXTI->PR |= (1 << 4);//clear interupt flag EXTI3
  CountValue++;    
}

void send_char(char data){
	while((USART1->SR & (1 << 6)) == 0); 
	USART1->DR = data;
}

void send_string(char *str){
	while(*str) send_char(*str++);
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
