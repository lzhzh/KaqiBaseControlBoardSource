#include "Configuration.h"

/**************************************************************************************
��������EXTI0_IRQHandler
������������EXTI0�жϵķ����ӳ���
���롡����
���������
���ء�����
**************************************************************************************/
void EXTI0_IRQHandler(void)
{
	// Read encoder and mpu6000 measurement
	MPU6000_ReadOnlyGZ();
	//���EXTI Line0����λ
	EXTI_ClearITPendingBit(EXTI_Line0);

}


/**************************************************************************************
��������EXTI1_IRQHandler
������������EXTI1�жϵķ����ӳ���
���롡����
���������
���ء�����
**************************************************************************************/
void EXTI1_IRQHandler(void)
{
	printf("ET1_IRQ...\r\n");
	DebugPrintfDMA();
	//���EXTI Line1����λ
	EXTI_ClearITPendingBit(EXTI_Line1);
}

/**************************************************************************************
��������EXTI2_IRQHandler
������������EXTI2�жϵķ����ӳ���
���롡����
���������
���ء�����
**************************************************************************************/
void EXTI2_IRQHandler(void)
{
	
	printf("ET2_IRQ...\r\n");
	DebugPrintfDMA();
	//���EXTI Line2����λ
	EXTI_ClearITPendingBit(EXTI_Line2);
}

/**************************************************************************************
��������EXTI3_IRQHandler
������������EXTI3�жϵķ����ӳ���
���롡����
���������
���ء�����
**************************************************************************************/
void EXTI3_IRQHandler(void)
{
	printf("ET3_IRQ...\r\n");
	DebugPrintfDMA();
	//���EXTI Line3����λ
	EXTI_ClearITPendingBit(EXTI_Line3);
}

/**************************************************************************************
��������EXTI4_IRQHandler
������������EXTI4�жϵķ����ӳ���
���롡����
���������
���ء�����
**************************************************************************************/
void EXTI4_IRQHandler(void)
{
	printf("ET4_IRQ...\r\n");
	DebugPrintfDMA();
	//���EXTI Line4����λ
	EXTI_ClearITPendingBit(EXTI_Line4);
}

/**************************************************************************************
��������EXTI15_10_IRQHandler
������������EXTI0�жϵķ����ӳ���
���롡����
���������
���ء�����
**************************************************************************************/
void EXTI15_10_IRQHandler(void)
{
	//U8 tmpU8 = 1;

	/*
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)
	{
		printf("ET10_IRQ...\r\n");
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	else if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
		printf("ET11_IRQ...\r\n");
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	else if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
		printf("ET12_IRQ...\r\n");
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	else if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
		printf("ET13_IRQ...\r\n");
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	else if(EXTI_GetITStatus(EXTI_Line14) != RESET)
	{
		printf("ET14_IRQ...\r\n");
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	else if(EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
		printf("ET15_IRQ...\r\n");
		EXTI_ClearITPendingBit(EXTI_Line15);
	} 
	*/

	if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
		printf("ET13_IRQ...\r\n");
		paramsReset(1);
		EXTI_ClearITPendingBit(EXTI_Line13);
	}

	DebugPrintfDMA();

}

/**************************************************************************************
��������EXTI9_5_IRQHandler
������������EXTI0�жϵķ����ӳ���
���롡����
���������
���ء�����
**************************************************************************************/
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		printf("ET5_IRQ...\r\n");
	 	EXTI_ClearITPendingBit(EXTI_Line5);
	}
	else if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		printf("ET6_IRQ...\r\n");
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	else if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
		printf("ET7_IRQ...\r\n");
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	else if(EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		printf("ET8_IRQ...\r\n");
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	else if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		printf("ET9_IRQ...\r\n");
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	DebugPrintfDMA();
}


