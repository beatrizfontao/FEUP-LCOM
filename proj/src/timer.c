#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int timer_hookid = 0; /*!< Timer's hook id */
int timer_counter = 0; /*!< Counter for the timer */

int(timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = timer_hookid;

  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hookid))
    return 1;

  return 0;
}

int(timer_unsubscribe_int)() {

  if(sys_irqrmpolicy(&timer_hookid))
    return 1;

  return 0;
}

void(timer_int_handler)() {
  timer_counter ++;
}
