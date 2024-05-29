#include "stm32f10x.h"                  // Device header
#include "LCD.h"
#include "TIM6_delay.h"
#include <stdint.h>
#include <stdio.h>

#define RS 12 // PB12 -> R/S
#define E 13 // PB13 -> E
#define DATA4 8 // PB8 -> D4
#define DATA5 9 // PB9 -> D5
#define DATA6 10 // PB10 -> D6
#define DATA7 11 // PB11 -> D7

void LCD_PortConf(void){
	// enable clock to GPIOB
	RCC->APB2ENR |= (1U<<3);
	// set PB8~13 as output
	GPIOB->CRH &= ~(0x00FFFFFFUL);
	GPIOB->CRH |= 0x00222222;
}

void LCD_SendCmd(uint8_t cmd){
	GPIOB->ODR &= ~(0x0FU<<DATA4); // reset PB8~PB11 
	GPIOB->ODR &= ~(1UL<<RS);	// R/S = 0
	GPIOB->ODR |= (1<<E); // E = 1
	GPIOB->ODR |= (cmd << DATA4); // Send 1 byte of command
	delay_ms(1);
	GPIOB->ODR &= ~(1UL<<E); // E = 0
	GPIOB->ODR |= (1<<RS); // R/S = 1	
	delay_ms(1);
}

void LCD_SendData(uint8_t data){
	GPIOB->ODR &= ~(0x0FU<<DATA4); // reset PB8~PB11 
	GPIOB->ODR |= (1UL<<RS);	// R/S = 1
	GPIOB->ODR |= (1<<E); // E = 1
	GPIOB->ODR |= (data << DATA4); // Send 1 byte of data
	delay_ms(1);
	GPIOB->ODR &= ~(1UL<<E); // E = 0
	delay_ms(1);
}

/*=============================================================================================================*/

void LCD_Init(void){
	delay_ms(3);
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
	delay_ms(1);
	LCD_SendCmd(0x01);
	delay_ms(1);
}

void LCD_DownLine(void){
	LCD_SendCmd(0x0C);
	delay_ms(1);
	LCD_SendCmd(0x00);
	delay_ms(1);
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
