#ifndef PROJ_ALIENBULLET_H
#define PROJ_ALIENBULLET_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "graphic.h"
#include "resources/alien_bullet.xpm"
#include "macros.h"
#include "ship.h"

/**
 * @brief Struct with the information of the alien bullet
 */
typedef struct {
    int x, y; /*!< x and y coordinates of the alien bullet */
    int speed; /*!< Number of pixels the bullet moves each interrupt of the timer */
    xpm_image_t img; /*!< xpm_image_t with the loaded xpm of the bullet */
    bool active; /*!< Whether or not the alien is active */
} AlienBullet;

AlienBullet *alienBullet; /*!< Pointer to the alien bullet object */

/**
 * @brief Loads the xpm of the bullet to the given xpm_image_t
 * 
 * @return xpm_image_t of the loaded xpm
 */
xpm_image_t loadAlienBulletXpm();

/**
 * @brief Allocates the memory for a new alien bullet
 */
void initAlienBullet();

/**
 * @brief Creates a Alien Bullet object
 * 
 * @param x Initial x coordinate of the alien bullet
 * @param y Initial y coordinate of the alien bullet
 * @param speed Initial speed the alien bullet
 * @param img xpm_image_t with the loaded xpm of the bullet
 */
void createAlienBullet(int x, int y, int speed, xpm_image_t img);

/**
 * @brief Updates the alien bullet position according to its position and speed
 */
void updateAlienBulletPosition();

/**
 * @brief Draws the alien bullet if it is active according to its position
 */
void drawAlienBullet();

/**
 * @brief Chooses a random alien and fires a bullet from it
 * 
 * @param aliens Array with all the aliens in game
 * @return int with the position in the array of the alien that fired
 */
int generateAlienBullet(Alien aliens[]);

/**
 * @brief Verifies if the ship was hit by an alien bullet. Updates the number of lives of the ship
 * 
 * @param ship Pointer to the ship to check if there was a colision with
 * @return true if a bullet hit the ship
 * @return false if a bullte didn't hit the ship
 */
bool verifyShipAndBulletCollision(Ship *ship);

/**
 * @brief Frees the memory that was allocated for the alien bullet
 */
void removeAlienBullet();

#endif //PROJ_ALIENBULLET_H
