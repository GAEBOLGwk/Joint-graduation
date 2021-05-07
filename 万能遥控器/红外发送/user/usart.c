#include "stm32f10x.h"
#include "usart.h"

 /**
  * @brief  USART1 GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void USART1_Config(uint32_t baud)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		
		/* config USART1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);	//GPIOA时钟打开 
		
		/* USART1 GPIO config */
		/* Configure USART1 Tx (PB6) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);	
			
		/* USART1 mode config */
  USART_InitStructure.USART_BaudRate = baud;    //设置串口通信时的波特率为9600
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //设置数据位长度8个位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;   //在帧结尾传输1个停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;   //设置校验位“无”
  //设置硬件控制流失能
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	
  USART_InitStructure.USART_Mode = USART_Mode_Tx;//设置发送和接收使能
  
  USART_Init(USART1,&USART_InitStructure);
  USART_Cmd(USART1,ENABLE);//开启USART2
}

///重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}
/*********************************************END OF FILE**********************/
