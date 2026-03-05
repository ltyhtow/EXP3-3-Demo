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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "iic_oled.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  /* USER CODE BEGIN 2 */
	OLED_Init();
	OLED_Clear();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		OLED_ShowChar(0 , 1 , 'H' , FSIZE8_6 , 1);//显示单个字母 H
		OLED_ShowChar(0 , 9 , 'H' , FSIZE12_6 , 1);
		OLED_ShowChar(0 , 22 , 'H' , FSIZE16_8 , 1);
		OLED_ShowChar(0 , 39 , 'H' , FSIZE24_12 , 1);
		HAL_Delay(500);
		OLED_ClearRAM();//清除缓存和OLED显示
		
		OLED_ShowStr(0 , 1 , (unsigned char *)"Hello" , FSIZE8_6 , 1);//显示字符串 Hello
		OLED_ShowStr(0 , 9 , (unsigned char *)"Hello" , FSIZE12_6 , 1);
		OLED_ShowStr(0 , 22 ,(unsigned char *)"Hello" , FSIZE16_8 , 1);
		OLED_ShowStr(0 , 39 ,(unsigned char *)"Hello" , FSIZE24_12 , 1);
		HAL_Delay(500);
		OLED_ClearRAM();//清除缓存和OLED显示
		
		OLED_ShowChinese(0 , 9 , (unsigned char *)"教" , FSIZE12_6 , 1);//显示中文文字
		OLED_ShowChinese(0 , 22 , (unsigned char *)"教" , FSIZE16_8 , 1);
		OLED_ShowChinese(0 , 39 , (unsigned char *)"教" , FSIZE24_12 , 1);
		HAL_Delay(500);
		OLED_ClearRAM();//清除缓存和OLED显示
		
		OLED_ShowStringChinese(0 , 9 , (unsigned char *)"风标教育/FBEDU"  , FSIZE12_6 , 1);//显示中文文字加英文字符串
		OLED_ShowStringChinese(0 , 22 ,(unsigned char *)"风标教育/FBEDU"  , FSIZE16_8 , 1);
		OLED_ShowStringChinese(0 , 39 ,(unsigned char *)"风标教育/FBEDU"  , FSIZE24_12 , 1);
		HAL_Delay(500);
		OLED_ClearRAM();//清除缓存和OLED显示
		
		OLED_DrawLine( 0 , 0 , 127 ,63 , 1, 1);//画线:从0，0坐标到127，63坐标
		HAL_Delay(500);
		OLED_DrawLine( 0 , 0 , 127 ,63 , 0, 1);//清除画线
		HAL_Delay(500);
		
		OLED_DrawRectangle( 0 , 10 , 120 ,60 , 1, 1);//画矩形,从0，0坐标到120，60坐标
		HAL_Delay(500);
		OLED_DrawRectangle( 0 , 10 , 120 ,60 , 0, 1);//清除矩形
		HAL_Delay(500);
		
		LCD_DrawCircle( 64 , 32 , 20  , 1, 1);//画圆,圆心坐标为64，32，半径为20
		HAL_Delay(500);
		LCD_DrawCircle( 64 , 32 , 20  , 0, 1);//清除圆
		HAL_Delay(500);
		
		OLED_ShowBMP(0 , 0 , 128 , 64 , BMP1 ,1);//显示BMP图片1
		HAL_Delay(500);
		OLED_ShowBMP(0 , 0 , 128 , 64 , BMP3 , 1);//显示BMP图片3
		HAL_Delay(500);
		OLED_ClearRAM();//清除缓存和OLED显示
		HAL_Delay(500);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
