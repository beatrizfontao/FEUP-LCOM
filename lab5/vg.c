#include "vg.h"

uint8_t BitsPerPixel;
uint16_t XResolution;
uint16_t YResolution;
phys_bytes PhysBasePtr;
static void *video_mem;

/*colors*/
uint8_t RedMaskSize;        /* size of direct color red mask in bits */
uint8_t RedFieldPosition;   /* bit position of lsb of red mask */
uint8_t GreenMaskSize;      /* size of direct color green mask in bits */
uint8_t GreenFieldPosition; /* bit position of lsb of green mask */
uint8_t BlueMaskSize;       /* size of direct color blue mask in bits */
uint8_t BlueFieldPosition;  /* bit position of lsb of blue mask */

uint8_t color_mode;

int kbd_hookid; // hookid, timer_counter;
uint8_t scancode, statuscode;

int vg_initialize(uint16_t mode){
  vbe_mode_info_t info;
  if(vg_get_mode_info(&info, mode))
    return 1;

  if(vg_set_mode(mode))
    return 1;

  return 0;
}

int vg_get_mode_info(vbe_mode_info_t *info, uint16_t mode) {

  if (vbe_get_mode_info(mode, info))
    return 1;

  BitsPerPixel = info->BitsPerPixel;
  XResolution = info->XResolution;
  YResolution = info->YResolution;
  PhysBasePtr = info->PhysBasePtr;

  RedMaskSize = info->RedMaskSize;
  RedFieldPosition = info->RedFieldPosition;
  GreenMaskSize = info->GreenMaskSize;
  GreenFieldPosition = info->GreenFieldPosition;
  BlueMaskSize = info->BlueMaskSize;
  BlueFieldPosition = info->BlueFieldPosition;

  color_mode = info->MemoryModel;

  struct minix_mem_range mr;
  unsigned int vram_base = PhysBasePtr;
  unsigned int vram_size = XResolution * YResolution * (BitsPerPixel / 8);
  int r;

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  return 0;
}

int vg_set_mode(uint16_t mode) {
  reg86_t r;
  memset(&r, 0, sizeof(r));

  r.ah = 0x4F;
  r.al = 0x02;
  r.bx = BIT(14) | mode;
  r.intno = 0x10;

  if (sys_int86(&r) != OK) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return 1;
  }

  if (r.al != 0x4F || r.ah != 0x00)
    return 1;

  return 0;
}

void vg_fillPixel(uint16_t x, uint16_t y, uint32_t color) {
  uint8_t *aux = video_mem;
  uint32_t n = (BitsPerPixel + 7) >> 3;
  aux = aux + n * (XResolution * y + x);
  memcpy(aux, &color, n);
}

int vg_drawrectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  for (uint16_t j = y; j < (y + height); j++) {
    for (uint16_t i = x; i < (x + width); i++) {
      vg_fillPixel(i, j, color);
    }
  }

  return 0;
}

int vg_drawpattern(uint8_t no_rectangles, uint32_t first, uint8_t step) {

  uint16_t width = XResolution / no_rectangles;
  uint16_t height = YResolution / no_rectangles;

  uint32_t color;

  if (color_mode == INDEXED_MODE) {
    for (int row = 0; row < no_rectangles; row++) {
      for (int column = 0; column < no_rectangles; column++) {
        color = (first + (row * no_rectangles + column) * step) % (1 << BitsPerPixel);
        vg_drawrectangle(column * width, row * height, width, height, color);
      }
    }
  }
  else if (color_mode == DIRECT_MODE) {
    uint8_t redFirst = (first & RedMaskSize) >> RedFieldPosition;
    uint8_t greenFirst = (first & GreenMaskSize) >> GreenFieldPosition;
    uint8_t blueFirst = (first & BlueMaskSize) >> BlueFieldPosition;
    uint16_t red, green, blue;
    for (int row = 0; row < no_rectangles; row++) {
      for (int column = 0; column < no_rectangles; column++) {
        red = (redFirst + column * step) % (1 << RedMaskSize);
        green = (greenFirst + row * step) % (1 << GreenMaskSize);
        blue = (blueFirst + (column + row) * step) % (1 << BlueMaskSize);
        color = 0 | (red << RedFieldPosition) | (green << GreenFieldPosition) | (blue << BlueFieldPosition);
        vg_drawrectangle(column * width, row * height, width, height, color);
      }
    }
  }
  else {
    return 1;
  }
  return 0;
}

int draw_xpm(xpm_image_t img, uint16_t x, uint16_t y) {
  int counter = 0;

  for (int i = y; i < img.height + y; i++) {
    for (int j = x; j < img.width + x; j++) {
      vg_fillPixel(j, i, img.bytes[counter++]);
    }
  }

  return 0;
}

int draw_next_xpm(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf) {

  xpm_image_t img;
  uint8_t* map;

  map = xpm_load(xpm, XPM_INDEXED, &img);

  // "Apagar" a imagem anterior
  if(vg_drawrectangle(xi, yi, img.width, img.height, 0))
    return 1;

  xpm_load(xpm, XPM_INDEXED, &img);

  //desenha a nova imagem
  if (draw_xpm(img, xf, yf))
    return 1;

  return 0;
}

int wait_esckey() {

  uint8_t kbd_bit_no = BIT(kbd_hookid);

  if (kbd_subscribe_int(&kbd_bit_no))
    return 1;

  int ipc_status, r;
  message msg;
  int size = 1;
  uint8_t bytes[2];

  while (scancode != ESC_BREAK) { /* You may want to use a different condition */
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & BIT(kbd_bit_no)) {
            kbc_ih();
            if (twoBytes(scancode)) {
              size = 2;
              continue;
            }
            if (size == 1) {
              bytes[0] = scancode;
            }
            else {
              bytes[0] = 0xE0;
              bytes[1] = scancode;
            }
            // kbd_print_scancode(makecode(scancode), size, bytes);
            size = 1;
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else {
    }
  }

  if (kbd_unsubscribe_int())
    return 1;

  return 0;
}
