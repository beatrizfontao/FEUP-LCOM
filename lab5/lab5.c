// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include "VBE.h"
#include "timer.h"
#include "vg.h"
#include <stdint.h>
#include <stdio.h>

//#include "pixmap.h"

#include "kbc.h"

extern int hookid;
extern int timer_counter;

extern int kbd_hookid; // hookid, timer_counter;
extern uint8_t scancode, statuscode;


static xpm_row_t const monster1[] = {
  "46 23 2",
  "  0",
  "x 63",
  "          xxxx                  xxxx          ",
  "          xxxx                  xxxx          ",
  "          xxxx                  xxxx          ",
  "              xxxx          xxxx              ",
  "              xxxx          xxxx              ",
  "              xxxx          xxxx              ",
  "          xxxxxxxxxxxxxxxxxxxxxxxxxx          ",
  "          xxxxxxxxxxxxxxxxxxxxxxxxxx          ",
  "          xxxxxxxxxxxxxxxxxxxxxxxxxx          ",
  "      xxxxxxxx    xxxxxxxxxxx    xxxxxxx      ",
  "      xxxxxxxx    xxxxxxxxxxx    xxxxxxx      ",
  "      xxxxxxxx    xxxxxxxxxxx    xxxxxxx      ",
  "  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  ",
  "  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  ",
  "  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  ",
  "  xxx     xxxxxxxxxxxxxxxxxxxxxxxxxx     xxx  ",
  "  xxx     xxxxxxxxxxxxxxxxxxxxxxxxxx     xxx  ",
  "  xxx     xxxxxxxxxxxxxxxxxxxxxxxxxx     xxx  ",
  "  xxx     xxxx                  xxxx     xxx  ",
  "  xxx     xxxx                  xxxx     xxx  ",
  "  xxx     xxxx                  xxxx     xxx  ",
  "              xxxxxxx    xxxxxxx              ",
  "              xxxxxxx    xxxxxxx              ",
  "              xxxxxxx    xxxxxxx              "};


extern uint8_t BitsPerPixel;
extern uint16_t XResolution;
extern uint16_t YResolution;
extern phys_bytes PhysBasePtr;

extern uint8_t RedMaskSize;        /* size of direct color red mask in bits */
extern uint8_t RedFieldPosition;   /* bit position of lsb of red mask */
extern uint8_t GreenMaskSize;      /* size of direct color green mask in bits */
extern uint8_t GreenFieldPosition; /* bit position of lsb of green mask */
extern uint8_t BlueMaskSize;       /* size of direct color blue mask in bits */
extern uint8_t BlueFieldPosition;  /* bit position of lsb of blue mask */

extern uint8_t color_mode;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {

  if (vg_initialize(mode)) {
    return 1;
  }

  sleep(delay);

  if (vg_exit())
    return 1;

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  vbe_mode_info_t info;
  if (vg_get_mode_info(&info, mode))
    return 1;

  if (vg_set_mode(mode))
    return 1;

  if (vg_drawrectangle(x, y, width, height, color)) {
    return 1;
  }

  if (wait_esckey())
    return 1;

  if (vg_exit())
    return 1;

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  vbe_mode_info_t info;
  if (vg_get_mode_info(&info, mode))
    return 1;

  if (vg_set_mode(mode))
    return 1;

  if (vg_drawpattern(no_rectangles, first, step)) {
    return 1;
  }

  if (wait_esckey())
    return 1;

  if (vg_exit())
    return 1;

  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  uint16_t mode = 0x105;

  vbe_mode_info_t info;
  if (vg_get_mode_info(&info, mode))
    return 1;

  if (vg_set_mode(mode))
    return 1;

  xpm_image_t img;

  xpm_load(monster1, XPM_INDEXED, &img);

  if(draw_xpm(img, x, y))
    return 1;

  if (wait_esckey())
    return 1;

  if(vg_exit())
    return 1;

  return 0;
}


int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {

  int fps = 60 / fr_rate;

  int mov_x, mov_y;

  if (speed > 0) {
    // movimento vertical
    if (xi == xf) {
      mov_y = speed;
    }
    // movimento horizontal
    else {
      mov_x = speed;
    }
  }

  else {
    fps = fps * -speed;
    // movimento vertical
    if (xi == xf) {
      mov_y = 1;
    }
    // movimento horizontal
    else {
      mov_x = 1;
    }
  }

  // verificar o sentido do movimento
  if (xf < xi) {
    mov_x = -mov_x;
  }
  if (yf < yi) {
    mov_y = -mov_y;
  }

  // inicializar com o modo 0x105
  if (vg_initialize(0x105)) {
    return 1;
  }

  xpm_image_t img;

  xpm_load(xpm, XPM_INDEXED, &img);

  // desenha a primeira imagem
  if (draw_xpm(img, xi, yi))
    return 1;

  uint8_t mouse_bit_no;

  if (timer_subscribe_int(&mouse_bit_no))
    return 1;

  uint8_t kbd_bit_no;

  if (kbd_subscribe_int(&kbd_bit_no))
    return 1;

  int ipc_status, r;
  message msg;
  int size = 1;
  uint8_t bytes[2];

  bool moving = true;

  uint16_t prev_x, prev_y;

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
            size = 1;
          }
          if (msg.m_notify.interrupts & BIT(mouse_bit_no)) {
            timer_int_handler();
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else {
    }
    if (moving) {
      if (timer_counter >= fps) {

        prev_x = xi;
        prev_y = yi;
        xi += mov_x;
        yi += mov_y;

        // ver se é o último movimento a fazer
        if ((xi >= xf && mov_x == 1) || (yi >= yf && mov_y == 1)) {
          if (draw_next_xpm(xpm, prev_x, prev_y, xf, yf)) {
            return 1;
          }
          moving = false;
        }
        else {
          if (draw_next_xpm(xpm, prev_x, prev_y, xf, yf)) {
            return 1;
          }
        }

        timer_counter = 0;
      }
      else if (timer_counter == -speed * fps) {
        xi += mov_x;
        yi += mov_y;

        if (xi == xf && mov_x == 1)
          moving = false;
        if (yi == yf && mov_y == 1)
          moving = false;

        if (draw_next_xpm(xpm, prev_x, prev_y, xf, yf)) {
          return 1;
        }

        timer_counter = 0;
      }
    }
  }

  if (kbd_unsubscribe_int())
    return 1;

  if (timer_unsubscribe_int())
    return 1;

  if (vg_exit())
    return 1;

  return 0;
}
