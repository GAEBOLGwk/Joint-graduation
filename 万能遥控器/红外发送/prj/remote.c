/*remote.c*/
#include "remote.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h"
/**
���䣺PA7-TIM3-CH2
���գ�PC8-TIM8-CH3

TIM8��
��������в����жϺͶ�ʱ�жϷֿ�������ͬһ�жϺ�����������Ҫ���������ж����ȼ���Ӧ�ÿ��Ժ���һ�����ã�
*/

/**
 *  @name			void Remote_Init(void) 
 *	@description	����ң�س�ʼ�� ����IO�Լ���ʱ��8�����벶��
 *                  Infrared remote initializer sets IO and timer 8 input capture
 *  @notice			
 */
void Remote_Init(void)    			  
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;  

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE); //ʹ��PORTBʱ�� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);                      //TIM8 ʱ��ʹ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                                //PC8 ���� 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                    //�������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_8);                                          //��ʼ��GPIOC.8

	TIM_TimeBaseStructure.TIM_Period = 9999;                                 //�趨�������Զ���װֵ ���10ms���  
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1);                             //Ԥ��Ƶ��,1M�ļ���Ƶ��,1us��1.	   	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                  //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;              //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);                          //����ָ���Ĳ�����ʼ��TIMx

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;                         // ѡ������� IC4ӳ�䵽TI4��
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;              //�����ز���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;                    //���������Ƶ,����Ƶ 
	TIM_ICInitStructure.TIM_ICFilter = 0x03;                                 //IC4F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
	TIM_ICInit(TIM8, &TIM_ICInitStructure);                                  //��ʼ����ʱ�����벶��ͨ��
	TIM_Cmd(TIM8,ENABLE );                                                   //ʹ�ܶ�ʱ��4

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;                       //TIM8TIM8����Ƚ��ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;                //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;                       //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                          //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);                                          //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���	
	TIM_ITConfig( TIM8,TIM_IT_CC3,ENABLE);                                   //��������ж� ,����CC4IE�����ж�		
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;                       //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;                //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;                       //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                          //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);                                          //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���	
	TIM_ITConfig( TIM8,TIM_IT_Update,ENABLE);                                //��������ж� ,����CC4IE�����ж�								 
}
/**
 *  @name			void TIM3_PWM_Init(u16 arr,u16 psc)
 *	@description	��ʼ����ʱ��3�����ã�����ʱ��3����PWM���ƣ�PWM�����Ϊ PA.7
 *	@param			arr --	u16,��ʱ����װֵ
					psc --	u16,��ʱ����Ƶֵ							
 *	@return		
 *  @notice			PWMƵ�� = 72M/((arr+1)*(psc+1)),�����������ⷢ����ز�����Ҫ����38kHz�ķ�������ȡarr = 1895,psc = 0��
 */
void TIM3_PWM_Init(u16 arr,u16 psc)
{
	/* ��ʼ���ṹ�嶨�� */
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;                        //��ʱ����������
	TIM_OCInitTypeDef 	TIM_OCInitStructure;                                  //��ʱ���Ƚ��������

	/* ʹ����Ӧ�˿ڵ�ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);                      //ʹ�ܶ�ʱ��2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); //ʹ��GPIO����ʱ��
	
	/* GPIOA.7��ʼ�� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                                 // TIM3 CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                           // PA.7 �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_7);

	/* TIM3 ��ʼ��*/
	TIM_TimeBaseInitStructure.TIM_Period = arr;                               //��һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                            //��ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;                          //ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;           //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	/* ��ʱ��TIM3 Ch2 PWMģʽ��ʼ�� */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                         //ѡ��ʱ��ģʽ:TIM PWM1   TIM_OCMode_PWM1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;             //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = (arr+1)/3;                                //ռ�ձ�1:3 (arr+1)/10  3
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                 //�������:TIM����Ƚϼ��Ը�   TIM_OCPolarity_High
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);

	/* ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ��� */
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 
}


u8 	RmtSta=0;                    //����ѧϰ�������½���־λ
u16 Dval;                        //�����ء��½��صļ�������ֵ
u8  RmtCnt=0;                    //��ʱ������ѧϰ������ʱ��־λ	  
u8  PulseTabCnt=0;               //�������½��ؼ�������ֵ
u16 PulseTab[MAX_PULSE_LEN]={0}; //����ѧϰ�洢����
u8  Flag_LearnState = 0;         //����ѧϰ��־λ

/**
  *@name  void TIM8_UP_IRQHandler(void)
  *@brief TIM8�Ķ�ʱ�����жϺ���
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
  *@brief ��������ز��ĸߵ͵�ƽ��ȣ���¼������PulseTab
  *       Capture the high and low level width of the infrared carrier and record it to the array PulseTab
  *@note
  */
void TIM8_CC_IRQHandler(void)
{ 		    	 
	if(TIM_GetITStatus(TIM8,TIM_IT_CC3)!=RESET)                 //��ȡ�����ػ��½���״̬
	{	  
		if(RDATA)                                               //�����ز���
		{
			Dval=TIM_GetCapture3(TIM8);                         //��ȡCCR4Ҳ������CC4IF��־λ
			
			TIM_OC3PolarityConfig(TIM8,TIM_ICPolarity_Falling); //CC4P=1	����Ϊ�½��ز���
			
			TIM_SetCounter(TIM8,0);                             //��ն�ʱ��ֵ
			
			if(RmtSta&0X01)
			{
				PulseTab[PulseTabCnt++] = Dval;
				
				RmtSta = 0x10;
			}
			else
			{
				RmtSta = 0X10;                                  //����������Ѿ�������
			}
		}
		else                                                    //�½��ز���
		{
			Dval=TIM_GetCapture3(TIM8);                         //��ȡCCR4Ҳ������CC4IF��־λ

			TIM_OC3PolarityConfig(TIM8,TIM_ICPolarity_Rising);  //CC4P=0	����Ϊ�����ز���
			
			TIM_SetCounter(TIM8,0);                             //��ն�ʱ��ֵ
			
			if(RmtSta&0X10)                                     //���һ�θߵ�ƽ���� 
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
  *@description     �����źŷ��亯��
  *@param           irdata --   u16,��������
                    irlen  --   u32,�������ݳ���							
  *@return		
  *@notice			
  */
void Infrared_Send(u16 *irdata,u32 irlen)
{
	u32 i;                                      //���������forѭ��

	for(i=0; i<irlen && irdata[i]!=0xffff; i++) //ѭ������i=0��ʼ����i<irlen ���� irdata[i] != 0xffff ʱ������������һ�����������˳�ѭ��
	{
		if(i%2 == 0)                            //ż�����±�������Ա��ʱ���ߵ�ƽ
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
