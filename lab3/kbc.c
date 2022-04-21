#include "kbc.h"

int kbd_hookid = 1;
uint8_t scancode = 0, statuscode = 0;

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

    uint8_t statuscode;

    util_sys_inb(STAT_REG, &statuscode);

    if((statuscode & (PARITY_BIT | TIMEOUT_BIT)) != 0) {
        scancode = 0;
        return;
    }

    util_sys_inb(OUT_BUF, &scancode);
}

int (twoBytes) (uint8_t scan_code){
    if(scan_code == 0xE0)
        return 1;
    else
        return 0;
}

bool (makecode) (uint8_t scan_code){
    if((scan_code & MAKE_BIT) >> 7)
        return false;
    return true;
}

int (readcommand)(uint8_t* commandbyte){
    int tries = 10;
    while (tries){
        if(util_sys_inb(STAT_REG, &statuscode)){
            continue;
        }
        if((statuscode & (IBF_BIT | OBF_BIT)) != 0){
            tries --;
            continue;
        }
        if(sys_outb(STAT_REG, READ_COMMAND))
            return 1;
        if(util_sys_inb(OUT_BUF, commandbyte))
            return 1;
        break;
    }
    if (tries == 0)
        return 1;
    return 0;
}

int (writecommand)(uint8_t commandbyte){
    int tries = 10;
    while (tries){
        if(util_sys_inb(STAT_REG, &statuscode)){
            continue;
        }
        if((statuscode & (IBF_BIT | OBF_BIT)) != 0){
            tries --;
            continue;
        }
        if(sys_outb(STAT_REG, WRITE_COMMAND))
            return 1;
        if(sys_outb(ARGS_REG, commandbyte))
            return 1;
        break;
    }
    if (tries == 0)
        return 1;
    return 0;
}

int (kbd_reenable_int) (){
    uint8_t commandByte;

    if(readcommand(&commandByte))
        return 1;

    commandByte |= ENABLE_KBD;

    if(writecommand(commandByte))
        return 1;

    return 0;
}
