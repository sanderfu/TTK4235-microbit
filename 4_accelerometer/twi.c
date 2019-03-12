#include "twi.h"
#include "gpio.h"
#include <stdint.h>
#include "uart.h"

void twi_init(){
    //Set SCL and SDA pins as input
    GPIO->DIRCLR |= (1 << 30);
    GPIO->DIRCLR |= 1;

    //Set pins to pullup
    GPIO->PIN_CNF[0] |= 0b11 << 2;
    GPIO->PIN_CNF[30] |= 0b11 << 2;

    //Set drive strength
    GPIO->PIN_CNF[0] &= ~(8 << 8);
    GPIO->PIN_CNF[30] &= ~(8 << 8);


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
    
    TWIO->TXDSENT = 0;
    
    int i;
    for (i = 0; i < 10; i++) {
        __asm("nop");
    }

    
    //Start reading
    TWIO->STARTRX = 1;
    

    //Wait for data to be received and read continuously
    for (i = 0; i < registers_to_read-1; i++) {
        
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
    }
    data_buffer[registers_to_read-1] = TWIO->RXD;
    TWIO->RXDREADY = 0;
}

void twi_multi_write(uint8_t slave_address, uint8_t start_register, int registers_to_write, uint8_t * data_buffer) {
    //Set slave address register
    TWIO->ADDRESS = slave_address;
    
    //Start write function
    TWIO->STARTTX = 1;

    TWIO->TXDSENT = 0;

    //Set start register
    TWIO->TXD = start_register;

    while(!(TWIO->TXDSENT)) {
        //Wait..
    }

    //Send registers_to_write to bus
    int i;
    for (i = 0; i < registers_to_write; i++) {
        //Set TCDSENT to 0 again
        TWIO->TXDSENT = 0;

        //Fill TXD register
        TWIO->TXD = data_buffer[i];
        
        while(!(TWIO->TXDSENT)) {
            //Wait..
        }

        

    }

    //Stop writing
    TWIO->STOP = 1;

}