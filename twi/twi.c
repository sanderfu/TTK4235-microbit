#include <stdint.h>
#include "twi.h"
#include "gpio.h"

#define TWI0 ((NRF_TWI_REG*)0x40003000)

typedef struct {
	// Tasks
	volatile uint32_t STARTRX;
	volatile uint32_t RESERVED0;
	volatile uint32_t STARTTX;
	volatile uint32_t RESERVED1[2];
	volatile uint32_t STOP;
	volatile uint32_t RESERVED2;
	volatile uint32_t SUSPEND;
	volatile uint32_t RESUME;
	// Events
	volatile uint32_t RESERVED3[56];
	volatile uint32_t STOPPED;
	volatile uint32_t RXDREADY;
	volatile uint32_t RESERVED4[4];
	volatile uint32_t TXDSENT;
	volatile uint32_t RESERVED5;
	volatile uint32_t ERROR;
	volatile uint32_t RESERVED6[4];
	volatile uint32_t BB;
	// Registers
	volatile uint32_t RESERVED7[49];
	volatile uint32_t SHORTS;
	volatile uint32_t RESERVED8[63];
	volatile uint32_t INTEN;
	volatile uint32_t INTENSET;
	volatile uint32_t INTENCLR;
	volatile uint32_t RESERVED9[110];
	volatile uint32_t ERRORSRC;
	volatile uint32_t RESERVED10[14];
	volatile uint32_t ENABLE;
	volatile uint32_t RESERVED11;
	volatile uint32_t PSELSCL;
	volatile uint32_t PSELSDA;
	volatile uint32_t RESERVED12[2];
	volatile uint32_t RXD;
	volatile uint32_t TXD;
	volatile uint32_t RESERVED13;
	volatile uint32_t FREQUENCY;
	volatile uint32_t RESERVED14[24];
	volatile uint32_t ADDRESS;
} NRF_TWI_REG;

void twi_init(){

    /* Your task: */

    /* 1) To use TWI, you must configure the SDA- and SCL lines in */
    /*    the GPIO module. Read the TWI section in the nRF51822 */
    /*    datasheet to determine which direction the pins should */
    /*    have, as well as what drive strength you should apply. */

    /* 2) Use pin 0 on the micro:bit as SCL; 30 as SDA. */

    /* 3) Use normal I2C speed, i.e. 100 kHz operation. */

}

void twi_multi_read(
		uint8_t slave_address,
		uint8_t start_register,
		int registers_to_read,
		uint8_t * data_buffer
		){

    /* Your task: */

    /* 1) Write the register address you want to the slave */
    /*    device. Busy-wait until the register address has */
    /*    been sent by the TWI peripheral. */



    /* As explained in the guidance lecture, these "no-operation" */
    /* instructions are necessary because of a timing issue between */
    /* nRF51822 and the LSM303AGR. The reason we use inline assembly */
    /* is to always force the compiler to keep these instructions, */
    /* regardless of optimization level. */
    for(int i = 0; i < 10; i++){
        __asm("nop");
    }

    /* Your task: */

    /* 1) Read back the register that you asked the slave to */
    /*    supply. This amounts to generating a repeated start */
    /*    condition, and reading the amount of registers you */
    /*    want. */

    /* 2) Remember that you need to generate a NACK at */
    /*    the of the sequence, read the TWI section to figure */
    /*    out how to do this. */

}

void twi_multi_write(
		uint8_t slave_address,
		uint8_t start_register,
		int registers_to_write,
		uint8_t * data_buffer
		){
	TWI0->ADDRESS = slave_address;
	TWI0->STARTTX = 1;

	TWI0->TXDSENT = 0;
	TWI0->TXD = start_register;
	while(!TWI0->TXDSENT);

	for(int i = 0; i < registers_to_write; i++){
		TWI0->TXDSENT = 0;
		TWI0->TXD = data_buffer[i];
		while(!TWI0->TXDSENT);
	}

	TWI0->STOP = 0;
}
