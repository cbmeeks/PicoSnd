#include <pico/time.h>
#include <pico/stdio.h>
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

//    audioWritePsg1(0, 7);
//    audioWritePsg1(1, 0b00111110);
//    audioWritePsg1(0, 8);
//    audioWritePsg1(1, 0b00001111);


    while (1) {
        tight_loop_contents();
//        play();
//        sleep_ms(50);

//        audioWritePsg1(0, 0);   // register 0
//        audioWritePsg1(1, 28);   // register 0 value
//        audioWritePsg1(0, 1);   // register 1
//        audioWritePsg1(1, 1);   // register 1 value
//
//        sleep_ms(250);
//
//        audioWritePsg0(0, 0);   // register 0
//        audioWritePsg0(1, 170);   // register 0 value
//        audioWritePsg0(0, 1);   // register 1
//        audioWritePsg0(1, 1);   // register 1 value
//
//        sleep_ms(250);
    }

    return 0;
}
