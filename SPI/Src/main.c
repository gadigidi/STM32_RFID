#include "tim2.h"
#include "timebase.h"
#include <stdint.h>
#include "stm32f446xx.h"

int main(void){

	timebase_init();
	
	uint32_t t_now;
	uint32_t t0;
	int secs = 0;

	t0 = timebase_show_ms();
	while (1) {
		//t0 = timebase_show_ms();
		//timebase_block_delay_ms(1000);
		if (timebase_nonblocking_delay_ms(t_now, 10000)) {
			t_now = timebase_show_ms();
			secs = (t_now - t0) / 1000;
		}
	}
}
