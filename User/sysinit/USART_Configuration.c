#include "Configuration.h"


/***************Configuration of USART1************************************************/
/**************************************************************************************
��������USART1_Configuration
������������USART1
���롡����
���������
���ء�����
**************************************************************************************/
void USART1_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //����USART1ʱ��

	//����PA9��ΪUSART1��Tx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA , &GPIO_InitStructure);

	//����PA10��ΪUSART1��Rx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA , &GPIO_InitStructure);

	//����USART1
	USART_InitStructure.USART_BaudRate = 230400; //115200, 230400;	//������115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 	//8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//��֡��β����1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;				//������żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 	//���͡�����ʹ��
	//����USART1ʱ��
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;		//ʱ�ӵ͵�ƽ�
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;		//SLCK������ʱ������ļ���->�͵�ƽ
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;		//ʱ�ӵڶ������ؽ������ݲ���
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //���һλ���ݵ�ʱ�����岻��SCLK���

	USART_Init(USART1, &USART_InitStructure);
	USART_ClockInit(USART1, &USART_ClockInitStruct);

	/* Enable USART1 DMA TX request */
  	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

	//ʹ��USART1�����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//ʹ��USART1
	USART_Cmd(USART1, ENABLE);	   

#ifdef CONFIG_DEBUG
	printf("Initializing USART1 ... Done.\r\n");
	DebugPrintf();
#endif

}


/**************************************************************************************
��������USART1_SendData
��������USART1���ͺ���
���롡����
���������
���ء�����
**************************************************************************************/
void USART1_SendData(uint16_t Data)
{ 
	USART1->DR = (Data & (uint16_t)0x01FF);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/***************Configuration of USART2************************************************/
/**************************************************************************************
��������USART2_Configuration
������������USART2
���롡����
���������
���ء�����
**************************************************************************************/
void USART2_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //����USART2ʱ��

	//����PA2��ΪUSART2��Tx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//����PA3��ΪUSART2��Rx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//����USART2
	USART_InitStructure.USART_BaudRate = 115200;		  			//������115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 	//8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//��֡��β����1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;				//������żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 	//���͡�����ʹ��
	//����USART2ʱ��
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;		//ʱ�ӵ͵�ƽ�
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;		//SLCK������ʱ������ļ���->�͵�ƽ
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;		//ʱ�ӵڶ������ؽ������ݲ���
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //���һλ���ݵ�ʱ�����岻��SCLK���

	USART_Init(USART2, &USART_InitStructure);
	USART_ClockInit(USART2, &USART_ClockInitStruct);

	/* Enable USART2 DMA TX request */
  	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	
	//ʹ��USART2�����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//ʹ��USART2
	USART_Cmd(USART2, ENABLE);	   

#ifdef CONFIG_DEBUG
	printf("Initializing USART2 ... Done.\r\n");
	DebugPrintf();
#endif
	
}

/**************************************************************************************
��������USART2_SendData
��������USART2���ͺ���
���롡����
���������
���ء�����
**************************************************************************************/
void USART2_SendData(uint16_t Data)
{ 
	USART2->DR = (Data & (uint16_t)0x01FF);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}


/***************Configuration of USART3************************************************/
/**************************************************************************************
��������USART3_Configuration
������������USART3
���롡����
���������
���ء�����
**************************************************************************************/
void USART3_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //����USART3ʱ��

	//����PB10��ΪUSART3��Tx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB , &GPIO_InitStructure);

	//����PB11��ΪUSART3��Rx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB , &GPIO_InitStructure);

	//����USART3
	USART_InitStructure.USART_BaudRate = 115200;						//������9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 	//8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//��֡��β����1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;				//������żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 	//���͡�����ʹ��
	//����USART3ʱ��
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;		//ʱ�ӵ͵�ƽ�
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;		//SLCK������ʱ������ļ���->�͵�ƽ
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;		//ʱ�ӵڶ������ؽ������ݲ���
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //���һλ���ݵ�ʱ�����岻��SCLK���

	USART_Init(USART3, &USART_InitStructure);
	USART_ClockInit(USART3, &USART_ClockInitStruct);

	/* Enable USART3 DMA TX request */
  	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);

	//ʹ��USART3�����ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	//ʹ��USART3
	USART_Cmd(USART3, ENABLE);	   

#ifdef CONFIG_DEBUG
	printf("Initializing USART3 ... Done\n");
	DebugPrintf();
#endif

}

/**************************************************************************************
��������USART3_SendData
��������USART3���ͺ���
���롡����
���������
���ء�����
**************************************************************************************/
void USART3_SendData(uint16_t Data)
{ 
	USART3->DR = (Data & (uint16_t)0x01FF);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}


