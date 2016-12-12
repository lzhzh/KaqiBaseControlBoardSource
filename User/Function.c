#include "Configuration.h"

void delayus( vu32 count )
{
	vu32 i = 0;
	for(;count != 0;count--)
	{
		for(i = 35; i != 0; i--)
		{
		}
	}
}

/*************************************************************************************
��������Delay1
�������������ӳ�ʱ�� (����ȷ��
���롡��nCount:�ƶ��ӳ�ʱ�䳤��	  0.1ms	  STM32ִ��һ��ָ���ԼΪ��ʮns
���������
���ء�����
*************************************************************************************/
void delayms(vu32 nCount)
{
	int i;
	for(;nCount != 0;nCount--)
	{
		for(i=0;i<1500;i++)
		{
		}
	}	
}

/*************************************************************************************
��������Delay2
�������������ӳ�ʱ�� (��ȷ�� ʹ��SysTick��
	���ڲ�ʹ��SysTick�жϹ���ʱ�������������ȷ��ʱ��
���롡��nCount:�ƶ��ӳ�ʱ�䳤��	  ms
���������
���ء�����
ע	����ע������Ϊ��1.0�ں������ڿ�3.0��û�У�����ֱ�Ӳ����Ĵ���
*************************************************************************************/
void delayms2(u32 nTime)
{
	//SysTick_CounterCmd(SysTick_Counter_Enable);		 // ʹ��SysTick������
//	SysTick->VAL = SysTick_Counter_Clear;		 // ���SysTick������
	//SysTick->CTRL |= SysTick_Counter_Enable;	 // ʹ��SysTick������		
	TimingDelay = nTime; 				// ��ȡ��ʱʱ��
	while(TimingDelay != 0); 			// �ж���ʱ�Ƿ����
	//SysTick_CounterCmd(SysTick_Counter_Disable); 	// �ر�SysTick������
	//SysTick->CTRL &= SysTick_Counter_Disable;		//�ر�SysTick������
	//SysTick_CounterCmd(SysTick_Counter_Clear); 		// ���SysTick������
//	SysTick->VAL = SysTick_Counter_Clear;		 // ���SysTick������
}

/******************************************************************************* 
* Function Name  : GetTemp 
* Description    : ����ADC��������¶� 
* Input          : u16 advalue 
* Output         : 
* Return         : u16 power, 0.01V
*******************************************************************************/ 
u16 GetPower(u16 advalue) 
{ 
	//return (U16)(advalue/0.95); 
	return (U16)(advalue); 
} 

/******************************************************************************* 
* Function Name  : u16 GetTemp(u16 advalue) 
* Description    : ����ADC������㹩���ѹ 
* Input          : u16 advalue 
* Output         : 
* Return         : u16 temp 
*******************************************************************************/ 
u16 GetTemp(u16 advalue) 
{ 
	u32 Vtemp_sensor; 
	s32 Current_Temp; 	  //signed int
	
	//    ADCת�������Ժ󣬶�ȡADC_DR�Ĵ����еĽ����ת���¶�ֵ���㹫ʽ���£� 
	//          V25 - VSENSE 
	//  T(��) = ------------  + 25 
	//           Avg_Slope 
	//   V25��  �¶ȴ�������25��ʱ �������ѹ������ֵ1.43 V�� 
	//  VSENSE���¶ȴ������ĵ�ǰ�����ѹ����ADC_DR �Ĵ����еĽ��ADC_ConvertedValue֮���ת����ϵΪ�� 
	//            ADC_ConvertedValue * Vdd 
	//  VSENSE = -------------------------- 
	//            Vdd_convert_value(0xFFF) 
	//  Avg_Slope���¶ȴ����������ѹ���¶ȵĹ�������������ֵ4.3 mV/�档 

	Vtemp_sensor = advalue * 330 / 4095;
	Current_Temp = (s32)(143 - Vtemp_sensor)*1000/43 + 2500;

	//   Current_Temp = (s32)((1.42 - advalue*3.3/4095)*1000/4.35) + 25; 
	return (s16)Current_Temp; 
} 

/*************************************************************************************
��������fputc
����������C����printf�����ض���USART
���롡����
���������
���ء�����
*************************************************************************************/

int fputc(int ch, FILE *f)
{
	//�ڴ�ʵ���Լ���Ҫ��fputc
	//����USART����һ���ַ���Ϊfputc
	/*USART_SendData(USART1, (u8)ch);
	//�ȴ��������
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{
		;
	}
	return ch;
	*/

	if( PrintfCn < PrintfBuf_Len )
	{
   		PrintfBuf[PrintfCn] = ch;
		PrintfCn++;
	}
	
	return ch;
}

/*************************************************************************************
��������average()
��������ȥ��ֵ��ֵ�˲�
���롡�����С����и�������ʼλ��
���������ֵ
���ء�����
*************************************************************************************/
u16 average(u16 *ADdata, u8 Num, u8 ADi)
{
	u16 max=0;
	u16 min=0xffff;
	u32 Sum=0;
	for(;ADi<Num*2;ADi=ADi+2)
	{
		Sum += ADdata[ADi];
		if(ADdata[ADi] > max)
		{
			max = ADdata[ADi];
		}	
		if(ADdata[ADi] < min)
		{
			min = ADdata[ADi];
		}
	}
	return (u16)((Sum-max-min)/(Num-2));

}

/*************************************************************************************
��������average2()
��������ȥ��ֵ��ֵ�˲�,ȡ�м�ֵ��3/5)���ֵ
���롡�����С����и�������ʼλ��
���������ֵ
���ء�����
*************************************************************************************/
u16 average2(u16 *ADdata, u8 Num, u8 ADi)
{
	u16 tempdata[50];
	u16 temp;
	u8 ti,tj;
	u32 Sum = 0;
	u8 cutnum;
	for(tj=0,ti=ADi;ti<Num*2;ti=ti+2,tj++)
	{
		tempdata[tj] = ADdata[ti];
	}
	for(ti=0;ti<Num;ti++)
	{
		for(tj=0;tj<Num-ti-1;tj++)
		{
			if(tempdata[tj] > tempdata[tj+1])
			{
				temp = tempdata[tj];
				tempdata[tj] = tempdata[tj+1];
				tempdata[tj+1] = temp;
			}
		}
	}
	if(Num > 5)
	{
		cutnum = Num/5;
	}
	for(ti=cutnum;ti<Num-cutnum;ti++)
	{
		Sum +=tempdata[ti];
	}
	return (u16)(Sum/(Num-2*cutnum));

}

float s16s8toFloat( S16 number, S8 evalue)
{
	float f = number;
	
	if( number == 0 )
		return 0;
	
	else if( evalue > 0 )
	{
		for( ; evalue > 0; evalue--)
			f = f/10.0;
	}

	return f;
}

void floatToBytes4( float *f, U8* b0, U8* b1, U8* b2, U8* b3 )
{
	FLOAT4 data;
	data.bytes[0] = *b0;
	data.bytes[1] = *b1;
	data.bytes[2] = *b2;
	data.bytes[3] = *b3;
	*f = data.f;
}

float bytes4ToFloat( U8 b0, U8 b1, U8 b2, U8 b3)
{
	FLOAT4 data;
	data.bytes[0] = b0;
	data.bytes[1] = b1;
	data.bytes[2] = b2;
	data.bytes[3] = b3;

	return data.f;
}


void U16ToChar(uint16 data, char * pk_ch)
{
/*
	uint8 i;
	for(i = 0; i < 5; i ++)
	{
		* (pk_ch + i) = data % 10 + '0';
		data = data / 10;
	}
*/
	* (pk_ch + 0) = data % 10 + '0';
	data = data / 10;
	* (pk_ch + 1) = data % 10 + '0';
	data = data / 10;
	* (pk_ch + 2) = data % 10 + '0';
	data = data / 10;
	* (pk_ch + 3) = data % 10 + '0';
	data = data / 10;
	* (pk_ch + 4) = data % 10 + '0';

}

void U8ToChar(uint8 data, char * pk_ch)
{
	* (pk_ch + 0) = data % 10 + '0';
	data = data / 10;
	* (pk_ch + 1) = data % 10 + '0';
}

