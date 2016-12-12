#include "Configuration.h"
/*
	ADC���ɼ�12��ͨ���ֱ�Ϊ:
		ADC_Channel_16	:	оƬ�ڲ��¶ȴ�����
		ADC_Channel_17	:	оƬ�ڲ�1.2V��׼
		ADC_Channel_10	:	
		ADC_Channel_11	:		
*/

/**************************************************************************************
��������ADC_Configuration
������������ADC, ADC1 ��ADC2  ������ͬ������ģʽ
���롡����
���������
���ء�����
**************************************************************************************/
void ADC_Configuration(void)
{
	ADC_InputPin_Configuration();	//����ADC��������
	ADC1_2_Configuration();			//����ADC1��ADC2������ͬ������ת��ģʽ

#if defined (CONFIG_DEBUG)
	printf("Initializing ADC1_2 ... Done\n");
	DebugPrintf();
#endif

}

/**************************************************************************************
��������ADC_InputPin_Configuration
������������ADC��������
���롡����
���������
���ء�����
**************************************************************************************/
void ADC_InputPin_Configuration(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//PC0, PC1ģ������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 |GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}

/**************************************************************************************
��������ADC1_2_Configuration
������������ADC, ADC2������ͬ������ģʽ
���롡����
���������
���ء�����
**************************************************************************************/
void ADC1_2_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	//����ADCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);


	//ADC1��ʼ��
	ADC_DeInit(ADC1);									//������ADC1��ȫ���Ĵ�������Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;	//ADC1��ADC2������ͬ������ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;		//ģ��ת�������ڶ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC3;	//
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = ADC1_2_CHANNEL_NUM / 2;			//�涨��˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);

	//ADC2��ʼ��
	ADC_DeInit(ADC2);									//������ADC2��ȫ���Ĵ�������Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;	//ADC1��ADC2������ͬ������ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;		//ģ��ת�������ڶ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC3;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = ADC1_2_CHANNEL_NUM / 2;			//�涨��˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC2, &ADC_InitStructure);
		
	 
	ADC_TempSensorVrefintCmd(ENABLE);	//ADC�����¶ȴ�����ʹ�ܣ�Ҫʹ��Ƭ���¶ȴ��������м�Ҫ��������
	
	//ADC1����ת��ͨ������
    	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_71Cycles5);	//����ת������1��ͨ��16���ڲ��¶ȴ�������Ҫ�����ʱ�����2.2us	
    	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 2, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 3, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 4, ADC_SampleTime_71Cycles5);	//
	ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 5, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 6, ADC_SampleTime_71Cycles5);
	//ADC2����ת��ͨ������
    	ADC_RegularChannelConfig(ADC2, ADC_Channel_10,  1, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_10,  2, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_10,  3, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_11,  4, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_11,  5, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_11,  6, ADC_SampleTime_71Cycles5);

	
	ADC_DMACmd(ADC1, ENABLE);	//����ADC1����DMA����
//	ADC_ITConfig(ADC2, ADC_IT_EOC, ENABLE);	//����ADCת�������ж� (����ת���������ж�)

	ADC_Cmd(ADC1, ENABLE);		//����ADC1
	ADC_Cmd(ADC2, ENABLE);		//����ADC2
	/* Enable ADC1 external trigger */ 
 	ADC_ExternalTrigConvCmd(ADC1, ENABLE);
	ADC_ExternalTrigConvCmd(ADC2, ENABLE);

	//ADC1У׼
	ADC_ResetCalibration(ADC1);	//����ADC1��λУ׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1)){}	  //���ADC1У׼�Ĵ����Ƿ�λ���
	ADC_StartCalibration(ADC1);	//����ADC1У׼
	while(ADC_GetCalibrationStatus(ADC1)){}		//���ADC1У׼�Ƿ����
	//ADC2У׼
	ADC_ResetCalibration(ADC2);	//����ADC1��λУ׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC2)){}	  //���ADC1У׼�Ĵ����Ƿ�λ���
	ADC_StartCalibration(ADC2);	//����ADC2У׼
	while(ADC_GetCalibrationStatus(ADC2)){}		//���ADC1У׼�Ƿ����
}


