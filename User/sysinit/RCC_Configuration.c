#include "Configuration.h"

/**************************************************************************************
��������RCC_Configuration
�����������ò�ͬ��ϵͳʱ��
���롡����
���������
���ء�����
**************************************************************************************/
void RCC_Configuration(void)
{
 	ErrorStatus HSEStartUpStatus;
	RCC_DeInit();					//������RCC�Ĵ�������ΪĬ��ֵ

	RCC_HSEConfig(RCC_HSE_ON);		//����HSE���ⲿ���پ���
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus == SUCCESS)	//HSE�Ƿ�ɹ�����
	{
		//����AHBʱ�ӣ�HCLK����AHBʱ�ӣ�ϵͳʱ��  72M
		RCC_HCLKConfig(RCC_SYSCLK_Div1);

		//���ø���AHBʱ�ӣ�PCLK2����APB2ʱ�ӣ�HCLK��72M
		RCC_PCLK2Config(RCC_HCLK_Div1);

		//���õ���AHBʱ�ӣ�PCLK1��	APB1ʱ�ӣ�HCLK/2	36M
		RCC_PCLK1Config(RCC_HCLK_Div2);

		 //����ADCʱ��Ƶ�� ADCʱ�� = PCLK2 / 8	9M
        	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 

		//����FLASH�洢����ʱʱ��������	2��ʱ����
		FLASH_SetLatency(FLASH_Latency_2);

		//Ԥȡָ����ʹ��
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		//����PLLʱ��Դ����Ƶϵ�� PLLCLK = 8MHz * 9 = 72MHz
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

		//ʹ��PLL
		RCC_PLLCmd(ENABLE);

		//�ȴ�PLL��Ч
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}

		//��PLL��Ϊϵͳʱ��Դ
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		//�ȴ�PLL��Ϊϵͳʱ��Դ��Ч
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}


	//ʹ��APB2����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
							RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
							RCC_APB2Periph_AFIO, ENABLE);

	//PWR_BackupAccessCmd(ENABLE);	//�����޸�RTC �ͺ󱸼Ĵ���
	//RCC_LSEConfig(RCC_LSE_OFF);		//�ر��ⲿ�����ⲿʱ���źŹ��� ��PC13 PC14 PC15 �ſ��Ե���ͨIO�á�
	//BKP_TamperPinCmd(DISABLE);	//�ر����ּ�⹦�ܣ�Ҳ���� PC13��Ҳ���Ե���ͨIO ʹ��

}
