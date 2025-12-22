#include "seg7.h"
#include "timebase.h"
#include "gpio.h"
#include <stdio.h>
#include <stdint.h>
#include "stm32f446xx.h"

int main(void){

	timebase_init();

	seg7_init();

	//seg7_update_buffer(0x7);
	seg7_set_buffer_for_scroll(0x76543210);
	seg7_enable_scroll(1);

	while (1) {
		seg7_scroll_digits();
    }
}
