#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "kbc.h"
#include "i8042.h"
#include "utils.h"
#include "timer.h"

extern int kbd_hookid, hookid, timer_counter;
extern uint8_t scancode, statuscode;

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/lab3/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int (kbd_test_scan)() {

    uint8_t kbd_bit_no = BIT(kbd_hookid);

    if (kbd_subscribe_int(&kbd_bit_no))
        return 1;

    int ipc_status, r;
    message msg;
    int size = 1;
    uint8_t bytes[2];

    while (scancode != ESC_BREAK) { /* You may want to use a different condition */
        /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & BIT(kbd_bit_no)) {
                        kbc_ih();
                        if (twoBytes(scancode)) {
                            size = 2;
                            continue;
                        }
                        if (size == 1) {
                            bytes[0] = scancode;
                        } else {
                            bytes[0] = 0xE0;
                            bytes[1] = scancode;
                        }
                        kbd_print_scancode(makecode(scancode), size, bytes);
                        size = 1;
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        } else {
        }
    }

    if (kbd_unsubscribe_int())
        return 1;

    return 0;
}

int (kbd_test_poll)() {

    int size = 1;
    uint8_t bytes[2];

    while (scancode != ESC_BREAK) { /* You may want to use a different condition */
        if (util_sys_inb(STAT_REG, &statuscode))
            return 1;
        if ((statuscode & (OBF_BIT | AUX_BIT)) != 0)
            continue;
        kbc_ih();
        if (twoBytes(scancode)) {
            size = 2;
            continue;
        }
        if (size == 1) {
            bytes[0] = scancode;
        } else {
            bytes[0] = 0xE0;
            bytes[1] = scancode;
        }
        kbd_print_scancode(makecode(scancode), size, bytes);
        size = 1;

    }

    if (kbd_reenable_int())
        return 1;

    return 0;
}

int (kbd_test_timed_scan)(uint8_t n) {
    uint8_t kbd_bit_no = BIT(kbd_hookid);
    uint8_t timer_bit_no = BIT(hookid);

    if (kbd_subscribe_int(&kbd_bit_no))
        return 1;

    if(timer_subscribe_int(&timer_bit_no))
        return 1;

    int ipc_status, r;
    uint32_t aux_counter;
    message msg;
    int size = 1;
    uint8_t bytes[2];

    while (scancode != ESC_BREAK && aux_counter <= n) { /* You may want to use a different condition */
        /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & BIT(kbd_bit_no)) {
                        kbc_ih();
                        if (twoBytes(scancode)) {
                            size = 2;
                            continue;
                        }
                        if (size == 1) {
                            bytes[0] = scancode;
                        } else {
                            bytes[0] = 0xE0;
                            bytes[1] = scancode;
                        }
                        kbd_print_scancode(makecode(scancode), size, bytes);
                        size = 1;
                        timer_counter = 0;
                        aux_counter = 0;
                    }
                    if (msg.m_notify.interrupts & BIT(timer_bit_no)) {
                        if (timer_counter % 60 == 0) //se passar um segundo, incrementamos o aux_counter
                        {
                            aux_counter ++;
                        }

                        timer_int_handler();
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        } else {
        }
    }

    if (timer_unsubscribe_int())
        return 1;

    if (kbd_unsubscribe_int())
        return 1;

    return 0;
}
