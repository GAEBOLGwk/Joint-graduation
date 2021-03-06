/**
  ******************************************************************************
  * @file    stm32f10x_rtc.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   This file contains all the functions prototypes for the RTC firmware 
  *          library.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_RTC_H
#define __STM32F10x_RTC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup RTC
  * @{
  */ 

/** @defgroup RTC_Exported_Types
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup RTC_Exported_Constants
  * @{
  */

/** @defgroup RTC_interrupts_define 
  * @{
  */

#define RTC_IT_OW            ((uint16_t)0x0004)  /*!< Overflow interrupt */
#define RTC_IT_ALR           ((uint16_t)0x0002)  /*!< Alarm interrupt */
#define RTC_IT_SEC           ((uint16_t)0x0001)  /*!< Second interrupt */
#define IS_RTC_IT(IT) ((((IT) & (uint16_t)0xFFF8) == 0x00) && ((IT) != 0x00))
#define IS_RTC_GET_IT(IT) (((IT) == RTC_IT_OW) || ((IT) == RTC_IT_ALR) || \
                           ((IT) == RTC_IT_SEC))
/**
  * @}
  */ 

/** @defgroup RTC_interrupts_flags 
  * @{
  */

#define RTC_FLAG_RTOFF       ((uint16_t)0x0020)  /*!< RTC Operation OFF flag */
#define RTC_FLAG_RSF         ((uint16_t)0x0008)  /*!< Registers Synchronized flag */
#define RTC_FLAG_OW          ((uint16_t)0x0004)  /*!< Overflow flag */
#define RTC_FLAG_ALR         ((uint16_t)0x0002)  /*!< Alarm flag */
#define RTC_FLAG_SEC         ((uint16_t)0x0001)  /*!< Second flag */
#define IS_RTC_CLEAR_FLAG(FLAG) ((((FLAG) & (uint16_t)0xFFF0) == 0x00) && ((FLAG) != 0x00))
#define IS_RTC_GET_FLAG(FLAG) (((FLAG) == RTC_FLAG_RTOFF) || ((FLAG) == RTC_FLAG_RSF) || \
                               ((FLAG) == RTC_FLAG_OW) || ((FLAG) == RTC_FLAG_ALR) || \
                               ((FLAG) == RTC_FLAG_SEC))
#define IS_RTC_PRESCALER(PRESCALER) ((PRESCALER) <= 0xFFFFF)

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup RTC_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup RTC_Exported_Functions
  * @{
  */

void RTC_ITConfig(uint16_t RTC_IT, FunctionalState NewState);//RTC????????
void RTC_EnterConfigMode(void);//RTC????????????
void RTC_ExitConfigMode(void);//RTC????????????
uint32_t  RTC_GetCounter(void);//????RTC??????
void RTC_SetCounter(uint32_t CounterValue);//??????????
void RTC_SetPrescaler(uint32_t PrescalerValue);//????????????????????
void RTC_SetAlarm(uint32_t AlarmValue);//????????
uint32_t  RTC_GetDivider(void);//????????????
void RTC_WaitForLastTask(void);//????????????RTC??????????????????
void RTC_WaitForSynchro(void);//????RTC????????RTC_CNT,RTC_ALR??RTC_PRL????RTC??APB??????????
FlagStatus RTC_GetFlagStatus(uint16_t RTC_FLAG);//??????????RTC??????????????
void RTC_ClearFlag(uint16_t RTC_FLAG);//????RTC??????????????
ITStatus RTC_GetITStatus(uint16_t RTC_IT);//??????????RTC????????????
void RTC_ClearITPendingBit(uint16_t RTC_IT);//????RTC??????????????

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_RTC_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
/*
RTC_CRH??????:????????
??????????0x00
????????0x0000
 15	14	13	12	11	10	9	8	7	6	5	4	3   2	 1	   0
 -----------------------------------------------------------------
|						????					 |OWIE|ALRIE|SECIE|
 -----------------------------------------------------------------
													rw	 rw	   rw
 -----------------------------------------------------------------
|??15:3 	|??????????????????0.								  |
|-----------------------------------------------------------------|
|??2		|OWIE??????????????.									  |
|			|0????????????????????????							  |																
|			|1??????????????										  |															
|-----------------------------------------------------------------|
|??1		|ALRIE??????????????.								  |
|			|0????????????????????????							  |																
|			|1??????????????										  |																
|-----------------------------------------------------------------|
|??0		|SECIE????????????.									  |
|			|0????????????????????????							  |																
|			|1????????????										  |																
 -----------------------------------------------------------------
 RTC_CRL??????
??????????0x04
????????0x0020
 15	14	13	12	11	10	9	8	7	6		 5		4		3 	  	2		 1	   		0
 ---------------------------------------------------------------------------------------------
|						????		 	  |RTOFF|  CNF	|  RSF	|  OWF	|	ALRF	|  SECF	  |
 ---------------------------------------------------------------------------------------------
											  r     rw     rc w0  rc  w0  rc  	w0	  rc  w0
 ----------------------------------------------------------------------------------------
|??15:6 	|??????????????????0.								  						 |
|-----------|----------------------------------------------------------------------------|
|??5		|RTOFF??RTC??????????								  						 |
|			|RTC??????????????????????????????????????????????????????????????????????????.	 |
|			|????????0????????????????????RTC????????????????????????????????				 |
|			|0??????????RTC????????????????????????							             |																
|			|1??????????RTC??????????????????????	??  									 |																 
|-----------|----------------------------------------------------------------------------|
|??4		|CNF??????????.						    								  	 |
|			|????????????????'1'??????????????????????????RTC_CNT,RTC_ALR,RTC_PRL????????	 |
|			|??????????????????????????1??????????????????????????????????????				 |
|			|0????????????????????????RTC????????								  			 |																
|			|1??????????????			            										 |																
|-----------|----------------------------------------------------------------------------|
|??3		|RSF????????????????.									       				 |
|			|????RTC_CNT????????RTC_DIV????????????????????????????????????????,??APB1      |
|			|??????????APB1??????????????????????????????????????????????????????,????       |
|			|??????????????????????????????????RTC_CNT,RTC_ALR??RTC_PRL????????????   		 |
|			|0????????????????	??									  					 |																
|			|1??????????????????	??									    				 |																 
| ---------------------------------------------------------------------------------------|
|??2		|OWF????????????								  						         |
|			|??32??????????????????????????????????????????RTC_CRH????????OWIE=1????????	 |
|			|??????????????????????????????????????????									 |
|			|0????????														             |																
|			|1??32????????????????????				  									 |																 
|-----------|----------------------------------------------------------------------------|
|??1		|ALRF??????????.						    								  	 |
|			|??32??????????????????RTC_ALR????????????????????????????????????????RTC_CRH	 |
|			|??????????ALRIE=1??????????????????????????????????????????????????			 |
|			|0????????	??													  			 |																
|			|1??????????				            										 |																
|-----------|----------------------------------------------------------------------------|
|??0		|SECF????????.									       						 |
|			|??32??????????????????????????????????????????????????RTC????????????????	     |
|			|??????????????????????RTC??????????????????????????????????1??????????			 |
|			|RTC_CRH??????????SECIE=1??????????????????????????????????????????????		 |
|			|??????																		 |
|			|0??????????????????	??									  					 |																
|			|1????????????????	??									    				 |																 
 ----------------------------------------------------------------------------------------



RTC_PRLH??????	??????RTC??????
??????????0x08
????????0x0000
 15	14	13	12	11	10	9	8	7	6		 5		4		3 	  	2		 1	   		0
 ---------------------------------------------------------------------------------------------
|						????		 	      			| 				 PRL[3:0]			  |
 ---------------------------------------------------------------------------------------------
															w		w		w			w
 ----------------------------------------------------------------------------------------
|??15:4 	|??????????????????0.								  						 |
|-----------|----------------------------------------------------------------------------|
|??[3:0]	|PRL[3:0]??RTC??????????????????								  				 | 		 
|			|??????????????????????????????????????????????								 |
|			|fTR_CLK=	fRTC_CLK		??												 |									 
|			|		 ----------------				               						 |																
|			|  		  ??PRL[19:0]+1??						 							 |																 
|			|  ??????????????0??????????????????????RTC??????????????						 |																 
 ----------------------------------------------------------------------------------------


RTC_PRLL??????	??????RTC??????
??????????0x10
????????0x8000

15	14	13	12	11	10	9	8	7	6		 5		4		3 	  	2		 1	   		0
 ---------------------------------------------------------------------------------------------
|										 PRL[15:0]			 								  |
 ---------------------------------------------------------------------------------------------
w	w	w	w	w	w	w	w	w	w		 w		w		w		w		 w			w											
 ----------------------------------------------------------------------------------------
|??[15:0]	|PRL[15:0]??RTC??????????????????								  			 | 		
|			|??????????????????????????????????????????????								 |
|			|fTR_CLK=	fRTC_CLK		??												 |									 
|			|		 ----------------				               						 |																
|			|  		  ??PRL[19:0]+1??						 							 |																 
|			|  ??????????????0??????????????????????RTC??????????????						 |																 
 ----------------------------------------------------------------------------------------

RTC????????????????????RTC_DIVH??	??
 15	14	13	12	11	10	9	8	7	6		 5		4		3 	  	2		 1	   		0
  ---------------------------------------------------------------------------------------------
 |										????			|	RTC_DIV[19:16] 					  |
  ---------------------------------------------------------------------------------------------
															r		r		 r			r
 ----------------------------------------------------------------------------------------
|??15:4 	|??????????????????0.								  						 |
|-----------|----------------------------------------------------------------------------|
|??[3:0]	|RTC_DIV[19:16]??RTC??????????????????????RTC clock divider high??			 | 		 
 ----------------------------------------------------------------------------------------

RTC??????????????????????????RIC_DIVL??
??????????0x14
????????0x8000
 15	14	13	12	11	10	9	8	7	6		 5		4		3 	  	2		 1	   		0
 ---------------------------------------------------------------------------------------------
|								RTC_DIV[15:0]	 			 								  |
 ---------------------------------------------------------------------------------------------
r	r	r 	r	 r	 r	r	r	r	r		 r		 r		r		r		  r			 r
 ----------------------------------------------------------------------------------------
|??[15:0]	|RTC_DIV[15:0]??RTCs????????????????RTC  clock divider low????	  			 | 		
 ----------------------------------------------------------------------------------------

RTC????????????????????RIC_CNTH??
??????????0x18
????????0x0000
 15	14	13	12	11	10	9	8	7	6		 5		4		3 	  	2		 1	   		0
 ---------------------------------------------------------------------------------------------
|								RTC_CNT[31:16]	 			 								  |
 ---------------------------------------------------------------------------------------------
rw	rw	rw 	rw	 rw	 rw	rw	rw	rw	rw		 rw		 rw		rw		rw		  rw		rw
 ----------------------------------------------------------------------------------------
|??[15:0]	|RTC_CNT[15:0]??RTC????????????RTC  counter high????				  			 | 
|			|????????RTC_CNTH????????????RTC????????????????????????????????????????		 |
|			|????????????????????????????													 |		
 ----------------------------------------------------------------------------------------

RTC????????????????????RIC_CNTL??
??????????0x1C
????????0x0000
 15	14	13	12	11	10	9	8	7	6		 5		4		3 	  	2		 1	   		0
 ---------------------------------------------------------------------------------------------
|								RTC_CNT[15:0]	 			 								  |
 ---------------------------------------------------------------------------------------------
rw	rw	rw 	rw	 rw	 rw	rw	rw	rw	rw		 rw		 rw		rw		rw		  rw		rw
 ----------------------------------------------------------------------------------------
|??[15:0]	|RTC_CNT[15:0]??RTC????????????RTC  counter high????				  			 | 
|			|????????RTC_CNTH????????????RTC????????????????????????????????????????		 |
|			|????????????????????????????													 |		
 ----------------------------------------------------------------------------------------

RTC??????????????????RIC_ALRH??
??????????0x20
????????0xFFFF
 15	14	13	12	11	10	9	8	7	6		 5		4		3 	  	2		 1	   		0
 ---------------------------------------------------------------------------------------------
|								RTC_ALR[31:16]	 			 								  |
 ---------------------------------------------------------------------------------------------
w	w	 w	 w	 w	w	w	w	w	w		w		w		w		w		 w			w
 ----------------------------------------------------------------------------------------
 |??[15:0]	|RTC_ALR[31:16]??RTC????????????RTC  Alarm  high????					  		 | 
|			|??????????????????????????????????????????????????????????????????????			 |
|			|??????????????????															 |		
 ----------------------------------------------------------------------------------------

RTC????????????????????RIC_ALRL??
??????????0x24
????????0xFFFF
 15	14	13	12	11	10	9	8	7	6		 5		4		3 	  	2		 1	   		0
 ---------------------------------------------------------------------------------------------
|								RTC_ALR[15:0]	 			 								  |
 ---------------------------------------------------------------------------------------------
w	w	 w	 w	 w	w	w	w	w	w		w		w		w		w		 w			w
 ----------------------------------------------------------------------------------------
|??[15:0]	|RTC_ALR[15:0]??RTC????????????RTC  alrm high????				  			 | 
|			|????????????????????????????????????????????????????????????????				 |
|			|????????????????????????????													 |		
 ----------------------------------------------------------------------------------------


RCC_BDCR??????????????????????????
	31	30	29	28	27	26	25	24	23	22	21	20	19	18	17	16	
 --------------------------------------------------------------------
|												 		   	   |BDRST|
 --------------------------------------------------------------------
																  rw
	15	14	13	12	11	10	9	8	7	6	5	4	3	2	1	0	
 --------------------------------------------------------------------
|	RTC|				   |RTCSEL|					  |
|   EN |				   |[1:0] |					  |
 --------------------------------------------------------------------
																  rw
	


*/
