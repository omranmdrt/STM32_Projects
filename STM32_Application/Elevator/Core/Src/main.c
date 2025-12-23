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
#include "hal_config.h"
#include "elevator.h"
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
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */
extern Elevator g_elevator;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_ADC1_Init(void);
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
  MX_TIM1_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); // PWM ÇIKIŞINI BAŞLAT
  ELEVATOR_Init(); // ASANSÖR SİSTEMİNİ BAŞLAT

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  ELEVATOR_MainLoop(); // ASANSÖRÜN ANA DÖNGÜSÜNÜ ÇALIŞTIR.

      // BEKLEME DURUMUNDA DÜŞÜK GÜÇ MODU
      if (g_elevator.state == STATE_IDLE) {
          HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
      }
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.LowPowerAutoPowerOff = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_7CYCLES_5;
  hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_7CYCLES_5;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 319;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.BreakAFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.Break2AFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Overload_LED_GPIO_Port, Overload_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Motor_Door_Close_GPIO_Port, Motor_Door_Close_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, Motor_Door_Open_Pin|Floor_LEDs_Pin|Phone_Module_Pin|Fan_Relay_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Motor_Down_Pin|Motor_Up_Pin|Buzzer_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Floor_N1_But_Pin Floor_0_But_Pin Floor_1_But_Pin Floor_2_But_Pin
                           Floor_3_But_Pin Alarm_But_Pin Door_Open_But_Pin Floor_0_Sensor_Pin
                           Floor_N1_Sensor_Pin */
  GPIO_InitStruct.Pin = Floor_N1_But_Pin|Floor_0_But_Pin|Floor_1_But_Pin|Floor_2_But_Pin
                          |Floor_3_But_Pin|Alarm_But_Pin|Door_Open_But_Pin|Floor_0_Sensor_Pin
                          |Floor_N1_Sensor_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Fan_But_Pin Phone_But_Pin */
  GPIO_InitStruct.Pin = Fan_But_Pin|Phone_But_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : Door_Close_But_Pin Door_Open_Sensor_Pin Door_Close_Sensor_Pin Floor_3_Sensor_Pin
                           Floor_2_Sensor_Pin Floor_1_Sensor_Pin Door_Obstacle_Sensor_Pin */
  GPIO_InitStruct.Pin = Door_Close_But_Pin|Door_Open_Sensor_Pin|Door_Close_Sensor_Pin|Floor_3_Sensor_Pin
                          |Floor_2_Sensor_Pin|Floor_1_Sensor_Pin|Door_Obstacle_Sensor_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : Overload_LED_Pin */
  GPIO_InitStruct.Pin = Overload_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Overload_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Motor_Door_Close_Pin */
  GPIO_InitStruct.Pin = Motor_Door_Close_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Motor_Door_Close_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Motor_Door_Open_Pin Floor_LEDs_Pin Phone_Module_Pin Fan_Relay_Pin */
  GPIO_InitStruct.Pin = Motor_Door_Open_Pin|Floor_LEDs_Pin|Phone_Module_Pin|Fan_Relay_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : Motor_Down_Pin Motor_Up_Pin Buzzer_Pin */
  GPIO_InitStruct.Pin = Motor_Down_Pin|Motor_Up_Pin|Buzzer_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// EXTI KESME GERİ ÇAĞIRMA FONKSİYONU
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint32_t current_tick = HAL_GetTick();

    // Kat tuşları
    if (GPIO_Pin == Floor_N1_But_Pin) {
        if (current_tick - last_button_press_time[0] > DEBOUNCE_TIME_MS) {
            ELEVATOR_AddCall(FLOOR_N1);
            last_button_press_time[0] = current_tick;
        }
    } else if (GPIO_Pin == Floor_0_But_Pin) {
        if (current_tick - last_button_press_time[1] > DEBOUNCE_TIME_MS) {
            ELEVATOR_AddCall(FLOOR_0);
            last_button_press_time[1] = current_tick;
        }
    } else if (GPIO_Pin == Floor_1_But_Pin) {
        if (current_tick - last_button_press_time[2] > DEBOUNCE_TIME_MS) {
            ELEVATOR_AddCall(FLOOR_1);
            last_button_press_time[2] = current_tick;
        }
    } else if (GPIO_Pin == Floor_2_But_Pin) {
        if (current_tick - last_button_press_time[3] > DEBOUNCE_TIME_MS) {
            ELEVATOR_AddCall(FLOOR_2);
            last_button_press_time[3] = current_tick;
        }
    } else if (GPIO_Pin == Floor_3_But_Pin) {
        if (current_tick - last_button_press_time[4] > DEBOUNCE_TIME_MS) {
            ELEVATOR_AddCall(FLOOR_3);
            last_button_press_time[4] = current_tick;
        }
    }
    // Özel tuşlar
    else if (GPIO_Pin == Alarm_But_Pin) {
        if (current_tick - last_button_press_time[5] > DEBOUNCE_TIME_MS) {
            if (g_elevator.state != STATE_ALARM) {
                g_elevator.state = STATE_ALARM;
            }
            last_button_press_time[5] = current_tick;
        }
    } else if (GPIO_Pin == Door_Open_But_Pin) {
        if (current_tick - last_button_press_time[6] > DEBOUNCE_TIME_MS) {
            if (g_elevator.state == STATE_IDLE || g_elevator.state == STATE_DOOR_OPEN || g_elevator.state == STATE_DOOR_CLOSING) {
                g_elevator.state = STATE_DOOR_OPENING;
            }
            last_button_press_time[6] = current_tick;
        }
    } else if (GPIO_Pin == Door_Close_But_Pin) {
        if (current_tick - last_button_press_time[7] > DEBOUNCE_TIME_MS) {
            // Kapı kapatma tuşu basıldığında doğrudan kapı kapanma durumuna geç
            if (g_elevator.state == STATE_DOOR_OPEN) {
                HAL_CloseDoor();
                g_elevator.state = STATE_DOOR_CLOSING;
            }
            last_button_press_time[7] = current_tick;
        }
    } else if (GPIO_Pin == Fan_But_Pin) {
        if (current_tick - last_button_press_time[8] > DEBOUNCE_TIME_MS) {
            g_elevator.ventilation_on = !g_elevator.ventilation_on;
            HAL_ControlVentilation(g_elevator.ventilation_on);
            last_button_press_time[8] = current_tick;
        }
    } else if (GPIO_Pin == Phone_But_Pin) {
        if (current_tick - last_button_press_time[9] > DEBOUNCE_TIME_MS) {
            if (g_elevator.state != STATE_PHONE) {
                g_elevator.state = STATE_PHONE;
            }
            last_button_press_time[9] = current_tick;
        }
    }
    // Kat sensörleri
    else if (GPIO_Pin == Floor_N1_Sensor_Pin) {
        // Asansör -1. kata ulaştı
        if (g_elevator.state == STATE_MOVING_DOWN && g_elevator.target_floor == FLOOR_N1) {
            // Hedef kata ulaşıldı, motoru durdur ve kapıyı aç
            HAL_StopMotor();
            g_elevator.current_floor = FLOOR_N1;
            g_elevator.state = STATE_DOOR_OPENING;
        }
    } else if (GPIO_Pin == Floor_0_Sensor_Pin) {
        // Asansör 0. kata ulaştı
        if ((g_elevator.state == STATE_MOVING_UP && g_elevator.target_floor == FLOOR_0) ||
            (g_elevator.state == STATE_MOVING_DOWN && g_elevator.target_floor == FLOOR_0)) {
            HAL_StopMotor();
            g_elevator.current_floor = FLOOR_0;
            g_elevator.state = STATE_DOOR_OPENING;
        }
    } else if (GPIO_Pin == Floor_1_Sensor_Pin) {
        // Asansör 1. kata ulaştı
        if ((g_elevator.state == STATE_MOVING_UP && g_elevator.target_floor == FLOOR_1) ||
            (g_elevator.state == STATE_MOVING_DOWN && g_elevator.target_floor == FLOOR_1)) {
            HAL_StopMotor();
            g_elevator.current_floor = FLOOR_1;
            g_elevator.state = STATE_DOOR_OPENING;
        }
    } else if (GPIO_Pin == Floor_2_Sensor_Pin) {
        // Asansör 2. kata ulaştı
        if ((g_elevator.state == STATE_MOVING_UP && g_elevator.target_floor == FLOOR_2) ||
            (g_elevator.state == STATE_MOVING_DOWN && g_elevator.target_floor == FLOOR_2)) {
            HAL_StopMotor();
            g_elevator.current_floor = FLOOR_2;
            g_elevator.state = STATE_DOOR_OPENING;
        }
    } else if (GPIO_Pin == Floor_3_Sensor_Pin) {
        // Asansör 3. kata ulaştı
        if (g_elevator.state == STATE_MOVING_UP && g_elevator.target_floor == FLOOR_3) {
            HAL_StopMotor();
            g_elevator.current_floor = FLOOR_3;
            g_elevator.state = STATE_DOOR_OPENING;
        }
    }
    // Kapı sensörleri
    else if (GPIO_Pin == Door_Open_Sensor_Pin) {
        // Kapı tamamen açık sensörü tetiklendiğinde
        if (g_elevator.state == STATE_DOOR_OPENING) {
            HAL_StopDoor(); // Kapı motorunu durdur
            g_elevator.state = STATE_DOOR_OPEN; // Durumu kapı açık olarak ayarla
            g_elevator.door_timer = HAL_GetTick(); // Kapı açık kalma süresi için zamanlayıcıyı başlat
        }
    } else if (GPIO_Pin == Door_Close_Sensor_Pin) {
        // Kapı tamamen kapalı sensörü tetiklendiğinde
        if (g_elevator.state == STATE_DOOR_CLOSING) {
            HAL_StopDoor(); // Kapı motorunu durdur
            g_elevator.state = STATE_IDLE; // Durumu boşta olarak ayarla
        }
    } else if (GPIO_Pin == Door_Obstacle_Sensor_Pin) {
        // Kapı kapanırken engel algılandı
        if (g_elevator.state == STATE_DOOR_CLOSING) {
            HAL_OpenDoor(); // Kapıyı tekrar aç
            g_elevator.state = STATE_DOOR_OPENING; // Durumu kapı açılıyor olarak ayarla
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
#ifdef USE_FULL_ASSERT
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
