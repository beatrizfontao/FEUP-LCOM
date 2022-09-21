#include "shipBullet.h"

xpm_image_t loadShipBulletXpm() {
    xpm_image_t shipBullet_xpm;
    xpm_load(ship_bullet, XPM_INDEXED, &shipBullet_xpm);
    return shipBullet_xpm;
}

void createShipBullet(int x, int y, int speed, xpm_image_t img) {

    ShipBullet *shipBullet = (ShipBullet *) malloc(sizeof(ShipBullet));

    shipBullet->x = x;
    shipBullet->y = y;
    shipBullet->speed = speed;
    shipBullet->img = img;
    shipBullet->hitAlien = false;

    for (int i = 0; i < MAX_SHIP_BULLETS; i++) {
        if (shipBullets[i] == NULL || shipBullets[i]->hitAlien) {
            shipBullets[i] = shipBullet;
        }
    }
}

void updateShipBulletPosition() {
    for (int i = 0; i < MAX_SHIP_BULLETS; i++) {
        if (shipBullets[i] != NULL && !shipBullets[i]->hitAlien) {
            if (shipBullets[i]->y - shipBullets[i]->speed >= 0) {
                shipBullets[i]->y = shipBullets[i]->y - shipBullets[i]->speed;
            } else {
                removeShipBullet(shipBullets[i]);
            }
        }
    }
}

void drawShipBullets() {
    for (int i = 0; i < MAX_SHIP_BULLETS; i++) {
        if (shipBullets[i] != NULL && !shipBullets[i]->hitAlien) {
            drawXpm(shipBullets[i]->x, shipBullets[i]->y, shipBullets[i]->img);
        }
    }
}

void removeShipBullet(ShipBullet *shipBullet) {
    free(shipBullet);
    shipBullet = NULL;
}

void initShipBullets() {
    for (int i = 0; i < MAX_SHIP_BULLETS; i++) {
        shipBullets[i] = NULL;
    }
}

void verifyAlienAndBulletCollision(Alien *a1, int *killCount, Ship *ship) {
    for (int j = 0; j < MAX_SHIP_BULLETS; j++) {
        if (shipBullets[j] == NULL) { continue; }
        if (a1->x < shipBullets[j]->x && ((a1->x + a1->width) > shipBullets[j]->x && a1->alive) &&
            shipBullets[j]->y > a1->y && shipBullets[j]->y < (a1->y + a1->height) && !shipBullets[j]->hitAlien) {
            a1->alive = false;
            (*killCount)++;
            increaseScore(a1->value, ship);
            shipBullets[j]->hitAlien = true;
            removeShipBullet(shipBullets[j]);
            return;
        }
    }
}

void removeAllShipBullets(){
    for(int i = 0; i < SIZE_OF_ALIENS; i++){
        removeShipBullet(shipBullets[i]);
    }
}
