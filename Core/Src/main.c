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
#include <stdbool.h>
#include "stm32l476g_discovery_glass_lcd.h"
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
LCD_HandleTypeDef hlcd;

RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_LCD_Init(void);
/* USER CODE BEGIN PFP */
void LCD_DisplayDigit(uint8_t digit, DigitPosition_Typedef pos, bool colon);
RTC_TimeTypeDef clockSetting();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_RTC_Init();
  //MX_LCD_Init();
  /* USER CODE BEGIN 2 */
  BSP_LCD_GLASS_Init();
  BSP_JOY_Init(JOY_MODE_GPIO);
  BSP_LED_Init(LED4);
  BSP_LCD_GLASS_BarLevelConfig(BATTERYLEVEL_FULL);

  LCD_DisplayDigit(0, 0, false);
  LCD_DisplayDigit(0, 1, true);

  LCD_DisplayDigit(0, 2, false);
  LCD_DisplayDigit(0, 3, true);

  LCD_DisplayDigit(0, 4, false);
  LCD_DisplayDigit(0, 5, false);

  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  RTC_TimeTypeDef new_time = clockSetting();
  HAL_RTC_SetTime(&hrtc, &new_time, RTC_FORMAT_BIN);

  int alarm = 0;
  RTC_TimeTypeDef alarmTime = {0};

  uint32_t alarmStartTicks = HAL_GetTick();
  int alarmBlink = 0;

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	/**  while(setup)
	  {
		  state = BSP_JOY_GetState();

		  switch(state){
		  case JOY_UP:

			  uint8_t maxval = 0;

			  switch(pos){
			  case 0: maxval = 2; break;

			  case 1:
				  if (ar[0] >= 2)
					  maxval = 3;
				  else
					  maxval = 9;
				  break;

			  case 2:
			  case 4: maxval = 5; break;

			  case 3:
			  case 5: maxval = 9; break;
			  }

			  if(ar[pos] < maxval)
				  ar[pos]++;

			  if(ar[0] == 2 && ar[1] > 3)
			  {
				  ar[1] = 3;
				  LCD_DisplayDigit(ar[1], 1, false);
			  }

			  LCD_DisplayDigit(ar[pos], pos, (pos%2 && pos != 5));
			  break;

		  case JOY_DOWN:
			  if(ar[pos] > 0)
			  	  ar[pos]--;

			  LCD_DisplayDigit(ar[pos], pos, (pos%2 && pos != 5));
			  break;

		  case JOY_LEFT:
			  if(pos > 0)
			  {
				  LCD_DisplayDigit(ar[pos], pos, (pos%2 && pos != 5));
				  blink = 0;

				  pos--;
			  }

			  break;

		  case JOY_RIGHT:
			  if(pos < 5)
			  {
				  LCD_DisplayDigit(ar[pos], pos, (pos%2 && pos != 5));
				  blink = 0;

				  pos++;
			  }

			  break;

		  case JOY_SEL:
			  	setup = 0;

				RTC_TimeTypeDef new_time = {0};

				new_time.Hours = ar[0]*10 + ar[1];
				new_time.Minutes = ar[2]*10 + ar[3];
				new_time.Seconds = ar[4]*10 + ar[5];
				HAL_RTC_SetTime(&hrtc, &new_time, RTC_FORMAT_BIN);
			  break;

		  default:
			  break;
		  }
		  HAL_Delay(135);

		  if((HAL_GetTick() - start_ticks) > 500){
			  if(!blink)
				  BSP_LCD_GLASS_DisplayChar("", POINT_OFF, (pos%2 && pos != 5) ? DOUBLEPOINT_ON : DOUBLEPOINT_OFF, pos);
			  else
				  LCD_DisplayDigit(ar[pos], pos, (pos%2 && pos != 5));

			  blink = !blink;

			  start_ticks = HAL_GetTick();
		  }
	  }
**/
	  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	  LCD_DisplayDigit(time.Hours/10, 0, false);
	  LCD_DisplayDigit(time.Hours%10, 1, true);

	  LCD_DisplayDigit(time.Minutes/10, 2, false);
	  LCD_DisplayDigit(time.Minutes%10, 3, true);

	  LCD_DisplayDigit(time.Seconds/10, 4, false);
	  LCD_DisplayDigit(time.Seconds%10, 5, false);
	  if(alarm)
		  BSP_LCD_GLASS_DisplayBar(LCD_BAR_3);


	  if(!alarm && BSP_JOY_GetState() == JOY_DOWN){
		  LCD_DisplayDigit(0, 0, false);
		  LCD_DisplayDigit(0, 1, true);

		  LCD_DisplayDigit(0, 2, false);
		  LCD_DisplayDigit(0, 3, true);

		  LCD_DisplayDigit(0, 4, false);
		  LCD_DisplayDigit(0, 5, false);

		  alarmTime = clockSetting();
		  alarm = 1;
	  }
	  else if((time.Hours >= alarmTime.Hours) && (time.Minutes >= alarmTime.Minutes) && (time.Seconds >= alarmTime.Seconds) && alarm == 1)
	  {
		  for(int i = 0; i < 20;)
		  if((HAL_GetTick() - alarmStartTicks) > 250){
		  			  if(!alarmBlink)
		  				BSP_LED_On(LED4);
		  			  else
		  				BSP_LED_Off(LED4);

		  			  alarmBlink = !alarmBlink;

		  			alarmStartTicks = HAL_GetTick();
		  			  i++;
		  		  }
		  BSP_LED_Off(LED4);
		  BSP_LCD_GLASS_ClearBar(LCD_BAR_0);
		  alarm = 0;
	  }

	  HAL_Delay(10);

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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief LCD Initialization Function
  * @param None
  * @retval None
  */
static void MX_LCD_Init(void)
{

  /* USER CODE BEGIN LCD_Init 0 */

  /* USER CODE END LCD_Init 0 */

  /* USER CODE BEGIN LCD_Init 1 */

  /* USER CODE END LCD_Init 1 */
  hlcd.Instance = LCD;
  hlcd.Init.Prescaler = LCD_PRESCALER_1;
  hlcd.Init.Divider = LCD_DIVIDER_16;
  hlcd.Init.Duty = LCD_DUTY_1_2;
  hlcd.Init.Bias = LCD_BIAS_1_4;
  hlcd.Init.VoltageSource = LCD_VOLTAGESOURCE_INTERNAL;
  hlcd.Init.Contrast = LCD_CONTRASTLEVEL_0;
  hlcd.Init.DeadTime = LCD_DEADTIME_0;
  hlcd.Init.PulseOnDuration = LCD_PULSEONDURATION_0;
  hlcd.Init.MuxSegment = LCD_MUXSEGMENT_DISABLE;
  hlcd.Init.BlinkMode = LCD_BLINKMODE_OFF;
  hlcd.Init.BlinkFrequency = LCD_BLINKFREQUENCY_DIV8;
  hlcd.Init.HighDrive = LCD_HIGHDRIVE_DISABLE;
  if (HAL_LCD_Init(&hlcd) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LCD_Init 2 */

  /* USER CODE END LCD_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pins : PA1 PA2 PA3 PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void LCD_DisplayDigit(uint8_t digit, DigitPosition_Typedef pos, bool colon)
{
	if(digit >= 10)
		return;

	  char buffer [2];
	  if(colon)
		  BSP_LCD_GLASS_DisplayChar(itoa(digit,buffer,10), POINT_OFF, DOUBLEPOINT_ON, pos);
	  else
		  BSP_LCD_GLASS_DisplayChar(itoa(digit,buffer,10), POINT_OFF, DOUBLEPOINT_OFF, pos);
}

RTC_TimeTypeDef clockSetting(){
	  JOYState_TypeDef state;
	  bool setup = 1;

	  uint8_t ar[6] = {0,0,0,0,0,0};
	  int pos = 0;
	  uint32_t start_ticks = HAL_GetTick();
	  int blink = 0;

	  while(setup)
	  {
		  state = BSP_JOY_GetState();

		  switch(state){
		  case JOY_UP:

			  uint8_t maxval = 0;

			  switch(pos){
			  case 0: maxval = 2; break;

			  case 1:
				  if (ar[0] >= 2)
					  maxval = 3;
				  else
					  maxval = 9;
				  break;

			  case 2:
			  case 4: maxval = 5; break;

			  case 3:
			  case 5: maxval = 9; break;
			  }

			  if(ar[pos] < maxval)
				  ar[pos]++;

			  if(ar[0] == 2 && ar[1] > 3)
			  {
				  ar[1] = 3;
				  LCD_DisplayDigit(ar[1], 1, false);
			  }

			  LCD_DisplayDigit(ar[pos], pos, (pos%2 && pos != 5));
			  break;

		  case JOY_DOWN:
			  if(ar[pos] > 0)
			  	  ar[pos]--;

			  LCD_DisplayDigit(ar[pos], pos, (pos%2 && pos != 5));
			  break;

		  case JOY_LEFT:
			  if(pos > 0)
			  {
				  LCD_DisplayDigit(ar[pos], pos, (pos%2 && pos != 5));
				  blink = 0;

				  pos--;
			  }

			  break;

		  case JOY_RIGHT:
			  if(pos < 5)
			  {
				  LCD_DisplayDigit(ar[pos], pos, (pos%2 && pos != 5));
				  blink = 0;

				  pos++;
			  }

			  break;

		  case JOY_SEL:
			  	setup = 0;

				RTC_TimeTypeDef new_time = {0};

				new_time.Hours = ar[0]*10 + ar[1];
				new_time.Minutes = ar[2]*10 + ar[3];
				new_time.Seconds = ar[4]*10 + ar[5];
				return new_time;
			  break;

		  default:
			  break;
		  }
		  HAL_Delay(135);

		  if((HAL_GetTick() - start_ticks) > 500){
			  if(!blink)
				  BSP_LCD_GLASS_DisplayChar("", POINT_OFF, (pos%2 && pos != 5) ? DOUBLEPOINT_ON : DOUBLEPOINT_OFF, pos);
			  else
				  LCD_DisplayDigit(ar[pos], pos, (pos%2 && pos != 5));

			  blink = !blink;

			  start_ticks = HAL_GetTick();
		  }
	  }
}

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
