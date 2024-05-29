#include "stm32f10x.h"
#include "LCD.h"
#include "Delay.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void LCD_DisplayChar(char c){
	LCD_SendData(c >> 4);    // send 4 upper bit
	LCD_SendData(c & 0x0f);  // send 4 lower bit
}

void LCD_DisplayString(char str[32]){
	uint8_t k = 0;
	while(str[k]!= '\0'){
		if(k==16){
			LCD_SendCmd(0x0C);   // move cursor to 2nd row
			LCD_SendCmd(0x00);
		}
		LCD_DisplayChar(str[k]);
		k++;
	}
}

void LCD_SendCmd(uint8_t cmd){
	GPIOB->ODR &= ~(0x0f<<8); // reset PB8~PB11 
	GPIOB->ODR &= ~(1UL<<12);	// R/S = 0
	GPIOB->ODR |= (1<<13); 	// E = 1
	GPIOB->ODR |= (cmd << 8); 
	Delay(1);
	GPIOB->ODR &= ~(1UL<<13); // E = 0
	GPIOB->ODR |= (1<<12);	// R/S = 1	
	Delay(1);
}

void LCD_SendData(uint8_t data){
	GPIOB->ODR &= ~(0x0f<<8); // reset PB8~PB11 
	GPIOB->ODR |= (1UL<<12);	// R/S = 1
	GPIOB->ODR |= (1<<13); 	// E = 1
	GPIOB->ODR |= (data << 8); 
	Delay(1);
	GPIOB->ODR &= ~(1UL<<13); // E = 0
	Delay(1);
}

/*=============================================================================================================*/

void LCD_Init(void){
	Delay(5);
	LCD_SendCmd(0x02);  	// 4-bit mode
	LCD_SendCmd(0x02);  	// 4-bit mode, 2 lines, 5x8 dots
	LCD_SendCmd(0x08);
	LCD_SendCmd(0x00);	// display on, cursor on, cursor blinking
	LCD_SendCmd(0x0F);
	LCD_SendCmd(0x00);	// display on, cursor on, cursor blinking
	LCD_SendCmd(0x06);	// Entry mode, increment cursor, no shift
}

void LCD_ClearAll(void){
	LCD_SendCmd(0x00);
	Delay(1);
	LCD_SendCmd(0x01);
	Delay(1);
}

void LCD_DownLine(void){
	LCD_SendCmd(0x0C);
	Delay(1);
	LCD_SendCmd(0x00);
	Delay(1);
}

void LCD_MoveCursor(uint8_t x, uint8_t y){
	if(y==0){
		LCD_SendCmd(0x08);	// Upper row
		LCD_SendCmd(x);
	} else {
		LCD_SendCmd(0x0C);	// Lower row
		LCD_SendCmd(x);
	}
}

void LCD_DisplayArray(uint8_t *data_array, uint8_t array_size){
	for(int i=0; i<array_size; i++){
		LCD_SendData(data_array[i]);
	}
}

void LCD_DisplayFloat(float input_float){
	char str[20];
    snprintf(str, sizeof(str), "%.2f", input_float);
	for(int i=0; str[i] != '\0'; i++){
		switch (str[i]){
			case '9':
				LCD_SendData(0x03);
				LCD_SendData(0x09);
				break;
			case '8':
				LCD_SendData(0x03);
				LCD_SendData(0x08);
				break;
			case '7':
				LCD_SendData(0x03);
				LCD_SendData(0x07);
				break;
			case '6':
				LCD_SendData(0x03);
				LCD_SendData(0x06);
				break;
			case '5':
				LCD_SendData(0x03);
				LCD_SendData(0x05);
				break;
			case '4':
				LCD_SendData(0x03);
				LCD_SendData(0x04);
				break;
			case '3':
				LCD_SendData(0x03);
				LCD_SendData(0x03);
				break;
			case '2':
				LCD_SendData(0x03);
				LCD_SendData(0x02);
				break;
			case '1':
				LCD_SendData(0x03);
				LCD_SendData(0x01);
				break;
			case '0':
				LCD_SendData(0x03);
				LCD_SendData(0x00);
				break;
			case '.':
				LCD_SendData(0x02);
				LCD_SendData(0x0E);
				break;
		}
	}
}
