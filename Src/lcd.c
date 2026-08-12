#include "lcd.h"
#include "main.h"
#include "tick.h"
#include <string.h>
#include <stdbool.h>

extern I2C_HandleTypeDef hi2c1;

#define LCD_I2C_ADDR   (0x27 << 1)

#define LCD_BIT_RS         0x01u
#define LCD_BIT_RW         0x02u
#define LCD_BIT_E          0x04u
#define LCD_BIT_BACKLIGHT  0x08u

#define LCD_MESSAGE_HOLD_MS 3000u

static uint8_t backlightState = LCD_BIT_BACKLIGHT;
static uint32_t revertTick = 0;
static bool     showingTemporary = false;

static void lcdI2cWrite(uint8_t data)
{
	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDR, &data, 1, 20);
	if(status != HAL_OK){
		HAL_I2C_DeInit(&hi2c1);
		HAL_I2C_Init(&hi2c1);
	}
}

static void lcdSendNibble(uint8_t nibble, uint8_t rs)
{
	uint8_t data = (nibble & 0xF0) | backlightState;
	if(rs){
		data |= LCD_BIT_RS;
	}

	lcdI2cWrite(data | LCD_BIT_E);
	HAL_Delay(1);
	lcdI2cWrite(data & ~LCD_BIT_E);
}

static void lcdSendByte(uint8_t value, uint8_t rs)
{
	lcdSendNibble(value & 0xF0, rs);
	lcdSendNibble((value << 4) & 0xF0, rs);
}

static void lcdCommand(uint8_t cmd)
{
	lcdSendByte(cmd, 0);
}

static void lcdData(uint8_t data)
{
	lcdSendByte(data, 1);
}

static void lcdShowIdle(void)
{
	lcdClear();
	lcdSetCursor(0, 0);
	lcdPrint("Whack-A-Mole");
}

void lcdInit(void)
{
	HAL_Delay(50);

	lcdSendNibble(0x30, 0);
	HAL_Delay(5);
	lcdSendNibble(0x30, 0);
	HAL_Delay(1);
	lcdSendNibble(0x30, 0);
	HAL_Delay(1);
	lcdSendNibble(0x20, 0);
	HAL_Delay(1);

	lcdCommand(0x28);
	lcdCommand(0x0C);
	lcdCommand(0x06);
	lcdClear();
	lcdShowIdle();
}

void lcdClear(void)
{
	lcdCommand(0x01);
	HAL_Delay(2);
}

void lcdSetCursor(uint8_t col, uint8_t row)
{
	static const uint8_t rowOffsets[2] = {0x00, 0x40};
	if(row > 1) row = 1;
	lcdCommand(0x80 | (col + rowOffsets[row]));
}

void lcdPrint(const char *str)
{
	while(*str){
		lcdData((uint8_t)(*str));
		str++;
	}
}

void lcdShowTemporary(const char *line0, const char *line1)
{
	lcdClear();
	lcdSetCursor(0, 0);
	lcdPrint(line0);
	lcdSetCursor(0, 1);
	lcdPrint(line1);

	revertTick = tickGet() + LCD_MESSAGE_HOLD_MS;
	showingTemporary = true;
}

void lcdUpdate(void)
{
	if(showingTemporary && (tickGet() >= revertTick)){
		lcdShowIdle();
		showingTemporary = false;
	}
}


