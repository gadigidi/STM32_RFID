#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#define ADC1EN				(1U<<8)
#define CR2_ADON			(1U<<0)
#define CR2_SWSTART			(1U<<30)
#define SR_EOC				(1U<<1)
#define CR2_CONT			(1U<<1)

void adc1_pa0_pa1_init(void);
void adc1_set_SMPR (int chan, uint8_t val);
void adc1_set_SQR (int seq, uint8_t chan);
void adc1_set_sequence_length(int len);
void adc1_set_adon(void);
void adc1_start_conversion(void);
void adc1_start_continuous_conversion(void);
uint32_t adc1_read(void);

#endif /* ADC_H_ */

