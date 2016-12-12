#include "Configuration.h"


main()
{	
	U8 byte = 0;
	//
	InitDevice();
	printf("/***************************************/\r\n");
	printf("RCT6 Robot Control Board Simple Version, v1.0.\r\n");
	printf("/***************************************/\r\n");
	DebugPrintf();

	while(1)
	{

		// Process gyroscope z-axis and encoder measurement
		if( is_imu_update == TRUE )
		{
			Motor_Control();
			is_imu_update = FALSE;
		}
	
		if(ADC1_2_OK == TRUE)	//ADC1_2 ����������£�Ƶ��50Hz
		{
			ADC1_2_OK = FALSE;	//�����־λ
			InternalTemp = GetTemp(ADC_ConvertedValue[ADC_CHANNEL_TEMP]);	//����ADC��������¶�
			Power = GetPower(ADC_ConvertedValue[ADC_CHANNEL_1V2]);

			// Send battery state
			SendBatteryState( (U16)ADC_ConvertedValue[ADC_CHANNEL_10] );
			//
			SendBreakerStatus( PB8_VALUE(), PB9_VALUE() ); // HIGH: 0, LOW 1
			
			//printf("Voltage: %d \r\n", (U16)ADC_ConvertedValue[ADC_CHANNEL_10] );
			//DebugPrintf();
			/*
			printf("\r\n Temp:%d\t%d", ADC_ConvertedValue[ADC_CHANNEL_TEMP], GetTemp(ADC_ConvertedValue[ADC_CHANNEL_TEMP]) );
			printf(" 1V2 Ref: %d,\t%f", ADC_ConvertedValue[ADC_CHANNEL_1V2], ADC_ConvertedValue[ADC_CHANNEL_1V2] * 3.31 / 4095 );
			printf("\r\n 10 = %d,\t11 = %d", ADC_ConvertedValue[ADC_CHANNEL_10], ADC_ConvertedValue[ADC_CHANNEL_11]);
			DebugPrintf();
			*/
		}

		// Base command
		//calBaseBuf();
		if( outBaseBuf(&byte) )
			ReceiveBaseCommand(byte);
		
		// UART2�����ֽ�
		if(IsU2RecOK == TRUE)
		{
			UART2_CAL();
			IsU2RecOK = FALSE;
		}

		// UART2����֡
		if(IsU2Send == TRUE)
		{
			OutU2FrameBuf();
			IsU2Send = FALSE;
		}


	}				    
	
}

