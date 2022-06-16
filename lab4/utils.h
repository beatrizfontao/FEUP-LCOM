#ifndef LAB4_UTILS_H
#define LAB4_UTILS_H


int (util_get_LSB)(uint16_t val, uint8_t *lsb);

int (util_get_MSB)(uint16_t val, uint8_t *msb);

int (util_sys_inb)(int port, uint8_t *value);


#endif // LAB4_UTILS_H
