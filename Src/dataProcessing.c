#include "dataProcessing.h"
#include "usart.h"

	uint8_t key1Flag = 0;
	uint8_t key2Flag = 0;
	uint8_t key3Flag = 0;

	uint8_t key1Temp = 0;
	uint8_t key2Temp = 0;
	uint8_t key3Temp = 0;

	uint8_t key1Read = 0;
	uint8_t key2Read = 0;
	uint8_t key3Read = 0;

	uint8_t key1InFlag = 0;
	uint8_t key2InFlag = 0;
	uint8_t key3InFlag = 0;

	uint8_t temp1[6] = { 0xff,0xff,0xff,0xff,0xff,0xff };
	uint8_t temp0[6] = { 0,0,0,0,0,0 };
	uint8_t backAddress[3] = { 0,0,0 };

	uint8_t usart_tx_temp[256];
	uint8_t usart3_tx_temp[128];
	uint8_t usart3_tx_temp2[128];

	/*
	函数功能：处理串口接收到的数据：
	1.如果是24ffff开头，设置m0,m1,m2。usart1,2成功返回ffffff，失败返回000000 ,usart3返回三位地址+ffffff/000000
	2.如果是25ffff开头，截取之后三位作为地址存储。删除25ffff后发送给三个串口。
	3.其他	（1）如果huart = huart1，2：前面加三位地址，发送给uart3
			（2）如果huart = huart3:取前三位做地址，其他发送给usart1，2.

	buffer:当前串口接收到的数据
	length:当前串口接收到的数据的长度
	*/

	void cmdProcessing(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma, unsigned char *buffer, unsigned int length) {

		if (buffer[0] == 0x24 && buffer[1] == 0xff && buffer[2] == 0xff)
		{
			if (length == 6)
			{
				if (buffer[3] == 1)
				{
					HAL_GPIO_WritePin(M0_GPIO_Port, M0_Pin, GPIO_PIN_SET);
				}
				else if (buffer[3] == 0)
				{
					HAL_GPIO_WritePin(M0_GPIO_Port, M0_Pin, GPIO_PIN_RESET);
				}

				if (buffer[4] == 1)
				{
					HAL_GPIO_WritePin(M1_GPIO_Port, M1_Pin, GPIO_PIN_SET);
				}
				else if (buffer[4] == 0)
				{
					HAL_GPIO_WritePin(M1_GPIO_Port, M1_Pin, GPIO_PIN_RESET);
				}

				if (buffer[5] == 1)
				{
					HAL_GPIO_WritePin(M2_GPIO_Port, M2_Pin, GPIO_PIN_SET);
				}
				else if (buffer[5] == 0)
				{
					HAL_GPIO_WritePin(M2_GPIO_Port, M2_Pin, GPIO_PIN_RESET);
				}
				if (huart == &huart1||huart == &huart2)
				{
					dma_send(&huart1, &hdma_usart1_tx, temp1, 3);
					dma_send(&huart2, &hdma_usart2_tx, temp1, 3);
				}
				else
				{
					usart_tx_temp[0] = backAddress[0];
					usart_tx_temp[1] = backAddress[1];
					usart_tx_temp[2] = backAddress[2];
					dma_send(&huart3, &hdma_usart3_tx, temp1, 6);
				}
			}
			else
			{

				if (huart == &huart1 || huart == &huart2)
				{
					dma_send(&huart1, &hdma_usart1_tx, temp0, 3);
					dma_send(&huart2, &hdma_usart2_tx, temp0, 3);
				}
				else
				{
					usart_tx_temp[0] = backAddress[0];
					usart_tx_temp[1] = backAddress[1];
					usart_tx_temp[2] = backAddress[2];
					dma_send(&huart3, &hdma_usart3_tx, temp0, 6);
				}
			}
		}
		else if (buffer[0] == 0x25 && buffer[1] == 0xff && buffer[2] == 0xff)
		{
			for (uint8_t i = 0; i < 128 - 3; i++)
			{
				usart3_tx_temp[i] = buffer[i + 3];
			}
			dma_send(&huart1, &hdma_usart1_tx, usart3_tx_temp, length - 3);
			dma_send(&huart2, &hdma_usart2_tx, usart3_tx_temp, length - 3);
			dma_send(&huart3, &hdma_usart3_tx, usart3_tx_temp, length - 3);
		}
		else
		{
			if (huart == &huart1 || huart == &huart2)
			{

				usart_tx_temp[0] = backAddress[0];
				usart_tx_temp[1] = backAddress[1];
				usart_tx_temp[2] = backAddress[2];

				for (uint8_t i = 0; i < usart1_tx_len; i++)
				{
					usart_tx_temp[i + 3] = usart1_tx_buffer[i];
				}

				dma_send(&huart3, &hdma_usart3_tx, usart_tx_temp, usart1_tx_len + 3);
			}

			if (huart == &huart3) {

				dma_send(&huart1, &hdma_usart1_tx, buffer, length);

				backAddress[0] = buffer[0];
				backAddress[1] = buffer[1];
				backAddress[2] = buffer[2];

				for (uint8_t i = 0; i < 128 - 3; i++)
				{
					usart3_tx_temp[i] = buffer[i + 3];
				}
				dma_send(&huart1, &hdma_usart1_tx, usart3_tx_temp, length - 3);
				dma_send(&huart2, &hdma_usart2_tx, usart3_tx_temp, length - 3);
			}
		}
	}


void dataProcessing() {

	if (usart1_rx_flag == 1)
	{
		cmdProcessing(&huart1, &hdma_usart1_tx, usart1_tx_buffer, usart1_tx_len);
		usart1_rx_flag = 0;
	}

	if (usart2_rx_flag == 1)
	{
		cmdProcessing(&huart2, &hdma_usart2_tx, usart2_tx_buffer, usart2_tx_len);
		usart2_rx_flag = 0;
	}

	if (usart3_rx_flag == 1)
	{
		cmdProcessing(&huart3, &hdma_usart3_tx, usart3_tx_buffer, usart3_tx_len);
		usart3_rx_flag = 0;
	}
}