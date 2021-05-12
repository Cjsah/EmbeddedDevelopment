/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "fft.h"
#include <math.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define  N    1024          //��������
#define  Fs   10240        //����Ƶ��
#define  F    10          //�ֱ���
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//FFT�������ݼ� ��������
complex  FFT_256PointIn[N];
//FFT�������ݼ� �������
float   FFT_256PointOut[N/2];										
//��������			
double result[N];
struct compx s[N];
void InitBufInArray()
{
 unsigned short i;
 for(i=0; i<N; i++)    
	{
       FFT_256PointIn[i].real  = 1 * sin(2*PI * i * 1000.0 / Fs) 
		                             +1;
		   FFT_256PointIn[i].imag = 0;
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
	  float  X,Y,P,Mag;
	 	c_abs(FFT_256PointIn,FFT_256PointOut,N/2);
    for(i=0; i<N/2; i++)
    {
			  X = FFT_256PointIn[i].real/N;    //����ʵ��
			  Y = FFT_256PointIn[i].imag/N;    //�����鲿
			  Mag = FFT_256PointOut[i]*2/N;    //�����ֵ
			  P = atan2(Y,X)*180/PI;           //������λ
				printf("%d      ",i);
				printf("%d      ",F*i); 
				printf("%f      \r\n",Mag);
//			  printf("%f      ",P);
//				printf("%f      ",X);
//				printf("%f      \r\n",Y);			
    }
}
void dsp_g2_test()
{
  u16 i=0;
  for(i=0;i<N;i++)
  {
    s[i].real = 32000 * sin(PI*2*i*(50.0f/Fs));
    s[i].real+= 16000 * sin(PI*2*i*(550.0f/Fs));
    s[i].real+= 9000 * sin(PI*2*i*(1150.0f/Fs));
		s[i].real+= 6000 * sin(PI*2*i*(2100.0f/Fs));
		s[i].real+= 4000 * sin(PI*2*i*(5000.0f/Fs));
    s[i].imag=0;
  }
  FFT(s,N);
  for(i=0;i<N/2;i++)
  {
		if(i==0)
			result[i] = sqrt(s[i].real * s[i].real + s[i].imag * s[i].imag)/N;
		else
			result[i] = sqrt(s[i].real * s[i].real + s[i].imag * s[i].imag)*2/N;
		printf("%d      ",i);
		printf("%d      ",10*i);
		printf("%f      \r\n",result[i]);
    //if(result[i] > 10)
    //printf("%4d,%4d,%ld\n",i,(u16)((double)i*Fs/NPT),(u32)result[i]);
  }
}
/**
  * @brief  ���ڴ�ӡ���
  * @param  None
  * @retval None
  */
int main(void)
{
	int i,t;
	SystemInit();//ϵͳʱ�ӳ�ʼ��
	USART_Configuration();//����1��ʼ��
	printf("����һ��FFT ����ʵ��\r\n");  
  InitBufInArray(); 
	fft(N,FFT_256PointIn);
	 
	printf("����   Ƶ��  ��ֵ   ʵ��  �鲿\n"); 
	//GetPowerMag();
	dsp_g2_test();
	while(1)
	{
		//������������ɱ�־λ�Ƿ���λ	
		if(USART_GetFlagStatus(USART1, USART_IT_RXNE) != RESET)
		{
		//�����յ������ݷ��ͳ�ȥ����USART_DR�Ķ��������Խ�USART_IT_RXNE���㡣
		printf("%c",USART_ReceiveData(USART1));
		}

	}
}

/*********************************END OF FILE**********************************/
