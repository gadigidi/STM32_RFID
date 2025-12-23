#ifndef RFID_H_
#define RFID_H_

#include <stdint.h>
#include <stdbool.h>

void rfid_init(void);
void rfid_enable_irq(void);
void rfid_spi1_config(void);
void rfid_fsm(void);
void rfid_update_irq_flag(void);

#endif /* RFID_H_ */
