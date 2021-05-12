#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

static uint     ledgpio = 25;   //  GPIO number of LED on board
static uint     wrap = 500;     //  TOP register value

static uint16_t levelFromCount(int i);

int main() {
    stdio_init_all();
    
    gpio_set_function(ledgpio, GPIO_FUNC_PWM);
    uint    slice = pwm_gpio_to_slice_num(ledgpio);
    uint    chan = pwm_gpio_to_channel(ledgpio);
    pwm_set_wrap(slice, wrap);
    pwm_set_chan_level(slice, chan, 0);
    pwm_set_clkdiv(slice, 10.0);
    pwm_set_enabled(slice, true);
    uint64_t    usleep100 = 1000;    //  sleep time for one stage in the period

    while (true) {
        int c = getchar_timeout_us(0);  //  check for input from USB
        if ((c != PICO_ERROR_TIMEOUT) && ('0' <= c)) {
            usleep100 = (uint64_t)((c - '0') * 100);
            printf("sleep time is set to %dusec\n", (int)usleep100);
        }
        int i;
        for (i = 0 ; i < wrap ; i ++) {
            pwm_set_chan_level(slice, chan, levelFromCount(i));
            sleep_us(usleep100);
        }
        for (i = wrap ; 0 < i ; i --) {
            pwm_set_chan_level(slice, chan, levelFromCount(i));
            sleep_us(usleep100);
        }
   }
    return 0;
}

static uint16_t levelFromCount(int i)
{
    return (uint16_t)(i * i / wrap);
}
