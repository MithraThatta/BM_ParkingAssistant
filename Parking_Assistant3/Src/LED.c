#include "stm32f103xb.h"
#include <stdio.h>
#include <stdint.h>



void LEDInit(void)
{
	RCC->APB2ENR |= (1U<<2); //enable clockbus access to GPIOA to setup LEDS
	RCC->APB2ENR |= (1U<<4); //enable clockbus access to GPIOC to setup LEDs

	//initialize PA5, arduino port D13, controls the red LEDs
	GPIOA->CRL &=~ (1U<<23);
	GPIOA->CRL &=~ (1U<<22);
	GPIOA->CRL |= (1U<<21);
	GPIOA->CRL &=~ (1U<<20);

	//iniitialize PA6, Arduino port D12, controls green light next to red leds
	GPIOA->CRL &=~ (1U<<27);
	GPIOA->CRL &=~ (1U<<26);
	GPIOA->CRL |= (1U<<25);
	GPIOA->CRL &=~ (1U<<24);


	//initialize PA7, Arduino port D11, controls middle green light
	GPIOA->CRL &=~ (1U<<31);
	GPIOA->CRL &=~ (1U<<30);
	GPIOA->CRL |= (1U<<29);
	GPIOA->CRL &=~ (1U<<28);


	//initialize PA9, Arduino port D8, controls green light next to blue LEDs
	GPIOA->CRH &=~ (1U<<7);
	GPIOA->CRH &=~ (1U<<6);
	GPIOA->CRH |= (1U<<5);
	GPIOA->CRH &=~ (1U<<4);


	//initialize PA8, Arduino port D7, controls blue light next to green LEDs
	GPIOA->CRH &=~ (1U<<3);
	GPIOA->CRH &=~ (1U<<2);
	GPIOA->CRH |= (1U<<1);
	GPIOA->CRH &=~ (1U<<0);


	//initialize PC7, Arduino port D9, controls edge blue light
	GPIOC->CRL &=~ (1U<<31);
	GPIOC->CRL &=~ (1U<<30);
	GPIOC->CRL |= (1U<<29);
	GPIOC->CRL &=~ (1U<<28);
	return;


}


