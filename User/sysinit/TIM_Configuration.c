#include "Configuration.h"

/***************Configuration of TIM1****************************************************/
//TIM1_CC3�¼����ڴ���ADC1_2�Ĺ�������ͨ��ת��
/**************************************************************************************
��������TIM1_Configuration
�����������ö�ʱ��
���롡����
���������
���ء�����
**************************************************************************************/
void TIM1_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//����TIM1ʱ��
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,  ENABLE);	//TIM1���ù�����ȫӳ��
	
	TIM_DeInit(TIM1);			//������TIM8�Ĵ�������Ϊȱʡֵ

	//TIM1�������������ã�����PWMƵ�ʣ�����20ms	 
	// TIM1��ʱ��ԴΪ72M	  ����AHBʱ��2��Ƶ��2��Ƶ����ʱ����Ƶ�����õ�
	//������ʱ��Ƶ�ʣ�CK_CNT = f_CK_PSC/(Prescaler + 1);
	//PWMƵ��:f=TIMCLK/(TIM_Period + 1)
	TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1;	   // 2khz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = 400-1;		 // 5hz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;		  
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;	 //�˲�������TIM1��TIM8����
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	//����ͨ��PWMģʽ��ռ�ձ�
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				 //CCMR1 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		 //OC1�ź��������Ӧ��������� CCER
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;	 //����������λCCER
	TIM_OCInitStructure.TIM_Pulse = 50 ;								   
	//TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	   //OC1�͵�ƽ��Ч��CCER
	//TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;	   //����������λCCER
	//TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;		//???����λCR2	 
	//TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	//CR2

//	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
//	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_OC1Ref);

	TIM_OCInitStructure.TIM_Pulse = 200;
	TIM_OC3Init(TIM1,&TIM_OCInitStructure);
	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_OC3Ref);
	

	TIM_Cmd(TIM1, ENABLE);                         //����TIM1
	/* TIM1 main Output Enable */
  	TIM_CtrlPWMOutputs(TIM1, ENABLE); 

#ifdef CONFIG_DEBUG
	printf("Initializing TIM1 ... Done\n");
	DebugPrintf();
#endif

}


// TIM2 CH1, CH2 as encoder input A and B.
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	

 	//Enable clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// Remap pin
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);	// PA15 CH1, PB3 CH2


	// PA15 to TIM1 CH1
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA , &GPIO_InitStructure);

	// PB3 to TIM2 CH2
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB , &GPIO_InitStructure);

	TIM_DeInit(TIM2);


	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 
	TIM_TimeBaseStructure.TIM_Period = 65535;	// Reload value
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// Clock divided by 1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);	// Encoder mode 3

	// Config capture channels
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 10;	//0x4;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);


	// Clear update flag
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
	// Enable interrupt
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	

	
	//Reset counter
	TIM_SetCounter(TIM2,0);
	// Enable TIM2
	TIM_Cmd(TIM2, ENABLE); 


#ifdef CONFIG_DEBUG
	printf("Initializing TIM2 ... Done\r\n");
	DebugPrintf();
#endif
	
}	



void TIM3_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

 	// Enable TIM3 clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);


	/*
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	*/

	// PB0, PB1 as PWM output
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB , &GPIO_InitStructure);

	TIM_DeInit(TIM3);

	//TIM3 clock source 72MHz
	// CK_CNT = f_CK_PSC/(Prescaler + 1)
	// feq = TIMCLK/(Period + 1)
	TIM_TimeBaseStructure.TIM_Prescaler = 24 -1;	// 6MHz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	// Count up
	TIM_TimeBaseStructure.TIM_Period = 300 - 1;	// 20kHZ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;		  
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;	// Only valid for TIM1 and TIM8
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	//����ͨ��PWMģʽ��ռ�ձ�
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0 ;								   
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; 
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	

	/*
	TIM_OCInitStructure.TIM_Pulse = 0 ; 
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = 0 ; 
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);
	*/

	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OC4Init(TIM3,&TIM_OCInitStructure);
	
	//����TIM3
	TIM_Cmd(TIM3, ENABLE);
	//���������
	TIM_CtrlPWMOutputs(TIM3, ENABLE);	

#ifdef CONFIG_DEBUG
	printf("Initializing TIM3 ... Done.\r\n");
	DebugPrintf();
#endif
	
}	



// TIM4 CH1, CH2 as encoder input A and B.
void TIM4_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	

 	//Enable clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


	// PB6 PB7 to TIM1 CH1 CH2
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB , &GPIO_InitStructure);
	

	TIM_DeInit(TIM4);


	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 
	TIM_TimeBaseStructure.TIM_Period = 65535;	// Reload value
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// Clock divided by 1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);	// Encoder mode 3

	// Config capture channels
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 10;	//0x4;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);


	// Clear update flag
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);	
	// Enable interrupt
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);	

	
	//Reset counter
	TIM_SetCounter(TIM4, 0);
	// Enable TIM2
	TIM_Cmd(TIM4, ENABLE); 


#ifdef CONFIG_DEBUG
	printf("Initializing TIM4 ... Done.\r\n");
	DebugPrintf();
#endif
	
}	

// TIM5 CH1, CH2 as PWM output.
void TIM5_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

 	//ʹ��APB1����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);


	// PA0 to TIM5 CH1
	// PA1 to TIM5 CH2
	/*
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	*/

	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA , &GPIO_InitStructure);

	TIM_DeInit(TIM5);

	//TIM4 clock source 72MHz
	// CK_CNT = f_CK_PSC/(Prescaler + 1)
	// feq = TIMCLK/(Period + 1)
	TIM_TimeBaseStructure.TIM_Prescaler = 12 -1;	// 6MHz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 300-1;	// 20kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;		  
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;	// only for TIM1 and TIM8
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	// Set channel
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			//CCMR1 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;								   
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	
	TIM_OC1Init(TIM5,&TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_Pulse = 0 ; 
	TIM_OC2Init(TIM5,&TIM_OCInitStructure);

	
	// Enable TIM5
	TIM_Cmd(TIM5, ENABLE);
	// Enable PWM output
	TIM_CtrlPWMOutputs(TIM5, ENABLE);	

#ifdef CONFIG_DEBUG
	printf("Initializing TIM5 ... Done\n");
	DebugPrintf();
#endif
	
}	

/***************Configuration of TIM8****************************************************/
//���ö�ʱ��8Ϊ��ͨ��PWM����
void TIM8_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

 	//ʹ��APB2����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	/* PC6 7 8 9����Ϊ���ܽ�*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	TIM_DeInit(TIM8);			//������TIM8�Ĵ�������Ϊȱʡֵ

	//TIM8�������������ã�����PWMƵ�ʣ�����20ms	 
	// TIM8��ʱ��ԴΪ72M	  ����AHBʱ��2��Ƶ��2��Ƶ����ʱ����Ƶ�����õ�
	//������ʱ��Ƶ�ʣ�CK_CNT = f_CK_PSC/(Prescaler + 1)=1M;
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;	   //������������ΪTIMʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = 65535; 	//5m, 14.7ms, 29412; // 10m, 29.4ms //65535;		 	//����������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ,���ɲ�����0��65535
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;		  
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;	 //�˲�������TIM1��TIM8����
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

	//����ͨ��1Ϊ�����벶��ģʽ
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;//ͨ��ѡ��
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//�����ز���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//CCMR->CCxS�ܽ���Ĵ�����Ӧ��ϵ
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//��Ƶ��
	TIM_ICInitStructure.TIM_ICFilter = 0x4;        //�˲����ã������������������϶������ȶ�0x0��0xF
	TIM_ICInit(TIM8, &TIM_ICInitStructure);        //��ʼ��
	
 	//����ͨ��2Ϊ�����벶��ģʽ
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;//ͨ��ѡ��
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//�����ز���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//CCMR->CCxS�ܽ���Ĵ�����Ӧ��ϵ
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//��Ƶ��
	TIM_ICInitStructure.TIM_ICFilter = 0x4;        //�˲����ã������������������϶������ȶ�0x0��0xF
	TIM_ICInit(TIM8, &TIM_ICInitStructure);        //��ʼ��

 	//����ͨ��3Ϊ�����벶��ģʽ
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;//ͨ��ѡ�
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//�����ز���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//CCMR->CCxS�ܽ���Ĵ�����Ӧ��ϵ
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//��Ƶ��
	TIM_ICInitStructure.TIM_ICFilter = 0x4;        //�˲����ã������������������϶������ȶ�0x0��0xF�
	TIM_ICInit(TIM8, &TIM_ICInitStructure);        //��ʼ��

 	//����ͨ��4Ϊ�����벶��ģʽ
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;//ͨ��ѡ��
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//�����ز���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//CCMR->CCxS�ܽ���Ĵ�����Ӧ��ϵ
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//��Ƶ��
	TIM_ICInitStructure.TIM_ICFilter = 0x4;        //�˲����ã������������������϶������ȶ�0x0��0xF
	TIM_ICInit(TIM8, &TIM_ICInitStructure);        //��ʼ��


	TIM_SelectInputTrigger(TIM8, TIM_TS_TI2FP2);   //SMCR->TS��101ѡ��ʱ�Ӵ���Դ���˲���Ķ�ʱ������1
	TIM_SelectSlaveMode(TIM8, TIM_SlaveMode_Trigger);//SMCR SMS 100 ������ʽ  ѡ��TIMx��ģʽ,ѡ�д����źţ�TRGI�����������س�ʼ���������������Ĵ����ĸ���
	TIM_SelectMasterSlaveMode(TIM8, TIM_MasterSlaveMode_Enable); //������ʱ���ı�������
	
	TIM_ITConfig(TIM8, TIM_IT_CC1, ENABLE);        //��ͨ��1�����ж�
	TIM_ITConfig(TIM8, TIM_IT_CC2, ENABLE);        //��ͨ��2�����ж�
 	TIM_ITConfig(TIM8, TIM_IT_CC3, ENABLE);        //��ͨ��3�����ж�
	TIM_ITConfig(TIM8, TIM_IT_CC4, ENABLE);        //��ͨ��4�����ж�

	//TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE); 	   //��TIM8����ж�

	//TIM_SetCounter(TIM8, 0);
	TIM_Cmd(TIM8, ENABLE);                         //����TIM8

#ifdef CONFIG_DEBUG
	printf("Initializing TIM8 ... Done\n");
	DebugPrintf();
#endif
	
}


