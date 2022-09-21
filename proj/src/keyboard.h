#ifndef PROJ_KEYBOARD_H
#define PROJ_KEYBOARD_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"

/**
 * @brief Subscribes the interruptions of the keyboard
 */
int (kbd_subscribe_int) (uint8_t *bit_no);

/**
 * @brief Unsubscribes the interruptions of the keyboard
 * 
 */
int (kbd_unsubscribe_int) ();

/**
 * @brief Keyboard Interrupt Handler. Receives the scancode of a key when there's an interrupt
 */
void (kbc_ih) ();

/**
 * @brief Check if the scancode is a two byte scan code or not
 * 
 * @return 1 if the scancode is a two byte scan code or 0 if it is a one byte scancode
 */
int (twoBytes) (uint8_t scan_code);


#endif //PROJ_KEYBOARD_H
