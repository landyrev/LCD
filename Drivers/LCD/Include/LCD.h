#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED
void setPin(uint16_t pin, int state);
void writeByte(unsigned char byte);
void WS0010_Command(char byte);
void WS0010_Data(char byte);
void waitBF();
void writeToLCD(char *s);
void changeDirection(uint32_t pin, uint32_t mode);
void initLCD();
void clearLCD();

#endif
