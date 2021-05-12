#include "stm32f10x.h"
#include "led.h"
#include "usart.h"
#include "fft.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h" 
#include "adc.h"
#include "img1.h"
#include <usar.h>
#include "timer.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "img.h"
#include "stm32_dsp.h"
#include "table_fft.h"
/****************************变量区***************************************/
#define PI2 6.28318530717959
#define Fs 10240                   //采样频率 Hz
#define NPT 1024                    //采样点数

u16 ADC_input[NPT];	//DMA读取的电压值
float thd;
long input[NPT];
long output[NPT];
double Mag[NPT];
float Mag_max;
u16 ADC_DataNum = 0;
s16 Fn_Num;
u16 ADC_flag = 0;
extern __IO uint16_t ADC_ConvertedValue[1024];    // ADC1转换的电压值通过MDA方式传到SRAM	
u8 temp1[20];
extern char auto_flag;
extern char auto_flag1;
extern char classify_flag;
float vpp=0;
/**********************函数区*******************************************/
/*************

					gpio初始化
*
*/
void gpio_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//输出速率	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|
																 GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8; //选择引脚 
	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOF.0
}
//
void gpio()
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);
	GPIO_ResetBits(GPIOF,GPIO_Pin_2);
	GPIO_ResetBits(GPIOF,GPIO_Pin_3);
	GPIO_ResetBits(GPIOF,GPIO_Pin_4);
	GPIO_ResetBits(GPIOF,GPIO_Pin_5);
	GPIO_ResetBits(GPIOF,GPIO_Pin_6);
	GPIO_ResetBits(GPIOF,GPIO_Pin_7);
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);
}	
void get_vpp()
{
	u16 i;
	u16 max=0,min=5000;
	max = 0; 
  for(i=0; i < NPT; i++)
  {
		if(max < ADC_input[i])
		{
			max = ADC_input[i];
		}
  }
	for(i=0; i < NPT; i++)
  {
		if(min > ADC_input[i])
		{
			min = ADC_input[i];
		}
  }
	vpp = (max-min)*3.3/4096;
	
}

/***************计算幅值*****************/
void PowerMag(u16 FFT_NPT)  
{

//	s16 lx, ly;
	signed short lx, ly;
	u32 i;
	float mag, x, y;
	
	for (i = 0; i < FFT_NPT/2; i++ )
	{

		lx = (output[i]<<16)>>16;
		ly = (output[i]>>16);
		
		 x = FFT_NPT*((float)lx)/32768;
		 y = FFT_NPT*((float)ly)/32768;
		mag = sqrt(x*x+y*y)/FFT_NPT;		
		Mag[i] = (unsigned long)(mag*65536)*3.30/4096;
//		printf("%d\r\n",i )	;	
//		printf("%\r\n",Mag[i] )	;	
	}
	Mag[0] = Mag[0]/2;	
}

/***********取最大值************/
void Compare_Max()
{
	u16 i;
	Mag_max = Mag[1]; 
    for(i=1; i < NPT/2; i++)
    {
		if(Mag_max < Mag[i])
		{
			Fn_Num = i;
			Mag_max = Mag[i];
		}
    }

}

/********FFT变换**********/
void DSP_FFT1024()
{
	u16 i;	
    
	for (i = 0; i <NPT ; i++)
	{
       input[i] = ADC_input[i]<<16;
    }
	
	cr4_fft_1024_stm32(output,input,NPT);   //FFT变换
	PowerMag(NPT);

}

/***********计算欧总谐振失真函数**********/
void GetTHD()
{
	unsigned short i=17;//控制字符平移
	float  Uo1,Uo2,Uo3,Uo4,Uo5;
	double THD,thd_fz=0,thd_fm=0,xs=0.76;
	Uo1=Mag[100]*xs;
	Uo2=Mag[200]*xs;
	Uo3=Mag[300]*xs;
	Uo4=Mag[400]*xs;
	Uo5=Mag[500]*xs;
	thd_fm=Uo1;
	thd_fz=Uo2*Uo2 + Uo3*Uo3 + Uo4*Uo4 + Uo5*Uo5;
	thd_fz=sqrt(thd_fz);
	THD=thd_fz/thd_fm*100;
	sprintf((unsigned char *)temp1,"Uo1:%5lfV",Uo1);
	LCD_ShowString(180,0+i,200,16,16,temp1);
	sprintf((unsigned char *)temp1,"Uo2:%5lfV",Uo2);
	LCD_ShowString(180,15+i,200,16,16,temp1);
	sprintf((unsigned char *)temp1,"Uo3:%5lfV",Uo3);
	LCD_ShowString(180,30+i,200,16,16,temp1);
	sprintf((unsigned char *)temp1,"Uo4:%5lfV",Uo4);
	LCD_ShowString(180,45+i,200,16,16,temp1);
	sprintf((unsigned char *)temp1,"Uo5:%5lfV",Uo5);
	LCD_ShowString(180,60+i,200,16,16,temp1);
	sprintf((unsigned char *)temp1,"THD:%5lf%%",THD);
	LCD_ShowString(180,75+i,200,16,16,temp1);
	sprintf((unsigned char *)temp1,"Vpp:%5lfV",vpp*xs*0.97);
	LCD_ShowString(180,90+i,200,16,16,temp1);
	//测试
//	printf("%lf      ",Uo1);
//	printf("%lf      ",Uo2);
//	printf("%lf      ",Uo3);
//	printf("%lf      ",Uo4);
//	printf("%lf      ",Uo5);
}

/*************

					画波形图
*
*/
void Getsignchart()
{
	int i,x0=25;
	double con=7.55,x_con=4;
	for(i=0;i<19;i++){
				if(i==0){
						LCD_DrawPoint(x0+i*x_con+1,99-ADC_input[i]*3.30/1024*con);
				}
				else
				{
					LCD_DrawPoint(x0+i*x_con+1,99-ADC_input[i]*3.30/1024*con);
					LCD_DrawLine(x0+(i-1)*x_con+1, 99-ADC_input[i-1]*3.30/1024*con, x0+i*x_con+1, 99-ADC_input[i]*3.30/1024*con);
				}
//				printf("%d      ",i);
//				printf("%lf      \n",data[i]*3.30/1024*con);
	 }
}


/*************

					画频谱图
*
*/
void Getfreqchart()
{
	int i;
	float xs=27.4;
	 for(i=0;i<256;i++){
				if(i==0){
					if(Mag[0]*xs==0)
						LCD_DrawPoint(20+i+1,220);
					else
						LCD_DrawPoint(20+i+1,Mag[0]*xs);
				} 
				else
				{
					LCD_DrawPoint(20+i+1,220-Mag[i*2]*xs);
					LCD_DrawLine(20+i-1+1, 220-Mag[(i-1)*2]*xs, 20+i+1, 220-Mag[i*2]*xs);
				}
			}
}


//画图设计函数
void LCD_Drawdesign()
{
	 u16 i ;
	/*屏幕设计频谱*/
	int x11=20;
	int y11=140,y12=220; //竖
	int x21=20,x22=280;//横
	int y21=140;
	/*屏幕设计波形*/
	int X1=25,X2=105;
	int Y1=1,Y2=101;
	int XX1=25,YY1=1;
	//printf("这是一个FFT 测试实验\r\n"); 
	 POINT_COLOR=RED;//设置字体为红色 WHITE
	//设置频谱参数
	 LCD_ShowString(0,0,32,16,16,"2.5");
	 LCD_ShowString(0,15,32,16,16,"2.0");
	 LCD_ShowString(0,30,32,16,16,"1.5");
	 LCD_ShowString(0,50,32,16,16,"1.0");
	 LCD_ShowString(0,70,32,16,16,"0.5");
	 LCD_ShowString(10,90,32,16,16,"0");
	 LCD_ShowString(100,100,32,16,16,"t");
	 //设计频谱参数
	 LCD_ShowString(285,200,32,16,16,"fre/");
	 LCD_ShowString(290,215,24,16,16,"Hz");
	 LCD_ShowString(0,120,32,16,16,"Am");
	 LCD_ShowString(1,135,32,16,16,"/V");
	 LCD_ShowString(65,221,24,16,16,"1k");
	 LCD_ShowString(110,221,24,16,16,"2k");
	 LCD_ShowString(158,221,24,16,16,"3k");
	 LCD_ShowString(208,221,24,16,16,"4k");
	 LCD_ShowString(265,221,24,16,16,"5k");
	 LCD_ShowNum(10,220,0,1,16);
	 POINT_COLOR=LGRAY;//设置字体为红色 WHITE
	 LCD_Display_Dir(1);	
	
	
	//画图波形区域
	 for(i=0;i<5;i++){
		LCD_DrawLine(XX1, Y1, XX1, Y2);
		XX1=XX1+20;
	 }
	 for(i=0;i<6;i++){
		LCD_DrawLine(X1, YY1, X2, YY1);
		YY1=YY1+20; 
	 }
	 //画图频谱区域
	 for(i=0;i<14;i++){
		LCD_DrawLine(x11, y11, x11, y12);
		x11=x11+20;
	 }
	 for(i=0;i<6;i++){
		LCD_DrawLine(x21, y21, x22, y21);
		y21=y21+20; 
	 }
	 
	
}

/***************************************************
*
*
*             显示种类汉字
*
*
*****************************************************/
void xianshi_classify()
{
	if(classify_flag==0)
	{
		POINT_COLOR=DARKBLUE;
		Show_Graph(180,0,(u8*)wu,16,0);
		Show_Graph(195,0,(u8*)ming,16,0);
		Show_Graph(210,0,(u8*)xian,16,0);
		Show_Graph(225,0,(u8*)shi,16,0);
		Show_Graph(240,0,(u8*)zhen,16,0);
		POINT_COLOR=RED;
	}
	if(classify_flag==1)
	{
		POINT_COLOR=DARKBLUE;
		Show_Graph(180,0,(u8*)ding,16,0);
		Show_Graph(195,0,(u8*)bu,16,0);
		Show_Graph(210,0,(u8*)shi,16,0);
		Show_Graph(225,0,(u8*)zhen,16,0);
		Show_Graph(240,0,(u8*)k,16,0);
		POINT_COLOR=RED;
	}
	if(classify_flag==2)
	{
		POINT_COLOR=DARKBLUE;
		Show_Graph(180,0,(u8*)di,16,0);
		Show_Graph(195,0,(u8*)bu,16,0);
		Show_Graph(210,0,(u8*)shi,16,0);
		Show_Graph(225,0,(u8*)zhen,16,0);
		Show_Graph(240,0,(u8*)k,16,0);
		POINT_COLOR=RED;
	}
	if(classify_flag==3)
	{
		POINT_COLOR=DARKBLUE;
		Show_Graph(180,0,(u8*)shuang,16,0);
		Show_Graph(195,0,(u8*)xiang,16,0);
		Show_Graph(210,0,(u8*)shi,16,0);
		Show_Graph(225,0,(u8*)zhen,16,0);
		Show_Graph(240,0,(u8*)k,16,0);
		POINT_COLOR=RED;
	}
	if(classify_flag==4)
	{
		POINT_COLOR=DARKBLUE;
		Show_Graph(180,0,(u8*)jiao,16,0);
		Show_Graph(195,0,(u8*)yue,16,0);
		Show_Graph(210,0,(u8*)shi,16,0);
		Show_Graph(225,0,(u8*)zhen,16,0);
		Show_Graph(240,0,(u8*)k,16,0);
		POINT_COLOR=RED;
	}
}
/**************************************************************
*
*
*
*
*                          全自动
*
*
*
*
***************************************************************/
//void quanzidong()
//{
//	u16 i;//控制状态切换
//	for()
//}
/**************************************************************
*
*
*
*
*                          主函数
*
*
*
*
***************************************************************/
int main(void)
{  
    u16 i ;
	SystemInit();//系统时钟初始化
	//继电器控制
	gpio_init();
	gpio();
	delay_init();    
  TIME2_Init();
	ADC1_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	USART_Configuration();//串口1初始化
	LCD_Init();
	KEY_Init();

	TIM3_Int_Init(500,7199);//10Khz的计数频率，计数到5000为500ms 
	POINT_COLOR=RED;//设置字体为红色
	Show_Graph(0,40,(u8*)tp1,240,1);
	delay_ms(1000);
	POINT_COLOR=BLACK;//
	LCD_Clear(WHITE);
	Show_Graph(0,40,(u8*)tp,240,1);
	delay_ms(1000);
	LCD_Clear(WHITE);
	LCD_Drawdesign();
	//	printf("点数   频率  幅值   实部  虚部\n"); 
  //	GetPowerMag();
    while (1)
	{  
		if(auto_flag==0){
			delay_ms(800);
			DMA_Cmd(DMA1_Channel1,ENABLE);
			TIM_Cmd(TIM2,ENABLE);    //开启定时器2
		}
		if(ADC_flag == 1)
		{
			LCD_Clear(WHITE);
			xianshi_classify();
			LCD_Drawdesign();
			TIM_Cmd(TIM2,DISABLE);
			DMA_Cmd(DMA1_Channel1,DISABLE);
			ADC_flag = 0;
			POINT_COLOR=RED;//设置字体为红色
			DSP_FFT1024();
			get_vpp();
			GetTHD();
			Getsignchart();
			Getfreqchart();
			DMA_Cmd(DMA1_Channel1,DISABLE);
			TIM_Cmd(TIM2,DISABLE);    //开启定时器2	
		}
		
	}
}

/**************************************************
*
*
*                  DMA中断服务函数
*
*
*
***************************************************/
void DMA1_Channel1_IRQHandler()  
{  
    int  i;
    if(DMA_GetITStatus(DMA_IT_TC))                      //判断DMA传输完成中断  
    {  	
            for(i=0;i<NPT;i++)
            {
                ADC_input[i] = ADC_ConvertedValue[i];
//							printf("%d\r\n",ADC_input[i]);
//							printf("%f\r\n",(float)ADC_input[i]*3.3/4096);
            }
            ADC_flag = 1;
    }
    DMA_ClearITPendingBit(DMA_IT_TC);                   //清除DMA中断标志位  
}  





