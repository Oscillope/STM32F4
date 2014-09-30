/*
 * STM32F4 Discovery test code
 */

#include "stm32f4xx_conf.h"
#include "display.h"
#define ALL_ONES_L	0x000000FFFFFFFFFF
#define ALL_ONES_H	0x000000FFFFFFFFFF
#define ROW_MASK	0x0000000000000FFF
int count;
int row;
long long shift_h = ALL_ONES_H;
long long shift_l = ALL_ONES_L;

void TIM2_IRQHandler(void) {
	if (TIM2->SR & TIM_SR_UIF) {
		int i, j;
		short cur_row = (count ? *(cur_buf->cur_row) : *(cur_buf->cur_row++));
		int idx = cur_buf->idx;
		char cur_glyph = cur_buf->glyph[idx / 80][cur_row];

		GPIOD->ODR ^= GPIO_ODR_ODR_15;
		if (count >= 80) {
			if (cur_row & 0x01) {
				GPIOD->ODR |= GPIO_ODR_ODR_13;
			} else {
				GPIOD->ODR &= ~GPIO_ODR_ODR_13;
			}
			if (count & 0x01) {
				cur_row >>= 1;
			}
		} else if ((cur_glyph >> (idx % 5)) & 0x01) {
			GPIOD->ODR |= GPIO_ODR_ODR_13;
		} else {
			GPIOD->ODR &= ~GPIO_ODR_ODR_13;
		}
		if (count & 0x01) {
			idx++;
		}
		if (count == 88) {
			GPIOD->ODR &= ~GPIO_ODR_ODR_15;
			//~ for(i = 0; i < 65535; i++) {
				//~ for(j = 0; j < 655; j++)
					//~ GPIOD->ODR ^= GPIO_ODR_ODR_14;
			//~ }
			count = 0;
		} else {
			count++;
		}

		cur_buf->idx = idx;
		*cur_buf->cur_row = cur_row;
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
	row = 0;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	GPIOD->MODER = GPIO_MODER_MODER11_0 |
		       GPIO_MODER_MODER12_0 |	/* Green LED, PD12 output */
		       GPIO_MODER_MODER13_0 |	/* Orange LED, PD13 output */
		       GPIO_MODER_MODER14_0 |	/* Red LED, PD14 output */
		       GPIO_MODER_MODER15_0;	/* Blue LED, PD15 output */
	GPIOE->MODER = GPIO_MODER_MODER7_0;

	GPIOE->ODR |= GPIO_ODR_ODR_7;

	GPIOE->PUPDR |= GPIO_PUPDR_PUPDR8_1;

	GPIOD->ODR &= ~GPIO_ODR_ODR_11;
	GPIOD->ODR ^= GPIO_ODR_ODR_12;
	GPIOD->ODR &= ~GPIO_ODR_ODR_13;
	GPIOD->ODR ^= GPIO_ODR_ODR_14;
	GPIOD->ODR &= ~GPIO_ODR_ODR_15;
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR11;
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12;
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13;
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR14;
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15;

	NVIC->ISER[0] |= 1 << (TIM2_IRQn);
	NVIC->ISER[0] |= 1 << (TIM3_IRQn);

	TIM2->PSC = TIM_PSC_PSC;		// No prescaler
	TIM2->DIER |= TIM_DIER_UIE;	// Update interrupt enable
	TIM2->ARR = 0x81;		// Count to 0x80, autoreload
	TIM2->CR1 |= TIM_CR1_ARPE |	// Autoreload on
		     TIM_CR1_CEN;	// Counter enable
	TIM2->EGR = TIM_EGR_UG;

	//RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->PSC = TIM_PSC_PSC;		// No prescaler
	TIM3->DIER |= TIM_DIER_UIE;	// Update interrupt enable
	TIM3->ARR = 0x10;		// Count to 0x80, autoreload
	TIM3->CR1 |= TIM_CR1_ARPE |	// Autoreload on
		     TIM_CR1_CEN;	// Counter enable
	TIM3->EGR = TIM_EGR_UG;

	while(1) {
	}
}
