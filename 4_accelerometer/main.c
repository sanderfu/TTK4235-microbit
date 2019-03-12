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

    int* accel_buffer;
    accel_buffer = (int *)malloc(3*sizeof(int));

    data_buffer[0] = 9;
    
    uart_init();
    
    twi_init();
    
    accel_init();
    

    int sleep = 0;


    while(1) {
        //twi_multi_read(ACCEL_SLAVE, 0x0F, 1, data_buffer);
        
        //utility_print(&uart_send, "WHO AM I: %d\n", data_buffer[0]);

        accel_read_x_y_z(accel_buffer);
        

        for (int i = 0; i < 3; i++) {
            uart_send('X'+i);
            utility_print(&uart_send, ": %d\t", accel_buffer[i]);
            //utility_print(&uart_send, "%d", accel_buffer[i]);
            
        }
        uart_send('\n');
        
        sleep = 10000;
        while (--sleep);

    }



    free(data_buffer);

    return 0;
}