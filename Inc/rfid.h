#ifndef RFID_H_
#define RFID_H_

#include <stdint.h>
#include <stdbool.h>

#define RFID_IRQ_PIN            1U //connected to PA1
#define SPI1_CS_BSRR        (1U<<1) //CS in this project is in PA1

void rfid_init(void);

#endif /* RFID_H_ */
