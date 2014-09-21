/*
 * STM32F4 Discovery test code
 */

#include "stm32f4xx_conf.h"
int count;
long long i;
int shift = 0x001F;
int row = 0x0006;

int get_shift(int test) {
	return test >> 4;
}

int set_shift(int set, int row) {
	return (set << 4) & row;
}

void TIM2_IRQHandler(void) {
	if (TIM2->SR & TIM_SR_UIF) {
		if (!count) {
			if (shift & 0x01) {
				GPIOD->ODR |= GPIO_ODR_ODR_13;
			} else {
				GPIOD->ODR &= ~GPIO_ODR_ODR_13;
			}
			shift >>= 1;
		}
		if (!shift) {
			RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN;
			RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
			TIM3->PSC = TIM_PSC_PSC;		// No prescaler
			TIM3->DIER |= TIM_DIER_UIE;	// Update interrupt enable
			TIM3->ARR = 0x08;		// Count to 0x80, autoreload
			TIM3->CR1 |= TIM_CR1_ARPE |	// Autoreload on
				     TIM_CR1_CEN;	// Counter enable
			TIM3->EGR = TIM_EGR_UG;
			//GPIOD->ODR &= ~GPIO_ODR_ODR_11;
		}
		GPIOD->ODR ^= GPIO_ODR_ODR_15;
		count ^= 0x01;
	}
	TIM2->SR = 0x00;
}

void TIM3_IRQHandler(void) {
	if (TIM3->SR & TIM_SR_UIF)
		GPIOD->ODR ^= GPIO_ODR_ODR_11;
	TIM3->SR = 0x00;
}

int main(void)
{
	count = 0;
	shift = 0x1f | (row << 5);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	GPIOD->MODER = GPIO_MODER_MODER11_0 |
		       GPIO_MODER_MODER12_0 |	/* Green LED, PD12 output */
		       GPIO_MODER_MODER13_0 |	/* Orange LED, PD13 output */
		       GPIO_MODER_MODER14_0 |	/* Red LED, PD14 output */
		       GPIO_MODER_MODER15_0;	/* Blue LED, PD15 output */

	GPIOD->ODR |= GPIO_ODR_ODR_11;
	GPIOD->ODR ^= GPIO_ODR_ODR_12;
	GPIOD->ODR ^= GPIO_ODR_ODR_14;
	GPIOD->ODR |= GPIO_ODR_ODR_15;

	NVIC->ISER[0] |= 1 << (TIM2_IRQn);
	NVIC->ISER[0] |= 1 << (TIM3_IRQn);

	TIM2->PSC = TIM_PSC_PSC;		// No prescaler
	TIM2->DIER |= TIM_DIER_UIE;	// Update interrupt enable
	TIM2->ARR = 0x80;		// Count to 0x80, autoreload
	TIM2->CR1 |= TIM_CR1_ARPE |	// Autoreload on
		     TIM_CR1_CEN;	// Counter enable
	TIM2->EGR = TIM_EGR_UG;

	while(1) {
	}
}
