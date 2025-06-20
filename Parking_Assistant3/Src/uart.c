#include "stm32f103xb.h"
#include <stdio.h>
#include <stdint.h>
#include <uart.h>
#include <timer.h>
#define Baud 115200UL //desired baud rate
#define SYS_FREQ 8000000UL //default system clock runs at 8 MHz
#define SR_TXE (1U<<7) // transmitter interrupt flag
#define IOPAEN (1U<<2) //enable bit for GPIOA module
#define UART2EN (1U<<17) //enable bit for USART2 module
#define SR_RXNE (1U<<5) // Reciever interrupt flag

static void uart2_set_baud(uint32_t pclk, uint32_t baud);
int __io_putchar(int ch);
void RXNEIEInterrupt(void);
uint8_t bufferpop(void);
void bufferReset(void);
int EndCheck(void);

static uint8_t buffer[256]; //buffer for printing character in uart
static uint8_t* head = buffer;
static uint8_t* tail = buffer;

uint8_t bufferpop(void) //function that returns the next item to be printed from the buffer
{
	uint8_t tmp = (*tail);
	tail = tail + 1;
	return tmp;
}

int __io_putchar(int ch) //printf defaults to this function, which adds characters to the buffer
{
	(*head) = ch;
	head = head+1;
	USART2->CR1 |= (1U<<7);//enable txie
	systickDelayuS(0);
	return ch;
}

void bufferReset(void) // used to reset the buffer after a string has been completely printed
{
	head = buffer;
	tail = buffer;
	return;
}

int EndCheck(void) //function to check if the printing of a string has ended by determining if &head = &tail
{
	if(head == tail)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


static void uart2_set_baud(uint32_t pclk, uint32_t baud) {

        USART2->BRR = ((pclk+(baud/2U))/baud);
    }


void RXNEIEInterrupt(void)
{
	//clock buses for UART,GPIOA activated//
		RCC->APB1ENR |= UART2EN;
	    RCC->APB2ENR |= IOPAEN;

	    //setting up PA2 for UART2 TX//
	    //set CRL to 1011 for PA2//
	    GPIOA->CRL |= (1U<<11);
	    GPIOA->CRL &=~ (1U<<10);
	    GPIOA->CRL |= (1U<<9);
	    GPIOA->CRL |= (1U<<8);

	    //set pa3, 0100//
	    GPIOA->CRL &=~ (1U<<12);
	    GPIOA->CRL &=~ (1U<<13);
	    GPIOA->CRL |= (1U<<14);
	    GPIOA->CRL &=~ (1U<<15);




	    //set baud rate//
	        uart2_set_baud(SYS_FREQ,Baud);

	    //enable usart transmitter//
	    USART2->CR1 = (1U<<3);
	    //enable usart reciever//
	    USART2->CR1 |= (1U<<2);
	    //enable RXIE in CR1
	    USART2->CR1 |= (1U<<5);
	    //enable in NVIC
	    NVIC_EnableIRQ(USART2_IRQn);


	    //enable USART module//
	    USART2->CR1 |= (1U<<13);
}
