#include "adc.h"
#include "uart2.h"
#include "gpio.h"
#include "stm32f446xx.h"
#include <stdint.h>

void adc1_pa0_pa1_init(void) {

	gpio_init(GPIOAEN);

	//set PA0 as analog
	gpio_pin_mode(GPIOA, 0, 3U);
	//set PA1 as analog
	gpio_pin_mode(GPIOA, 1, 3U);

	//Enable clock access to ADC1
	RCC->APB2ENR |= ADC1EN;
}

void adc1_set_SMPR (int chan, uint8_t val){
	//Set sample time
	if (chan>9){
		ADC1->SMPR1 &= ~(7<<(chan*3));
		ADC1->SMPR1 |= val<<(chan*3);
	}
	else{
		ADC1->SMPR2 &= ~(7<<(chan*3));
		ADC1->SMPR2 |= val<<(chan*3);
	}
}

void adc1_set_sequence_length (int len){
	ADC1->SQR1 &= ~(0xFU<<20);
	ADC1->SQR1 |= (len<<20);
}

void adc1_set_SQR (int seq, uint8_t chan){
	//Set which channel will be converted at sequence number seq
	if (seq>12){
		ADC1->SQR1 &= ~(7<<(seq*5));
		ADC1->SQR1 |= chan<<(seq*5);
	}
	else if(seq>6){
		ADC1->SQR2 &= ~(7<<(seq*5));
		ADC1->SQR2 |= chan<<(seq*5);
	}
	else{
		ADC1->SQR3 &= ~(7<<(seq*5));
		ADC1->SQR3 |= chan<<(seq*5);
	}
}

void adc1_set_adon(void){
	//Enable ADC1 module
	ADC1->CR2 |= CR2_ADON;
}

void adc1_start_conversion(void) {
	//Start conversion
	ADC1->CR2 |= CR2_SWSTART;
}

void adc1_start_continuous_conversion(void) {
	//Enable continuous mode
	ADC1->CR2 |= CR2_CONT;
	//Start conversion
	ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc1_read(void) {
	//Wait for conversion to complete
	while (!(ADC1->SR & SR_EOC)) {}

	//Read converted result
	return ADC1->DR;
}
