#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
#include "sys.h" 
#include "delay.h"
#include "usart.h"
#include "adc.h"
#include "timer.h"
#include "lcd.h" 



#define KEY1 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_6)
#define KEY2 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5)
#define KEY3 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_4)
#define KEY4 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_3)
#define KEY5 GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2)
#define KEY6 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)
#define KEY7 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)
#define KEY8 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)
extern __IO uint16_t ADC_ConvertedValue[1024];    // ADC1转换的电压值通过MDA方式传到SRAM	

void KEY_Init(void);
void KEY_Scan();
#endif