#include "Configuration.h"

#define ADC1_DR_Adress 		((u32)0x4001244C)
#define ADC3_DR_Address    	((u32)0x40013C4C)
#define USART1_DR_Adress 	((u32)0x40013804)
#define USART2_DR_Adress 	((u32)0x40004404)
#define USART3_DR_Adress 	((u32)0x40004804)
/**************************************************************************************
��������DMA_Configuration
������������DMA. USART1_TX,USART2_TX,USART3_TX,ADC1_2,ADC3ʹ��DMA.
			ADC1_2		DMA1 Channel1
			USART3_TX	DMA1_Channel2
			USART1_TX	DMA1_Channel4
			USART2_TX	DMA1_Channel7
			ADC3		DMA2_Channel5
���롡����
���������
���ء�����
**************************************************************************************/
void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	//ADC1_2  use DMA1 Channel1
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Adress;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC1_2_ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = ADC1_2_CHANNEL_NUM / 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	DMA_Cmd(DMA1_Channel1, ENABLE);				//����DMAͨ��1
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC,ENABLE);	//��������ж�

	//USART1_TX use DMA1_Channel4
	DMA_DeInit(DMA1_Channel4); 	//��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Adress;	//�Զ���DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART1_Send_Buf ; //����DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	//������Ϊ���ݴ����Ŀ��
	DMA_InitStructure.DMA_BufferSize = USART1_BUF;	//����ָ��DMAͨ����DMA����Ĵ�С����λΪ���ݵ�λ
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //����8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;		//������ ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;		//DMAͨ��1ӵ�и����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);

	//DMA_Cmd(DMA1_Channel4, ENABLE);		//����DMAͨ��4
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);	 //��������ж�

	//USART2_TX use DMA1_Channel7
	DMA_DeInit(DMA1_Channel7); 	//��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_DR_Adress;	//�Զ���DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART2_Send_Buf ; //����DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	//������Ϊ���ݴ����Ŀ��
	DMA_InitStructure.DMA_BufferSize = USART2_BUF;	//����ָ��DMAͨ����DMA����Ĵ�С����λΪ���ݵ�λ
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //����8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;		//������ ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;		//DMAͨ��1ӵ�и����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);

	//DMA_Cmd(DMA1_Channel7, ENABLE);		//����DMAͨ��7
	DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);	 //��������ж�

	/*
	//USART3_TX use DMA1_Channel2
	DMA_DeInit(DMA1_Channel2); 	//��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART3_DR_Adress;	//�Զ���DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART3_Send_Buf ; //����DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	//������Ϊ���ݴ����Ŀ��
	DMA_InitStructure.DMA_BufferSize = USART3_BUF;	//����ָ��DMAͨ����DMA����Ĵ�С����λΪ���ݵ�λ
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //����8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;		//������ ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;		//DMAͨ��1ӵ�и����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);

	DMA_Cmd(DMA1_Channel2, ENABLE);		//����DMAͨ��2
	DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);	 //��������ж�
	*/
	

#if defined (CONFIG_DEBUG)
	printf("Initializing DMA ... Done.\r\n");
	DebugPrintf();
#endif
	
}
