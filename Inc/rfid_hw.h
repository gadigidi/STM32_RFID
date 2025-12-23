#ifndef RFID_HW_H_
#define RFID_HW_H_

#include <stdint.h>
#include <stdbool.h>

#define RFID_IRQ_PIN                1U //IRQ input connected to PA1
#define RFID_CS_PIN                 4U //CS output connected to PA4
#define RFID_CS_HIGH_BSRR           (1U<<RFID_CS_PIN)
#define RFID_CS_LOW_BSRR            (1U<<(RFID_CS_PIN+16))
#define RFID_RST_PIN                8U //RST output connected to PA8
#define RFID_RST_HIGH_BSRR          (1U<<RFID_RST_PIN)
#define RFID_RST_LOW_BSRR           (1U<<(RFID_RST_PIN+16))

void rfid_hw_init(void);
void rfid_assert_reset(void);
void rfid_deassert_reset(void);
void rfid_assert_cs(void);
void rfid_deassert_cs(void);

#endif /* RFID_HW_H_ */
