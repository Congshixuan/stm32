/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* LED 引脚定义（顺序：LED1~LED4） */
#define LED1_PIN   GPIO_PIN_4
#define LED1_PORT  GPIOB
#define LED2_PIN   GPIO_PIN_5
#define LED2_PORT  GPIOB
#define LED3_PIN   GPIO_PIN_6
#define LED3_PORT  GPIOB
#define LED4_PIN   GPIO_PIN_8
#define LED4_PORT  GPIOB

/* 流水灯移动间隔（毫秒），可调整速度 */
#define FLOW_INTERVAL  500
/* 按键消抖时间 */
#define DEBOUNCE_TIME  50

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* 当前流水灯模式：0-无，1-前→后，2-后→前，3-内→外，4-外→内 */
volatile uint8_t led_mode = 0;

/* 上一次模式切换的时间戳（用于消抖） */
static uint32_t last_mode_tick = 0;

/* 流水灯步进时间戳 */
static uint32_t last_flow_tick = 0;

/* 所有 LED 引脚和端口的数组，方便循环操作 */
static const uint16_t led_pins[] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN};
static GPIO_TypeDef* const led_ports[] = {LED1_PORT, LED2_PORT, LED3_PORT, LED4_PORT};
#define LED_COUNT  (sizeof(led_pins)/sizeof(led_pins[0]))

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* 四个流水灯函数声明 */
static void flow_forward(void);
static void flow_backward(void);
static void flow_inner_to_outer(void);
static void flow_outer_to_inner(void);

/* 辅助函数声明 */
static void all_led_off(void);
static void led_on(uint8_t index);
static void set_leds_by_mask(uint8_t mask);
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

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
	switch (led_mode)
{
    case 1: flow_forward(); break;
    case 2: flow_backward(); break;
    case 3: flow_inner_to_outer(); break;
    case 4: flow_outer_to_inner(); break;
    default: all_led_off(); break;
}
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
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint32_t current_tick = HAL_GetTick();
    // 消抖：距离上次模式切换超过 DEBOUNCE_TIME 才响应
    if (current_tick - last_mode_tick < DEBOUNCE_TIME)
        return;

    switch (GPIO_Pin)
    {
        case GPIO_PIN_2:   // PA2 → 模式1：从前往后
            led_mode = 1;
            last_mode_tick = current_tick;
            break;
        case GPIO_PIN_4:   // PA4 → 模式2：从后往前
            led_mode = 2;
            last_mode_tick = current_tick;
            break;
        case GPIO_PIN_6:   // PA6 → 模式3：从内向外
            led_mode = 3;
            last_mode_tick = current_tick;
            break;
        case GPIO_PIN_0:   // PB0 → 模式4：从外向内
            led_mode = 4;
            last_mode_tick = current_tick;
            break;
        default:
            break;
    }
}

/* 熄灭所有 LED */
static void all_led_off(void)
{
    for (int i = 0; i < LED_COUNT; i++) {
        HAL_GPIO_WritePin(led_ports[i], led_pins[i], GPIO_PIN_SET);  // 高电平熄灭（因输出初始高）
    }
}

/* 点亮指定索引的 LED（0~3） */
static void led_on(uint8_t index)
{
    if (index < LED_COUNT) {
        HAL_GPIO_WritePin(led_ports[index], led_pins[index], GPIO_PIN_RESET); // 低电平点亮
    }
}

/* 根据 4 位掩码设置 LED 状态（bit0→LED1, bit1→LED2, … 1=点亮） */
static void set_leds_by_mask(uint8_t mask)
{
    for (int i = 0; i < LED_COUNT; i++) {
        if (mask & (1 << i))
            HAL_GPIO_WritePin(led_ports[i], led_pins[i], GPIO_PIN_RESET);
        else
            HAL_GPIO_WritePin(led_ports[i], led_pins[i], GPIO_PIN_SET);
    }
}

/* 模式1：从前往后依次点亮（LED1→LED2→LED3→LED4 循环） */
static void flow_forward(void)
{
    static uint8_t step = 0;
    uint32_t now = HAL_GetTick();
    if (now - last_flow_tick >= FLOW_INTERVAL) {
        all_led_off();
        led_on(step);
        step = (step + 1) % LED_COUNT;
        last_flow_tick = now;
    }
}

/* 模式2：从后往前依次点亮（LED4→LED3→LED2→LED1 循环） */
static void flow_backward(void)
{
    static uint8_t step = 0;
    uint32_t now = HAL_GetTick();
    if (now - last_flow_tick >= FLOW_INTERVAL) {
        all_led_off();
        led_on(LED_COUNT - 1 - step);
        step = (step + 1) % LED_COUNT;
        last_flow_tick = now;
    }
}

/* 模式3：从内向外扩散（先亮中间两个 LED2,LED3，再全亮，再全灭，循环） */
static void flow_inner_to_outer(void)
{
    static uint8_t step = 0;
    uint32_t now = HAL_GetTick();
    if (now - last_flow_tick >= FLOW_INTERVAL) {
        switch (step) {
            case 0: set_leds_by_mask(0x06); break;  // 亮 LED2, LED3 (二进制 0110)
            case 1: set_leds_by_mask(0x0F); break;  // 全亮 (二进制 1111)
            case 2: all_led_off(); break;           // 全灭
        }
        step = (step + 1) % 3;
        last_flow_tick = now;
    }
}

/* 模式4：从外向内（先亮两边 LED1,LED4，再全亮，再全灭，循环） */
static void flow_outer_to_inner(void)
{
    static uint8_t step = 0;
    uint32_t now = HAL_GetTick();
    if (now - last_flow_tick >= FLOW_INTERVAL) {
        switch (step) {
            case 0: set_leds_by_mask(0x09); break;  // 亮 LED1, LED4 (二进制 1001)
            case 1: set_leds_by_mask(0x0F); break;  // 全亮
            case 2: all_led_off(); break;           // 全灭
        }
        step = (step + 1) % 3;
        last_flow_tick = now;
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
