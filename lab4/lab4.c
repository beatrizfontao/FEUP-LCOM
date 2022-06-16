// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "mouse.h"
#include "timer.h"

// Any header files included below this line should have been created by you
extern int mouse_hookid, hookid;
extern uint8_t statuscode, scancode;
extern int ih_error;
extern uint32_t timer_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(mouse_test_packet)(uint32_t cnt) {

  uint8_t bit_no;

  if(enable_mouse(ENABLE_MOUSE))
    return 1;

  if (mouse_subscribe_int(&bit_no))
    return 1;

  int currenByte = 1;
  uint8_t bytes[3];

  struct packet pp;

  int ipc_status, r;
  message msg;

  while (cnt > 0) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(bit_no)) {
            mouse_ih();
            if (ih_error)
              continue;
            if (currenByte == 1) {
              if ((scancode & BIT(3)) == 0)
                continue;
              bytes[0] = scancode;
              currenByte = 2;
            }
            else if (currenByte == 2) {
              bytes[1] = scancode;
              currenByte = 3;
            }
            else if (currenByte == 3) {
              bytes[2] = scancode;
              currenByte = 1;
              mouse_getPacket(bytes, &pp);
              mouse_print_packet(&pp);
              cnt--;
            }
          }
          break;
        default:
          break;
      }
    }
    else {
    }
  }

  if (mouse_unsubscribe_int())
    return 1;

  if(enable_mouse(DISABLE_MOUSE))
    return 1;

  return 0;
}

int(mouse_test_async)(uint8_t idle_time) {

  uint8_t bit_no, timer_bit_no;

  if(enable_mouse(ENABLE_MOUSE))
    return 1;

  if (mouse_subscribe_int(&bit_no))
    return 1;

  if (timer_subscribe_int(&timer_bit_no))
    return 1;

  int currenByte = 1;
  uint8_t bytes[3];

  struct packet pp;

  int aux_counter = 0;
  int ipc_status, r;
  message msg;

  while (aux_counter <= idle_time) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(bit_no)) {
            mouse_ih();
            if (ih_error)
              continue;
            if (currenByte == 1) {
              if ((scancode & BIT(3)) == 0)
                continue;
              bytes[0] = scancode;
              currenByte = 2;
            }
            else if (currenByte == 2) {
              bytes[1] = scancode;
              currenByte = 3;
            }
            else if (currenByte == 3) {
              bytes[2] = scancode;
              currenByte = 1;
              mouse_getPacket(bytes, &pp);
              mouse_print_packet(&pp);
            }
            aux_counter = 0;
            timer_counter = 0;
          }
          if (msg.m_notify.interrupts & BIT(timer_bit_no)) {
            if(timer_counter % 60 == 0)
              aux_counter ++;
            timer_int_handler();
          }
          break;
        default:
          break;
      }
    }
    else {
    }
  }

  if(timer_unsubscribe_int())
    return 1;

  if (mouse_unsubscribe_int())
    return 1;

  if(enable_mouse(DISABLE_MOUSE))
    return 1;

  return 0;
}

int (mouse_test_gesture)() {
    printf("%s: under construction\n", __func__);
    return 1;
}

int(mouse_test_remote)(uint16_t period, uint8_t cnt) {
  /* This year you need not implement this. */
  printf("%s(%u, %u): under construction\n", __func__, period, cnt);
  return 1;
}
