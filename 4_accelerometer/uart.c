#include <stdint.h>
#include "uart.h"
#include "gpio.h"

#define UART ((NRF_UART_REG*)0x40002000)
#define RXD_PIN 25
#define TXD_PIN 24


typedef struct {
    volatile uint32_t STARTRX;
    volatile uint32_t STOPRX;
    volatile uint32_t STARTTX;
    volatile uint32_t STOPTX;
    volatile uint32_t RESERVED_0[3];
    volatile uint32_t SUSPEND;

    volatile uint32_t RESERVED_1[56];
    volatile uint32_t CTS;
    volatile uint32_t NCTS;
    volatile uint32_t RXDRDY;
    volatile uint32_t RESERVED_2[4];
    volatile uint32_t TXDRDY;
    volatile uint32_t RESERVED_3;
    volatile uint32_t ERROR;
    volatile uint32_t RESERVED_4[7];
    volatile uint32_t RXTO;
    volatile uint32_t RESERVED_5[110];

    volatile uint32_t INTEN;
    volatile uint32_t INTENSET;
    volatile uint32_t INTENCLR;
    volatile uint32_t RESERVED_6[93];
    volatile uint32_t ERRORSRC;
    volatile uint32_t RESERVED_7[31];
    volatile uint32_t ENABLE;
    volatile uint32_t RESERVED_8;
    volatile uint32_t PSELRTS;
    volatile uint32_t PSELTXD;
    volatile uint32_t PSELCTS;
    volatile uint32_t PSELRXD;
    volatile uint32_t RXD;
    volatile uint32_t TXD;
    volatile uint32_t RESERVED_9;
    volatile uint32_t BAUDRATE;
    volatile uint32_t RESERVED_10[17];
    volatile uint32_t CONFIG;


} NRF_UART_REG;

void uart_init() {
    //Set up TXD_PIN and RXD_PIN for UART
    GPIO->DIRSET |= (1 << TXD_PIN); //output
    GPIO->DIRCLR |= (1 << RXD_PIN); //input

    //Set pins for RXD and TXD
    UART->PSELTXD = TXD_PIN;
    UART->PSELRXD = RXD_PIN;

    //SET baudrate to 9600
    UART->BAUDRATE = 0x00275000;

    //Disable CTS
    UART->PSELCTS = 0xFFFFFFFF;
    
    //Disable RTS
    UART->PSELRTS = 0xFFFFFFFF;

    //Enable uart
    UART->ENABLE = 4;

    //Start recieving
    UART->STARTRX = 1;

}

void uart_send(char letter) {
    //Start transmitting 
    UART->STARTTX = 1;

    //Write byte to send to TXD register
    UART->TXD = letter;

    //Check if byte has been sent
    while(!(UART->TXDRDY)){
        //wait..
    }
    //Set register low again to set up for new transmission
    UART->TXDRDY = 0;

    //Stop transmission
    UART->STOPTX = 1;

}

char uart_read(){
    //Start recieving
    UART->STARTRX = 1;

    //Clear RXDRDY
    UART->RXDRDY = 0;

    //Read data
    char data = UART->RXD;

    if(data) {
        return data;
    }
    else {
        return '\0';
    }

    //Stop recieving
    UART->STOPRX = 1;
}
