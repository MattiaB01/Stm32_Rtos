/*
 * exti.c
 *
 *  Created on: 21 nov 2021
 *      Author: Mattia
 */


#include "exti.h"



void pa0_inter_init(void)
{
	/* disable global interrupts*/
	__disable_irq();


	RCC->AHB1ENR |= (1U<<0); //enable clk port A

	/*Set PA0 as input */
	GPIOA->MODER &=~(1<<0);
	GPIOA->MODER &=~(1<<1);

	/*Select PORTA for EXTI0*/
	SYSCFG->EXTICR[0] &= ~(1U<<0);
	SYSCFG->EXTICR[0] &= ~(1U<<1);
	SYSCFG->EXTICR[0] &= ~(1U<<2);
	SYSCFG->EXTICR[0] &= ~(1U<<3);
/*Enable clock access to SYSCFG*/
	 RCC->APB2ENR |= (1U<<14);



	/*Unmask bit 0 */
	EXTI->IMR |=(1<<0);

	/*si può scegliere di configurare l'interrupt su rising edge o falling edge. In questo caso si utlizza la seconda modalità*/
	/*Select falling edge trigger*/
	EXTI->FTSR |=(1U<<0);

	/*Select rising edge trigger*/
	EXTI->RTSR |=(1U<<0);

	/*Enable EXTI0 in NVIC nested vector interrupt*/
	NVIC_EnableIRQ(EXTI0_IRQn);

	/*enable global interrupts*/
	__enable_irq();
}
