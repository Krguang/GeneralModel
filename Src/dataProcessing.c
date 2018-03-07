#include "dataProcessing.h"
#include "usart.h"

	uint8_t temp1[3] = { 0xff,0xff,0xff };
	uint8_t temp0[3] = { 0,0,0 };

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
			dma_send(&huart3, &hdma_usart3_tx, usart1_tx_buffer, usart1_tx_len);
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
		else {
			dma_send(&huart3, &hdma_usart3_tx, usart2_tx_buffer, usart2_tx_len);
		}
		
		usart2_rx_flag = 0;
	}

	if (usart3_rx_flag == 1)
	{
		dma_send(&huart1, &hdma_usart1_tx, usart3_tx_buffer, usart3_tx_len);
		dma_send(&huart2, &hdma_usart2_tx, usart3_tx_buffer, usart3_tx_len);
		usart3_rx_flag = 0;
	}
}