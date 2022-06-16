#ifndef LAB3_KBC_H
#define LAB3_KBC_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "utils.h"

int (kbd_subscribe_int)(uint8_t *bit_no);

int (kbd_unsubscribe_int)();

void (kbc_ih)();

int (twoBytes)(uint8_t scan_code);

bool (makecode)(uint8_t scan_code);

int (readcommand)(uint8_t* commandbyte);

int (writecommand)(uint8_t commandbyte);

int (kbd_reenable_int)();

#endif //LAB3_KBC_H
