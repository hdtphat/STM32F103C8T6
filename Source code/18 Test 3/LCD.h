#ifndef LCD_H
#define LCD_H

void LCD_DisplayString(char str[32]);
void LCD_DisplayChar(char c);

void LCD_SendCmd(uint8_t cmd);
void LCD_SendData(uint8_t data);

void LCD_Init(void);
void LCD_ClearAll(void);
void LCD_DownLine(void);
void LCD_MoveCursor(uint8_t x, uint8_t y);

void LCD_DisplayArray(uint8_t *data, uint8_t size);
void LCD_DisplayFloat(float input_float);

#endif
