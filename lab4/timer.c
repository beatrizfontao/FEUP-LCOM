#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hookid = 0;
uint32_t timer_counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

    if (timer < 0 || timer > 2)
        return 1;

    if (freq < 19 || freq > TIMER_FREQ)
        return 1;

    uint8_t st;

    if (timer_get_conf(timer, &st))
        return 1;

    st = st & (BIT(0) | BIT(1) | BIT(2) | BIT(3));

    uint8_t controlword = TIMER_LSB_MSB | st;

    uint8_t timer_reg = TIMER_0;

    switch (timer) {
        case 0:
            controlword |= TIMER_SEL0;
            break;
        case 1:
            controlword |= TIMER_SEL1;
            timer_reg = TIMER_1;
            break;
        case 2:
            controlword |= TIMER_SEL2;
            timer_reg = TIMER_2;
            break;
        default:
            return 1;
    }

    if (sys_outb(TIMER_CTRL, controlword))
        return 1;

    uint16_t f = TIMER_FREQ / freq;
    uint8_t msb, lsb;

    if(util_get_LSB(f, &lsb))
        return 1;

    if(util_get_MSB(f, &msb))
        return 1;

    if(sys_outb(timer_reg, lsb))
        return 1;

    if(sys_outb(timer_reg, msb))
        return 1;

    return 0;
}

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

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

    uint8_t readBack = TIMER_RB_COUNT_ | TIMER_RB_CMD | TIMER_RB_SEL(timer);

    uint32_t port = TIMER_0;

    switch (timer) {
        case 0:
            port = TIMER_0;
            break;
        case 1:
            port = TIMER_1;
            break;
        case 2:
            port = TIMER_2;
            break;
        default:
            return 1;
    }

    if (sys_outb(TIMER_CTRL, readBack))
        return 1;

    if (util_sys_inb(port, st))
        return 1;

    return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                         enum timer_status_field field) {

    union timer_status_field_val val;

    switch (field) {
        case tsf_all:
            val.byte = st;
            break;
        case tsf_initial:
            val.in_mode = (st & (BIT(4) | BIT(5))) >> 4;
            break;
        case tsf_mode:
            val.count_mode = (st & (BIT(1) | BIT(2) | BIT(3))) >> 1;
            break;
        case tsf_base:
            if (st & BIT(0))
                val.bcd = true;
            else
                val.bcd = false;
            break;
        default:
            return 1;
    }

    if (timer_print_config(timer, field, val))
        return 1;

    return 0;
}
