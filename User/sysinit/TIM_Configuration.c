#include "Configuration.h"

/***************Configuration of TIM1****************************************************/
//TIM1_CC3事件用于触发ADC1_2的规则输入通道转换
/**************************************************************************************
函数名：TIM1_Configuration
描述　：配置定时器
输入　：无
输出　：无
返回　：无
**************************************************************************************/
void TIM1_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//启动TIM1时钟
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,  ENABLE);	//TIM1复用功能完全映射
	
	TIM_DeInit(TIM1);			//将外设TIM8寄存器重设为缺省值

	//TIM1基本计数器设置（设置PWM频率）周期20ms	 
	// TIM1的时钟源为72M	  其由AHB时钟2分频再2倍频（定时器倍频器）得到
	//计数器时钟频率：CK_CNT = f_CK_PSC/(Prescaler + 1);
	//PWM频率:f=TIMCLK/(TIM_Period + 1)
	TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1;	   // 2khz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_Period = 400-1;		 // 5hz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;		  
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;	 //此参数仅对TIM1和TIM8有用
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	//设置通道PWM模式及占空比
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				 //CCMR1 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		 //OC1信号输出到对应的输出引脚 CCER
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;	 //？？？保留位CCER
	TIM_OCInitStructure.TIM_Pulse = 50 ;								   
	//TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	   //OC1低电平有效　CCER
	//TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;	   //？？？保留位CCER
	//TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;		//???保留位CR2	 
	//TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	//CR2

//	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
//	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_OC1Ref);

	TIM_OCInitStructure.TIM_Pulse = 200;
	TIM_OC3Init(TIM1,&TIM_OCInitStructure);
	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_OC3Ref);
	

	TIM_Cmd(TIM1, ENABLE);                         //启动TIM1
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

	//设置通道PWM模式及占空比
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
	
	//允许TIM3
	TIM_Cmd(TIM3, ENABLE);
	//主输出允许
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

 	//使能APB1外设时钟
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
//设置定时器8为四通道PWM输入
void TIM8_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

 	//使能APB2外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	/* PC6 7 8 9设置为功能脚*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	TIM_DeInit(TIM8);			//将外设TIM8寄存器重设为缺省值

	//TIM8基本计数器设置（设置PWM频率）周期20ms	 
	// TIM8的时钟源为72M	  其由AHB时钟2分频再2倍频（定时器倍频器）得到
	//计数器时钟频率：CK_CNT = f_CK_PSC/(Prescaler + 1)=1M;
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;	   //设置了用来作为TIM时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_Period = 65535; 	//5m, 14.7ms, 29412; // 10m, 29.4ms //65535;		 	//设置了在下一个更新事件装入活动的自动重装载寄存器周期的值,即可测周期0－65535
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;		  
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;	 //此参数仅对TIM1和TIM8有用
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

	//设置通道1为：输入捕获模式
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;//通道选择
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//CCMR->CCxS管脚与寄存器对应关系
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//分频器
	TIM_ICInitStructure.TIM_ICFilter = 0x4;        //滤波设置，经历几个周期跳变认定波形稳定0x0～0xF
	TIM_ICInit(TIM8, &TIM_ICInitStructure);        //初始化
	
 	//设置通道2为：输入捕获模式
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;//通道选择
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//CCMR->CCxS管脚与寄存器对应关系
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//分频器
	TIM_ICInitStructure.TIM_ICFilter = 0x4;        //滤波设置，经历几个周期跳变认定波形稳定0x0～0xF
	TIM_ICInit(TIM8, &TIM_ICInitStructure);        //初始化

 	//设置通道3为：输入捕获模式
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;//通道选�
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//CCMR->CCxS管脚与寄存器对应关系
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//分频器
	TIM_ICInitStructure.TIM_ICFilter = 0x4;        //滤波设置，经历几个周期跳变认定波形稳定0x0～0xF�
	TIM_ICInit(TIM8, &TIM_ICInitStructure);        //初始化

 	//设置通道4为：输入捕获模式
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;//通道选择
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//CCMR->CCxS管脚与寄存器对应关系
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//分频器
	TIM_ICInitStructure.TIM_ICFilter = 0x4;        //滤波设置，经历几个周期跳变认定波形稳定0x0～0xF
	TIM_ICInit(TIM8, &TIM_ICInitStructure);        //初始化


	TIM_SelectInputTrigger(TIM8, TIM_TS_TI2FP2);   //SMCR->TS　101选择时钟触发源：滤波后的定时器输入1
	TIM_SelectSlaveMode(TIM8, TIM_SlaveMode_Trigger);//SMCR SMS 100 触发方式  选择TIMx从模式,选中触发信号（TRGI）的上升沿重初始化计数器并触发寄存器的更新
	TIM_SelectMasterSlaveMode(TIM8, TIM_MasterSlaveMode_Enable); //启动定时器的被动触发
	
	TIM_ITConfig(TIM8, TIM_IT_CC1, ENABLE);        //打开通道1捕获中断
	TIM_ITConfig(TIM8, TIM_IT_CC2, ENABLE);        //打开通道2捕获中断
 	TIM_ITConfig(TIM8, TIM_IT_CC3, ENABLE);        //打开通道3捕获中断
	TIM_ITConfig(TIM8, TIM_IT_CC4, ENABLE);        //打开通道4捕获中断

	//TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE); 	   //打开TIM8溢出中断

	//TIM_SetCounter(TIM8, 0);
	TIM_Cmd(TIM8, ENABLE);                         //启动TIM8

#ifdef CONFIG_DEBUG
	printf("Initializing TIM8 ... Done\n");
	DebugPrintf();
#endif
	
}


