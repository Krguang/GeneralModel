#include "dataProcessing.h"
#include "usart.h"

	uint8_t temp1[3] = { 0xff,0xff,0xff };
	uint8_t temp0[3] = { 0,0,0 };
	uint8_t backAddress[3] = { 0,0,0 };

	/*
	函数功能：处理串口接收到的数据：
	1.如果是24ffff开头，设置m0,m1,m2。
	2.如果是25ffff开头，截取之后三位作为地址存储。删除数据前六位。

	buffer:当前串口接收到的数据
	length:当前串口接收到的数据的长度
	*/
static void rxArrayProcess(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma, unsigned char *buffer, unsigned int length) {

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

			dma_send(huart, hdma, temp1, 3);
		}
		else
		{
			dma_send(huart, hdma, temp0, 3);
		}
	}
	else if (buffer[0] == 0x25 && buffer[1] == 0xff && buffer[2] == 0xff)
	{
		uint8_t usart3_tx_temp[128];
		for (uint8_t i = 0; i < 128-3; i++)
		{
			usart3_tx_temp[i] = buffer[i + 3];
		}
		dma_send(&huart3, &hdma_usart3_tx, usart3_tx_temp, length - 3);

	} else{

		uint8_t usart_tx_temp[256];

		usart_tx_temp[0] = backAddress[0];
		usart_tx_temp[1] = backAddress[1];
		usart_tx_temp[2] = backAddress[2];

		for (uint8_t i = 0; i < usart1_tx_len; i++)
		{
			usart_tx_temp[i + 3] = usart1_tx_buffer[i];
		}

		dma_send(&huart3, &hdma_usart3_tx, usart_tx_temp, usart1_tx_len + 3);
	}

}


void dataProcessing() {

	if (usart1_rx_flag == 1)
	{
		if (usart1_tx_buffer[0] == 0x24 && usart1_tx_buffer[1] == 0xff && usart1_tx_buffer[2] == 0xff)
		{
			if (usart1_tx_len == 6)
			{
				if (usart1_tx_buffer[3] == 1)
				{
					HAL_GPIO_WritePin(M0_GPIO_Port, M0_Pin, GPIO_PIN_SET);
				}
				else if(usart1_tx_buffer[3] == 0)
				{
					HAL_GPIO_WritePin(M0_GPIO_Port, M0_Pin, GPIO_PIN_RESET);
				}

				if (usart1_tx_buffer[4] == 1)
				{
					HAL_GPIO_WritePin(M1_GPIO_Port, M1_Pin, GPIO_PIN_SET);
				}
				else if (usart1_tx_buffer[4] == 0)
				{
					HAL_GPIO_WritePin(M1_GPIO_Port, M1_Pin, GPIO_PIN_RESET);
				}

				if (usart1_tx_buffer[5] == 1)
				{
					HAL_GPIO_WritePin(M2_GPIO_Port, M2_Pin, GPIO_PIN_SET);
				}
				else if(usart1_tx_buffer[5] == 0)
				{
					HAL_GPIO_WritePin(M2_GPIO_Port, M2_Pin, GPIO_PIN_RESET);
				}

				dma_send(&huart1, &hdma_usart1_tx, temp1, 3);
			}
			else
			{
				dma_send(&huart1, &hdma_usart1_tx, temp0, 3);
			}
		}
		else {

			uint8_t usart1_tx_temp[256];

			usart1_tx_temp[0] = backAddress[0];
			usart1_tx_temp[1] = backAddress[1];
			usart1_tx_temp[2] = backAddress[2];

			for (uint8_t i = 0; i < usart1_tx_len; i++)
			{
				usart1_tx_temp[i + 3] = usart1_tx_buffer[i];
			}

			dma_send(&huart3, &hdma_usart3_tx, usart1_tx_temp, usart1_tx_len + 3);
		}
		usart1_rx_flag = 0;
	}

	if (usart2_rx_flag == 1)
	{
		if (usart2_tx_buffer[0] == 0x24 && usart2_tx_buffer[1] == 0xff && usart2_tx_buffer[2] == 0xff)
		{
			if (usart2_tx_len == 6)
			{
				if (usart2_tx_buffer[3] == 1)
				{
					HAL_GPIO_WritePin(M0_GPIO_Port, M0_Pin, GPIO_PIN_SET);
				}
				else if (usart2_tx_buffer[3] == 0)
				{
					HAL_GPIO_WritePin(M0_GPIO_Port, M0_Pin, GPIO_PIN_RESET);
				}

				if (usart2_tx_buffer[4] == 1)
				{
					HAL_GPIO_WritePin(M1_GPIO_Port, M1_Pin, GPIO_PIN_SET);
				}
				else if (usart2_tx_buffer[4] == 0)
				{
					HAL_GPIO_WritePin(M1_GPIO_Port, M1_Pin, GPIO_PIN_RESET);
				}

				if (usart2_tx_buffer[5] == 1)
				{
					HAL_GPIO_WritePin(M2_GPIO_Port, M2_Pin, GPIO_PIN_SET);
				}
				else if (usart2_tx_buffer[5] == 0)
				{
					HAL_GPIO_WritePin(M2_GPIO_Port, M2_Pin, GPIO_PIN_RESET);
				}

				dma_send(&huart2, &hdma_usart2_tx, temp1, 3);
			}
			else
			{
				dma_send(&huart2, &hdma_usart2_tx, temp0, 3);
			}
		}
		else{
			uint8_t usart2_tx_temp[256];

			usart2_tx_temp[0] = backAddress[0];
			usart2_tx_temp[1] = backAddress[1];
			usart2_tx_temp[2] = backAddress[2];

			for (uint8_t i = 0; i < usart2_tx_len; i++)
			{
				usart2_tx_temp[i + 3] = usart2_tx_buffer[i];
			}

			dma_send(&huart3, &hdma_usart3_tx, usart2_tx_temp, usart2_tx_len + 3);
		}
		
		usart2_rx_flag = 0;
	}

	if (usart3_rx_flag == 1)
	{

		if (usart2_tx_buffer[0] == 0x25 && usart2_tx_buffer[1] == 0xff && usart2_tx_buffer[2] == 0xff)
		{

			uint8_t usart3_tx_temp[128];

			backAddress[0] = usart2_tx_buffer[3];
			backAddress[1] = usart2_tx_buffer[4];
			backAddress[2] = usart2_tx_buffer[5];

			for (uint8_t i = 0; i < usart3_tx_len - 3; i++)
			{
				usart3_tx_temp[i] = usart3_tx_buffer[i + 3];
			}

			dma_send(&huart1, &hdma_usart1_tx, usart3_tx_temp, usart3_tx_len - 3);
			dma_send(&huart2, &hdma_usart2_tx, usart3_tx_temp, usart3_tx_len - 3);
		}
		else
		{
			dma_send(&huart1, &hdma_usart1_tx, usart1_tx_buffer, usart3_tx_buffer);
			dma_send(&huart2, &hdma_usart2_tx, usart1_tx_buffer, usart3_tx_buffer);
		}

		usart3_rx_flag = 0;
	}
}