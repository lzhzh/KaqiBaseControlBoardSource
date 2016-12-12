#include "Configuration.h"

bool debug_motor = FALSE;
bool debug_uart = FALSE;

//оƬ�ڲ��¶�
u16 InternalTemp;
//��Դ��ѹ
u16 Power;
float battery_voltage = 0;

/*��ʱ����*/
u32 Time = 0;
u32 Second = 0;
bool time_locked = FALSE;
/*��ȷ��ʱ�����ĸ�������*/
u32 TimingDelay = 0;  			//��ʱʱ��

u32 ADC1_2_OK = FALSE;
u32 ADC1_2_ConvertedValue[ADC1_2_CHANNEL_NUM/2];	//ADC1��2ת�����DMA��ʱ����
u32 ADC_ConvertedValue[ADC_CHANNEL_NUM];	//ADCת�����ս��
/***************************************************************
***************************************************************/
//define of PWM input
U16 TIM8_RT[4] = {0,0,0,0};		//��¼TIM8����PWM������ʱ��
U16 TIM8_FT[4] = {0,0,0,0};		//��¼TIM8����PWM�½���ʱ��
U8  TIM8_flag[4] = {0,0,0,0};		//��־�˿�����PWM�Ǹߵ�ƽ���ǵ͵�ƽ	
//RC�������
U16 inPWM1 = 1500;		//inPWM1  ��������
U16 inPWM2 = 1500;		//inPWM2  ��������
U16 inPWM3 = 1500;		//inPWM3  ��������
U16 inPWM4 = 1500;		//inPWM4  ��������

/***************************************************************
**������ر���
***************************************************************/
/***********************Para of stdio*****************************/
U8 PrintfBuf[PrintfBuf_Len] = {0};
U16 PrintfCn = 0;

/***********************Para of USART1**************************/
U8 IsU1Send = FALSE;	//����1���Ϳ���

U8 USART1_Send_Buf[USART1_BUF] = {0};	//USART1���ͻ�����(DMA)
U16 U1SendCn = 0;						//����1���ͼ�����
U8 IsU1TxEmpty = 1;						//��־USART1���ͻ������Ƿ�Ϊ��

U8  tU1SendBuf[U1_FRAME_LEN]={0};	     	//USART1������ʱ������

U8  U1SendBuf[USART1_BUF]={0};	     		//USART1���ͻ�����
U16  U1SendBufPi=0;	                                 	//USART1���ͻ��������ָ��	
U16  U1SendBufPo=0;	                          		//USART1���ͻ��������ָ��

U8  U1FrameBuf[U1_FRAMEBUF_LEN][U1_FRAME_LEN]={0};   //�巢�ͻ�����
U8  U1FrameBufPi = 0;					//�巢�ͻ���������ָ��
U8  U1FrameBufPo = 0;					//�巢�ͻ��������ָ�

U8  U1BufP = 0;                                		//USART1���ջ�����ָ��
U8  U1Buf[U1_FRAME_LEN]={0};		 		//USART1���ջ�����		 

U8  IsU1RecOK = FALSE;
U8  U1RecBufP = 0;				  		//USART1������ʱ������ָ��
U8  U1RecBuf[U1_FRAME_LEN]={0};	  		//USART1������ʱ������

/*************/
U8 BaseBuf[BASE_BUF_LEN] = {0};
U16 BaseBufPi = 0;
U16 BaseBufPo = 0;

U8 BaseCmdBufP = 0;
U8 BaseCmdBuf[32] = {0};

U8 IsBaseCmdOK = FALSE;
U8 BaseRecBufP = 0;
U8 BaseRecBuf[32] = {0};


/***********************Para of USART2**************************/
U8 IsU2Send = FALSE;	//����2���Ϳ���

U8 USART2_Send_Buf[USART2_BUF] = {0};	//USART2���ͻ�����(DMA)
U16 U2SendCn = 0;						//����2���ͼ�����
U8 IsU2TxEmpty = 1;						//��־USART2���ͻ������Ƿ�Ϊ��

U8  tU2SendBuf[U2_FRAME_LEN]={0};	     	//USART2������ʱ������

U8  U2SendBuf[USART2_BUF]={0};	     		//USART2���ͻ�����
U16  U2SendBufPi=0;	                                	//USART2���ͻ��������ָ��	
U16  U2SendBufPo=0;	                          		//USART2���ͻ��������ָ��

U8  U2FrameBuf[U2_FRAMEBUF_LEN][U2_FRAME_LEN]={0};   //�巢�ͻ�����
U8  U2FrameBufPi = 0;					//�巢�ͻ���������ָ��
U8  U2FrameBufPo = 0;					//�巢�ͻ��������ָ��

U8  U2BufP = 0;                                		//USART2���ջ�����ָ��
U8  U2Buf[U2_FRAME_LEN]={0};		 		//USART2���ջ�����		 

U8  IsU2RecOK = FALSE;
U8  U2RecBufP = 0;				  		//USART2������ʱ������ָ��
U8  U2RecBuf[U2_FRAME_LEN]={0};	  		//USART2������ʱ������

/***********************Para of USART3**************************/
U8 IsU3Send = FALSE;	//����3���Ϳ���

U8 USART3_Send_Buf[USART3_BUF] = {0};	//USART3���ͻ�����(DMA)
U16 U3SendCn = 0;						//����3���ͼ�����
U8 IsU3TxEmpty = 1;						//��־USART3���ͻ������Ƿ�Ϊ��

U8  tU3SendBuf[U3_FRAME_LEN]={0};	     	//USART3������ʱ������

U8  U3SendBuf[USART3_BUF]={0};	     		//USART3���ͻ�����
U16  U3SendBufPi=0;	                                 	//USART3���ͻ��������ָ��	
U16  U3SendBufPo=0;	                          		//USART3���ͻ��������ָ��

U8  U3FrameBuf[U3_FRAMEBUF_LEN][U3_FRAME_LEN]={0};   //�巢�ͻ�����
U8  U3FrameBufPi = 0;					//�巢�ͻ���������ָ��
U8  U3FrameBufPo = 0;					//�巢�ͻ��������ָ��

U8  U3BufP = 0;                                		//USART3���ջ�����ָ��
U8  U3Buf[U3_FRAME_LEN]={0};		 		//USART3���ջ�����		 

U8  IsU3RecOK = FALSE;
U8  U3RecBufP = 0;				  		//USART3������ʱ������ָ��
U8  U3RecBuf[U3_FRAME_LEN]={0};	  		//USART3������ʱ������


