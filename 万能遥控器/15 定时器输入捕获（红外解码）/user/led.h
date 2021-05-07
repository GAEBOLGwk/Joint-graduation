#ifndef __LED_H
#define __LED_H 			   


#define ON     0
#define OFF    1

/**************************************************************************************************/
//第一种端口宏定义方法
/*
// 把“位段地址＋位序号”转换别名地址宏 
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
//把该地址转换成一个指针 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入
*/
/**************************************************************************************************/

/**************************************************************************************************/
//第二种端口宏定义方法

#define  DS0(x)   GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_5) | (x ? GPIO_Pin_5 : 0) 
#define  DS1(x)   GPIOE->ODR=(GPIOE->ODR&~GPIO_Pin_5) | (x ? GPIO_Pin_5 : 0) 
#define  BEEP(x)   GPIOB->ODR=(GPIOB->ODR&~GPIO_Pin_8) | (x ? GPIO_Pin_8 : 0) 


/**************************************************************************************************/

/**************************************************************************************************/
//第三种端口宏定义方法
/*
#define  LED5ON     GPIO_ResetBits(GPIOE,GPIO_Pin_3)
#define  LED5OFF    GPIO_SetBits(GPIOE,GPIO_Pin_3)

#define  LED6ON     GPIO_ResetBits(GPIOE,GPIO_Pin_4)
#define  LED6OFF    GPIO_SetBits(GPIOE,GPIO_Pin_4)

#define  LED7ON     GPIO_ResetBits(GPIOE,GPIO_Pin_5)
#define  LED7OFF    GPIO_SetBits(GPIOE,GPIO_Pin_5)

#define  LED8ON     GPIO_ResetBits(GPIOE,GPIO_Pin_6)
#define  LED8OFF    GPIO_SetBits(GPIOE,GPIO_Pin_6)

#define  LED9ON     GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define  LED9OFF    GPIO_SetBits(GPIOB,GPIO_Pin_1)

*/
/**************************************************************************************************/


void LedInit(void);

#endif
