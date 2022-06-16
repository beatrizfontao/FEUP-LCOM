#ifndef LAB4_MOUSE_H
#define LAB4_MOUSE_H
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

int (utils_sys_inb)(int port, uint8_t *value);

void (mouse_ih)();

void (mouse_getPacket)(uint8_t bytes[3], struct packet* pp);

int (enable_mouse)(uint8_t cmd);


#endif // LAB4_MOUSE_H
