#include <stdio.h>
#include "uart.h"
#include "gpio.h"


void read_function(){
    //Read uart
    char data = uart_read();

    //Check if any data is present
    if (data == '\0'){
        return;
    }
    else {
        //Toggle on LED-matrix
        for(int i = 13; i <= 15; i++)
	    {
		    GPIO->OUT ^= (1 << i);
	    }
    }
}

ssize_t _write(int fd, const void *buf, size_t count) {
    char* letter = (char*)(buf);
    for (int i = 0; i < count; i++) {
        uart_send(*letter);
        letter++;
    }
    return count;
}

ssize_t _read(int fd, void *buf, size_t count) {
    char* str = (char*)(buf);
    char letter;
    do {
        letter = uart_read();
    } while (letter == '\0');

    *str = letter;
    return 1;
}

void ask_for_numbers() {
    
    int a;
    int b;    
    
    iprintf("Please write a number: \n\r");
    scanf(" %d", &a);
    iprintf("Please write another number: \n\r");
    scanf("%d", &b);
    int product = a*b;
    iprintf("%d * %d = %d\n\r", a,b,product);
}

int main() {
    
    // Configure buttons
	GPIO->PIN_CNF[17] = 0;
	GPIO->PIN_CNF[26] = 0;

    int sleep = 0;

    //Initialize uart
    uart_init();

    // Configure LED Matrix
	for(int i = 4; i <= 15; i++){
		GPIO->DIRSET |= (1 << i);
		GPIO->OUTCLR |= (1 << i);
	}

    

    while(1){

        /* Check if button A is pressed;
        * Send char 'A' if it is */

        if(!(GPIO->IN & (1 << 17)))
        {
            //uart_send('A');
            //iprintf("Norway has %d counties. \n\r", 18);
            ask_for_numbers();
        }

        /* Check if button B is pressed;
        * Send char 'B' if it is */
        
        if(!(GPIO->IN & (1 << 26)))
        {
            uart_send('B');
        }

        //read_function();
        

        sleep = 10000;
	    while(--sleep);

        
    }

    
    
   

    return 0;
}