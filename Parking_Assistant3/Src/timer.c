#include "stm32f103xb.h"
#include <stdio.h>
#include <stdint.h>
//default clock speed is 8mhz,
void Tim4InputCaptureSetup(void)
{
	//configuring the pb6 (tim4 ch1) pin
	RCC->APB2ENR |= (1U<<3); //clock bus access to GPIOB ports
	GPIOB->CRL |= (1U<<27);
	GPIOB->CRL &=~ (1U<<26); // sets CNFy to 10 for Input with pull up/pull down
	GPIOB->CRL &=~ (1U<<25);
	GPIOB->CRL &=~ (1U<<24); // sets MODE to 00 for input
	GPIOB->ODR &=~ (1U<<6); // sets the input pin as pull down-when not driven by ECHO it will be zero



	//configuring the timer4 module
	RCC->APB1ENR |= (1U<<2); //enable clock access to timer4
	TIM4->PSC = 7; //clock speed is now 1 mhz-each tick is 1 microsecond
	TIM4->ARR = 65000; //sets largest possible period to avoid having to deal with wraparound logic on input capture

	TIM4->CCMR1 &=~ (1U<<1);
	TIM4->CCMR1 |= (1U<<0); //set CCIS in ccmr reg to 01

	TIM4->CCER &=~ (1U<<1); //cc1p bit set to zero-checks for rising edge
	TIM4->CCER |= (1U<<0); //capture/compare 1 is enabled

	TIM4->DIER |= (1U<<1); //enable capture/compare interrupt for channel 1
	TIM4->SR &=~ (1U<<1); //clear the cc1If flag and set it to zero
	NVIC_EnableIRQ(TIM4_IRQn);
	__enable_irq(); //enables global interrupts

	TIM4->CR1 |= (1U<<0); //enable the counter
	printf("doneIC\n"); //used to check if function successfully executes
}



void systickDelayuS(int Delay)
{
	SysTick->LOAD = 7; // the counter flags everytime 8 (7+1) is reached, which happens every microsecond as default clock speed is 8 MHz
	SysTick->VAL = 0; // starting value of the counter which allows for the 1 uS delay
	SysTick->CTRL = (1U<<0) | (1U<<2); //enable the counter

	for(int i = 0;i<Delay;i++) //for loop which executes for Delay number of times. Due to overhead, this function creates a delay 11-13 uS more than Delay
	{
		while((SysTick->CTRL & (1U<<16))==0)
		{

		}
	}
	SysTick->CTRL = 0; //disable timer after for loop has executed Delay number of times
}

void testpulseINIT(void)
{
	//setup PB10 to provide test output
	RCC->APB2ENR |= (1U<<3); //enable clock access to GPIOB module
	GPIOB->CRH &=~ (1U<<11);
	GPIOB->CRH &=~ (1U<<10);
	GPIOB->CRH |= (1U<<9);
	GPIOB->CRH &=~ (1U<<8); //CNFMode set to 0010, output

}
void testpulse(void) // sends a roughly 11 microsecond pulse by turning on PB10, waiting 11 uS, and turning it off
{
	GPIOB->BSRR = (1U<<10);
	systickDelayuS(0); //although I pass a zero in, due to the execution time of the systick delay function, this is effectively a 11 uS delay
	GPIOB->BSRR = (1U<<26);
}

