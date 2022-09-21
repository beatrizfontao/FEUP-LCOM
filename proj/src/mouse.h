#ifndef PROJ_MOUSE_H
#define PROJ_MOUSE_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "utils.h"
#include "macros.h"

#include "resources/mouse.xpm"

/**
 * @brief Struct with the information of the mouse
 */
typedef struct {
    int x, y; /*!< x and y coordinates of the mouse */
    xpm_image_t img; /*!< Loaded xpm of the mouse */
    bool lb_pressed; /*!< Boolean with the info of whether the mouse left button is pressed or not*/
} Mouse;

/**
 * @brief Subscribes the mouse interruptions
 * 
 * @param bit_no 
 * @return 0 on success or 1 otherwise
 */
int (mouse_subscribe_int)(int *bit_no);

/**
 * @brief Unsubscribes the mouse interruptions
 *  
 * @return 0 on success or 1 otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler. Reads the status code, checks for errors and gets the scancode of the mouse
 */
void (mouse_ih)();

/**
 * @brief Composes the mouse packet with the information previously read
 * 
 * @param pp packet to be composed
 * @param bytes array with the bytes read from the interrupts
 */
void (getMousePacket)(struct packet *pp, uint8_t bytes[3]);

/**
 * @brief Sends the given cmd to the keyboard controller
 * 
 * @param cmd command to be sent
 * @return 0 on success or 1 otherwise
 */
int (send_mouse_command)(uint8_t cmd);

/**
 * @brief Creates a mouse object with the specified characteristics
 * 
 * @param x Initial x coordinate
 * @param y Initial y coordinate
 * @return Pointer with the newly created mouse object
 */
Mouse *(createMouse)(int x, int y);

/**
 * @brief Updates the mouse position according to the packet received 
 * 
 * @param pp Packet with the information about the interrupt
 * @param mouse Pointer to the mouse to be altered
 */
void (updateMouse)(struct packet *pp, Mouse *mouse);

/**
 * @brief Frees the allocated memory for the mouse object
 * 
 * @param mouse Pointer to the mouse being removed
 */
void (removeMouse)(Mouse *mouse);

#endif //PROJ_MOUSE_H
