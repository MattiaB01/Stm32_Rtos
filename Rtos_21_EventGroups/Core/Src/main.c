
#include "main.h"
#include "cmsis_os.h"

#include "uart.h"

#define TASK1BIT  (1<<0)
#define TASK2BIT  (1<<1)

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int __io_putchar(int ch);


EventGroupHandle_t xEventGroup;
static void vBitSet(void *pvParameters);
static void vBitRead(void *pvParameters);

int main(void) {
	/* USER CODE BEGIN 1 */
	HAL_Init();

	SystemClock_Config();

	MX_GPIO_Init();

	USART1_UART_Init();

	xEventGroup = xEventGroupCreate();

	xTaskCreate(vBitSet, "bit set", 128, NULL, 1, NULL);
	xTaskCreate(vBitRead, "bit read", 128, NULL, 1, NULL);

	vTaskStartScheduler();

	while (1) {

	}
}

static void vBitSet(void *pvParameters) {
	const TickType_t xDelay500ms = pdMS_TO_TICKS(1500);

	while (1) {
		vTaskDelay(xDelay500ms);
		printf("Bit set task1 \r\n");
		xEventGroupSetBits(xEventGroup, TASK1BIT);
		vTaskDelay(xDelay500ms);
		printf("Bit set task2 \r\n");
		xEventGroupSetBits(xEventGroup, TASK2BIT);
	}

}

static void vBitRead(void *pvParameters) {
	EventBits_t xEventGroupValue;
	const EventBits_t xWaitForBit = { TASK1BIT | TASK2BIT };
	const TickType_t xDelay200ms = pdMS_TO_TICKS(200);
	while (1) {
		xEventGroupValue = xEventGroupWaitBits(xEventGroup, xWaitForBit,
		pdTRUE, //clear on exit
				//pdFALSE //wait for all bits
				pdTRUE,
				portMAX_DELAY);

		if ((xEventGroupValue & TASK1BIT) != 0) {
			printf("Reading bit set TASK1 \r\n");
		}
		if ((xEventGroupValue & TASK2BIT) != 0) {
			printf("Reading bit set TASK2 \r\n");
		}
		vTaskDelay(xDelay200ms);
	}
}

int __io_putchar(int ch) {
	//HAL_UART_Transmit(&huart1, (uint8_t *) &ch , 1,  0xFFFF);
	usart1_write(ch);
	return ch;
}

int usart1_write(int ch) {
	while (!(USART1->SR & 0x0080)) {
	}

	USART1->DR = (ch & 0xFF);
	return ch;

}

void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
//static void MX_USART1_UART_Init(void) {
//
//	/* USER CODE BEGIN USART1_Init 0 */
//
//	/* USER CODE END USART1_Init 0 */
//
//	/* USER CODE BEGIN USART1_Init 1 */
//
//	/* USER CODE END USART1_Init 1 */
//	huart1.Instance = USART1;
//	huart1.Init.BaudRate = 115200;
//	huart1.Init.WordLength = UART_WORDLENGTH_8B;
//	huart1.Init.StopBits = UART_STOPBITS_1;
//	huart1.Init.Parity = UART_PARITY_NONE;
//	huart1.Init.Mode = UART_MODE_TX_RX;
//	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
//	if (HAL_UART_Init(&huart1) != HAL_OK) {
//		Error_Handler();
//	}
//	/* USER CODE BEGIN USART1_Init 2 */
//
//	/* USER CODE END USART1_Init 2 */
//
//}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM1) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
