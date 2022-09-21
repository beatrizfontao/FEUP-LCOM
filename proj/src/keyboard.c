#include "keyboard.h"

int kbd_hookid = 1;
uint8_t keyboard_scancode = 0, keyboard_statuscode = 0;

int (kbd_subscribe_int) (uint8_t *bit_no){

    *bit_no = kbd_hookid;

    if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hookid))
        return 1;

    return 0;
}

int (kbd_unsubscribe_int) (){

    if(sys_irqrmpolicy(&kbd_hookid))
        return 1;

    return 0;
}

void (kbc_ih) (){

    uint8_t keyboard_statuscode;

    util_sys_inb(STAT_REG, &keyboard_statuscode);

    if((keyboard_statuscode & (PARITY_BIT | TIMEOUT_BIT)) != 0) {
        keyboard_scancode = 0;
        return;
    }

    util_sys_inb(OUT_BUF, &keyboard_scancode);
}

int (twoBytes) (uint8_t scan_code){
    if(scan_code == 0xE0)
        return 1;
    else
        return 0;
}
