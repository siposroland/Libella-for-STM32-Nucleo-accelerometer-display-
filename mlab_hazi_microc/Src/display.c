/**
*****************************************************************************
** KommunikÄ‚Ë‡ciÄ‚Ĺ‚s mÄ‚Â©rÄ‚Â©s - glcd.c
** A grafikus LCD fÄ‚Ä˝ggvÄ‚Â©nyei
*****************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"
#include "display.h"
#include "bsp_globalfunctions.h"
#include "font.h"

void GLCD_MSP_Init(void);
uint16_t GLCD_Read_Pins(void);
void GLCD_Write_Pins(uint16_t dataToWrite);

const uint16_t pins[8] ={STM_LCD_0_Pin, STM_LCD_1_Pin, STM_LCD_2_Pin,
		STM_LCD_3_Pin, STM_LCD_4_Pin, STM_LCD_5_Pin, STM_LCD_6_Pin, STM_LCD_7_Pin};

/* ----------------- Publikus fÄ‚Ä˝ggvÄ‚Â©nyek ----------------- */

void GLCD_Init(void)
{
	GLCD_MSP_Init();

	// Init GLCD functions
	HAL_GPIO_WritePin(STM_E_GPIO_Port, STM_E_Pin, GPIO_PIN_RESET);	// GLCD_E = 0
	HAL_GPIO_WritePin(STM_RST_GPIO_Port, STM_RST_Pin, GPIO_PIN_SET);	// GLCD_RESET = 1

	GLCD_Write(3,0,0x3F); 	//GLCD bekapcsolÄ‚Ë‡sa
	GLCD_Write(3,0,0xC0);	//GLCD Start Line
	GLCD_Clear();			//KijelzÄ‚Âµ tÄ‚Â¶rlÄ‚Â©se
}

void GLCDEN(int newState)
{
	if(newState)
	{
		HAL_GPIO_WritePin(STM_OE_NEG_GPIO_Port, STM_OE_NEG_Pin, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(STM_OE_NEG_GPIO_Port, STM_OE_NEG_Pin, GPIO_PIN_SET);
	}
}

/** BeÄ‚Â­rja a g_data Ä‚Â©rtÄ‚Â©ket a kivÄ‚Ë‡lasztott kijelzÄ‚ÂµvezÄ‚Â©rlÄ‚Âµbe (cs_s->CS1, CS2) utasÄ‚Â­tÄ‚Ë‡s/adat paramÄ‚Â©ternek megfelelÄ‚Âµen.
* Ä‚ďż˝ltalÄ‚Ë‡nosan felhasznÄ‚Ë‡lhatÄ‚Ĺ‚ 8bit (adat/utasÄ‚Â­tÄ‚Ë‡s) beÄ‚Â­rÄ‚Ë‡sÄ‚Ë‡ra a kijelzÄ‚Âµ vezÄ‚Â©rlÄ‚ÂµjÄ‚Â©be.
* 	cs_s, 1 = CS1, 2 = CS2, 3 = CS1&CS2
* 	d_i, 0 = instruction, 1 = data
*/
void GLCD_Write(char cs_s,char d_i,char g_data)
{
	volatile uint16_t data = 0x0000;
	switch(cs_s)
	{
	case 1:
		HAL_GPIO_WritePin(STM_CS1_NEG_GPIO_Port, STM_CS1_NEG_Pin, GPIO_PIN_SET);
		break;
	case 2:
		HAL_GPIO_WritePin(STM_CS2_NEG_GPIO_Port, STM_CS2_NEG_Pin, GPIO_PIN_SET);
		break;
	case 3:
		HAL_GPIO_WritePin(STM_CS1_NEG_GPIO_Port, STM_CS1_NEG_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(STM_CS2_NEG_GPIO_Port, STM_CS2_NEG_Pin, GPIO_PIN_SET);
		break;
	}
	switch(d_i)
	{
	case 0:
		HAL_GPIO_WritePin(STM_DI_NEG_GPIO_Port, STM_DI_NEG_Pin, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(STM_DI_NEG_GPIO_Port, STM_DI_NEG_Pin, GPIO_PIN_SET);
		break;
	}


	data &= 0xFF00;
	data |= g_data;
	GLCD_Write_Pins(data);
	data = GLCD_Read_Pins();

	HAL_Delay(1);
	HAL_GPIO_WritePin(STM_E_GPIO_Port, STM_E_Pin, GPIO_PIN_SET);
	HAL_Delay(2);
	HAL_GPIO_WritePin(STM_E_GPIO_Port, STM_E_Pin, GPIO_PIN_RESET);
	HAL_Delay(4);
	HAL_GPIO_WritePin(STM_CS1_NEG_GPIO_Port, STM_CS1_NEG_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(STM_CS2_NEG_GPIO_Port, STM_CS2_NEG_Pin, GPIO_PIN_RESET);
}

/** KijelzÄąâ€� tÄ‚Â¶rlÄ‚Â©se. */
void GLCD_Clear(void)
{
	char x,y;
	for(x=0;x<8;x++)
	{
		GLCD_Write(3,0,0x40);
		GLCD_Write(3,0,(0xB8|x));
		for(y=0;y<64;y++)
		{
			GLCD_Write(3,1,0x00);
		}
	}
}

/** A kijelzÄ‚Âµ adott sor-oszlop metszetÄ‚Â©nek beÄ‚Ë‡llÄ‚Â­tÄ‚Ë‡sa az m_data Ä‚Â©rtÄ‚Â©knek megfelelÄ‚Âµen.
 * 		m_data: adott metszet rajzolata hexÄ‚Ë‡ba kÄ‚Ĺ‚dolva
 * 		cX: sor (0-7)
 * 		cY: oszlop (0-127) */
void GLCD_Write_Block(char m_data,char cX,char cY)
{
	char chip=1;
	if(cY>=64)
	{
		chip=2;
		cY-=64;
	}
	GLCD_Write(chip,0,(0x40|cY));
	GLCD_Write(chip,0,(0xB8|cX));
	GLCD_Write(chip,1,m_data);
}

/** String kiÄ‚Â­rÄ‚Ë‡sa a kijelzÄąâ€�re. */
void GLCD_WriteString(const char* string,char Y, char X)
{
	char temp = 0;
	int i=0;
	while(string[i]!='\0')
	{
		temp = string[i];
		GLCD_Write_Char(temp-32,X,Y+6*i);
		i++;
	}
}

/** Karakter Ä‚Â­rÄ‚Ë‡sa a kijelzÄąâ€�re. */
void GLCD_Write_Char(char cPlace,char cX,char cY)
{
	char i=0;
	char chip=1;
	if(cY>=64)
	{
		chip=2;
		cY-=64;
	}
	GLCD_Write(chip,0,(0x40|cY));
	GLCD_Write(chip,0,(0xB8|cX));
	for (i=0;i<5;i++)
	{
	  if (cY+i >= 64)
	  {
		  chip=2;
		  GLCD_Write(chip,0,(0x40|(cY+i-64)));
		  GLCD_Write(chip,0,(0xB8|cX));
	  }
	  GLCD_Write(chip,1,fontdata[cPlace*5+i]);
	}
}

/* ----------------- BelsÄąâ€� fÄ‚Ä˝ggvÄ‚Â©nyek ----------------- */

/** Alacsony szintÄąÂ± inicializÄ‚Ë‡lÄ‚Ë‡s. */
void GLCD_MSP_Init(void)
{
	// Init ports
	/*__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();*/

	/*GPIO_InitTypeDef portInit;*/
	/* data: PC0-PC7
	 * RW: PA10
	 * CS1: PA7
	 * CS2: PA6
	 * E: PA5
	 * DI: PA9
	 * RESET: PD2
	 * #GLCDEN: PA8
	 * Backlight PWM: PA15
	 */
	/*portInit.Mode = GPIO_MODE_OUTPUT_PP;
	portInit.Pull = GPIO_NOPULL;
	portInit.Speed = GPIO_SPEED_FAST;

	// Port C
	portInit.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	HAL_GPIO_Init(GPIOC, &portInit);
	// Port D
	portInit.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOD, &portInit);
	// Port A
	portInit.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7|GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10|GPIO_PIN_15;
	HAL_GPIO_Init(GPIOA, &portInit);*/

	// Minden alapÄ‚Ë‡llapotba
	HAL_GPIO_WritePin(STM_RW_NEG_GPIO_Port, STM_RW_NEG_Pin, GPIO_PIN_RESET);	// GLCD_RW = 0
	HAL_GPIO_WritePin(STM_E_GPIO_Port, STM_E_Pin, GPIO_PIN_SET);		// GLCD_E = 1
	HAL_GPIO_WritePin(STM_RST_GPIO_Port, STM_RST_Pin, GPIO_PIN_RESET);	// GLCD_RESET = 0
	HAL_GPIO_WritePin(STM_DI_NEG_GPIO_Port, STM_DI_NEG_Pin, GPIO_PIN_RESET);	// GLCD_RESET = 0
	HAL_GPIO_WritePin(STM_OE_NEG_GPIO_Port, STM_OE_NEG_Pin, GPIO_PIN_RESET);	// #GLCDEN = 0
	HAL_GPIO_WritePin(STM_CS1_NEG_GPIO_Port, STM_CS1_NEG_Pin, GPIO_PIN_RESET);	// /CS1 = 1
	HAL_GPIO_WritePin(STM_CS2_NEG_GPIO_Port, STM_CS2_NEG_Pin, GPIO_PIN_RESET);	// /CS2 = 1
	//HAL_GPIO_WritePin(GPIOA, LEDPWM, GPIO_PIN_SET);		// Backlight = 1 (PWM 100%)
}

uint16_t GLCD_Read_Pins()
{
	volatile uint16_t temp = 0;
	uint16_t container = 0;
	uint16_t i = 0;
	GPIO_TypeDef* port = GPIOC;

	for (i=0; i<8; i++)
	{
		if(i == 1) {port = GPIOB;}
		if(i == 2) {port = GPIOC;}
		if(i == 5) {port = GPIOB;}
		temp = HAL_GPIO_ReadPin(port, pins[i]);
		if (i != 0)
		{
			temp = temp << i;
		}
		container |= temp;
	}
	return container;
}

/* set the display pins */
void GLCD_Write_Pins(uint16_t dataToWrite)
{
	volatile uint16_t temp = 1;
	volatile uint16_t container = dataToWrite;
	uint16_t i = 0;
	GPIO_TypeDef* port = GPIOC;

	for (i=0; i<8; i++)
	{
		if (i != 0)
		{
			container = container >> i; // eltolás 0. helyiértékre
		}
		container &= temp; // maszkolás
		if(i == 1) {port = GPIOB;}
		if(i == 2) {port = GPIOC;}
		if(i == 5) {port = GPIOB;}
		HAL_GPIO_WritePin(port, pins[i], container); // GPIO write
		container = dataToWrite; // érték visszaírás
	}
}

