#ifndef LAB5_VG_H
#define LAB5_VG_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "VBE.h"
#include "kbc.h"
#include "timer.h"

int vg_initialize(uint16_t mode);

int vg_get_mode_info(vbe_mode_info_t *info, uint16_t mode);

int vg_set_mode(uint16_t mode);

void vg_fillPixel(uint16_t x, uint16_t y, uint32_t color);

int vg_drawrectangle (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int vg_drawpattern (uint8_t no_rectangles, uint32_t first, uint8_t step);

int wait_esckey();

int draw_next_xpm(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf);

int draw_xpm(xpm_image_t img, uint16_t x, uint16_t y);

#endif // LAB5_VG_H
