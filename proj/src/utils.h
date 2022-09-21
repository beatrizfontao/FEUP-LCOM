#ifndef LAB3_UTILS_H
#define LAB3_UTILS_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
/**
 * @brief Calls the sys_inb function and, instead of a uint32_t value, gets a uint8_t
 * @param port Port to read the information from
 * @param byte Used to store what was read
 * @return 1 on failure and 0 on success
 */ 
int util_sys_inb (int port, uint8_t* byte);

#endif //LAB3_UTILS_H
