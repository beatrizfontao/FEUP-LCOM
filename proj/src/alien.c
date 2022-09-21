#include "alien.h"

void change_alien_x_coordinates(Alien *a1, const int xf) {
    a1->x = a1->x + xf;
}

void change_alien_y_coordinates(Alien *a1, const int yf) {
    a1->y = a1->y + yf;
}

Alien createAlien(int xi, int yi, xpm_map_t img, xpm_map_t img_mov, int value) {
    xpm_image_t alien_xpm_mov;
    xpm_load(img_mov, XPM_INDEXED, &alien_xpm_mov);
    xpm_image_t alien_xpm;
    xpm_load(img, XPM_INDEXED, &alien_xpm);
    return (Alien) {.x = xi, .y = yi, .img = alien_xpm, .img_mov = alien_xpm_mov, .width=60, .height=40, .alive = true, .value = value};
}

void decrement_alien_x_coordinates(Alien *a1) {
    a1->x = a1->x - 1;
}

void decrement_alien_y_coordinates(Alien *a1) {
    a1->y = a1->y - 1;
}

void increment_alien_x_coordinates(Alien *a1) {
    a1->x = a1->x + 1;
}

int alien_get_x(Alien *a1) {
    return a1->x;
}

int alien_get_y(Alien *a1) {
    return a1->y;
}

int alien_get_width(Alien *a1) {
    return a1->width;
}

int alien_get_height(Alien *a1) {
    return a1->height;
}

void change_all_y(Alien aliens[], int yf, int size_array) {
    for (int i = 0; i < size_array; i++) {
        Alien *a1 = &aliens[i];
        change_alien_y_coordinates(a1, yf);
    }
}

