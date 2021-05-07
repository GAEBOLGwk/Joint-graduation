/************************************************
文件创建时间：2014-7-19
文件版本：    V1
版本作者：    中山火炬职业技术学院电子信息工程专业    杨立宏
文件实现功能： 用通用定时器TIM3实现一个LED灯的闪烁
************************************************/
#include "stm32f10x.h"	   //系统文件，必须要包含
#include "user.h"
#include "led.h"
#include "usart.h"

uint32_t IR_Key = 0;
uint8_t  dat=0;
uint8_t flag=0;

/*************主函数**************************/
int main(void)
{
  Capture_Config();
	USART1_Config(9600);
	LedInit();
  while(1)
   {
	 if(flag==1)
	 {
	   flag = 0;
	   dat = (uint8_t)(IR_Key>>8);
		 printf("编码：%x\r\n",dat);
      if(dat == 0x30)
				 DS0(ON);
			 if(dat == 0x18)
				  DS0(OFF); 
	 }
	 
   }
}

/****************************************
  0xa2      0x62      0xe2
  0x22      0x02      0xc2
  0xe0      0xa8      0x90
  0x68      0x98      0xb0
  0x30      0x18      0x7a
  0x10      0x38      0x5a
  0x42      0x4a      0x52


*****************************************/



