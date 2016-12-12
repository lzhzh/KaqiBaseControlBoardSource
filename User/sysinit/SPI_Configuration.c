#include "Configuration.h"

/**************************************************************************************
��������SPI2_Configuration
������������SPI2
���롡����
���������
���ء�����
**************************************************************************************/
void SPI2_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable SPI2 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 , ENABLE);

	/* Configure SPI2 pins: NSS, SCK, MISO and MOSI */
	/*PB13,PB14,PB15��©���ù���*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	/* Configure PB12 as Output push-pull, used as Flash Chip select */
	/*PB12��©�������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Deselect the Chip*/
	SPI2_Deselected();

	SPI2_HighSpeed(DISABLE);


#if defined (CONFIG_DEBUG)
	printf("Initializing SPI2 ... Done\n");
	DebugPrintf();
#endif
 
}


void SPI2_HighSpeed(FunctionalState newState)
{
	SPI_InitTypeDef  SPI_InitStructure;

	/* Deselect the Chip*/
	SPI2_Deselected();

	SPI_Cmd(SPI2, DISABLE);	// disable first

	/* SPI2 configuration */ 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //ѡ��2�ߵ�������ģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;					   //���豸����
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				  //8Ϊ���ݳ���
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;  			//����ʱSCKΪ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 		//���ݲ����ӵڶ���ʱ�ӱ��ؿ�ʼ
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	   	//������豸����ʹ�ܣ�NSS�������뱻CR1��SSIλ�е�ֵ���
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;   //APB1ʱ��64��Ƶ, 36/64 = 562.5kHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //�ȷ��͸�λ
	SPI_InitStructure.SPI_CRCPolynomial = 7;							  //����ʽΪ7

	if(newState == ENABLE)
	{
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;   //APB2ʱ��2��Ƶ, 36 /4 = 9MHz
	}
	
	SPI_Init(SPI2, &SPI_InitStructure);

	/* Enable SPI2  */
	SPI_Cmd(SPI2, ENABLE);   										  //ʹ��SPI2

}

/**************************************************************************************
��������SPI2_SendByte
��������SPI2����
���롡����
���������
���ء�����
**************************************************************************************/
uint8 SPI2_SendByte(uint8 byte)
{
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
//	while((SPI2->SR & 0x02) == 0);
	/* Send byte through the SPI2 peripheral */
	SPI_I2S_SendData(SPI2, byte);
//	SPI2->DR = byte;
	/* Wait to receive a byte */
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
//	while((SPI2->SR & 0x01) == 0);
	
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI2);
//	return (SPI2->DR);

}



/**************************************************************************************
��������SPI3_Configuration
������������SPI3
���롡����
���������
���ء�����
**************************************************************************************/
void SPI3_Configuration(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable SPI3 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3 , ENABLE);

	// pin remap
	GPIO_PinRemapConfig(GPIO_Remap_SPI3, ENABLE);

	/* Configure SPI3 pins: NSS, SCK, MISO and MOSI */
	/*PC10,PC11,PC12��©���ù���*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	/* Configure PA15 as Output push-pull, used as Flash Chip select */
	/*PA15��©�������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Deselect the Chip*/
	SPI3_Deselected();

	/* SPI2 configuration */ 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //ѡ��2�ߵ�������ģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;					   //���豸����
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				  //8Ϊ���ݳ���
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;  			//����ʱSCKΪ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 		//���ݲ����ӵڶ���ʱ�ӱ��ؿ�ʼ
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	   	//������豸����ʹ�ܣ�NSS�������뱻CR1��SSIλ�е�ֵ���
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;   //APB1ʱ��8��Ƶ, 36/8 = 4.5 MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //�ȷ��͸�λ
	SPI_InitStructure.SPI_CRCPolynomial = 7;							  //����ʽΪ7
	SPI_Init(SPI3, &SPI_InitStructure);

	/* Enable SPI3  */
	SPI_Cmd(SPI3, ENABLE);   										  //ʹ��SPI3

#if defined (CONFIG_DEBUG)
	printf("Initializing SPI3 ... Done\n");
	DebugPrintf();
#endif
 
}

/**************************************************************************************
��������SPI3_SendByte
��������SPI3����
���롡����
���������
���ء�����
**************************************************************************************/
uint8 SPI3_SendByte(uint8 byte)
{
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
//	while((SPI3->SR & 0x02) == 0);

	/* Send byte through the SPI2 peripheral */
	SPI_I2S_SendData(SPI3, byte);
//	SPI3->DR = byte;

	/* Wait to receive a byte */
	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
//	while((SPI3->SR & 0x01) == 0);
	
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI3);
//	return (SPI3->DR);

}

