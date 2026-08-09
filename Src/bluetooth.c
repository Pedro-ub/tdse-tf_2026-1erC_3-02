#include "bluetooth.h"
#include "main.h"
#include "game.h"
#include "storage.h"
#include "tick.h"
#include "lcd.h"
#include <string.h>
#include <stdio.h>

extern UART_HandleTypeDef huart1;

#define BT_RX_BUFFER_SIZE   32u
#define BT_RX_IDLE_MS       50u   /* si no llega nada nuevo en 50ms, se considera terminado el mensaje */

static uint8_t  rxByte;
static char     rxLine[BT_RX_BUFFER_SIZE];
static uint8_t  rxIndex = 0;
static uint32_t lastByteTick = 0;
static volatile bool byteArrived = false;

static void processLine(const char *line)
{
	int value = -1;
	if(sscanf(line, "DIFF:%d", &value) == 1){
		if(value >= 0 && value < GAME_DIFFICULTY_COUNT){
			gameSetDifficulty((uint8_t)value);
			storageSetDifficulty((uint8_t)value);
			lcdShowTemporary("Dificultad:", gameGetDifficultyName());
		}
	}
}

void bluetoothInit(void)
{
	rxIndex = 0;
	lastByteTick = tickGet();
	HAL_UART_Receive_IT(&huart1, &rxByte, 1);
}

void bluetoothUpdate(void)
{
	if(rxIndex > 0 && (tickGet() - lastByteTick) >= BT_RX_IDLE_MS){
		rxLine[rxIndex] = '\0';
		processLine(rxLine);
		rxIndex = 0;
	}
}

void bluetoothSendScore(uint16_t score)
{
	char msg[24];
	int len = snprintf(msg, sizeof(msg), "SCORE:%u", score);
	HAL_UART_Transmit(&huart1, (uint8_t *)msg, (uint16_t)len, 100);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){
		if(rxByte != '\n' && rxIndex < (BT_RX_BUFFER_SIZE - 1)){
			rxLine[rxIndex++] = (char)rxByte;
		}
		lastByteTick = tickGet();
		HAL_UART_Receive_IT(&huart1, &rxByte, 1);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){
		volatile uint32_t errorCode = huart->ErrorCode;
		(void)errorCode;
		HAL_UART_Receive_IT(&huart1, &rxByte, 1);   /* intenta recuperar la recepcion */
	}
}
