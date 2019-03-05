#include "twi.h"
#include "gpio.h"
#include <stdint.h>
#include "uart.h"

void twi_init(){
    //Set SCL and SDA pins as input
    GPIO->DIRCLR |= (1 << 30);
    GPIO->DIRCLR |= 1;

    //Enable TWI
    TWIO->ENABLE = 5;

    //Set pins for SCL and SDA
    TWIO->PSELSCL = 0;
    TWIO->PSELSDA = 30;

    //Set speed
    TWIO->FREQUENCY = 0x01980000;
}

void twi_multi_read(uint8_t slave_address, uint8_t start_register, int registers_to_read, uint8_t* data_buffer) {
    //Set slave address register
    TWIO->ADDRESS = slave_address;
    
    //Start write function
    TWIO->STARTTX = 1;

    //Set TXDSENT = 0 just to be sure
    TWIO->TXDSENT = 0;
    
    //Fill TXD register with start_register
    TWIO->TXD = start_register;
    
    //Wait for TXDSENT acknowledge that the message has been sent
    while(!(TWIO->TXDSENT)){
        //Wait..
    }
    uart_send('Q');
    TWIO->TXDSENT = 0;

    //Start reading
    TWIO->STARTRX = 1;

    //Wait for data to be received and read continuously
    for (int i = 0; i < registers_to_read-1; i++) {
        while (!(TWIO->RXDREADY)) {
                //Wait..
               
            }
        data_buffer[i] = TWIO->RXD;
        TWIO->RXDREADY = 0;
    }

    //Stop receiving
    TWIO->STOP = 1;

    //Read last byte and NACK slave
    while(!(TWIO->RXDREADY)){
        //Wait..
        uart_send('Y');
    }
    data_buffer[registers_to_read-1] = TWIO->RXD;
    TWIO->RXDREADY = 0;
}