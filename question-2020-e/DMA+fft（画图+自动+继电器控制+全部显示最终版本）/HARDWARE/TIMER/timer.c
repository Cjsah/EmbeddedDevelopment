#include "timer.h"
#include<stm32f10x.h>
#include "led.h"
#include "delay.h"
#include "key.h"
extern char auto_flag;
int tt;
/*************************************************
函数: 
功能: 定时器配置
参数: 无
返回: 无
**************************************************/
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM2_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    
	TIM_DeInit(TIM2);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	TIM2_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM2_InitStructure.TIM_RepetitionCounter = 0;
	TIM2_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//=====================采样率调节↓=====================================
	TIM2_InitStructure.TIM_Period =585;  //  1170  389      585   780   239   //(799,43)频率2048     //59,29(40K)   //44,19(80K)
	TIM2_InitStructure.TIM_Prescaler = 11;//     5  17          11   8      29
//==============================================================
	TIM_TimeBaseInit(TIM2, &TIM2_InitStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //
    TIM_OCInitStructure.TIM_Pulse = 50;     //占空比（大于0就OK）
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_Low;   //TIM输出比较级性低
    TIM_OC2Init(TIM2,&TIM_OCInitStructure);
        
    TIM_InternalClockConfig(TIM2);    //设置TIMx内部时钟
    TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);  //使能或失能TIMx在CCR2上的预装载寄存器
    TIM_UpdateDisableConfig(TIM2,DISABLE);    //使能或失能TIMx更新事件
    
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  //使能或失能指定的TIM中断
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_NVIC_Configuration(void)
{
	
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
}

void TIME2_Init()
{
    TIM2_NVIC_Configuration();
    TIM2_Configuration();
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)==SET)
	{
//=================定时器2中断处理函数================================
        
//====================测试用，可以用示波器测量定时频率↓==================         
//          if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3))
//              led4(ON);
//          else
//              led4(OFF);
//===============================================================                
		TIM_ClearFlag(TIM2, TIM_IT_Update); //清除溢出中断标志
	}
}


void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
		KEY_Scan();
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
		}
}










