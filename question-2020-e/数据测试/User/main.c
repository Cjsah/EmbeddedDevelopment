/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "fft.h"
#include <math.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define  N    1024          //采样点数
#define  Fs   10240        //采样频率
#define  F    10          //分辨率
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//FFT测试数据集 输入数组
complex  FFT_256PointIn[N];
//FFT测试数据集 输出数组
float   FFT_256PointOut[N/2];										
//填入数组			
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
	  float  X,Y,P,Mag;
	 	c_abs(FFT_256PointIn,FFT_256PointOut,N/2);
    for(i=0; i<N/2; i++)
    {
			  X = FFT_256PointIn[i].real/N;    //计算实部
			  Y = FFT_256PointIn[i].imag/N;    //计算虚部
			  Mag = FFT_256PointOut[i]*2/N;    //计算幅值
			  P = atan2(Y,X)*180/PI;           //计算相位
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
  * @brief  串口打印输出
  * @param  None
  * @retval None
  */
int main(void)
{
	int i,t;
	SystemInit();//系统时钟初始化
	USART_Configuration();//串口1初始化
	printf("这是一个FFT 测试实验\r\n");  
  InitBufInArray(); 
	fft(N,FFT_256PointIn);
	 
	printf("点数   频率  幅值   实部  虚部\n"); 
	//GetPowerMag();
	dsp_g2_test();
	while(1)
	{
		//检查接收数据完成标志位是否置位	
		if(USART_GetFlagStatus(USART1, USART_IT_RXNE) != RESET)
		{
		//将接收到的数据发送出去，对USART_DR的读操作可以将USART_IT_RXNE清零。
		printf("%c",USART_ReceiveData(USART1));
		}

	}
}

/*********************************END OF FILE**********************************/
