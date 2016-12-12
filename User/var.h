#ifndef VAR_H
#define VAR_H

#define true 		TRUE
#define false 		FALSE

#define BIT(x)	(1 << (x))
#define set_bit(reg, bit)		((reg) |= (1 << (bit)))
#define clear_bit(reg, bit)		((reg) &= ~ (1 << (bit)))
#define toggle_bit(reg, bit)	((reg) ^= (1 << (bit)))
#define get_bit(reg, bit)		(((reg) >> (bit)) & 1)
#define get_twobit(reg, bit)	(((reg) >> (bit)) & 3)

#define getSystemTickValue()	(SysTick->VAL)

extern bool debug_motor;
extern bool debug_uart;

//оƬ�ڲ��¶�
extern u16 InternalTemp;
//��Դ��ѹ
extern u16 Power;
extern float battery_voltage;

/*��ʱ����*/
extern u32 Time;
extern u32 Second;
extern bool time_locked;
/*��ȷ��ʱ�����ĸ�������*/
extern u32 TimingDelay;  			//��ʱʱ��

/***************************************************************
***************************************************************/
	//define of PWM input
extern U16 TIM8_RT[4];		//��¼TIM8����PWM������ʱ��
extern U16 TIM8_FT[4];		//��¼TIM8����PWM�½���ʱ��
extern U8  TIM8_flag[4];		//��־�˿�����PWM�Ǹߵ�ƽ���ǵ͵�ƽ	

//RC�������
extern U16 inPWM1 ;		//inPWM1  ��������
extern U16 inPWM2 ;		//inPWM2  ��������
extern U16 inPWM3 ;		//inPWM3  ��������
extern U16 inPWM4 ;		//inPWM4  ��������
//

#endif

#ifndef    ADCVAR_H
#define    ADCVAR_H
//ADCת��ͨ������
#define ADC_CHANNEL_NUM	4
#define ADC_CHANNEL_TEMP	0
#define ADC_CHANNEL_1V2	1
#define ADC_CHANNEL_10		2
#define ADC_CHANNEL_11 	3
//ת�����У׼ֵ����

//ת������Ĵ�������
#define ADC1_2_CHANNEL_NUM 12
#define ADC_CHANNEL_TEMP_FILTER_NUM	3
#define ADC_CHANNEL_1V2_FILTER_NUM	3
#define ADC_CHANNEL_10_FILTER_NUM	3
#define ADC_CHANNEL_11_FILTER_NUM	3

extern u32 ADC1_2_OK;
extern u32 ADC1_2_ConvertedValue[ADC1_2_CHANNEL_NUM / 2];//ADC1��2ת�����DMA��ʱ����
extern u32 ADC_ConvertedValue[ADC_CHANNEL_NUM];	//ADCת�����ս��

#endif //ADCVAR


/***************************************************************
**������ر���
***************************************************************/
#ifndef USARTVAR
#define USARTVAR

/***********************Para of stdio*****************************/
#define PrintfBuf_Len	500

extern U8 PrintfBuf[PrintfBuf_Len];
extern U16 PrintfCn;

/**********************define of devices address**************************/
#define PC_IP		0x01	//��λ����ַ
#define LOCAL_IP		0x02	//������ַ
#define ALL_IP		0x00	//�㲥��ַ

//����Э��
#define  DH0      	0xA5      //��ͷ0  ����
#define  DH1    	0x5A      //��ͷ0   ����

#define  H0        	0      //��ͷ0   λ��
#define  H1        	1      //��ͷ1    λ��

#define  Dest  	2      //Ŀ�� λ��
#define  Dnum	3      //Դ      λ��
#define  CMD    	4      //������ λ��
#define  D0	5
#define  D1	6
#define  D2	7
#define  D3	8
#define  D4	9
#define  D5	10
#define  D6	11
#define  D7	12
#define  D8	13
#define  D9	14
#define  D10	15
#define  D11	16
#define  D12	17
#define  D13	18
#define  D14	19
#define  D15	20


#define CMD_ST	0xA0
/////////////////////////////////////////////////
#define FOMC_IP		0x01	//��������������ַ
#define ST188_IP		0x03	//����Ѱ�ߴ���ģ��

/***********************Para of USART1**************************/
#define USART1_BUF			200
#define U1_FRAME_LEN		30
#define U1_FRAMEBUF_LEN	10

extern U8 IsU1Send;						//����1���Ϳ���
extern U8 USART1_Send_Buf[USART1_BUF];	//USART1���ͻ�����(DMA)
extern U16 U1SendCn;						//����1���ͼ�����
extern U8 IsU1TxEmpty;					//��־USART1���ͻ������Ƿ�Ϊ��

extern U8  tU1SendBuf[U1_FRAME_LEN];	     	//USART1������ʱ������

extern U8  U1SendBuf[USART1_BUF];	     	//USART1���ͻ�����
extern U16  U1SendBufPi;	                            //USART1���ͻ��������ָ��	
extern U16  U1SendBufPo;	                          	//USART1���ͻ��������ָ��

extern U8  U1FrameBuf[U1_FRAMEBUF_LEN][U1_FRAME_LEN];   //�巢�ͻ�����
extern U8  U1FrameBufPi;                                       //�巢�ͻ���������ָ��
extern U8  U1FrameBufPo;                                      //�巢�ͻ��������ָ��

extern U8  U1BufP;                                		//USART1���ջ�����ָ��
extern U8  U1Buf[U1_FRAME_LEN];		 	//USART1���ջ�����		 

extern U8  IsU1RecOK;
extern U8  U1RecBufP;				  		//USART1������ʱ������ָ��
extern U8  U1RecBuf[U1_FRAME_LEN];	  	//USART1������ʱ������

/*************/
#define BASE_BUF_LEN	500
extern U8 BaseBuf[BASE_BUF_LEN];
extern U16 BaseBufPi;
extern U16 BaseBufPo;

extern U8 BaseCmdBufP;
extern U8 BaseCmdBuf[32];

extern U8 IsBaseCmdOK;
extern U8 BaseRecBufP;
extern U8 BaseRecBuf[32];

/***********************Para of USART2**************************/
#define USART2_BUF			500
#define U2_FRAME_LEN		30
#define U2_FRAMEBUF_LEN	5

extern U8 IsU2Send;						//����2���Ϳ���
extern U8 USART2_Send_Buf[USART2_BUF];	//USART2���ͻ�����(DMA)
extern U16 U2SendCn;						//����2���ͼ�����
extern U8 IsU2TxEmpty;					//��־USART2���ͻ������Ƿ�Ϊ��

extern U8  tU2SendBuf[U2_FRAME_LEN];	     	//USART2������ʱ������

extern U8  U2SendBuf[USART2_BUF];	     	//USART2���ͻ�����
extern U16  U2SendBufPi;	                            //USART2���ͻ��������ָ��	
extern U16  U2SendBufPo;	                          	//USART2���ͻ��������ָ��

extern U8  U2FrameBuf[U2_FRAMEBUF_LEN][U2_FRAME_LEN];   //�巢�ͻ�����
extern U8  U2FrameBufPi;                                       //�巢�ͻ���������ָ��
extern U8  U2FrameBufPo;                                      //�巢�ͻ��������ָ��

extern U8  U2BufP;                                		//USART2���ջ�����ָ��
extern U8  U2Buf[U2_FRAME_LEN];		 	//USART2���ջ�����		 

extern U8  IsU2RecOK;
extern U8  U2RecBufP;				  		//USART2������ʱ������ָ��
extern U8  U2RecBuf[U2_FRAME_LEN];	  	//USART2������ʱ������

/***********************Para of USART3**************************/
#define USART3_BUF			200
#define U3_FRAME_LEN		70
#define U3_FRAMEBUF_LEN	10

extern U8 IsU3Send;						//����3���Ϳ���
extern U8 USART3_Send_Buf[USART3_BUF];	//USART3���ͻ�����(DMA)
extern U16 U3SendCn;						//����3���ͼ�����
extern U8 IsU3TxEmpty;					//��־USART3���ͻ������Ƿ�Ϊ��

extern U8  tU3SendBuf[U3_FRAME_LEN];	     	//USART3������ʱ������

extern U8  U3SendBuf[USART3_BUF];	     	//USART3���ͻ�����
extern U16  U3SendBufPi;	                            //USART3���ͻ��������ָ��	
extern U16  U3SendBufPo;	                          	//USART3���ͻ��������ָ��

extern U8  U3FrameBuf[U3_FRAMEBUF_LEN][U3_FRAME_LEN];   //�巢�ͻ�����
extern U8  U3FrameBufPi;                                       //�巢�ͻ���������ָ��
extern U8  U3FrameBufPo;                                      //�巢�ͻ��������ָ��

extern U8  U3BufP;                                		//USART3���ջ�����ָ��
extern U8  U3Buf[U3_FRAME_LEN];		 	//USART3���ջ�����		 

extern U8  IsU3RecOK;
extern U8  U3RecBufP;				  		//USART3������ʱ������ָ��
extern U8  U3RecBuf[U3_FRAME_LEN];	  	//USART3������ʱ������


#endif//USARTVAR


