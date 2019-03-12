#ifndef UART_H
#define UART_H

void uart_init();

void uart_send(char letter);

char uart_read();

void uart_print_string(char * str);

void uart_print_int(int number);

#endif
