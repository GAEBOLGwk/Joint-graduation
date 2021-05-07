/*remote.c*/
#include "remote.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h"
/**
发射：PA7-TIM3-CH2
接收：PC8-TIM8-CH3

TIM8：
下面代码中捕获中断和定时中断分开，不在同一中断函数，所以需要配置两次中断优先级（应该可以合在一起配置）
*/

/**
 *  @name			void Remote_Init(void) 
 *	@description	红外遥控初始化 设置IO以及定时器8的输入捕获
 *                  Infrared remote initializer sets IO and timer 8 input capture
 *  @notice			
 */
void Remote_Init(void)    			  
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;  

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE); //使能PORTB时钟 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);                      //TIM8 时钟使能 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                                //PC8 输入 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                    //浮空输入 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_8);                                          //初始化GPIOC.8

	TIM_TimeBaseStructure.TIM_Period = 9999;                                 //设定计数器自动重装值 最大10ms溢出  
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1);                             //预分频器,1M的计数频率,1us加1.	   	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                  //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;              //TIM向上计数模式
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);                          //根据指定的参数初始化TIMx

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;                         // 选择输入端 IC4映射到TI4上
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;              //上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;                    //配置输入分频,不分频 
	TIM_ICInitStructure.TIM_ICFilter = 0x03;                                 //IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
	TIM_ICInit(TIM8, &TIM_ICInitStructure);                                  //初始化定时器输入捕获通道
	TIM_Cmd(TIM8,ENABLE );                                                   //使能定时器4

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;                       //TIM8TIM8捕获比较中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;                //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;                       //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                          //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);                                          //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器	
	TIM_ITConfig( TIM8,TIM_IT_CC3,ENABLE);                                   //允许更新中断 ,允许CC4IE捕获中断		
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;                       //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;                //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;                       //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                          //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);                                          //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器	
	TIM_ITConfig( TIM8,TIM_IT_Update,ENABLE);                                //允许更新中断 ,允许CC4IE捕获中断								 
}
/**
 *  @name			void TIM3_PWM_Init(u16 arr,u16 psc)
 *	@description	初始化定时器3的设置，将定时器3用于PWM调制，PWM输出口为 PA.7
 *	@param			arr --	u16,定时器重装值
					psc --	u16,定时器分频值							
 *	@return		
 *  @notice			PWM频率 = 72M/((arr+1)*(psc+1)),这里用作红外发射的载波，需要生成38kHz的方波，故取arr = 1895,psc = 0。
 */
void TIM3_PWM_Init(u16 arr,u16 psc)
{
	/* 初始化结构体定义 */
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;                        //定时器基本设置
	TIM_OCInitTypeDef 	TIM_OCInitStructure;                                  //定时器比较输出配置

	/* 使能相应端口的时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);                      //使能定时器2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); //使能GPIO外设时钟
	
	/* GPIOA.7初始化 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                                 // TIM3 CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                           // PA.7 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_7);

	/* TIM3 初始化*/
	TIM_TimeBaseInitStructure.TIM_Period = arr;                               //下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                            //作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;                          //时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;           //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	/* 定时器TIM3 Ch2 PWM模式初始化 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                         //选择定时器模式:TIM PWM1   TIM_OCMode_PWM1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;             //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = (arr+1)/3;                                //占空比1:3 (arr+1)/10  3
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                 //输出极性:TIM输出比较极性高   TIM_OCPolarity_High
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);

	/* 使能TIM3在CCR1上的预装载寄存器 */
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 
}


u8 	RmtSta=0;                    //红外学习上升或下降标志位
u16 Dval;                        //上升沿、下降沿的计数器的值
u8  RmtCnt=0;                    //定时器红外学习计数超时标志位	  
u8  PulseTabCnt=0;               //上升沿下降沿计数器的值
u16 PulseTab[MAX_PULSE_LEN]={0}; //红外学习存储数据
u8  Flag_LearnState = 0;         //红外学习标志位

/**
  *@name  void TIM8_UP_IRQHandler(void)
  *@brief TIM8的定时计数中断函数
  *       The timing count interrupt function of TIM8
  *@note
  */
void TIM8_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM8,TIM_IT_Update)!=RESET)
	{
		if(RmtCnt++>50)
		{
			RmtCnt = 0;

			if(RmtSta)
			{
				RmtSta = 0;
				Flag_LearnState = 1;
			}
		}
	}
	TIM_ClearITPendingBit(TIM8,TIM_IT_Update);	 	    
}

/**
  *@name  void TIM8_CC_IRQHandler(void)
  *@brief 捕获红外载波的高低电平宽度，记录到数组PulseTab
  *       Capture the high and low level width of the infrared carrier and record it to the array PulseTab
  *@note
  */
void TIM8_CC_IRQHandler(void)
{ 		    	 
	if(TIM_GetITStatus(TIM8,TIM_IT_CC3)!=RESET)                 //获取上升沿或下降沿状态
	{	  
		if(RDATA)                                               //上升沿捕获
		{
			Dval=TIM_GetCapture3(TIM8);                         //读取CCR4也可以清CC4IF标志位
			
			TIM_OC3PolarityConfig(TIM8,TIM_ICPolarity_Falling); //CC4P=1	设置为下降沿捕获
			
			TIM_SetCounter(TIM8,0);                             //清空定时器值
			
			if(RmtSta&0X01)
			{
				PulseTab[PulseTabCnt++] = Dval;
				
				RmtSta = 0x10;
			}
			else
			{
				RmtSta = 0X10;                                  //标记上升沿已经被捕获
			}
		}
		else                                                    //下降沿捕获
		{
			Dval=TIM_GetCapture3(TIM8);                         //读取CCR4也可以清CC4IF标志位

			TIM_OC3PolarityConfig(TIM8,TIM_ICPolarity_Rising);  //CC4P=0	设置为上升沿捕获
			
			TIM_SetCounter(TIM8,0);                             //清空定时器值
			
			if(RmtSta&0X10)                                     //完成一次高电平捕获 
			{
				PulseTab[PulseTabCnt++] = Dval;
				
				RmtSta = 0x01;
			}
			else 
			{
				RmtSta = 0x01;
			}
		}				 		     	    					   
	}
	TIM_ClearITPendingBit(TIM8,TIM_IT_CC3);	 	    
}

/**
  *@name            void Infrared_Send_IR1(u16 *irdata,u32 irlen)
  *@description     红外信号发射函数
  *@param           irdata --   u16,红外数据
                    irlen  --   u32,红外数据长度							
  *@return		
  *@notice			
  */
void Infrared_Send(u16 *irdata,u32 irlen)
{
	u32 i;                                      //用于下面的for循环

	for(i=0; i<irlen && irdata[i]!=0xffff; i++) //循环，从i=0开始，当i<irlen 并且 irdata[i] != 0xffff 时成立，当其中一个不成立，退出循环
	{
		if(i%2 == 0)                            //偶数的下标的数组成员延时拉高电平
		{
			TIM_Cmd(TIM3,ENABLE);
			
			delay_us(irdata[i]);
			
			TIM_Cmd(TIM3,DISABLE);
			
			GPIO_SetBits(GPIOA,GPIO_Pin_7);			
		}
		else
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_7);
			
			delay_us(irdata[i]);
		}
	}
	
	delay_us(555);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	
	return ;
}
