#include "main.h"
#include "cmsis_os.h"

UART_HandleTypeDef huart1;
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

void senderTask1(void *pvParameters);
void senderTask2(void *pvParameters);

void receiverTask(void *pvParameters);

//implementando putchar / printf
int __io_putchar(int ch);

//
//Aggiungere in FreeRTOSConfig
//#define configUSE_QUEUE_SET 1
//#include semphr.h
//

TaskHandle_t sendHandle, receiveHandle;

static QueueHandle_t xQueue1 = NULL, xQueue2 = NULL;

static QueueSetHandle_t xQueueSet = NULL;

int main(void) {
	/* USER CODE BEGIN 1 */
	HAL_Init();

	SystemClock_Config();

	MX_GPIO_Init();
	MX_USART1_UART_Init();

	xQueue1 = xQueueCreate(1, sizeof(char*));

	xQueue2 = xQueueCreate(1, sizeof(char*));


	//crea un set di code per tenere 2 elementi coda di lunghezza 1
	xQueueSet = xQueueCreateSet(1*2);

	//aggiungere le code al code set
	xQueueAddToSet(xQueue1, xQueueSet);
	xQueueAddToSet(xQueue2, xQueueSet);



	xTaskCreate(senderTask1, "Sender Task1",200, NULL, 1, &sendHandle);

	xTaskCreate(senderTask2, "Sender Task2", 200, NULL, 1, &sendHandle);

	xTaskCreate(receiverTask, "Receiver Task", 200, NULL, 1, &receiveHandle);
	vTaskStartScheduler();

	while (1) {

	}
}

void senderTask1(void *pvParameters) {

	const TickType_t wait_time = pdMS_TO_TICKS(500);

	char * msg = "Welcome from Task1 \r\n";

	while (1) {

		vTaskDelay(wait_time);
		xQueueSend(xQueue1, &msg, 0);

	}
}

void senderTask2(void *pvParameters) {
	const TickType_t wait_time = pdMS_TO_TICKS(1000);

	char *  msg = "Welcome from Task2 \r\n";

	while (1) {

		vTaskDelay(wait_time);
		xQueueSend(xQueue2, &msg, 0);
	}
}



void receiverTask(void *pvParameters) {

	QueueHandle_t xQueueThatContainsData;

	char * receivedString;


	while(1){
		xQueueThatContainsData = (QueueHandle_t) xQueueSelectFromSet(xQueueSet, portMAX_DELAY);

		xQueueReceive(xQueueThatContainsData, &receivedString, 0);

		printf("Stringa ricevuta: %s",receivedString,"\r\n");

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

	USART1->DR = (ch & 0xff);
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
static void MX_USART1_UART_Init(void) {

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

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
