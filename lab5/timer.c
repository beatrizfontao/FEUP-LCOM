#include "timer.h"

int hookid = 0;
int timer_counter = 0;

int (timer_subscribe_int)(uint8_t *bit_no) {

    *bit_no = hookid;

    if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookid))
        return 1;

    return 0;
}

int (timer_unsubscribe_int)() {

    if(sys_irqrmpolicy(&hookid))
        return 1;

    return 0;
}

void (timer_int_handler)() {
    timer_counter ++;
}
