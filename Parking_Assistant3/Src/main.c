#include "stm32f103xb.h"
#include <stdio.h>
#include <stdint.h>
#include <uart.h>
#include <timer.h>
#include <LED.h>



static int volatile flag = 0;
static volatile uint32_t capture1 = 0;
static volatile uint32_t capture2 = 0;
static volatile int sum = 0;
static volatile int oldsum = 0;
static volatile int PrintFlag = 0;
static volatile int LEDtimer = 0;
static volatile int LEDAutoOFF=0;
void LEDUpdate(void);
void printResult(void);
int main(void)
{
	RXNEIEInterrupt();
	testpulseINIT();
	Tim4InputCaptureSetup();
	LEDInit();

    while(1)
    {

    	testpulse();
    	systickDelayuS(10000);
    	 while (!PrintFlag){};

    	if(PrintFlag ==1)
    	    	{
    	    		printResult();
    	    		LEDUpdate();
    	    		printf("%d\n", LEDAutoOFF);

    	    	}
    	oldsum = sum; //oldsum used to optimize LED light setting-if sum= oldsum, no change in distance, so no LED changes required
    	systickDelayuS(70000);
    }

}

void USART2_IRQHandler(void)
{

	if (USART2->SR & (1U<<7))
	{
		if(EndCheck()==1)
			{
				USART2->CR1 &=~ (1U<<7);
				bufferReset();

			}

			if (EndCheck()==0)
			{
				USART2->DR = bufferpop();
			}
	}
}

void TIM4_IRQHandler(void)
{
	if(TIM4->SR & (1U<<1)) //if CC1F is 1, a CCR value has been stored
	{
		if(flag == 0)
		{

			capture1 = TIM4->CCR1; //this automatically clears CC1IF
			flag = 1;
			TIM4->CCER |= (1U<<1); //makes the next update event a falling edge instead of rising edge
		}

		else if (flag == 1)
		{

			capture2 = TIM4->CCR1; //this automatically clears CC1IF
			TIM4->CCER &=~ (1U<<1); //reset polarity bit to trigger on rising edge again
			flag = 0;
			PrintFlag = 1;

		}

	}

}

void printResult(void)
{
	if(capture1 >= capture2) // wraparound logic, if second time < first time, wraparound occurred
	    	    		{
	    	    			sum = (65000-capture1 + 1) + capture2; //the +1 accounts for the zero that the timer starts on
	    	    			printf("raw time = %d   ", sum);
	    	    			sum = sum/58;
	    	    			printf("%d cm is the distance wraparound \n", sum);

	    	    			PrintFlag = 0;
	    	    			return;
	    	    		}

	    	    		else
	    	    		{
	    	    			sum = capture2 - capture1;
	    	    			printf("raw time = %d   ", sum);
	    	    			 sum = sum/58;

	    	    			printf("%d cm is the distance  \n",  sum);

	    	    			PrintFlag = 0;
	    	    			return;

	    	    		}

}

void LEDUpdate(void)
{



	if(oldsum == sum || oldsum == (sum+1) || oldsum == (sum-1)) // if old sensor reading is approximately same as new one, no movement detected
	{
		if(LEDAutoOFF <= 16) // prevents LEDAutoOFF from reaching very high numbers when there are large idle times
		{
			LEDAutoOFF++; //increment AutoOFF if no movement detected
		}
	}

	else //if oldsum is not approximately new sum, movement is detected, so restart the AutoOFF count
	{
		LEDAutoOFF = 0; //proceed down to the sum >=... statements to turn on/off the corresponding LEDs
	}

	if(LEDAutoOFF >=16)
	{
		return; //if counter is already past 15, just return-after 15 readings, the LEDS are already turned off, so don't need to re-off them. saves time, power
	}

	if(LEDAutoOFF==15) // if counter hits 15, turn off all LEDs.
	{
		//no motion detected for 15 sensor readings-turn off LEDs
		GPIOA->BSRR |= (1U<<21);
		GPIOA->BSRR |= (1U<<22);
		GPIOA->BSRR |= (1U<<23);
		GPIOA->BSRR |= (1U<<25);
		GPIOA->BSRR |= (1U<<24);
		GPIOC->BSRR = (1U<<23);
		return;
	}

	else
	{
		if(sum >= 400)
		{
			//turn off all LEDs-no object is nearby
			GPIOA->BSRR |= (1U<<21);
			GPIOA->BSRR |= (1U<<22);
			GPIOA->BSRR |= (1U<<23);
			GPIOA->BSRR |= (1U<<25);
			GPIOA->BSRR |= (1U<<24);
			GPIOC->BSRR = (1U<<23);
			return;
		}

		if(sum <= 18)
		{
			//turn on all 3 red LEDs LEDs
			GPIOA->BSRR = (1U<<5);
			//turn off all other lights
			GPIOA->BSRR |= (1U<<22) | (1U<<23) | (1U<<24) | (1U<<25);
			GPIOC->BSRR = (1U<<23);
			//return
			//wait, do a systick delay, and turn off all LEDs
			return;
		}

		if (sum <= 25)
		{
			//turn on both blue LEDs + all 3 green LEDs
			GPIOA->BSRR = (1U<<8) | (1U<<6) | (1U<<7) | (1U<<9);
			GPIOC->BSRR = (1U<<7);

			//turn off red LEDs
			GPIOA->BSRR |= (1U<<21);

			//return
			return;
		}
		if (sum <= 40)
		{
			//turn on both blue LEDs + both green LEDs
			GPIOC->BSRR = (1U<<7);
			GPIOA->BSRR |= (1U<<9) | (1U<<7) | (1U<<8);

			//turn off 1 green LED, red LEDs
			GPIOA->BSRR |= (1U<<21);
			GPIOA->BSRR |= (1U<<22);
			//return
			return;
		}
		if (sum <= 60)
		{
			//turn on both blue LEDs + one green LED
			GPIOC->BSRR = (1U<<7);
			GPIOA->BSRR |= (1U<<8) | (1U<<9);

			// turn off red LEDs, 2 green LEDs
			GPIOA->BSRR |= (1U<<21);
			GPIOA->BSRR |= (1U<<22);
			GPIOA->BSRR |= (1U<<23);
			//return
			return;
		}
		if (sum <= 80)
		{
			//turn on both blue LEDs
			GPIOC->BSRR = (1U<<7);
			GPIOA->BSRR |= (1U<<8);

			// turn off all red, green LEDs
			GPIOA->BSRR |= (1U<<21);
			GPIOA->BSRR |= (1U<<22);
			GPIOA->BSRR |= (1U<<23);
			GPIOA->BSRR |= (1U<<25);
			//return
			return;
		}
		if (sum > 80 && sum <= 400)
		{
			// turn on bottom blue LED
			GPIOC->BSRR = (1U<<7);
			// turn off all the other LEDs
			GPIOA->BSRR |= (1U<<21);
			GPIOA->BSRR |= (1U<<22);
			GPIOA->BSRR |= (1U<<23);
			GPIOA->BSRR |= (1U<<25);
			GPIOA->BSRR |= (1U<<24);
			return;
		}
	}
}


