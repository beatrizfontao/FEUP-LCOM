#pragma once

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "macros.h"
#include "resources/alien1.xpm"
#include "resources/alien1_m.xpm"
#include "resources/alien2.xpm"
#include "resources/alien2_m.xpm"
#include "resources/alien3.xpm"
#include "resources/alien3_m.xpm"

/**
 * @brief Struct with the information of an alien
 */
typedef struct{
    int x; /*!< x coordinate of alien*/
    int y; /*!< y coordinate of the alien */
    int width; /*!< Width of the alien's xpm */
    int height; /*!< Height of the alien's xpm */
    xpm_image_t img; /*!< xpm_image_t with the loaded xpm of the alien*/
    xpm_image_t img_mov; /*!< xpm_image_t with the second loaded xpm of the alien used for movement*/
    bool alive; /*!< Boolean indicating whether the alien is alive or not*/
    int value; /*!< Points used to calculate the ship's score */
}Alien;

/**
 * @brief Increases/Decreased the x coordinate of the alien by an xf amount
 * 
 * @param a1 Pointer to the alien to have its x coordinate changed
 * @param xf Amount to have the x coordinate increased/decreased
 */
void change_alien_x_coordinates(Alien *a1, const int xf);

/**
 * @brief Increases/Decreased the y coordinate of the alien by an xf amount
 * 
 * @param a1 Pointer to the alien to have its y coordinate changed
 * @param yf Amount to have the y coordinate increased/decreased
 */
void change_alien_y_coordinates(Alien *a1, const int yf);

/**
 * @brief Allocates memory and creates an Alien object
 * 
 * @param xi Initial x coordinate of the alien
 * @param yi Initial y coordinate of the alien
 * @param img name of the xpm of the alien 
 * @param img_mov name of the second xpm of the alien
 * @param value amount of points the alien is worth
 * @return Newly created alien object
 */
Alien createAlien(int xi, int yi, xpm_map_t img, xpm_map_t img_mov, int value);

/**
 * @brief Decreases the x coordinate of the alien by one
 * 
 * @param a1 Pointer to the alien to have its x coordinate decreased
 */
void decrement_alien_x_coordinates(Alien *a1);

/**
 * @brief Decreases the y coordinate of the alien by one
 * 
 * @param a1 Pointer to the alien to have its y coordinate decreased
 */
void decrement_alien_y_coordinates(Alien *a1);

/**
 * @brief Increases the x coordinate of the alien by one
 * 
 * @param a1 Pointer to the alien to have its x coordinate increased
 */
void increment_alien_x_coordinates(Alien *a1);

/**
 * @brief Gets the x coordinate of the alien specified
 * 
 * @param a1 Pointer to the alien
 * @return int with the x coordinate of the alien
 */
int alien_get_x(Alien *a1);

/**
 * @brief Gets the y coordinate of the alien specified
 * 
 * @param a1 Pointer to the alien
 * @return int with the y coordinate of the alien
 */
int alien_get_y(Alien *a1);

/**
 * @brief Gets the width of the alien specified
 * 
 * @param a1 Pointer to the alien
 * @return int with the width of the alien
 */
int alien_get_width(Alien *a1);

/**
 * @brief Gets the height of the alien specified
 * 
 * @param a1 Pointer to the alien
 * @return int with the height of the alien
 */
int alien_get_height(Alien* a1);

/**
 * @brief Increases/Decreases all of the aliens y coordinates by the amount yf
 * 
 * @param aliens Array with all the aliens to have its y coordinate changed
 * @param yf Amount to have the y coordinate increased/decreased
 * @param size_array Size of the aliens array
 */
void change_all_y(Alien aliens[], int yf, int size_array);
