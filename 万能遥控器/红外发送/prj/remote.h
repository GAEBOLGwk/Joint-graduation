

/*remote.h*/
#ifndef __RED_H
#define __RED_H  
#include "stm32f10x.h"

#define RDATA 	PCin(8)	 	//�������������

//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
//����ѡ�õ�ң����ʶ����Ϊ0
#define REMOTE_ID 0      		   
#define MAX_PULSE_LEN 400 //500  300

extern u8  RmtCnt;			//�������µĴ���
extern u16 PulseTab[MAX_PULSE_LEN];
extern u8  Flag_LearnState ;
extern u8  PulseTabCnt;//�������½��ؼ�������ֵ

void Remote_Init(void);    	//���⴫��������ͷ���ų�ʼ��
void TIM3_PWM_Init(u16 arr,u16 psc);
void Infrared_Send(u16 *irdata,u32 irlen);

#endif
