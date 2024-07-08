#include <pico/time.h>
#include <pico/stdio.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/clocks.h>
#include "vgm.h"


struct repeating_timer audio_update_timer;


bool audio_update_callback(struct repeating_timer *t) {
    play();
    audioUpdate();
    return true;
}


int main(void) {
    stdio_init_all();

    initAudio();

    add_repeating_timer_us(23, audio_update_callback, NULL, &audio_update_timer);


    while (1) {
        tight_loop_contents();
    }

    return 0;
}
