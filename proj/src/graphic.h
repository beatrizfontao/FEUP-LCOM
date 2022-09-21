#ifndef PROJ_GRAPHIC_H
#define PROJ_GRAPHIC_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "alien.h"
#include "mouse.h"

#include "resources/background.xpm"
#include "resources/initial_screen.xpm"

/**
 * @brief Initializes packed vbe_mode__info_t structure passed as an address with the VBE information on the input mode,
 * by calling VBE function 0x01, Return VBE Mode Information, and copying the ModeInfoBlock struct returned by that
 * function. Returns information on the input VBE mode, including screen dimensions, color depth and VRAM physical address.
 * 
 * @param mode mode whose information should be returned
 * @param info address of vbe_mode_info_t structure to be initialized
 * 
 * @return 0 on success, non-zero otherwise
 */
int vg_get_mode_info(uint16_t *mode, vbe_mode_info_t* info);


/**
 * @brief Initializes specified mode.
 * 
 * @param mode Mode to be set
 * 
 * @return 0 on success, non-zero otherwise
 */
int vg_set_mode(uint16_t *mode);


/**
 * @brief Draws on screen a pixel.
 * 
 * @param x x coordinates of pixel that is to be drawn.
 * @param y y coordinates of pixel that is to be drawn.
 * @param color color of pixel that is to be drawn.
 * 
 */
void drawPixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Loads the xpm specified
 * 
 * @param xpm Name of the xpm to be loaded
 * @return xpm_image_t with the loaded xpm
 */
xpm_image_t (loadXpm)(xpm_map_t xpm);

/**
 * @brief Draws the game background.
 * 
 * @param img xpm_image_t of the background.
 */
void drawBackground(xpm_image_t img);

/**
 * @brief Draws the specified xpm img on the screen on the correct coordinates
 * 
 * @param x x coordinate to draw the img
 * @param y y coordinate to draw the img
 * @param img xpm_image_t to be drawn of the screen
 * @return 0 on success
 */
int drawXpm(int x, int y, xpm_image_t img);

/**
 * @brief Loads the xpm of the background of the game
 * 
 * @return xpm_image_t with the backgroung of the game
 */
xpm_image_t loadBackground();

/**
 * @brief Loads the xpm of the initial screen with the title of the game
 * 
 * @return xpm_image_t with the initial screen with the title off the game
 */
xpm_image_t loadInitialScreen();

/**
 * @brief Draws the background of the game on screen
 */
void drawBackground();

/**
 * @brief Displays the screen of the game
 */
void displayScreen();

/**
 * @brief Draws an alien.
 * 
 * @param a1 alien that is supposed to be drawn.
 * @param mov specifies which of the alien phases is drawn (moving or not moving).
 * 
 * @return 0 on success, non-zero otherwise
 */
int drawAlien(Alien *a1, bool mov);

/**
 * @brief Draws the mouse.
 * 
 * @param mouse Mouse object that will be drawn
 */
void drawMouse(Mouse *mouse);

#endif //PROJ_GRAPHIC_H
