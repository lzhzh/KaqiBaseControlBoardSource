#include "Configuration.h"


/**************************************************************************************
��������USART2_IRQHandler
������������USART2�жϵķ����ӳ���
���롡����
���������
���ء�����
**************************************************************************************/
void USART2_IRQHandler(void)  
{	
	U8 t = 0;    
	static U8 FLen = 10;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		t = USART_ReceiveData(USART2);

		if(U2BufP < 4)
		{
			if(t == DH0 && U2BufP == H0)
			{
				U2Buf[U2BufP] = t;
				U2BufP++;
			}
			else if(t == DH1 && U2BufP == H1)
			{
				U2Buf[U2BufP] = t;
				U2BufP++;
			}
			else if(U2BufP == Dest)
			{
				U2Buf[U2BufP] = t;
				U2BufP++;
			}
			else if( t <= 64 && U2BufP == Dnum)
			{
				FLen = t + 6; 
				U2Buf[U2BufP] = t;
				U2BufP++;
			}
			else
			{
				U2BufP = 0;
			}
		}
		else if(U2BufP < FLen)		//�������ݺ�У��ֵ
		{
			U2Buf[U2BufP] = t;
			U2BufP++;
			
			if(U2BufP >= FLen)
			{
			//����һ֡
				if (U2RecBufP == 0)
				{
					for (t = 0; t < U2BufP; t++)
					{
						U2RecBuf[t] = U2Buf[t];
					}
					
					U2RecBufP = U2BufP;
					U2BufP = 0;
					IsU2RecOK = TRUE;
				}
				else
				{
					//������һ֡��û�д����꣬������֡
					U2BufP = 0;
				}
			}
		}
	}

	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
}


//UART2֡���������λ��
U8 UART2_CAL(void)
{
//	U8 i;
	U8 TempU8=0;
	S8 TempS8 = 0;
//	U16 TempU16=0;
	S16 TempS16=0;
//	S32 TempS32=0;
	float TempF = 0;

	U8 DLen;//���ݳ���
//	U8 Chk;	//У����λ��
	U8 DestU8;//��ַ

	DLen = U2RecBuf[Dnum];
/*	Chk = DLen + 5;
	   
	TempU8 = 0;
	for (i = 0; i < Chk; i++)
	       TempU8 = TempU8 + U2RecBuf[i];

	if (TempU8 != U2RecBuf[Chk])
	{  			  	  
		  ////У��ʹ��󣬽�֡�����������
		  U2RecBufP = 0;
		  return FALSE;
	}
*/

	DestU8 = U2RecBuf[Dest] & 0x0F;
	if((DestU8 == LOCAL_IP)||(DestU8 == ALL_IP))
	{
		//�ж��Ƿ��͵��ñ�ſ�����������,�����ǹ㲥����
		DestU8 = (U2RecBuf[Dest] >> 4) & 0x0F;
		if((DestU8 == PC_IP) ||(DestU8== ALL_IP))  
		{  
			switch(U2RecBuf[CMD])
			{	
				case	'T':	// t = 0x54
				case	't':	// T = 0x74
						//��������
						if(DLen < 2)	//����2 ���ֽڳ���
						{
							break;
						}
						//�ظ�״̬
						tU2SendBuf[Dest]  = ALL_IP << 4 | LOCAL_IP;  //Դ/Ŀ���ַ����
						tU2SendBuf[Dnum]  = 0x02;  				//�������ݸ���
						tU2SendBuf[CMD]  = 'T';					//����ID
						tU2SendBuf[D0] = 0x55;
						tU2SendBuf[D1] = 0xAA;
						InU2SendBuf();
						break;
				case	'D':
				case	'd':
						if( DLen < 2)
							break;
						if( U2RecBuf[D0] == 'M' || U2RecBuf[D0] == 'm' )
						{
							debug_motor = (bool)U2RecBuf[D1];
							printf("Debug motor = %d\r\n", debug_motor );
							DebugPrintfDMA();
						}
						else if( U2RecBuf[D0] == 'U' || U2RecBuf[D0] == 'u' )
						{
							debug_uart = (bool)U2RecBuf[D1];
							printf("Debug uart = %d\r\n", debug_uart );
							DebugPrintfDMA();
						}
						break;

				case	'P':	// 0x50
				case	'p':	// D0 as command id, D1 as parameter index
						if( DLen < 2 ) //
							break;
						if( U2RecBuf[D0] == 'S' || U2RecBuf[D0] == 's' )	// 0x53, set parameter value, 
						{
							if( DLen < 6 )
								break;
							TempU8 = U2RecBuf[D1];	// param index
							if( paramValidIndex(TempU8) )
							{
								paramSetBytes4( TempU8, &U2RecBuf[D2]);
								printf("Params set %s = %f:\r\n", param_vector[TempU8].name, param_vector[TempU8].value.f );
								DebugPrintfDMA();
							}
						}
						else if( U2RecBuf[D0] == 'G'  || U2RecBuf[D0] == 'g' )	// 0x47, get parameter value
						{
							TempU8 = U2RecBuf[D1];	// param index
							if( paramValidIndex(TempU8) )
							{
								// Send parameter value
								tU2SendBuf[Dest]  = ALL_IP << 4 | LOCAL_IP; 
								tU2SendBuf[Dnum]  = 0x06;	// bytes number
								tU2SendBuf[CMD]  = 'P';	//command id
								tU2SendBuf[D0] = 'G';	// action id
								tU2SendBuf[D1] = TempU8;	// param index
								paramGetBytes4( TempU8, &tU2SendBuf[D2] );
								InU2SendBuf();
								//
								printf("Params get %s = %f:\r\n", param_vector[TempU8].name, param_vector[TempU8].value.f );
								DebugPrintfDMA();
							}
						}
						else if( U2RecBuf[D0] == 'P' || U2RecBuf[D0] == 'p' )	// 0x50, print parameters names
						{
							paramsInfoPrint();
						}
						else if( U2RecBuf[D0] == 'F' || U2RecBuf[D0] == 'f' )	// refresh parameters
						{
							paramsRefresh();
							printf("Refresh params to system variables, size = %d.\r\n", PARAM_NUM );
							DebugPrintfDMA();
						}
						else if( U2RecBuf[D0] == 'R' || U2RecBuf[D0] == 'r' )		// 0x52, reset parameters to default
						{
							
							paramsReset(0);
							printf("Reset params to default, size = %d.\r\n", PARAM_NUM );
							DebugPrintfDMA();
						}
						else if( U2RecBuf[D0] == 'L' || U2RecBuf[D0] == 'l' )		// 0x4C, load parameters from flash
						{
							paramsLoad();
							printf("Load params from flash, size = %d.\r\n", PARAM_NUM );
							DebugPrintfDMA();
						}
						else if( U2RecBuf[D0] == 'A' || U2RecBuf[D0] == 'a' )	// 0x41, save parameters to flash
						{
							paramsSave();
							printf("Save params to flash, size = %d.\r\n", PARAM_NUM );
							DebugPrintfDMA();
						}
						
						break;

				case	'S':	// S = 0x53
				case	's':	// s = 0x73
						// Set paramter
						if( DLen < 4 )	//����2 ���ֽڳ���
						{
							break;
						}
						TempU8 = U2RecBuf[D0];
						TempS16 = U8toS16(U2RecBuf[D2], U2RecBuf[D1]);
						TempS8 = (S8)U2RecBuf[D3];
						if( TempU8 < PARAM_NUM )
						{
							TempF = s16s8toFloat( TempS16, TempS8 );
							param_vector[TempU8].value.f = TempF;
							printf(" Set %s = %f\r\n", param_vector[TempU8].name, param_vector[TempU8].value.f );
							DebugPrintfDMA();
						}

				case	'G':	// G = 0x47
				case	'g':	// g = 0x67
						// Set paramter
						if( DLen < 2 )	//����2 ���ֽڳ���
						{
							break;
						}
						TempU8 = U2RecBuf[D0];
						if( TempU8 < PARAM_NUM )
						{
							printf(" Get %s = %f\r\n", param_vector[TempU8].name, param_vector[TempU8].value.f );
							DebugPrintfDMA();
						}
						
						break;

				default:

						break;	  

			}    
			
		}
		
	}
			
	//��֡�����������
	U2RecBufP = 0;
	return TRUE;
}


// 2�����͵�Э���建������ FrameBuf            ��ʱ��ָ���
// tU2SendBuf --> U2SendBuf
U8 InU2FrameBuf(void)
{
	U8 i=0;
	U8 TempU8=0; 
	U8   Chk=0;

	//����Э�����ӵ�����
	tU2SendBuf[H0] =  DH0;
	tU2SendBuf[H1] =  DH1;


	Chk = tU2SendBuf[Dnum] + 5;

	for(i = 0; i< Chk;i ++)
		TempU8 += tU2SendBuf[i]; 

	tU2SendBuf[Chk] = TempU8;
	//�峤�� Chk+1 
	memcpy(U2FrameBuf[U2FrameBufPi], tU2SendBuf, Chk+1);  //ע�� ������    
	U2FrameBufPi=(U2FrameBufPi+1)%U2_FRAMEBUF_LEN;  //ָ����λ    

	return TRUE;
}

// 3����Э���建���������ݷ��͵����ڷ��ͻ�����  U3SendBuf    ��ʱ
// FrameBuf --> U3SendBuf
U8 OutU2FrameBuf(void)
{
    
    U8   Chk=0;
    U8   i=0;
   
	if(U2FrameBufPo != U2FrameBufPi) //Э���建��������
	{

		Chk = U2FrameBuf[U2FrameBufPo][Dnum] + 5;
		 //�峤�� Chk+1 
		for(i=0;i<Chk+1;i++)
		{
			U2SendBuf[U2SendBufPi] = U2FrameBuf[U2FrameBufPo][i];
			U2SendBufPi = (U2SendBufPi +1)%USART2_BUF;  //���ָ����λ 
		}		
	       U2FrameBufPo=(U2FrameBufPo+1)%U2_FRAMEBUF_LEN;  //ָ����λ      

		if(IsU2TxEmpty == 1)
		{
			DMA1_Channel7_IRQHandler();	
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
U8 InU2SendBuf(void)
 {
 
	U8 i=0;
	U8 TempU8=0; 
	U8   Chk=0;

	//����Э�����ӵ�����
	tU2SendBuf[H0] =  DH0;
	tU2SendBuf[H1] =  DH1;

	Chk = tU2SendBuf[Dnum] + 5;

	for(i=0;i<Chk;i++)
		TempU8 += tU2SendBuf[i]; 

	tU2SendBuf[Chk] = TempU8;
	//�峤�� Chk+1 
	for(i=0;i<Chk+1;i++)			
	{
		U2SendBuf[U2SendBufPi] = tU2SendBuf[i];
		U2SendBufPi = (U2SendBufPi +1)%USART2_BUF;  //���ָ����λ 
	}	
	if(IsU2TxEmpty == 1)
	{
		DMA1_Channel7_IRQHandler();	
	}		
	//USART2->CR1 |= 0x80;  // ͬʱ����THRE�ж�,�����Ϳ��ж�

	return TRUE;
 }

//USART2 ���printf
void U2Printf(void)
{
	U16 i=0;
	
	for(i = 0; i < PrintfCn; i++)			
	{
		USART2_SendData( PrintfBuf[i] );
	}	
	PrintfCn = 0;
}

void U2PrintfDMA(void)
{
	U16 i=0;
	
	for(i = 0; i < PrintfCn; i++)			
	{
		U2SendBuf[U2SendBufPi] = PrintfBuf[i];
		U2SendBufPi = (U2SendBufPi +1) % USART2_BUF;  //���ָ����λ 
	}	

	PrintfCn = 0;
	
	if(IsU2TxEmpty == 1)
	{
		DMA1_Channel7_IRQHandler();	
	}
}


