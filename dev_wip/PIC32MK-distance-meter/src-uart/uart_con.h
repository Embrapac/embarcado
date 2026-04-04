#ifndef UART_CON_H
#define UART_CON_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

// Ajuste aqui quando definir o clock real do projeto
#define PBCLK2_FREQ     50000000UL   // valor temporário
#define UART_BAUD       115200UL

void UART2_Init(void);
void UART2_Write(char c);
void UART2_WriteString(const char *s);
char UART2_Read(void);
bool UART2_Available(void);
void PPS_Config(void);

#endif // UART_CON_H
