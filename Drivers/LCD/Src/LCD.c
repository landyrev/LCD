#include <stdlib.h>
#include "stm32f2xx_hal.h"
#include "LCD.h"

#define pinE GPIO_PIN_8
#define pinRS GPIO_PIN_10
#define pinRW GPIO_PIN_9
#define DB7 GPIO_PIN_0
#define DB6 GPIO_PIN_1
#define DB5 GPIO_PIN_2
#define DB4 GPIO_PIN_3
#define DB3 GPIO_PIN_4
#define DB2 GPIO_PIN_5
#define DB1 GPIO_PIN_6
#define DB0 GPIO_PIN_7

void initLCD()
{
  HAL_Delay(500);
  WS0010_Command(0x3E); //Function set
  WS0010_Command(0x0C); //Display ON/OFF
  WS0010_Command(0x01); //Clear display
  WS0010_Command(0x02); //Return Home
  WS0010_Command(0x06); //Mode set
}

void changeDirection(uint32_t pin, uint32_t mode)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = pin;
  GPIO_InitStruct.Mode = mode;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  if (mode==GPIO_MODE_OUTPUT_PP) GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void WS0010_Command(char byte)
{
  waitBF();
  setPin(pinRS, 0); setPin(pinRW,0);
  writeByte(byte);
  setPin(pinE, 1); HAL_Delay(1);
  setPin(pinE, 0);
}

void WS0010_Data(char bt)
{
  waitBF(); //Wait until previous operation ends
  setPin(pinRS, 1); setPin(pinRW, 0);
  writeByte(bt); //Write Data
  setPin(pinE, 1);
  HAL_Delay(1); //Wait for some ms
  setPin(pinE, 0);
}

void writeByte(unsigned char byte)
{
  setPin(DB0, (byte >> 0) & 1); //set DB0
  setPin(DB1, (byte >> 1) & 1); //set DB0
  setPin(DB2, (byte >> 2) & 1); //set DB0
  setPin(DB3, (byte >> 3) & 1); //set DB0
  setPin(DB4, (byte >> 4) & 1); //set DB0
  setPin(DB5, (byte >> 5) & 1); //set DB0
  setPin(DB6, (byte >> 6) & 1); //set DB0
  setPin(DB7, (byte >> 7) & 1); //set DB0
}

void setPin(uint16_t pin, int state)
{
  if (state==0)
  {
    HAL_GPIO_WritePin(GPIOA, pin, GPIO_PIN_RESET); //reset E
  } else
  {
    HAL_GPIO_WritePin(GPIOA, pin, GPIO_PIN_SET); //set E
  }
}

void waitBF()
{
  changeDirection(DB7, GPIO_MODE_INPUT);
  setPin(pinRS, 0); setPin(pinRW, 1);
  GPIO_PinState flag = GPIO_PIN_SET;
  HAL_Delay(1);
  while (flag==GPIO_PIN_SET) {
    HAL_Delay(1);
    flag=HAL_GPIO_ReadPin(GPIOA, DB7);
  }
  setPin(pinRS, 0); setPin(pinRW, 0);
  HAL_Delay(1);
  changeDirection(DB7, GPIO_MODE_OUTPUT_PP);
}

void writeToLCD(char *s)
{
  WS0010_Command(0x8); //Turn off screen
  int i=0;
  while(s[i]!=0)
  {
    WS0010_Data(s[i]);
    i++;
  }
  WS0010_Command(0xC); //Turn off screen
}

void clearLCD()
{
  WS0010_Command(0x01); //Clear display
  WS0010_Command(0x02); //Return Home
}
