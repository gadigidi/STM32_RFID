#ifndef RC522_PORTS_H_
#define RC522_PORTS_H_

#include <stdint.h>
#include <stdbool.h>

#define RFID_IRQ_PIN                1U //IRQ input connected to PA1
#define RFID_CS_PIN                 4U //CS output connected to PA4
#define RFID_CS_HIGH_BSRR           (1U<<RFID_CS_PIN)
#define RFID_CS_LOW_BSRR            (1U<<(RFID_CS_PIN+16))
#define RFID_RST_PIN                8U //RST output connected to PA8
#define RFID_RST_HIGH_BSRR          (1U<<RFID_RST_PIN)
#define RFID_RST_LOW_BSRR           (1U<<(RFID_RST_PIN+16))

#define RFID_MISO_PIN               6U

void rc522_ports_init(void);
void rc522_assert_reset(void);
void rc522_deassert_reset(void);
void rc522_assert_cs(void);
void rc522_deassert_cs(void);

#endif /* RC522_PORTS_H_ */
