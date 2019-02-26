#include "gpio.h"
#include "gpiote.h"
#include "ppi.h"


void GPIOTE_init() {
    //Configure LED-matrix
    for (int i = 4; i <= 12; i++){
        GPIO->DIRSET |= (1 << i);
        GPIO->OUTCLR |= (1 << i);
    }

    //Config channel A
    GPIOTE->CONFIG[0] |= (0b10001 << 8); //Set pin 17 (BTN A) as port in PSEL
    GPIOTE->CONFIG[0] |= (1); //Set BTN A as event
    GPIOTE->CONFIG[0] |= (0b10 << 16); //Set polarity high to low

    //Config channel B
    GPIOTE->CONFIG[1] |= (0b1101 << 8); //Set pin 13 as port in PSEL
    GPIOTE->CONFIG[1] |= (3);  //Set pin 13 as task
    GPIOTE->CONFIG[1] |= (0b11 << 16); //Set pin 13 to toggle
    GPIOTE->CONFIG[1] |= (1 << 20); //Set init value

    //Config channel C
    GPIOTE->CONFIG[2] |= (0b1110 << 8); //Set pin 14 as port in PSEL
    GPIOTE->CONFIG[2] |= (3);  //Set pin 14 as task
    GPIOTE->CONFIG[2] |= (0b11 << 16); //Set pin 14 to toggle
    GPIOTE->CONFIG[2] |= (1 << 20); //Set init value
    
    //Config channel D
    GPIOTE->CONFIG[3] |= (0b1111 << 8); //Set pin 15 as port in PSEL
    GPIOTE->CONFIG[3] |= (3);  //Set pin 15 as task
    GPIOTE->CONFIG[3] |= (0b11 << 16); //Set pin 15 to toggle
    GPIOTE->CONFIG[3] |= (1 << 20); //Set init value


}

void PPI_init() {
    //Enable channels 0,1,2
    PPI->CHEN |= 0b111;

    //Set channel endpoints
    PPI->PPI_CH[0].EEP = (uint32_t)&(GPIOTE->IN[0]);
    PPI->PPI_CH[0].TEP = (uint32_t)&(GPIOTE->OUT[1]);

    PPI->PPI_CH[1].EEP = (uint32_t)&(GPIOTE->IN[0]);
    PPI->PPI_CH[1].TEP = (uint32_t)&(GPIOTE->OUT[2]);

    PPI->PPI_CH[2].EEP = (uint32_t)&(GPIOTE->IN[0]);
    PPI->PPI_CH[2].TEP = (uint32_t)&(GPIOTE->OUT[3]);
    
}

int main() {

    GPIOTE_init();

    PPI_init();

    while(1){

    }


    return 0;
}