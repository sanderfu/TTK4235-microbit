#include "ubit_button.h"
#include "gpio.h"

void ubit_button_init(){
	GPIO->PIN_CNF[17] = 0;
	GPIO->PIN_CNF[26] = 0;
}

int ubit_button_press_a(){
	return (!(GPIO->IN & (1 << 17)));
}

int ubit_button_press_b(){
	return (!(GPIO->IN & (1 << 26)));
}
