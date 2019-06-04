/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "LSM6DS3_ACC_GYRO_driver.h"
#include "display.h"
#include "bsp_globalfunctions.h"
#include "math.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
volatile int16_t pwm_value = 1000;
volatile int16_t step = 0;
volatile  GPIO_PinState pin1 = GPIO_PIN_SET;
volatile  GPIO_PinState pin1Old = GPIO_PIN_SET;
volatile uint8_t state = 0;
volatile uint8_t low = 0;
volatile uint8_t high = 0;
volatile uint16_t accValue = 0;
uint8_t receive = 0;
uint8_t transmit = 0;
uint8_t spi_check_state = HAL_ERROR;
uint8_t sor[64];
uint8_t data[64];
volatile uint16_t titleOld = 0;
volatile float rollToValOld = 0;
extern uint8_t exti_flag;
volatile float rollOffset = 0;
volatile float pitchOffset = 0;
volatile uint8_t isOK = 0;

extern UART_HandleTypeDef huart2;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void user_pwm_setvalue(uint16_t value);
uint8_t user_spi_check();
void user_pwm_detect_edge_and_modify();
void user_spi_read_register(uint8_t register_id);
void user_lcd_write_uint16_t(uint16_t integer, uint8_t positionX, uint8_t positionY);
uint16_t user_8bit_to_16bit(uint8_t lower, uint8_t higher);
void user_spi_read_acc_values();
float user_16bit_2s_complement_to_decimal(uint16_t number);
void user_generate_arrays();

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_SPI3_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* start user functions and generate arrays */
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  spi_check_state = user_spi_check();
  user_generate_arrays();
  user_pwm_setvalue(pwm_value);
  GlobalFunctions_Init();
  GLCD_Init();

  /* sensor check and error handler*/
  if (HAL_OK == spi_check_state)
  {
	 GLCD_WriteString("Sensor is ok", 1,20);
	 isOK = 1;
	 HAL_Delay(1000);
	 GLCD_Clear();
  }
  else
  {
  	 GLCD_WriteString("Sensor error", 1,20);
  	 isOK = 0;
  }

  uint8_t testDataToSend[8];

    for (uint8_t i = 0; i < 8; i++)
    {
      testDataToSend[i] = i;
    }
   uint8_t cntr = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	HAL_Delay(100);
	user_pwm_detect_edge_and_modify();
	if (isOK == 1 )
	{
		user_spi_read_acc_values();

	}
	if (cntr == 10)
	{
		cntr = 0;

		//HAL_UART_Transmit(&huart2,"23.4",8,100);
	}
	cntr++;
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* generate lcd control arrays */
void user_generate_arrays ()
{
	  int i = 0;
	  int db = 0;
	  /* fill the array the numbers of the current row 0...4*/
	  for (i=0; i < 64; i++)
	  {
		  if(((i % 8) == 0) && (i != 0))
		  {
			  db++;
		  }
		  sor[i] = db;
	  }

	  /* fill the array powers of 2 in 8 bit with circular */
	  i = 0;
	  db = 0;
	  for (i=0; i < 64; i++)
	  {
		  if((i % 8) == 0)
		  {
			  db = 1;
		  }
		  data[i] = db;
		  db = db*2;
	  }
}

/* pwm value setter */
void user_pwm_setvalue(uint16_t value)
{
    TIM_OC_InitTypeDef sConfigOC;

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = value;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
}

uint8_t user_spi_check()
{
	/* piece together the options of accelerometer */
	uint8_t dataToWrite = 0;
	dataToWrite |= LSM6DS3_ACC_GYRO_BW_XL_100Hz;
	dataToWrite |= LSM6DS3_ACC_GYRO_FS_XL_8g;
	dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_104Hz;

	/* read the who i am register */
	transmit = (LSM6DS3_ACC_GYRO_WHO_AM_I_REG | LSM6DS3_ACC_GYRO_READ_BIT);
	HAL_GPIO_WritePin(STM_SPI_CS_GPIO_Port, STM_SPI_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3,&transmit,1,50);
	HAL_SPI_Receive(&hspi3,&receive,1,50);
	HAL_GPIO_WritePin(STM_SPI_CS_GPIO_Port, STM_SPI_CS_Pin, GPIO_PIN_SET);

	/* write the options to the CTRL register */
	transmit = (LSM6DS3_ACC_GYRO_CTRL1_XL | LSM6DS3_ACC_GYRO_WRITE_BIT);
	HAL_GPIO_WritePin(STM_SPI_CS_GPIO_Port, STM_SPI_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3,&transmit,1,50);
	HAL_SPI_Transmit(&hspi3,&dataToWrite,1,50);
	HAL_GPIO_WritePin(STM_SPI_CS_GPIO_Port, STM_SPI_CS_Pin, GPIO_PIN_SET);

	/* check the who i am constant value */
	if (LSM6DS3_ACC_GYRO_WHO_AM_I != receive)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}

uint8_t y = 1;

void user_spi_read_acc_values()
{
	/* read acc datas */
	float accX = 0;
	float accY = 0;
	float accZ = 0;
	user_spi_read_register(LSM6DS3_ACC_GYRO_OUTX_H_XL);
	high = receive;
	user_spi_read_register(LSM6DS3_ACC_GYRO_OUTX_L_XL);
	low = receive;
	accX= user_16bit_2s_complement_to_decimal(user_8bit_to_16bit(low, high));

	user_spi_read_register(LSM6DS3_ACC_GYRO_OUTY_H_XL);
	high = receive;
	user_spi_read_register(LSM6DS3_ACC_GYRO_OUTY_L_XL);
	low = receive;
	accY= user_16bit_2s_complement_to_decimal(user_8bit_to_16bit(low, high));

	user_spi_read_register(LSM6DS3_ACC_GYRO_OUTZ_H_XL);
	high = receive;
	user_spi_read_register(LSM6DS3_ACC_GYRO_OUTZ_L_XL);
	low = receive;
	accZ= user_16bit_2s_complement_to_decimal(user_8bit_to_16bit(low, high));

	/* calculate tilt angles and draw */
	volatile float roll = (atan2(accY, accZ) * -57.3) + 90 - rollOffset;
	volatile float pitch = (atan2((- accX) , sqrt(accY * accY + accZ * accZ)) * 57.3) + 90 - pitchOffset;

	/* scale angles to pixel locations */
	volatile float temp = (pitch/180)*64;
	volatile uint16_t title = (uint16_t)temp;
	volatile float rollToVal = round((roll/180)*128);

	/* delete the older point */
	if ((titleOld != title) || (rollToVal != rollToValOld))
	{
		GLCD_Write_Block(0x00, sor[titleOld], rollToValOld);
	}

	/* in case of two point are in same byte data column */
	if ((rollToVal == 64) && (sor[title] == sor[32]))
	{
		GLCD_Write_Block((data[title] | data[32]), sor[title], 64);
	}
	else
	{
		GLCD_Write_Block(data[title], sor[title], rollToVal);
		GLCD_Write_Block(data[32], sor[32], 64);
	}

	/* save older states */
	titleOld = title;
	rollToValOld = rollToVal;

	/* set new offset and center */
	if (exti_flag == 1)
	{
		rollOffset = roll - 90 + rollOffset;
		pitchOffset = pitch - 90 + pitchOffset;
		exti_flag = 0;
	}
}


float user_16bit_2s_complement_to_decimal(uint16_t number)
{
	/* check the sign */
	GPIO_PinState isNegative = (number & (1 << 15)) != 0;

	if( isNegative )
	{
		/* when it is negative -> calculate to decimal */
		number = (~number) + 1;
	}

	/* cast to float and write sign */
	float ret = (float)number;

	if( isNegative )
	{
		return ret*(-1);
	}
	return ret;
}

void user_lcd_write_uint16_t(uint16_t integer, uint8_t positionX, uint8_t positionY)
{
	float fValue =0;
	uint8_t number[6];
	int i = 0;

	/* check sign and set first caracter code (+/-)*/
	if ((integer & 0x8000) != 0)
	{
		fValue = user_16bit_2s_complement_to_decimal(integer)*(-1);
		integer = (uint16_t)fValue;
		number[0] = 13;
	}
	else
	{
		number[0] = 11;
	}

	/* dispart the decimals and write array to LCD*/
	number[1] = (integer / 10000);
	number[2] = (integer / 1000) - (number[1]*10);
	number[3] = (integer / 100) - (number[1]*100) - (number[2]*10);
	number[4] = (integer / 10) - (number[1]*1000) - (number[2]*100) - (number[3]*10);
	number[5] = integer - (number[1]*10000) - (number[2]*1000) - (number[3]*100) - (number[4]*10);
	GLCD_Write_Char((number[0]), positionY, positionX);
	for(i = 1; i < 6; i++)
	{
			positionX += 5;
			GLCD_Write_Char((number[i]+16), positionY, positionX);
	}
}

/* default spi read method */
void user_spi_read_register(uint8_t register_id)
{
	transmit = (register_id | LSM6DS3_ACC_GYRO_READ_BIT);
	HAL_GPIO_WritePin(STM_SPI_CS_GPIO_Port, STM_SPI_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3,&transmit,1,50);
	HAL_SPI_Receive(&hspi3,&receive,1,50);
	HAL_GPIO_WritePin(STM_SPI_CS_GPIO_Port, STM_SPI_CS_Pin, GPIO_PIN_SET);
}

/* converts 2 8bit number to 16 bit */
uint16_t user_8bit_to_16bit(uint8_t lower, uint8_t higher)
{
	return (lower + (higher*256));
}

/* circular pwm value setter */
void user_pwm_detect_edge_and_modify()
{
	/* read btn1 pin */
	pin1 = HAL_GPIO_ReadPin(STM_BTN1_GPIO_Port, STM_BTN1_Pin);

	/* detect rising edge */
	if (pin1 == GPIO_PIN_RESET && pin1Old == GPIO_PIN_SET)
	{
		if (state == 0){ step = 300;}
		else {step = -300;}
	}
	else
	{
		step = 0;
	}

	/* turn back to select increase/decrease the PWM value */
	if (pwm_value >= 3000){ state=1;}
	if (pwm_value <= 100) {state=0;}

	/* add changeable value to pwm duty cycle and set*/
	pwm_value += step;
	user_pwm_setvalue(pwm_value);

	/* save the old state */
	pin1Old = pin1;
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
