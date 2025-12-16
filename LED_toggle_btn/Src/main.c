#include "gpio.h"
#include <stdint.h>
#include "stm32f446xx.h"


int main(void){
    
    gpio_init(GPIOAEN);
    gpio_init(GPIOCEN);
    
    // PA5 as OUTPUT
    gpio_pin_mode(GPIOA, 5, GPIO_MODE_OUTPUT);
    
    // PC13 as INPUT
    gpio_pin_mode(GPIOC, 13, GPIO_MODE_INPUT);

    uint8_t led_state = 0;
    uint8_t prev_btn = 1;   //Active-High

    while(1){
        uint8_t btn = (GPIOC->IDR & BTN_PIN) ? 1 : 0;

        // Detect falling edge: prev = 1, current = 0
        if(prev_btn == 1 && btn == 0){
            led_state ^= 1;  // toggle bit

            if(led_state){
				gpio_set_LED();
            }
            else {
				gpio_reset_LED();
            }
        }

        prev_btn = btn;
    }
}
