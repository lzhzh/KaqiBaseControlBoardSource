#include "Configuration.h"


/**************************************************************************************
��������USART1_IRQHandler
������������USART1_IRQHandler�жϵķ����ӳ���
**************************************************************************************/
void USART1_IRQHandler(void)  
{
	U8 t = 0;    	
//	static U8 FLen = 10;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		t = USART_ReceiveData(USART1);
		inBaseBuf( t );

		/*
		//�ӽ��ջ������ж�ȡ����
		//USART1_SendByte(t);
		if(U1BufP < 4)
		{
			if(t == DH0 && U1BufP == H0)
			{
				U1Buf[U1BufP] = t;
				U1BufP++;
			}
			else if(t == DH1 && U1BufP == H1)
			{
				U1Buf[U1BufP] = t;
				U1BufP++;
			}
			else if(U1BufP == Dest) 
			{

				U1Buf[U1BufP] = t;
				U1BufP++;
			}
			else if( t <= 64 && U1BufP == Dnum)
			{
				FLen = t + 6;
				U1Buf[U1BufP] = t;
				U1BufP++;
			}
			else
			{
				U1BufP = 0;
			}
		}
		else if(U1BufP < FLen)		//�������ݺ�У��ֵ
		{
			U1Buf[U1BufP] = t;
			U1BufP++;
			
			if(U1BufP >= FLen)
			{
			//����һ֡
				if (U1RecBufP == 0)
				{
					for (t = 0; t < U1BufP; t++)
					{
						U1RecBuf[t] = U1Buf[t];
					//	USART1_SendData(U0Buf[t]);
					}
					
					U1RecBufP = U1BufP;
					U1BufP = 0;
					IsU1RecOK = TRUE;
				//	LED2_TOGGLE;
				}
				else
				{
					//������һ֡��û�д����꣬������֡
					U1BufP = 0;
				}
			}
		}*/
		
	}
	
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	
}

//UART1֡���������λ��
U8 UART1_CAL(void)
{
//	U8 i;
//	U8 TempU8=0;
//	S8 TempS8 = 0;
	U16 TempU16=0;
//	S16 TempS16=0;
//	S32 TempS32=0;

	U8 DLen;//���ݳ���
//	U8 Chk;	//У����λ��
	U8 DestU8;//��ַ

	DLen = U1RecBuf[Dnum];
/*	Chk = DLen + 5;
	   
	TempU8 = 0;
	for (i = 0; i < Chk; i++)
	       TempU8 = TempU8 + U1RecBuf[i];

	if (TempU8 != U1RecBuf[Chk])
	{  			  	  
		  ////У��ʹ��󣬽�֡�����������
		  U1RecBufP = 0;
		  return FALSE;
	}
*/

	DestU8 = U1RecBuf[Dest] & 0x0F;
	if((DestU8 == LOCAL_IP)||(DestU8 == ALL_IP))
	{
		//�ж��Ƿ��͵��ñ�ſ�����������,�����ǹ㲥����
		DestU8 = (U1RecBuf[Dest] >> 4) & 0x0F;
		if((DestU8 == PC_IP) || (DestU8== ALL_IP))  
		{  
			switch(U1RecBuf[CMD])
			{
				case	't':	// t = 0x74
				case	'T':	// T = 0x54
						//��������
						if(DLen < 2)	//����2 ���ֽڳ���
						{
							break;
						}
						//�ظ�״̬
						tU1SendBuf[Dest]  = ALL_IP << 4 | LOCAL_IP;  //Դ/Ŀ���ַ����
						tU1SendBuf[Dnum]  = 0x02;  				//�������ݸ���
						tU1SendBuf[CMD]  = 'T';					//����ID
						tU1SendBuf[D0] = 0x55;
						tU1SendBuf[D1] = 0xAA;
						InU1SendBuf();
						break;

				case	'a':	// a = 0x61
				case	'A':	// A = 0x41
						//��������
						if(DLen < 2)	//����2 ���ֽڳ���
						{
							break;
						}
						TempU16 = (U1RecBuf[D0] << 8) + U1RecBuf[D1];

						if( TempU16 > 300 )
							TempU16 = 300;
						if( TempU16 < 30 )
							TempU16 = 0;
						PWML_Output(TempU16);
						printf("\r\nSet PWM A to %d", TempU16);
						DebugPrintfDMA();
						
						break;

				case	'b':	// b = 0x62
				case	'B':	// B = 0x42
						//��������
						if(DLen < 2)	//����2 ���ֽڳ���
						{
							break;
						}
						TempU16 = (U1RecBuf[D0] << 8) + U1RecBuf[D1];

						if( TempU16 > 300 )
							TempU16 = 300;
						if( TempU16 < 30 )
							TempU16 = 0;

						PWMR_Output(TempU16);
						printf("\r\nSet PWM B to %d", TempU16);
						DebugPrintfDMA();
						
						break;


				case	'd':	// d = 0x64
				case	'D':	// D = 0x44
						//��������
						if(DLen < 2)	//����2 ���ֽڳ���
						{
							break;
						}
						
						if( U1RecBuf[D0] == 0 )
						{
							MOTOR_LEFT_STOP;
							MOTOR_RIGHT_STOP;
							printf("\r\nMOTOR A&B STOP");
						}
						else if( U1RecBuf[D0] == 1 )
						{
							MOTOR_LEFT_FORWARD;
							MOTOR_RIGHT_FORWARD;
							printf("\r\nMOTOR A&B FORWARD");
						}
						else if( U1RecBuf[D0] == 2 )
						{
							MOTOR_LEFT_BACKWARD;
							MOTOR_RIGHT_BACKWARD;
							printf("\r\nMOTOR A&B BACKWARD");
						}
						DebugPrintfDMA();
						
						break;

				default:

						break;	   
			}    
			
		}
		
	}
			
	//��֡�����������
	U1RecBufP = 0;
	return TRUE;
}


// 2�����͵�Э���建������ FrameBuf            ��ʱ��ָ���
// tU1SendBuf --> U1SendBuf
U8 InU1FrameBuf(void)
{
	U8 i=0;
	U8 TempU8=0; 
	U8   Chk=0;

	//����Э�����ӵ�����
	tU1SendBuf[H0] =  DH0;
	tU1SendBuf[H1] =  DH1;


	Chk = tU1SendBuf[Dnum] + 5;

	for(i = 0; i< Chk;i ++)
		TempU8 += tU1SendBuf[i]; 

	tU1SendBuf[Chk] = TempU8;
	//�峤�� Chk+1 
	memcpy(U1FrameBuf[U1FrameBufPi], tU1SendBuf, Chk+1);  //ע�� ������    
	U1FrameBufPi=(U1FrameBufPi+1)%U1_FRAMEBUF_LEN;  //ָ����λ    

	return TRUE;
}

// 3����Э���建���������ݷ��͵����ڷ��ͻ�����  U3SendBuf    ��ʱ
// FrameBuf --> U3SendBuf
U8 OutU1FrameBuf(void)
{
    
    U8   Chk=0;
    U8   i=0;
   
	if(U1FrameBufPo != U1FrameBufPi) //Э���建��������
	{

		Chk = U1FrameBuf[U1FrameBufPo][Dnum] + 5;
		 //�峤�� Chk+1 
		for(i=0;i<Chk+1;i++)
		{
			U1SendBuf[U1SendBufPi] = U1FrameBuf[U1FrameBufPo][i];
			U1SendBufPi = (U1SendBufPi +1)%USART1_BUF;  //���ָ����λ 
		}		
	       U1FrameBufPo=(U1FrameBufPo+1)%U1_FRAMEBUF_LEN;  //ָ����λ      

		if(IsU1TxEmpty == 1)
		{
			DMA1_Channel4_IRQHandler();	
		}
			
//		USART2->CR1 |= 0x80;  // ͬʱ����THRE�ж�,�����Ϳ��ж�
	}	
	else 	//��
	{
		return FALSE;
	}
	
    return TRUE;
}


//ֱ�����뷢�ͻ��壬��ǰ֡���ͽ�������������
U8 InU1SendBuf(void)
 {
 
	U8 i=0;
	U8 TempU8=0; 
	U8   Chk=0;

	//����Э�����ӵ�����
	tU1SendBuf[H0] =  DH0;
	tU1SendBuf[H1] =  DH1;

	Chk = tU1SendBuf[Dnum] + 5;

	for(i=0;i<Chk;i++)
		TempU8 += tU1SendBuf[i]; 

	tU1SendBuf[Chk] = TempU8;
	//�峤�� Chk+1 
	for(i=0;i<Chk+1;i++)			
	{
		U1SendBuf[U1SendBufPi] = tU1SendBuf[i];
		U1SendBufPi = (U1SendBufPi +1)%USART1_BUF;  //���ָ����λ 
	}	
	if(IsU1TxEmpty == 1)
	{
		DMA1_Channel4_IRQHandler();	
	}		
	//USART2->CR1 |= 0x80;  // ͬʱ����THRE�ж�,�����Ϳ��ж�

	return TRUE;
 }



//USART1 ���printf
void U1Printf(void)
{
	U16 i=0;
	
	for(i = 0; i < PrintfCn; i++)			
	{
		U1SendBuf[U1SendBufPi] = PrintfBuf[i];
		U1SendBufPi = (U1SendBufPi +1) % USART1_BUF;  //���ָ����λ 
	}	

	PrintfCn = 0;
	
	if(IsU1TxEmpty == 1)
	{
		DMA1_Channel4_IRQHandler();	
	}

}

void inBaseBuf( U8 byte )
{
	BaseBuf[BaseBufPi] = byte;
	BaseBufPi = (BaseBufPi + 1) % BASE_BUF_LEN;
}

U8 outBaseBuf( U8* byte )
{
	if( BaseBufPi == BaseBufPo )
		return FALSE;

	*byte = BaseBuf[BaseBufPo];
	BaseBufPo = (BaseBufPo + 1) % BASE_BUF_LEN;
	return TRUE;
}

void calBaseBuf(void)
{
	U8 byte = 0;
	
	while( outBaseBuf(&byte) )
	{
		ReceiveBaseCommand(byte);
	}
}

// 9 bytes, # E C LM LS RM RS SUM \n
void SendEncoders( U16 left, U16 right)
{
	U8 i=0;
	U8 TempU8=0; 

	//simulateEncoder( &left, &right );

	tU1SendBuf[0] =  '#';
	tU1SendBuf[1] =  'E';
	tU1SendBuf[2] = 'C';
	tU1SendBuf[3] = (left  >> 8) & 0xFF;
	tU1SendBuf[4] = left & 0xFF;
	tU1SendBuf[5] = (right  >> 8) & 0xFF;
	tU1SendBuf[6] = right & 0xFF;
	tU1SendBuf[7] = 0;
	tU1SendBuf[8] = '\n';
	

	for(i=0;i<7;i++)
		TempU8 += tU1SendBuf[i]; 
	tU1SendBuf[7] = TempU8;
	
	//�峤�� Chk+1 
	for(i=0;i<9;i++)			
	{
		U1SendBuf[U1SendBufPi] = tU1SendBuf[i];
		U1SendBufPi = (U1SendBufPi +1)%USART1_BUF;  //���ָ����λ 
	}	
	if(IsU1TxEmpty == 1)
	{
		DMA1_Channel4_IRQHandler();	
	}		

}

// 11 bytes, # E C LH LL RH RL GH GL SUM \n
void SendEncodersAndGyro( U16 left, U16 right, S16 gyro )
{
	U8 i=0;
	U8 TempU8=0; 

	tU1SendBuf[0] =  '#';
	tU1SendBuf[1] =  'E';
	tU1SendBuf[2] = 'C';
	tU1SendBuf[3] = (left  >> 8) & 0xFF;
	tU1SendBuf[4] = left & 0xFF;
	tU1SendBuf[5] = (right  >> 8) & 0xFF;
	tU1SendBuf[6] = right & 0xFF;
	tU1SendBuf[7] = (gyro  >> 8) & 0xFF;
	tU1SendBuf[8] = gyro & 0xFF;
	tU1SendBuf[9] = 0;
	tU1SendBuf[10] = '\n';
	

	for(i=0;i<9;i++)
		TempU8 += tU1SendBuf[i]; 
	tU1SendBuf[9] = TempU8;
	
	//�峤�� Chk+1 
	for(i=0;i<11;i++)			
	{
		U1SendBuf[U1SendBufPi] = tU1SendBuf[i];
		U1SendBufPi = (U1SendBufPi +1)%USART1_BUF;  //���ָ����λ 
	}	
	if(IsU1TxEmpty == 1)
	{
		DMA1_Channel4_IRQHandler();	
	}		

}

// 21 bytes, # E C LH LL RH RL GXH GXL GYH GYL GZH GZL AXH AXL AYH AYL AZH AZL SUM \n
void SendEncodersAndMpu6000(U16 left, U16 right, S16 gx, S16 gy, S16 gz, S16 ax, S16 ay, S16 az)
{
	U8 i=0;
	U8 TempU8=0; 

	tU1SendBuf[0] =  '#';
	tU1SendBuf[1] =  'E';
	tU1SendBuf[2] = 'C';
	tU1SendBuf[3] = (left  >> 8) & 0xFF;
	tU1SendBuf[4] = left & 0xFF;
	tU1SendBuf[5] = (right  >> 8) & 0xFF;
	tU1SendBuf[6] = right & 0xFF;
	tU1SendBuf[7] = (gx  >> 8) & 0xFF;
	tU1SendBuf[8] = gx & 0xFF;
	tU1SendBuf[9] = (gy  >> 8) & 0xFF;
	tU1SendBuf[10] = gy & 0xFF;
	tU1SendBuf[11] = (gz  >> 8) & 0xFF;
	tU1SendBuf[12] = gz & 0xFF;
	tU1SendBuf[13] = (ax  >> 8) & 0xFF;
	tU1SendBuf[14] = ax & 0xFF;
	tU1SendBuf[15] = (ay  >> 8) & 0xFF;
	tU1SendBuf[16] = ay & 0xFF;
	tU1SendBuf[17] = (az  >> 8) & 0xFF;
	tU1SendBuf[18] = az & 0xFF;
	tU1SendBuf[19] = 0;
	tU1SendBuf[20] = '\n';
	

	for(i=0;i<19;i++)
		TempU8 += tU1SendBuf[i]; 
	tU1SendBuf[19] = TempU8;
	
	//�峤�� Chk+1 
	for(i=0;i<21;i++)			
	{
		U1SendBuf[U1SendBufPi] = tU1SendBuf[i];
		U1SendBufPi = (U1SendBufPi +1)%USART1_BUF;  //���ָ����λ 
	}	
	if(IsU1TxEmpty == 1)
	{
		DMA1_Channel4_IRQHandler();	
	}		

}



// 9 bytes, # B S b0 b1 b2 b3 SUM \n
void SendBatteryState( U16 voltage_adc )
{
	U8 i=0;
	U8 TempU8=0; 

	//simulateEncoder( &left, &right );

	tU1SendBuf[0] =  '#';
	tU1SendBuf[1] =  'B';
	tU1SendBuf[2] = 'S';
	tU1SendBuf[3] = (voltage_adc >> 8) & 0xFF;
	tU1SendBuf[4] = voltage_adc & 0xFF;
	tU1SendBuf[5] = (4096 >> 8) & 0xFF;
	tU1SendBuf[6] = 4096 & 0xFF;
	tU1SendBuf[7] = 0;
	tU1SendBuf[8] = '\n';

	for(i=0;i<7;i++)
		TempU8 += tU1SendBuf[i]; 
	tU1SendBuf[7] = TempU8;
	
	//�峤�� Chk+1 
	for(i=0;i<9;i++)
	{
		U1SendBuf[U1SendBufPi] = tU1SendBuf[i];
		U1SendBufPi = (U1SendBufPi +1)%USART1_BUF;  //���ָ����λ 
	}	
	if(IsU1TxEmpty == 1)
	{
		DMA1_Channel4_IRQHandler();
	}		

}

void SendBreakerStatus( U8 b0, U8 b1)
{
	U8 i=0;
	U8 TempU8=0; 

	//simulateEncoder( &left, &right );

	tU1SendBuf[0] =  '#';
	tU1SendBuf[1] =  'B';
	tU1SendBuf[2] = 'K';
	tU1SendBuf[3] = b0;
	tU1SendBuf[4] = b1;
	tU1SendBuf[5] = 0;
	tU1SendBuf[6] = '\n';

	for(i=0;i<5;i++)
		TempU8 += tU1SendBuf[i]; 
	tU1SendBuf[5] = TempU8;
	
	//�峤�� Chk+1 
	for(i=0;i<7;i++)
	{
		U1SendBuf[U1SendBufPi] = tU1SendBuf[i];
		U1SendBufPi = (U1SendBufPi +1)%USART1_BUF;  //���ָ����λ 
	}	
	if(IsU1TxEmpty == 1)
	{
		DMA1_Channel4_IRQHandler();
	}	
}

// 9 bytes, # E C LM LS RM RS SUM \n
void SendParam( float value )
{
	U8 i=0;
	U8 TempU8=0; 
	ParamValue param;

	param.f = value;

	tU1SendBuf[0] =  '#';
	tU1SendBuf[1] =  'G';
	tU1SendBuf[2] = 'P';
	tU1SendBuf[3] = param.bytes[0];
	tU1SendBuf[4] = param.bytes[1];
	tU1SendBuf[5] = param.bytes[2];
	tU1SendBuf[6] = param.bytes[3];
	tU1SendBuf[7] = 0;
	tU1SendBuf[8] = '\n';
	

	for(i=0;i<7;i++)
		TempU8 += tU1SendBuf[i]; 

	tU1SendBuf[7] = TempU8;
	//�峤�� Chk+1 
	for(i=0;i<9;i++)			
	{
		U1SendBuf[U1SendBufPi] = tU1SendBuf[i];
		U1SendBufPi = (U1SendBufPi +1)%USART1_BUF;  //���ָ����λ 
	}	
	if(IsU1TxEmpty == 1)
	{
		DMA1_Channel4_IRQHandler();	
	}		

}

void ReceiveBaseCommand( U8 byte )
{
	U8 i = 0;

	if( BaseCmdBufP == 0  && byte == '#' )
	{
		BaseCmdBuf[0] = '#';
		BaseCmdBufP = 1;
	}
	else if( BaseCmdBufP > 0 && BaseCmdBufP < 32)
	{
		if( byte != '\n' )
		{
			BaseCmdBuf[BaseCmdBufP] = byte;
			BaseCmdBufP ++;
		}
		else
		{
			BaseCmdBuf[BaseCmdBufP] = byte;
			BaseCmdBufP ++;
			
			if( BaseRecBufP == 0 )
			{
				for( i = 0; i < BaseCmdBufP; i++)
				{
					BaseRecBuf[i] = BaseCmdBuf[i];
				}
				BaseRecBufP = BaseCmdBufP;
				IsBaseCmdOK = TRUE;
				calBaseCmd();
				IsBaseCmdOK = FALSE;
			}
		}
	}
	else
	{
		BaseCmdBufP = 0;
	}
}

U8 calBaseCmd(void)
{
	U8 tempU8;
	float tempF;

	if( BaseRecBufP == 13 && BaseRecBuf[1] == 'M' && BaseRecBuf[2] == 'S' )	// 0x4d, 0x53,  12 valid bytes, 13rh byte is '\n' 
	{
		// Check sum ?

		// Read speed
		VelocityBuff.left_buff= bytes4ToFloat( BaseRecBuf[3], BaseRecBuf[4], BaseRecBuf[5], BaseRecBuf[6] );
		VelocityBuff.right_buff = bytes4ToFloat( BaseRecBuf[7], BaseRecBuf[8], BaseRecBuf[9], BaseRecBuf[10] );
		getTimeStamp( &(VelocityBuff.stamp) );

		// Limit
		limitMinMax( VelocityBuff.left_buff, VelocityBuff.min, VelocityBuff.max );
		limitMinMax( VelocityBuff.right_buff, VelocityBuff.min, VelocityBuff.max );

		// Deadzone
		/*
		if( VelocityBuff.velocity_left < VelocityBuff.velocity_deadzone && VelocityBuff.velocity_left > -VelocityBuff.velocity_deadzone)
			VelocityBuff.velocity_left = 0;
		if( VelocityBuff.velocity_right < VelocityBuff.velocity_deadzone && VelocityBuff.velocity_right > -VelocityBuff.velocity_deadzone)
			VelocityBuff.velocity_right = 0;
		*/

		if( debug_uart )
		{
			// Print info
			//printf(" MS %d: %f %f rad/s\n", BaseRecBufP, VelocityBuff.velocity_left, VelocityBuff.velocity_right );
			printf(" MS = %d\r\n", Time );
			DebugPrintfDMA();
		}
				
	}
	else if( BaseRecBufP == 7 && BaseRecBuf[1] == 'B' && BaseRecBuf[2] == 'K' )
	{
		setBreakerStatus( BaseRecBuf[3], BaseRecBuf[4] );
		// Print info
		printf(" BK %d set %d = %d\n", Time, BaseRecBuf[3], BaseRecBuf[4] );
		DebugPrintfDMA();
	}
	else if( BaseRecBufP == 10 && BaseRecBuf[1] == 'S' && BaseRecBuf[2] == 'P' )
	{
		tempU8 = BaseRecBuf[3];
		if( tempU8 < PARAM_NUM )
		{
			tempF= bytes4ToFloat( BaseRecBuf[4], BaseRecBuf[5], BaseRecBuf[6], BaseRecBuf[7] );
			paramWriteFloat( param_vector[tempU8].name, tempF );
			paramsSave();
		}
				
		// Print info
		if( tempU8 < PARAM_NUM )
		{
			printf(" SP %d, %s = %f\n", BaseRecBufP, param_vector[tempU8].name, tempF );
			DebugPrintfDMA();
		}

	}
	else if( BaseRecBufP == 6 && BaseRecBuf[1] == 'G' && BaseRecBuf[2] == 'P' )
	{
		tempU8 = BaseRecBuf[3];
		if( tempU8 < PARAM_NUM )
		{
			tempF= param_vector[tempU8].value.f;
			SendParam( tempF );
		}
				
		// Print info
		if( tempU8 < PARAM_NUM )
		{
			printf(" GP %d, %s = %f\n", BaseRecBufP, param_vector[tempU8].name, param_vector[tempU8].value.f );
			DebugPrintfDMA();
		}

	}
	else if( BaseRecBufP == 9 && BaseRecBuf[1] == 'P' && BaseRecBuf[2] == 'P' )
	{
		PidBuff = VelocityPid[MOTOR_LEFT].pid.gains;
		PidBuff.p_gain = bytes4ToFloat( BaseRecBuf[3], BaseRecBuf[4], BaseRecBuf[5], BaseRecBuf[6] );
		VelocityPid[MOTOR_LEFT].pid.gains = PidBuff;
		VelocityPid[MOTOR_RIGHT].pid.gains = PidBuff;

		paramWriteFloat( "velocity_pid_p", PidBuff.p_gain );
		paramsSave();
				
		// Print info
		//printf(" PP %d: p = %f\n", BaseRecBufP, PidBuff.p_gain );
		//DebugPrintfDMA();

	}
	else if( BaseRecBufP == 9 && BaseRecBuf[1] == 'P' && BaseRecBuf[2] == 'I' )
	{
		PidBuff = VelocityPid[MOTOR_LEFT].pid.gains;
		PidBuff.i_gain = bytes4ToFloat( BaseRecBuf[3], BaseRecBuf[4], BaseRecBuf[5], BaseRecBuf[6] );
		VelocityPid[MOTOR_LEFT].pid.gains = PidBuff;
		VelocityPid[MOTOR_RIGHT].pid.gains = PidBuff;

		paramWriteFloat( "velocity_pid_i", PidBuff.i_gain );
		paramsSave();

		// Print info
		printf(" PI %d: i = %f\n", BaseRecBufP, PidBuff.i_gain );
		DebugPrintfDMA();

	}
	else if( BaseRecBufP == 9 && BaseRecBuf[1] == 'P' && BaseRecBuf[2] == 'D' )
	{
		PidBuff = VelocityPid[MOTOR_LEFT].pid.gains;
		PidBuff.d_gain = bytes4ToFloat( BaseRecBuf[3], BaseRecBuf[4], BaseRecBuf[5], BaseRecBuf[6] );
		VelocityPid[MOTOR_LEFT].pid.gains = PidBuff;
		VelocityPid[MOTOR_RIGHT].pid.gains = PidBuff;

		paramWriteFloat( "velocity_pid_d", PidBuff.d_gain );
		paramsSave();
				
		printf(" PD %d: d = %f\n", BaseRecBufP, PidBuff.d_gain );
		DebugPrintfDMA();

	}
	else if( BaseRecBufP == 9 && BaseRecBuf[1] == 'P' && BaseRecBuf[2] == 'C' )
	{
		PidBuff = VelocityPid[MOTOR_LEFT].pid.gains;
		PidBuff.i_max = bytes4ToFloat( BaseRecBuf[3], BaseRecBuf[4], BaseRecBuf[5], BaseRecBuf[6] );
		PidBuff.i_min = -PidBuff.i_max;
		VelocityPid[MOTOR_LEFT].pid.gains = PidBuff;
		VelocityPid[MOTOR_RIGHT].pid.gains = PidBuff;

		paramWriteFloat( "velocity_pid_clamp", PidBuff.i_max );
		paramsSave();
				
		// Print info
		printf(" PC %d: i_max = %f, i_min = %f \n", BaseRecBufP, PidBuff.i_max, PidBuff.i_min );
		DebugPrintfDMA();

	}

	BaseRecBufP = 0;
	return TRUE;
}

void simulateEncoder( U16* left, U16 * right )
{
	static U16 l = 0, r = 0;
	U16 value = getSystemTickValue();

	l += value % 23;
	r += value % 17;

	*left = l;
	*right = r;
}


