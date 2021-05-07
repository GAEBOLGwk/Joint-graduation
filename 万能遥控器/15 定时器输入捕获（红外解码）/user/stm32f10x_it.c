/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */
/*************************************************************
程序说明：红外数据从第一个下降沿开始到解完码最后一个下降沿一共34个，
因此对下降沿计数到34时即表示已经解完码。如果一直按着按键，会进入连发状态，
为了对连发次数计数，即判断按键何时抬起，可以采用溢出中断配合，在输入捕获中断中对
溢出中断次数清零，因此如果按键抬起，没有接收到码，即不会进入到捕获中断，这时溢出
中断次数将不会被清零，如果一个以上的计数周期仍没有下降沿来，就对解码计数值、状态等
清零，重新解码。
**************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usart.h"

extern uint32_t IR_Key;
extern uint8_t flag;
  uint8_t IR_Sta = 0;
  uint8_t IR_Up = 0;
  uint32_t IR_Code = 0;
  uint8_t IR_PulseCnt = 0;
  uint16_t IR_ThisPulse = 0;
  uint16_t IR_LastPulse = 0;
  uint16_t IR_PulseSub = 0;
  uint16_t LianfaCnt;

 //http://blog.csdn.net/mhjerry/article/details/6600414

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

//TIM3中断入口函数
void TIM4_IRQHandler(void)
{ 
  if(TIM_GetITStatus(TIM4 , TIM_IT_CC4) != RESET ) 
    {    
      TIM_ClearITPendingBit(TIM4 , TIM_IT_CC4);
	  IR_Up = 0;                                //每次进入捕获中断就清除溢出次数计数值
	  IR_ThisPulse = TIM_GetCapture4(TIM4);	    //获取通道2的捕获值
	  if(IR_ThisPulse>IR_LastPulse)	            //这次捕获的值大于上次捕获的值
	   { 
		 IR_PulseSub = IR_ThisPulse-IR_LastPulse;  //得到时间差
	   }
	  else										//小于时要加一个0xffff
	   { 
		 IR_PulseSub = 0xffff-IR_LastPulse+IR_ThisPulse;  //得到时间差
	   }
	  IR_LastPulse = IR_ThisPulse;  //将本次得到值作为下一位编码解码的前一个值
	  IR_PulseCnt++;   //解码位数加1
	  if(IR_PulseCnt == 2)
	   {  
          if((IR_PulseSub>6000)&&(IR_PulseSub<8000))  	 //引导码范围，13.5ms	6750
		   {
	         IR_Sta = 0x01;				              //标志位1，表示引导码已收到
		   }
		  else
		   IR_PulseCnt = 0;							   //如果时间差没在引导码范围内，中断次数清零，重新解码
	   }
	  if((IR_PulseCnt > 2)&&(IR_Sta == 0x01))		  //如果中断次数大于2，并且引导码已经解完
	   { 
	     IR_Code <<= 1;								   //存储红外编码的寄存器向左移一位，以便于存储下一个位放到最低位
		 if((IR_PulseSub>450)&&(IR_PulseSub<700))	   //编码‘0’范围，1.125ms 562.5
		  {
		    
		    IR_Code|=0x00;							   //存储0
		  }
		 else if((IR_PulseSub>800)&&(IR_PulseSub<1300))	   //编码‘1’范围，2.25ms 1125
		        {
		          IR_Code|=0x01;						  //存储1
				  
				}
			  else										 //如果不是0码也不是1码，变量清零，重新解码
			   { 
				 IR_Sta = 0;
	       IR_Code = 0;
				 IR_PulseCnt = 0;
			   }
	   }

	   if(IR_PulseCnt == 34)						 //如果解完码，第34次进入中断正好解完一帧码
	    { 
	      IR_Key = IR_Code;							 //存放解出的码
		  IR_Sta = 0x02;								 //进入连发码状态
		  flag = 1;									 //解完码标志位置1，进行编码处理
	    }
	   if((IR_PulseCnt == 36)&&(IR_Sta == 0x02))
	     {
		    IR_PulseCnt = 34;
		    if((IR_PulseSub>4500)&&(IR_PulseSub<6000))
			  {
				 LianfaCnt++;
				 IR_Key = IR_Code;
				 flag = 1;
			  }
		 }	      
	} 

  if(TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET)
   {
	 TIM_ClearITPendingBit(TIM4 , TIM_IT_Update);
	 if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 1)
	  {
	    IR_Up++;
		if(IR_Up>=2)
		 {
		   IR_Code = 0;
		   IR_Sta  = 0;
		   IR_PulseCnt = 0;
		   LianfaCnt = 0;
		 }
	  }
   }
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
