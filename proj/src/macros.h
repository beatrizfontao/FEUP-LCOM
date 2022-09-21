#ifndef PROJ_MACROS_H
#define PROJ_MACROS_H

#define NUMLIVES 3 /*!< Number of lives of the player */

#define SHIP_YPOS 630 /*!< Initial y coordinate of the ship */
#define X_MAX 1000 /*!< Max x coordinate */
#define X_MIN 30 /*!< Min x coordinate */

#define X_RES 1024 /*!< Size of the screen (x axis) */
#define Y_RES 768 /*!< Size of the screen (y axis) */

#define MAX_SHIP_BULLETS 7 /*!< Max number of bullets of the ship */

#define SHIP_BULLET_SPEED 4 /*!< Number of pixels the ship bullet moves each interrupt of the timer*/
#define ALIEN_BULLET_SPEED 4 /*!< Number of pixels the alien bullet moves each interrupt of the timer*/

#define SIZE_OF_ALIENS 24 /*!< Max number of aliens on the screen at the same time */

#define X_LEFT_BORDER 20 /*!< Defines the right border of the screen */
#define X_RIGHT_BORDER 1004 /*!< Defines the left border of the screen */

#define INITIAL_ALIEN_SPEED 1 /*!< Alien speed at the start of the game */

#define TERRITORY 600 /*!< If the aliens cross this y coordinate the game ends */

#define ROW_ALIENS 3 /*!< max number of rows of aliens on screen at the same time */
#define COL_ALIENS 8/*!< max number of columns of aliens on screen at the same time */

#define COLOR 0 

#endif //PROJ_MACROS_H
