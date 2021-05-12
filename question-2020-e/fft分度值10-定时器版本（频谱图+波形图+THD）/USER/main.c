/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "fft.h"
#include <math.h>
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"
#include "timer.h"
#include <stdio.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define  N    1024          //采样点数
#define  Fs   10240        //采样频率
#define  F    10          //分辨率
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern float data[1024];
extern int end;
/*屏幕设计频谱*/
int x11=20;
int y11=120,y12=220; //竖
int x21=20,x22=280;//横
int y21=120;
/*屏幕设计波形*/
int X1=20,X2=160;
int Y1=1,Y2=101;
int XX1=20,YY1=1;
//FFT测试数据集 输入数组
complex  FFT_256PointIn[N];
//FFT测试数据集 输出数组
float   FFT_256PointOut[N/2];
float   Mag[N/2];

u8 temp1[20];
//填入数组												测试				
//void InitBufInArray()
//{
// unsigned short i;
// for(i=0; i<N; i++)    
//	{
//       FFT_256PointIn[i].real  = 1500 * sin(2*PI * i * 2000.0 / Fs) 
//		                             +10 * sin(2*PI * i * 1000.0 / Fs) 
//		                             +4000 * sin(2*PI * i * 4000.0 / Fs);
//		   FFT_256PointIn[i].imag = 0;
//    }	
//}

void InitBufInArray()
{
 unsigned short i;
 for(i=0; i<N; i++)    
	{
       FFT_256PointIn[i].real  = data[i]*3.30/4096;//
		   FFT_256PointIn[i].imag = 0;
			 //测试
//		   printf("%d      ",i);
//			 printf("%lf      \n",FFT_256PointIn[i].real);
			
    }	
}
/******************************************************************
函数名称:GetPowerMag()
函数功能:计算各次谐波幅值
参数说明:
备　　注:先将FFT_256PointIn分解成实部(X)和虚部(Y)，
         然后计算幅值:(sqrt(X*X+Y*Y)*2/N
         然后计算相位:atan2(Y/X)
作　　者:土耳其冰激凌
*******************************************************************/
void GetPowerMag()
{
    unsigned short i;
	  float  X,Y,P;
	 	c_abs(FFT_256PointIn,FFT_256PointOut,N/2);
    for(i=0; i<N/2; i++)
    {
			  X = FFT_256PointIn[i].real/N;    //计算实部
			  Y = FFT_256PointIn[i].imag/N;    //计算虚部
				if(i==0)
					Mag[i] = FFT_256PointOut[i]/N;    //计算幅值
				else
					Mag[i] = FFT_256PointOut[i]*2/N;
			  P = atan2(Y,X)*180/PI;           //计算相位
			
				printf("%d      ",i);
				printf("%d      ",F*i); 
				printf("%f      \r\n",Mag[i]);		
    }
}
/**
*计算欧总谐振失真函数
**/
void GetTHD()
{
	unsigned short i=20;//控制字符平移
	float  Uo1,Uo2,Uo3,Uo4,Uo5;
	double THD,thd_fz=0,thd_fm=0;
	Uo1=Mag[100];
	Uo2=Mag[200];
	Uo3=Mag[300];
	Uo4=Mag[400];
	Uo5=Mag[500];
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
	int i;
	int con=8,x_con=4;
	for(i=0;i<35;i++){
				if(i==0){
						LCD_DrawPoint(20+i*x_con+1,101-data[i+4]*3.30/1024*con);
				}
				else
				{
					LCD_DrawPoint(20+i*x_con+1,101-data[i+4]*3.30/1024*con);
					LCD_DrawLine(20+(i-1)*x_con+1, 101-data[i-1+4]*3.30/1024*con, 20+i*x_con+1, 101-data[i+4]*3.30/1024*con);
				}
//				printf("%d      ",i);
//				printf("%lf      \n",data[i]*3.30/1024*con);
			}
	
//	for(i=0;i<40;i++){
//				if(i==0){
//					if(data[0]*50==0)
//						LCD_DrawPoint(20+i*2+1,101);
//					else
//						LCD_DrawPoint(20+i*2+1,101);
//				} 
//				else
//				{
//					LCD_DrawPoint(20+i*2+1,101-10);
//					LCD_DrawLine(20+i*2-1+1, 101-10, 20+i+1, 101-10);
//				}
//			}
}
int main(void)
{
	int i,t;
	delay_init();	    	 //延时函数初始化
	SystemInit();//系统时钟初始化
	USART_Configuration();//串口1初始化
	Adc_Init();		  		//ADC初始化
	TIM3_Int_Init(780*3+2,2);//1952*2
	//TIM3_Int_Init(780,8);//1952*2     
	//TIM3_Int_Init(98,71);//1952*2
	LCD_Init();	
	//printf("这是一个FFT 测试实验\r\n"); 
	 POINT_COLOR=GRAY;//设置字体为红色 
	 LCD_Display_Dir(1);	
	 LCD_ShowNum(10,220,0,1,16);
	
	//画图频谱区域
	 for(i=0;i<8;i++){
		LCD_DrawLine(XX1, Y1, XX1, Y2);
		XX1=XX1+20;
	 }
	 for(i=0;i<6;i++){
		LCD_DrawLine(X1, YY1, X2, YY1);
		YY1=YY1+20; 
	 }
	 //设计频谱参数
	 LCD_ShowString(285,200,32,16,16,"fre/");
	 LCD_ShowString(290,215,24,16,16,"Hz");
	 LCD_ShowString(0,120,32,16,16,"|A");
	 LCD_ShowString(1,135,32,16,16,"/V");
	 LCD_ShowString(65,221,24,16,16,"1k");
	 LCD_ShowString(110,221,24,16,16,"2k");
	 LCD_ShowString(158,221,24,16,16,"3k");
	 LCD_ShowString(208,221,24,16,16,"4k");
	 LCD_ShowString(265,221,24,16,16,"5k");
	 for(i=0;i<14;i++){
		LCD_DrawLine(x11, y11, x11, y12);
		x11=x11+20;
	 }
	 for(i=0;i<6;i++){
		LCD_DrawLine(x21, y21, x22, y21);
		y21=y21+20; 
	 }
	//	printf("点数   频率  幅值   实部  虚部\n"); 
//	GetPowerMag();
	while(1)
	{
		
		if(end==1) //接受一次
		{			
			InitBufInArray(); 
			fft(N,FFT_256PointIn);
			POINT_COLOR=RED;//设置字体为红色
			
			//printf("点数   频率  幅值   实部  虚部\n"); 
			
			GetPowerMag();
			end=0;
			GetTHD();
			Getsignchart();
			for(i=0;i<256;i++){
				if(i==0){
					if(Mag[0]*50==0)
					LCD_DrawPoint(20+i+1,220);
				} 
				else
				{
					LCD_DrawPoint(20+i+1,220-Mag[i*2]*25);
					LCD_DrawLine(20+i-1+1, 220-Mag[(i-1)*2]*25, 20+i+1, 220-Mag[i*2]*25);
				}
			}
		}
	}
}

