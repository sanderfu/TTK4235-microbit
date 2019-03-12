#include <stdint.h>
#include "uart.h"
#include "gpio.h"

#define UART ((NRF_UART_REG*)0x40002000)

typedef struct {
	// Tasks
	volatile uint32_t STARTRX;
	volatile uint32_t STOPRX;
	volatile uint32_t STARTTX;
	volatile uint32_t STOPTX;
	volatile uint32_t RESERVED0[3];
	volatile uint32_t SUSPEND;
	// Events
	volatile uint32_t RESERVED1[56];
	volatile uint32_t CTS;
	volatile uint32_t NCTS;
	volatile uint32_t RXDRDY;
	volatile uint32_t RESERVED2[4];
	volatile uint32_t TXDRDY;
	volatile uint32_t RESERVED3;
	volatile uint32_t ERROR;
	volatile uint32_t RESERVED4[7];
	volatile uint32_t RXTO;
	// Registers
	volatile uint32_t RESERVED5[110];
	volatile uint32_t INTEN;
	volatile uint32_t INTENSET;
	volatile uint32_t INTENCLR;
	volatile uint32_t RESERVED6[93];
	volatile uint32_t ERRORSRC;
	volatile uint32_t RESERVED7[31];
	volatile uint32_t ENABLE;
	volatile uint32_t RESERVED8;
	volatile uint32_t PSELRTS;
	volatile uint32_t PSELTXD;
	volatile uint32_t PSELCTS;
	volatile uint32_t PSELRXD;
	volatile uint32_t RXD;
	volatile uint32_t TXD;
	volatile uint32_t RESERVED9;
	volatile uint32_t BAUDRATE;
	volatile uint32_t RESERVED10[17];
	volatile uint32_t CONFIG;
} NRF_UART_REG;

void uart_init(){
	GPIO->PIN_CNF[24] = 1; // Configure output
	GPIO->PIN_CNF[25] = 0; // Configure input

	UART->PSELRTS = 0xFFFFFFFF; // Disable RTS
	UART->PSELCTS = 0xFFFFFFFF; // Disable CTS

	UART->PSELTXD = 24;
	UART->PSELRXD = 25;

	UART->BAUDRATE = 0x00275000; // 9600 baud

	UART->ENABLE = 4; // 4: Enable, 0: Disable

	UART->STARTRX = 1; // Start accepting messages
}

void uart_send(char letter){
	UART->STARTTX = 1;
	UART->TXD = letter;
	while(!UART->TXDRDY);
	UART->TXDRDY = 0;
	UART->STOPTX = 1;
}

char uart_read(){
	if(!UART->RXDRDY)
		return '\0';

	char received;
	UART->RXDRDY = 0;
	received = UART->RXD;
	return received;
}

void uart_print_string(char * str){
	int index = 0;
	UART->STARTTX = 1;
	while(str[index] != '\0'){
		UART->TXD = str[index];
		while(!UART->TXDRDY);
		UART->TXDRDY = 0;
		index++;
	}
	UART->STOPTX = 1;
}

void uart_print_int(int number){
	if(number == 0){
		UART->STARTTX = 1;
		UART->TXD = '0';
		while(!UART->TXDRDY);
		UART->TXDRDY = 0;
		return;
	}

	int digits = 0;
	int reverse = 0;
	int print_negative_sign = 0;

	if(number < 0){
		print_negative_sign = 1;
		number = -number;
	}

	while(number){
		reverse *= 10;
		reverse += (number % 10);
		number /= 10;
		digits++;
	}

	UART->STARTTX = 1;
	if(print_negative_sign){
		UART->TXD = '-';
		while(!UART->TXDRDY);
		UART->TXDRDY = 0;
	}
	while(reverse){
		UART->TXD = (char)(reverse % 10 + '0');
		while(!UART->TXDRDY);
		UART->TXDRDY = 0;
		reverse /= 10;
		digits--;
	}
	while(digits){
		UART->TXD = '0';
		while(!UART->TXDRDY);
		UART->TXDRDY = 0;
		digits--;
	}
	UART->STOPTX = 1;
}
