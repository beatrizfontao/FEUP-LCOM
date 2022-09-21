#ifndef PROJ_SHIP_H
#define PROJ_SHIP_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "graphic.h"

#include "macros.h"
#include "graphic.h"

#include "resources/ship.xpm"
#include "resources/0.xpm"
#include "resources/1.xpm"
#include "resources/2.xpm"
#include "resources/3.xpm"
#include "resources/4.xpm"
#include "resources/5.xpm"
#include "resources/6.xpm"
#include "resources/7.xpm"
#include "resources/8.xpm"
#include "resources/9.xpm"

xpm_image_t zero_xpm, one_xpm, two_xpm, three_xpm, four_xpm, five_xpm, six_xpm, seven_xpm,eight_xpm, nine_xpm; /*!< XPMs of the numbers */

/**
 * @brief Struct with the information of the player's ship
 */
typedef struct {
    int x, y; /*!< Coordinates of the ship*/
    int speed; /*!< Number of pixels the ship moves each interrupt of the keyboard/timer*/
    xpm_image_t img; /*!< xpm_image_t with the loaded xpm of the ship */
    int lives; /*!< Number of lives of the ship i.e. the number of times ship can still be hit by an alien bullet */
    int score; /*!< Current score of the ship */
} Ship;

/**
 * @brief Enumerated type for identifying the key being pressed/released
 */
typedef enum {
    A_Pressed, /*!< Key A is being pressed*/ 
    A_Released, /*!< Key A is being released*/ 
    D_Pressed, /*!< Key D is being pressed*/ 
    D_Released, /*!< Key A is being released*/ 
    Invalid_key /*!< Invalid key is being used*/ 
} KeyActivity;

/**
 * @brief Allocates memory and creates a Ship object
 * 
 * @param x X coordinate of the ship
 * @param y Y coordinate of the ship
 * @param speed Initial speed of the ship
 * @return Pointer of the newly created ship with all the specified values
 */
Ship *createShip(int x, int y, int speed);

/**
 * @brief Calls the fuction that draws XPMs to draw the ship
 * 
 * @param ship Pointer to the ship object
 */
void drawShip(Ship *ship);

/**
 * @brief Updates the ship's position based on the key being currently pressed/released
 * 
 * @param ship Pointer to the ship object
 * @param key Key being currently pressed/released
 * @return Pointer to the ship object that has been altered
 */
Ship *updateShipPosition(Ship *ship, KeyActivity key);

/**
 * @brief Frees the memory that was allocated for the ship
 * 
 * @param ship Pointer to the ship object to be removed
 */
void removeShip(Ship *ship);

/**
 * @brief Draws the score of the ship (based on how many and on the type of aliens that were killed)
 * 
 * @param ship Pointer to the ship object (contains the score)
 */
void drawScore(Ship *ship);

/**
 * @brief Draws the current number of lives of the ship (based on how many times the ship was hit)
 * 
 * @param ship Pointer to the ship object (contains the number of lives)
 */
void drawLives(Ship *ship);

/**
 * @brief Increases the score of the ship
 * 
 * @param scoreToAdd Amount of points to be added to the current score
 * @param ship Pointer to the ship object (contains the score)
 */
void increaseScore(int scoreToAdd, Ship *ship);

/**
 * @brief Initializes the xpms of the numbers 0-9
 */
void initNumbers();

#endif //PROJ_SHIP_H
