#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdint.h>

void joystick_init (void);
uint16_t joystick_read_axis_val (char axis);

#endif /* JOYSTICK_H_ */
