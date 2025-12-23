#ifndef RC522_H_
#define RC522_H_

#include <stdint.h>
#include <stdbool.h>

void rc522_init(void);
void rc522_hard_reset(void);
void rc522_write(uint8_t addr, uint8_t data);
uint8_t rc522_read(uint8_t addr);
void rc522_config_init(void);
void rc522_clean_irq(void);


#endif /* RC522_H_ */
