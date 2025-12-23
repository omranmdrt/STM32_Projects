/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "ft24c02a_eeprom.h"
#include "stdio.h"
#include "string.h"
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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;


/* USER CODE BEGIN PV */

// FT24C02A yapılandırma yapısı
FT24C02A_HandleTypeDef hft24c02a;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// I2C yazma fonksiyonu (STM32 HAL kütüphanesi için)
int FT24C02A_I2C_Write(uint8_t devAddr, const uint8_t* pData, uint16_t size, uint32_t timeout)
{
  return HAL_I2C_Master_Transmit(&hi2c1, devAddr << 1, (uint8_t*)pData, size, timeout);
}

// I2C okuma fonksiyonu (STM32 HAL kütüphanesi için)
int FT24C02A_I2C_Read(uint8_t devAddr, uint8_t* pData, uint16_t size, uint32_t timeout)
{
  return HAL_I2C_Master_Receive(&hi2c1, devAddr << 1, pData, size, timeout);
}

// EEPROM kütüphanesini başlatma
void FT24C02A_Init(void)
{
  // I2C haberleşme fonksiyonlarını ayarla
  hft24c02a.IO.i2c_write = FT24C02A_I2C_Write;
  hft24c02a.IO.i2c_read = FT24C02A_I2C_Read;
  hft24c02a.IO.delay_ms = HAL_Delay;

  // Cihaz adresini ayarla
  hft24c02a.DevAddr = FT24C02A_DEFAULT_ADDR;
}

// EEPROM test fonksiyonu
void FT24C02A_Test(void)
{

// Tekli veri için

  uint8_t writeData1 = 244;
  uint8_t readData1 = 0;
  uint8_t memAddr1 = 0x10;

  uint8_t writeData2 = 24;
  uint8_t readData2 = 0;
  uint8_t memAddr2 = 0x20;

// Çoklu veri için

  uint8_t writeData3[4] = {11,22,33,44};
  uint8_t readData3[4] = {0};
  uint8_t memAddr3 = 0x30;

// Sayfaya yazmak için

  uint8_t writeData4[5] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE};
  uint8_t readData4[5] = {0};
  uint8_t memAddr4 = 0x40;

//Kelime yazdırma

  char mesaj[] = "Merhaba Exentech'e Hos geldiniz , STM32 ile EEPROM sayfa yazma!";
  char okunan[100] = {0};
  uint8_t memAddr5 = 0x60;


  FT24C02A_Status status;

  // ilk Veri yazma
  status = FT24C02A_WriteByte(&hft24c02a, memAddr1, writeData1);
  if (status != FT24C02A_OK) {
    printf(" tek Yazma hatasi: %d\r\n", status);
    return;
  }

  // ilk  Veri okuma
  status = FT24C02A_ReadByte(&hft24c02a, memAddr1, &readData1);
  if (status != FT24C02A_OK) {
    printf("tek Okuma hatasi: %d\r\n", status);
    return;
  }



  // ikinci Veri yazma
   status = FT24C02A_WriteByte(&hft24c02a, memAddr2, writeData2);
   if (status != FT24C02A_OK) {
     printf(" tek_1 Yazma hatasi: %d\r\n", status);
     return;
   }

   // ikinci Veri okuma
   status = FT24C02A_ReadByte(&hft24c02a, memAddr2, &readData2);
   if (status != FT24C02A_OK) {
     printf("tek_1 Okuma hatasi: %d\r\n", status);
     return;
   }



   // Çoklu veri yazma
   status = FT24C02A_WriteBuffer(&hft24c02a, memAddr3, writeData3, sizeof (writeData3));
   if(status != FT24C02A_OK){
	    printf("Coklu yazima hata: %d\r\n", status);
	   return;
   }

   //Çoklu veri okuma
   status = FT24C02A_ReadBuffer(&hft24c02a, memAddr3, readData3, sizeof(readData3));
   if (status != FT24C02A_OK) {
        printf(" Coklu Okuma hatasi: %d\r\n", status);
        return;
      }



   // Sayfa yazma
   status = FT24C02A_WritePage(&hft24c02a, memAddr4, writeData4, 5);
   if (status != FT24C02A_OK) {
     printf("Sayfa yazma hatasi: %d\r\n", status);
     return;
   }

   //Sayfa okuma
   status = FT24C02A_ReadPage(&hft24c02a, memAddr4, readData4, 5);
   if (status != FT24C02A_OK) {
     printf("Sayfa okuma hatasi: %d\r\n", status);
     return;
   }



   //yazı yazma
   status = FT24C02A_WriteLongData(&hft24c02a, memAddr5,(uint8_t*)mesaj , sizeof(mesaj));
   if (status != FT24C02A_OK) {
     printf(" Yazi Yazma hatasi: %d\r\n", status);
     return;
   }

   // yazı okuma
   status = FT24C02A_ReadLongData(&hft24c02a, memAddr5, (uint8_t*)okunan, sizeof(okunan));
   if (status != FT24C02A_OK) {
     printf("Yazi Okuma hatasi: %d\r\n", status);
     return;
   }




  // Doğrulama
  if (readData1 == writeData1 && readData2 == writeData2) {
	printf("# TEKLI VERI #\r\n");
    printf("Test basarili! Yazilan: 0x%02X,     Okunan: 0x%02X\r\n", writeData1, readData1);
    printf("Test basarili! Yazilan: 0x%02X,     Okunan: 0x%02X\r\n", writeData2, readData2);

    printf("# COKLU VERI #\r\n");

    for (int i = 0; i < 4; i++) {
    printf("Test basarili! Yazilan: 0x%02X,      Okunan: 0x%02X\r\n", writeData3[i], readData3[i]);
    }

    printf(" # SAYFA YAZMA OKUMA #\r\n");
     for (int i = 0; i < 5; i++) {
       printf("Test basarili! Yazilan: 0x%02X,     Okunan: 0x%02X\r\n", writeData4[i], readData4[i]);
     }
     printf("# YAZI YAZDIRMA #\n");
     printf("Yazilan mesaj: %s\nOkunan mesaj:  %s\r\n",mesaj, okunan);




  } else {
    printf("Test basarisiz! Yazilan: 0x%02X,     Okunan: 0x%02X\r\n", writeData1, readData1);
    printf("Test basarisiz! Yazilan: 0x%02X,     Okunan: 0x%02X\r\n", writeData2, readData2);
  }
//  FT24C02A_Clear(&hft24c02a);

}




// printf için gerekli yapılandırma
int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

int _write(int file, char *ptr, int len)
{
  int DataIdx;
  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    __io_putchar(*ptr++);
  }
  return len;
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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  // EEPROM kütüphanesini başlat
   FT24C02A_Init();

   // EEPROM test fonksiyonunu çağır
   FT24C02A_Test();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  uint8_t komut = 0;
//	  // UART'tan 1 byte oku (bloklayıcı)
//	      if (HAL_UART_Receive(&huart2, &komut, 1, HAL_MAX_DELAY) == HAL_OK)
//	      {
//	          if (komut == 'c' || komut == 'C')  // 'c' komutu: clear
//	          {
//	              printf("\r\nEEPROM siliniyor...\r\n");
//	              FT24C02A_Clear(&hft24c02a);
//	          }
//	          else if (komut == 't' || komut == 'T') // 't' komutu: test
//	          {
//	              printf("\r\nTest başlatılıyor...\r\n");
//	              FT24C02A_Test();
//	          }
//	          else
//	          {
//	              printf("Geçersiz komut: %c\r\n", komut);
//	          }
//	      }
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00503D58;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

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
