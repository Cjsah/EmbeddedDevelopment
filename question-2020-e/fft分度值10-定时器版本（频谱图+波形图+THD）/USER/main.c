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
#define  N    1024          //��������
#define  Fs   10240        //����Ƶ��
#define  F    10          //�ֱ���
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern float data[1024];
extern int end;
/*��Ļ���Ƶ��*/
int x11=20;
int y11=120,y12=220; //��
int x21=20,x22=280;//��
int y21=120;
/*��Ļ��Ʋ���*/
int X1=20,X2=160;
int Y1=1,Y2=101;
int XX1=20,YY1=1;
//FFT�������ݼ� ��������
complex  FFT_256PointIn[N];
//FFT�������ݼ� �������
float   FFT_256PointOut[N/2];
float   Mag[N/2];

u8 temp1[20];
//��������												����				
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
			 //����
//		   printf("%d      ",i);
//			 printf("%lf      \n",FFT_256PointIn[i].real);
			
    }	
}
/******************************************************************
��������:GetPowerMag()
��������:�������г����ֵ
����˵��:
������ע:�Ƚ�FFT_256PointIn�ֽ��ʵ��(X)���鲿(Y)��
         Ȼ������ֵ:(sqrt(X*X+Y*Y)*2/N
         Ȼ�������λ:atan2(Y/X)
��������:�����������
*******************************************************************/
void GetPowerMag()
{
    unsigned short i;
	  float  X,Y,P;
	 	c_abs(FFT_256PointIn,FFT_256PointOut,N/2);
    for(i=0; i<N/2; i++)
    {
			  X = FFT_256PointIn[i].real/N;    //����ʵ��
			  Y = FFT_256PointIn[i].imag/N;    //�����鲿
				if(i==0)
					Mag[i] = FFT_256PointOut[i]/N;    //�����ֵ
				else
					Mag[i] = FFT_256PointOut[i]*2/N;
			  P = atan2(Y,X)*180/PI;           //������λ
			
				printf("%d      ",i);
				printf("%d      ",F*i); 
				printf("%f      \r\n",Mag[i]);		
    }
}
/**
*����ŷ��г��ʧ�溯��
**/
void GetTHD()
{
	unsigned short i=20;//�����ַ�ƽ��
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
	//����
//	printf("%lf      ",Uo1);
//	printf("%lf      ",Uo2);
//	printf("%lf      ",Uo3);
//	printf("%lf      ",Uo4);
//	printf("%lf      ",Uo5);
}
/*************

					������ͼ
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
	delay_init();	    	 //��ʱ������ʼ��
	SystemInit();//ϵͳʱ�ӳ�ʼ��
	USART_Configuration();//����1��ʼ��
	Adc_Init();		  		//ADC��ʼ��
	TIM3_Int_Init(780*3+2,2);//1952*2
	//TIM3_Int_Init(780,8);//1952*2     
	//TIM3_Int_Init(98,71);//1952*2
	LCD_Init();	
	//printf("����һ��FFT ����ʵ��\r\n"); 
	 POINT_COLOR=GRAY;//��������Ϊ��ɫ 
	 LCD_Display_Dir(1);	
	 LCD_ShowNum(10,220,0,1,16);
	
	//��ͼƵ������
	 for(i=0;i<8;i++){
		LCD_DrawLine(XX1, Y1, XX1, Y2);
		XX1=XX1+20;
	 }
	 for(i=0;i<6;i++){
		LCD_DrawLine(X1, YY1, X2, YY1);
		YY1=YY1+20; 
	 }
	 //���Ƶ�ײ���
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
	//	printf("����   Ƶ��  ��ֵ   ʵ��  �鲿\n"); 
//	GetPowerMag();
	while(1)
	{
		
		if(end==1) //����һ��
		{			
			InitBufInArray(); 
			fft(N,FFT_256PointIn);
			POINT_COLOR=RED;//��������Ϊ��ɫ
			
			//printf("����   Ƶ��  ��ֵ   ʵ��  �鲿\n"); 
			
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

