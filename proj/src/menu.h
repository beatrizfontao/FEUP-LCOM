#ifndef PROJ_MENU_H
#define PROJ_MENU_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "graphic.h"
#include "resources/instructions.xpm"
#include "resources/instructions_back.xpm"
#include "resources/menu_instructions.xpm"
#include "resources/menu_exit.xpm"
#include "resources/menu_start.xpm"
#include "resources/menu.xpm"

/**
 * @brief Enumerated type for identifying the cuttons of the menu
 * 
 */
typedef enum {
    START_BUTTON, /*!< Start button (for playing the game) */
    INSTRUCTIONS_BUTTON, /*!< Instructions button (for displaying the instructions) */
    EXIT_BUTTON, /*!< Exit button (to exit the game) */
    INITIAL /*!< No button being hovered */
} Button;

/**
 * @brief Loads all the xpms related to the menu
 */
void initMenuXpm();

/**
 * @brief Draws the menu according to the button being hovered
 * 
 * @param button Button currently being hovered
 * @return 0 if the draw was successful
 */
int drawMenu(Button button);

/**
 * @brief Checks collisions with the main menu and gets the button currently being hovered.
 * 
 * @param x Current x position of the mouse
 * @param y Current y position of the mouse
 * @return Button currently being hoverd
 */
Button getButton(uint16_t x, uint16_t y);

/**
 * @brief Checks collisions with the instructions menu and checks if the back option is being hovered
 * 
 * @param x Current x position of the mouse
 * @param y Current y position of the mouse
 * @return true if the back button is being hovered
 * @return false if the back button is not being hovered
 */
bool getInstructionButton(uint16_t x, uint16_t y);


/**
 * @brief Draws the instructions menu according to if the back button is being hovered or not
 * 
 * @param button Whether the back button is being hovered or not
 * @return 0 if the function was a success
 */
int drawInstructions(bool button);

#endif //PROJ_MENU_H
