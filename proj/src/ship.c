#include "ship.h"

Ship *createShip(int x, int y, int speed) {
    Ship *ship = (Ship *) malloc(sizeof(Ship));

    ship->x = x;
    ship->y = y;
    ship->lives = NUMLIVES;
    ship->speed = speed;
    ship->score = 0; 

    xpm_image_t ship_xpm;
    xpm_load(ship_image, XPM_INDEXED, &ship_xpm);

    ship->img = ship_xpm;

    return ship;
}

void drawShip(Ship *ship) {
    drawXpm(ship->x, ship->y, ship->img);
}

static int clamp(int min, int max, int value) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

Ship *updateShipPosition(Ship *ship, KeyActivity key) {
    int xf;
    switch (key) {
        case A_Pressed:
            xf = ship->x - ship->speed;
            break;
        case A_Released:
            return ship;
        case D_Pressed:
            xf = ship->x + ship->speed;
            break;
        case D_Released:
            return ship;
        case Invalid_key:
            return ship;
        default:
            break;
    }

    xf = clamp(X_MIN, X_MAX - ship->img.width, xf);
    ship->x = xf;

    return ship;
}

void removeShip(Ship *ship) {
    free(ship);
    ship = NULL;
}

void drawScore(Ship *ship){
    xpm_image_t temp;
    if(ship->score == 0){
        temp = zero_xpm;
        drawXpm(120, 735, temp);
        return; 
    }
    
    int count = 0; 
    int div = ship->score;
    while(div>0){
        div /= 10; 
        count++;
    }
    xpm_image_t* score; 
    score = (xpm_image_t*)malloc(sizeof(xpm_image_t)*count);
    int x = 120;
    int res;
    div = ship->score;
    int aux = count;
    while(count != 0){
        res = div%10; 
        div /= 10; 
        switch(res){
            case 0:
                temp = zero_xpm;
                break;
            case 1:
                temp = one_xpm;
                break;
            case 2:
                temp = two_xpm;
                break;
            case 3:
                temp = three_xpm;
                break;
            case 4:
                temp = four_xpm;
                break;
            case 5:
                temp = five_xpm;
                break;
            case 6:
                temp = six_xpm;
                break;
            case 7:
                temp = seven_xpm;
                break;
            case 8:
                temp = eight_xpm;
                break;
            case 9: 
                temp = nine_xpm;
                break;
        }
        score[count-1] = temp;
        count--;
    }
    for(int i = 0; i < aux; i++){
        drawXpm(x, 735, score[i]);
        x+=24;
    }
    
    free(score);
}

void drawLives(Ship *ship){
    xpm_image_t live;
    switch(ship->lives){
        case 1:
            live = one_xpm;
            break;
        case 2:
            live = two_xpm;
            break;
        case 3:
            live = three_xpm;
            break;
    }
    drawXpm(1000, 735, live);
}

void increaseScore(int scoreToAdd, Ship* ship){
    ship->score += scoreToAdd;
}

void initNumbers(){
    xpm_load(zero, XPM_INDEXED, &zero_xpm);
    xpm_load(one, XPM_INDEXED, &one_xpm);
    xpm_load(two, XPM_INDEXED, &two_xpm);
    xpm_load(three, XPM_INDEXED, &three_xpm);
    xpm_load(four, XPM_INDEXED, &four_xpm);
    xpm_load(five, XPM_INDEXED, &five_xpm);
    xpm_load(six, XPM_INDEXED, &six_xpm);
    xpm_load(seven, XPM_INDEXED, &seven_xpm);
    xpm_load(eight, XPM_INDEXED, &eight_xpm);
    xpm_load(nine, XPM_INDEXED, &nine_xpm);
}
