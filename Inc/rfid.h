#ifndef RFID_H_
#define RFID_H_

#include <stdint.h>
#include <stdbool.h>

#define RFID_POLL_PERIOD_MS             300
#define RFID_REQA_TIMEOUT_MS            60
#define RFID_UID_TIMEOUT_MS             100
#define RFID_COOLDOWN_MS                500


void rfid_init(void);
void rfid_enable_irq(void);
void rfid_spi1_config(void);
void rfid_fsm(void);
void rfid_update_irq_flag(void);
bool rfid_show_pending_irq_status(void);
//bool rfid_check_error(void);

#endif /* RFID_H_ */
