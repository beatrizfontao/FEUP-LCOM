#include "graphic.h"

extern uint16_t Xres; // horizontal resolution
static uint16_t Yres; // vertical resolution
static int bitsPerPixel;

static uint8_t color_mode;

void *video_mem;
void *display_mem;

int vg_get_mode_info(uint16_t *mode, vbe_mode_info_t *info) {

    struct minix_mem_range mr;
    int r;

    if (vbe_get_mode_info(*mode, info)) {
        return 1;
    }

    //atribuir os valores obtidos às variaveis
    Xres = info->XResolution;
    Yres = info->YResolution;
    bitsPerPixel = info->BitsPerPixel;

    color_mode = info->MemoryModel;

    unsigned int vram_base, vram_size;
    vram_base = info->PhysBasePtr;
    vram_size = Xres * Yres * (bitsPerPixel / 8); // para ficar em bytes

    //mapear memória
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    //permissions
    if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    display_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

    video_mem = malloc(Xres * Yres * bitsPerPixel / 8);

    if (display_mem == MAP_FAILED)
        panic("couldn't map video memory");

    return 0;
}

int vg_set_mode(uint16_t *mode) {
    if (*mode != 0x105 && *mode != 0x110 && *mode != 0x115 && *mode != 0x11A && *mode != 0x14C) {
        return 1;
    }

    if (mode == NULL)
        return 1;

    reg86_t aux;
    memset(&aux, 0, sizeof(aux));

    aux.intno = 0x10;
    aux.ah = 0x4F;
    aux.al = 0x02; // function 0x02 = set VBE mode
    aux.bx = BIT(14) | *mode;

    /* Make the BIOS call */
    if (sys_int86(&aux) != OK) {
        printf("\tvideo_test_init(): sys_int86() failed \n");
        return 1;
    }

    if (!(aux.ah == 0x00 && aux.al == 0x4F))
        return 1;

    return 0;
}

xpm_image_t (loadXpm)(xpm_map_t xpm) {
    xpm_image_t img;

    xpm_load(xpm, XPM_INDEXED, &img);

    return img;
}

void drawPixel(uint16_t x, uint16_t y, uint32_t color) {
    uint8_t *aux = video_mem;
    uint32_t n = (bitsPerPixel + 7) >> 3; //arredonda para cima o nº de bytes na cor - tipo o ceil
    aux = aux + n * (y * Xres + x); // calcula onde colocar a cor
    memcpy(aux, &color, n); //copia n bytes da color para aux
}

xpm_image_t loadBackground(){
    xpm_image_t background_img;
    background_img = loadXpm(background);
    return background_img;
}
xpm_image_t loadInitialScreen(){
    xpm_image_t initial_screen_img;
    initial_screen_img = loadXpm(initial_screen);
    return initial_screen_img;
}


void drawBackground(xpm_image_t img) {
    memcpy(video_mem, img.bytes, img.height * img.width);
}

int drawXpm(int x, int y, xpm_image_t img) {
    int counter = 0;

    for (int i = y; i < img.height + y; i++) {
        for (int j = x; j < img.width + x; j++) {
            drawPixel(j, i, img.bytes[counter++]);
        }
    }

    return 0;
}

void drawMouse(Mouse *mouse){
    int counter = 0;

    for (int i = mouse->y; i < mouse->img.height + mouse->y; i++) {
        for (int j = mouse->x; j < mouse->img.width + mouse->x; j++) {
            if(mouse->img.bytes[counter] != 0)
                drawPixel(j, i, mouse->img.bytes[counter]);
            counter++;
        }
    }
}

//double buffer
void displayScreen() {
    int bytes = (bitsPerPixel + 7) >> 3;
    memcpy(display_mem, video_mem, Yres * Xres * bytes);
}

int drawAlien(Alien *a1, bool mov) {
    
    if(!a1->alive){
        return 1;
    }
    if (mov) {
        a1->width = a1->img_mov.width;
        a1->height = a1->img_mov.height;
        return drawXpm(a1->x, a1->y, a1->img_mov);
    } else {
        a1->width = a1->img.width;
        a1->height = a1->img.height;
        return drawXpm(a1->x, a1->y, a1->img);
    }
}
