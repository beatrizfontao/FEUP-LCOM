#ifndef LAB3_UTILS_H
#define LAB3_UTILS_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"

int util_sys_inb (int port, uint8_t* value);

#endif //LAB3_UTILS_H
