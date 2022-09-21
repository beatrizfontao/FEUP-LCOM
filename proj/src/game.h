#ifndef PROJ_GAME_H
#define PROJ_GAME_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "graphic.h"
#include "i8042.h"
#include "i8254.h"
#include "keyboard.h"
#include "menu.h"
#include "utils.h"
#include "mouse.h"
#include "ship.h"
#include "timer.h"
#include "shipBullet.h"
#include "alienBullet.h"
#include "alien.h"
#include "rtc.h"

#include "macros.h"

/**
 * @brief Enum with the possible game states.
 */
typedef enum {
    MENU_DISPLAY, /*!<Menu display screen*/
    INSTRUCTIONS_DISPLAY, /*!<Instructions display screen*/
    PLAYING, /*!<User is playing*/
    NEXT_ROUND, /*!<Player moves on to the next round*/
    EXIT/*!<Exit game*/
} game_state;

/**
 * @brief Function that initiates game objects (Aliens, Alien Bullets, Ship Bullets, etc.)
 * 
 * @param ship Ship object to be initialized
 */
void (initGame)(Ship *ship);

/**
 * @brief Function containing the main interrupt loop
 *
 * @return 0 if the function ends successfully or 1 otherwise
 */
int (game_loop)();

#endif //PROJ_GAME_H
