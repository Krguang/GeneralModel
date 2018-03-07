#include "dataProcessing.h"
#include "usart.h"


void e70Config() {

}


void dataProcessing() {

	if (usart1_rx_flag == 1)
	{
		if (usart1_tx_buffer[0] == '$')
		{

		}
		else {
			dma_send(&huart3, &hdma_usart3_tx, usart1_tx_buffer, usart1_tx_len);
		}
		usart1_rx_flag = 0;
	}

	if (usart2_rx_flag == 1)
	{
		if (usart2_tx_buffer[0] == '$')
		{

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