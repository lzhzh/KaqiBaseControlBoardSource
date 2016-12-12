#include "Configuration.h"
/**************************************************************************************
��������USART3_IRQHandler
������������USART3�жϵķ����ӳ���
���롡����
���������
���ء�����
**************************************************************************************/
void USART3_IRQHandler(void)  
{	
	U8 t = 0;    
	static U8 FLen = 10; 

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		t = USART_ReceiveData(USART3);

		if(U3BufP < 5)
		{
			if(t == DH0 && U3BufP == H0)
			{
				U3Buf[U3BufP] = t;
				U3BufP++;
			}
			else if(t == DH1 && U3BufP == H1)
			{
				U3Buf[U3BufP] = t;
				U3BufP++;
			}
			else if (U3BufP == Dest)
			{

				U3Buf[U3BufP] = t;
				U3BufP++;
			}
			else if( t <= 64 && U3BufP == Dnum)
			{
				FLen = t + 6;
				U3Buf[U3BufP] = t;
				U3BufP++;
			}
			else
			{
				U3BufP = 0;
			}
		}
		else if(U3BufP < FLen)		//�������ݺ�У��ֵ
		{
			U3Buf[U3BufP] = t;
			U3BufP++;
			
			if(U3BufP >= FLen)
			{
			//����һ֡
				if (U3RecBufP == 0)
				{
					for (t = 0; t < U3BufP; t++)
					{
						U3RecBuf[t] = U3Buf[t];
					//	USART1_SendData(U2Buf[t]);
					}
					
					U3RecBufP = U3BufP;
					U3BufP = 0;
					IsU3RecOK = TRUE;
				//	LED2_TOGGLE;
				}
				else
				{
					//������һ֡��û�д����꣬������֡
					U3BufP = 0;
				}
			}
		}
	}

	USART_ClearITPendingBit(USART3,USART_IT_RXNE);
}


//UART3֡���������λ��
U8 UART3_CAL(void)
{
//	U8 i;
//	U8 TempU8=0;
//	S8 TempS8 = 0;
//	U16 TempU16=0;
//	S16 TempS16=0;
//	S32 TempS32=0;

	U8 DLen;//���ݳ���
//	U8 Chk;	//У����λ��
	U8 DestU8;//��ַ

	DLen = U3RecBuf[Dnum];
/*	Chk = DLen + 5;
	   
	TempU8 = 0;
	for (i = 0; i < Chk; i++)
	       TempU8 = TempU8 + U3RecBuf[i];

	if (TempU8 != U3RecBuf[Chk])
	{  			  	  
		  ////У��ʹ��󣬽�֡�����������
		  U3RecBufP = 0;
		  return FALSE;
	}
*/

	DestU8 = U3RecBuf[Dest] & 0x0F;
	if((DestU8 == FOMC_IP)||(DestU8 == ALL_IP))
	{
		//�ж��Ƿ��͵��ñ�ſ�����������,�����ǹ㲥����
		DestU8 = (U3RecBuf[Dest] >> 4) & 0x0F;
		if((DestU8 == PC_IP))//||(DestU8== ALL_IP))  
		{  
			switch(U3RecBuf[CMD])
			{
				case	0x50:	
						//��������
						if(DLen < 2)	//����2 ���ֽڳ���
						{
							break;
						}
						//�ظ�״̬
						tU3SendBuf[Dest]  = ALL_IP << 4 | FOMC_IP;  //Դ/Ŀ���ַ����
						tU3SendBuf[Dnum]  = 0x02;  				//�������ݸ���
						tU3SendBuf[CMD]  = 0x50;					//����ID
						tU3SendBuf[D0] = 0x55;
						tU3SendBuf[D1] = 0xAA;
						InU3FrameBuf();
						break;

				default:

						break;	   
			}    
			
		}
		
	}
			
	//��֡�����������
	U3RecBufP = 0;
	return TRUE;
}


// 2�����͵�Э���建������ FrameBuf            ��ʱ��ָ���
// tU3SendBuf --> U3SendBuf
U8 InU3FrameBuf(void)
{
	U8 i=0;
	U8 TempU8=0; 
	U8   Chk=0;

	//����Э�����ӵ�����
	tU3SendBuf[H0] =  DH0;
	tU3SendBuf[H1] =  DH1;


	Chk = tU3SendBuf[Dnum] + 5;

	for(i = 0; i< Chk;i ++)
		TempU8 += tU3SendBuf[i]; 

	tU3SendBuf[Chk] = TempU8;
	//�峤�� Chk+1 
	memcpy(U3FrameBuf[U3FrameBufPi], tU3SendBuf, Chk+1);  //ע�� ������    
	U3FrameBufPi=(U3FrameBufPi+1)%U3_FRAMEBUF_LEN;  //ָ����λ    

	return TRUE;
}

// 3����Э���建���������ݷ��͵����ڷ��ͻ�����  U3SendBuf    ��ʱ
// FrameBuf --> U3SendBuf
U8 OutU3FrameBuf(void)
{
    
    U8   Chk=0;
    U8   i=0;
   
	if(U3FrameBufPo != U3FrameBufPi) //Э���建��������
	{

		Chk = U3FrameBuf[U3FrameBufPo][Dnum] + 5;
		 //�峤�� Chk+1 
		for(i=0;i<Chk+1;i++)
		{
			U3SendBuf[U3SendBufPi] = U3FrameBuf[U3FrameBufPo][i];
			U3SendBufPi = (U3SendBufPi +1)%USART3_BUF;  //���ָ����λ 
		}		
	       U3FrameBufPo=(U3FrameBufPo+1)%U3_FRAMEBUF_LEN;  //ָ����λ      

		if(IsU3TxEmpty == 1)
		{
			DMA1_Channel2_IRQHandler();	
		}
			
//		USART3->CR1 |= 0x80;  // ͬʱ����THRE�ж�,�����Ϳ��ж�
	}	
	else 	//��
	{
		return FALSE;
	}
	
    return TRUE;
}


//ֱ�����뷢�ͻ��壬��ǰ֡���ͽ�������������
U8 InU3SendBuf(void)
 {
 
	U8 i=0;
	U8 TempU8=0; 
	U8   Chk=0;

	//����Э�����ӵ�����
	tU3SendBuf[H0] =  DH0;
	tU3SendBuf[H1] =  DH1;

	Chk = tU3SendBuf[Dnum] + 5;

	for(i=0;i<Chk;i++)
		TempU8 += tU3SendBuf[i]; 

	tU3SendBuf[Chk] = TempU8;
	//�峤�� Chk+1 
	for(i=0;i<Chk+1;i++)			
	{
		U3SendBuf[U3SendBufPi] = tU3SendBuf[i];
		U3SendBufPi = (U3SendBufPi +1)%USART3_BUF;  //���ָ����λ 
	}	
	if(IsU3TxEmpty == 1)
	{
		DMA1_Channel2_IRQHandler();	
	}		
	//USART3->CR1 |= 0x80;  // ͬʱ����THRE�ж�,�����Ϳ��ж�

	return TRUE;
 }

//USART3 ���printf
void U3Printf(void)
{
	U16 i=0;
	
	for(i = 0; i < PrintfCn; i++)			
	{
		U3SendBuf[U3SendBufPi] = PrintfBuf[i];
		U3SendBufPi = (U3SendBufPi +1) % USART3_BUF;  //���ָ����λ 
	}	

	PrintfCn = 0;
	
	if(IsU3TxEmpty == 1)
	{
		DMA1_Channel2_IRQHandler();	
	}

}


