/**
*****************************************************************************
** Kommunikációs mérés - glcd.h
** A grafikus LCD header file-ja
*****************************************************************************
*/
#pragma once
#ifndef _DISPLAY_H__
#define _DISPLAY_H__



#define E 			GPIO_PIN_5
#define CS2 		GPIO_PIN_6
#define CS1 		GPIO_PIN_7
#define nGLCDEN 	GPIO_PIN_8
#define DI 			GPIO_PIN_9
#define RW 			GPIO_PIN_10
#define LEDPWM 		GPIO_PIN_15
#define RESET 		GPIO_PIN_2

void GLCD_Init(void);
void GLCDEN(int newState);
void GLCD_Write(char cs_s,char d_i,char g_data);
void GLCD_Clear(void);
void GLCD_Delay(char value);
void GLCD_Write_Block(char m_data,char cX,char cY);
void GLCD_Write_Char(char cPlace,char cX,char cY);
void GLCD_WriteString(const char* string,char X, char Y);

#endif

