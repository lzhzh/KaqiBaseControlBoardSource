#include "Configuration.h"


/**************************************************************************************
��������SysTick_Handler
������������SysTick�ж�
���롡����
���������
���ء�����
**************************************************************************************/
void SysTick_Handler(void)
{
	if(TimingDelay != 0)	//��ȷ��ʱ
		TimingDelay --;
	
	Time++;
	if(Time >= 1000)
	{
	 	Time = 0;
		Second ++;
	}

	//if( Time % controller_cycle == 7 )
	//	is_control_motor = TRUE;
	

	if(Time % 100 == 33)
		IsU1Send = TRUE;


	if(Time % 100 == 43)
		IsU2Send = TRUE;

	

	//ϵͳ����LED��˸
	if(Time == 500)
	{
		LED1_ON;
		LED2_ON;
	}
	else if(Time == 800)
	{
		LED1_OFF;
		LED2_OFF;
	}

}
