#ifndef PROJ_BULLET_H
#define PROJ_BULLET_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "graphic.h"
#include "resources/ship_bullet.xpm"
#include "macros.h"
#include "ship.h"

/**
 * @brief Struct for keeping track of a bullet of ship
 */
typedef struct {
    int x, y; /* Coordinates of the bullet */
    int speed; /* Number of pixels the bullet moves each interrupt of the timer */
    xpm_image_t img; /* xpm_image_t with the loaded xpm of the bullet */
    bool hitAlien; /* Bool containing information on whether the bullet has or has not hit an alien*/
} ShipBullet;

ShipBullet *shipBullets[MAX_SHIP_BULLETS]; /* Array with at most MAX_SHIP_BULLETS bullets */

/**
 * @brief Loads the xpm for the ship bullet
 * 
 * @return xpm_image_t of the loaded xpm
 */
xpm_image_t loadShipBulletXpm();

/**
 * @brief Creates a Ship Bullet object and adds it the array of ship bullets
 * 
 * @param x Initial x coordinate
 * @param y Initial y coordinate
 * @param speed Initial speed of the bullet
 * @param img Loaded xpm of the bullet
 */
void createShipBullet(int x, int y, int speed, xpm_image_t img);

/**
 * @brief Initializes the array of ship bullet with NULL values
 */
void initShipBullets();

/**
 * @brief Draws all the ship's bullets currently being shot
 */
void drawShipBullets();

/**
 * @brief Updates all the positions of the ship's bullets acording to their speed
 */
void updateShipBulletPosition();

/**
 * @brief Frees the memory allocated for the ship bullet
 * 
 * @param shipBullet Bullet to be removed
 */
void removeShipBullet(ShipBullet *shipBullet);

/**
 * @brief Checks if there's been a colision between the bullets and and alien. Updates the score and the kill count
 * 
 * @param a1 Alien to check if there's been a colision with
 * @param killCount Number of aliens already killed
 * @param ship Player's ship
 */
void verifyAlienAndBulletCollision(Alien *a1, int *killCount, Ship *ship);

/**
 * @brief Calls the function removeShipBullet for every bullet in the array shipBullets
 */
void removeAllShipBullets();

#endif //PROJ_BULLET_H
