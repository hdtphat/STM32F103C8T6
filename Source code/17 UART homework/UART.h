#ifndef UART_H
#define UART_H

// UART1
void UART1_Setup(void);
void UART1_PortInit(void);
void UART1_SendChar (char c);
uint8_t UART1_GetChar (void);
void UART1_SerialPrint(char *input);
void UART1_SerialPrintln(char *input);
void UART1_SerialRead(char rx_buffer[], int buffer_lenght);

// UART2
void UART2_PortInit(void);
void UART2_Setup(void);
void UART2_SendChar (char c);
uint8_t UART2_GetChar (void);
void UART2_SerialPrint(char *input);
void UART2_SerialPrintln(char *input);

#endif