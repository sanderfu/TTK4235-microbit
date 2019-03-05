#include <stdio.h>
#include <stdlib.h>
#include "twi.h"
#include "accel.h"
#include "utility.h"
#include "gpio.h"
#include "uart.h"

int main(){

    uint8_t* data_buffer;
    data_buffer = (uint8_t *)malloc(1 * sizeof(uint8_t));

    data_buffer[0] = 9;

    uart_init();
    twi_init();
    
    int sleep = 0;


    while(1) {
        twi_multi_read(0x5A, 0x0D, 1, data_buffer);

        uart_send('0' + (char)data_buffer[0]);
        //uart_send('C');


        sleep = 10000;
        while (--sleep);

    }



    free(data_buffer);

    return 0;
}