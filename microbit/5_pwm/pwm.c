#include "pwm.h"
#include "gpiote.h"
#include "ppi.h"
#include "timer.h"

void pwm_init(int prescaler, int period, int init_duty){
	// Task, pin 20, toggle, start on
	GPIOTE->CONFIG[0] = 3 | (20 << 8) | (3 << 16) | (1 << 20);

	PPI->PPI_CH[0].EEP = (uint32_t)&(TIMER1->COMPARE[0]);
	PPI->PPI_CH[0].TEP = (uint32_t)&(GPIOTE->OUT[0]);
	PPI->PPI_CH[1].EEP = (uint32_t)&(TIMER1->COMPARE[1]);
	PPI->PPI_CH[1].TEP = (uint32_t)&(GPIOTE->OUT[0]);

	PPI->PPI_CH[2].EEP = (uint32_t)&(TIMER1->COMPARE[2]);
	PPI->PPI_CH[2].TEP = (uint32_t)&(TIMER1->CAPTURE[0]);
	PPI->PPI_CH[3].EEP = (uint32_t)&(TIMER1->COMPARE[2]);
	PPI->PPI_CH[3].TEP = (uint32_t)&(GPIOTE->OUT[0]);

	PPI->CHENSET = (1 << 0) | (1 << 1) | (1 << 2);

	TIMER1->MODE = 0;					// Timer mode
	TIMER1->BITMODE = 3;				// 32 bit width
	TIMER1->SHORTS = (1 << 1);			// Clear on CC[1]
	TIMER1->PRESCALER = prescaler;
	TIMER1->CC[0] = init_duty;
	TIMER1->CC[1] = period;
	TIMER1->CC[2] = init_duty;
	TIMER1->START = 1;
}

void pwm_set_ticks(int ticks){
	if(ticks <= 0)
		ticks = 1;
	if(ticks >= TIMER1->CC[1])
		ticks = TIMER1->CC[1] - 1;

	if(ticks + (TIMER1->PRESCALER == 0 ? 1 : 0) < TIMER1->CC[0])
		PPI->CHENSET = (1 << 3);
	else
		PPI->CHENCLR = (1 << 3);
	TIMER1->CC[2] = ticks;
}
