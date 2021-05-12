#include "key.h"

char auto_flag;
char auto_flag1,classify_flag;
u8 flag_zidongkz;//自动控制状态
double thd_1[5]={0,0,0,0,0};
double thd_2[5]={0,0,0,0,0};
double thd_3[5]={0,0,0,0,0};
double thd_4[5]={0,0,0,0,0};
double thd_5[5]={0,0,0,0,0};
u8 cc=1;
u8 zt=0;

/*-------------------------------------------------------------------------------
程序名称：KEY_Init
程序描述：按键引脚初始化，打开端口时钟，配置端口引脚，端口工作频率，端口输入模式 
输入参数：无
返回参数：无
备    注：无
---------------------------------------------------------------------------------*/
void KEY_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 |GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}
void KEY_Scan()
{	 
	static u16 key1_sum=0,key2_sum=0,key3_sum=0,key4_sum=0,key5_sum=0\
		,key6_sum=0,key7_sum=0,key8_sum=0;
	u8 z;
	if(KEY1==0)
	{
		key1_sum++;
		if(key1_sum==1)
		{
			/*功能补充*/
			wushizhen
			delay_ms(800);
			classify_flag=0;
			//printf("		KEY1\r\n");
		}
	}
	else
		key1_sum=0;
	if(KEY2==0)
	{
		key2_sum++;
		if(key2_sum==1)
		{
			/*功能补充*/
			dingbushizhen
			delay_ms(800);
			classify_flag=1;
			
		}
	}
	else
		key2_sum=0;
	
	if(KEY3==0)
	{
		key3_sum++;
		if(key3_sum==1)
		{
			dibushizhen
			delay_ms(800);
			classify_flag=2;
			
			/*功能补充*/	
		}
	}
	else
		key3_sum=0;
	if(KEY4==0)
	{
		key4_sum++;
		if(key4_sum==1)
		{
			/*功能补充*/
			shuangxiangshizhen
			delay_ms(800);
			classify_flag=3;
		}
	}
	else
		key4_sum=0;
	
	if(KEY6==0)
	{
		key6_sum++;
		if(key6_sum==1)
		{
			/*功能补充*/
			jiaoyueshizhen
			delay_ms(800);
			classify_flag=4;
		}
	}
	else
		key6_sum=0;
	
	
	
	if(KEY5==0)
	{
		key5_sum++;
		if(key5_sum==1)
		{
			/*功能补充*/
			auto_flag1=0;
			cc=1;
			zt=0;
			classify_flag=0;
			flag_zidongkz=0;
			for(z=0; z < 5; z++)
			{
				thd_1[z]=0;
				thd_2[z]=0;
				thd_3[z]=0;
				thd_4[z]=0;
				thd_5[z]=0;
			}
		}
	}
	else
		key5_sum=0;

	
	if(KEY7==0)
	{
		key7_sum++;
		if(key7_sum==1)
		{
			/*功能补充*/
			auto_flag=0;
				
		}
	}
	else
		key7_sum=0;
	//暂停
	if(KEY8==0)
	{
		key8_sum++;
		if(key8_sum==1)
		{
			/*功能补充*/
			if(cc==0)
			{
				auto_flag=1;
				delay_ms(800);
			  DMA_Cmd(DMA1_Channel1,ENABLE);
        TIM_Cmd(TIM2,ENABLE);    //开启定时器2
			  delay_ms(1000);
			}
			else{
				delay_ms(1000);
				cc=0;
				auto_flag1=1;
				
				DMA_Cmd(DMA1_Channel1,DISABLE);
        TIM_Cmd(TIM2,DISABLE);
			}
		}
	}
	else
		key8_sum=0;
	
	
	
}