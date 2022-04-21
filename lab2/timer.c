#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hookid = 0;
int timer_counter = 0;

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {

    if(freq < 19 || freq > TIMER_FREQ)
      return 1;

    if(timer < 0 || timer > 2)
      return 1;

    uint8_t status;

    if(timer_get_conf(timer, &status))
      return 1;

    //conservar os 4 bits menos significativos
    status = (status & (BIT(0) | BIT(1) | BIT(2) | BIT(3)));
    uint8_t controlWord = status | TIMER_LSB_MSB;

    uint8_t timer_reg = TIMER_0;
    uint16_t frequency = TIMER_FREQ / freq;

    switch(timer){
      case 0:
        controlWord = controlWord | TIMER_SEL0;
        break;
      case 1:
        controlWord = controlWord | TIMER_SEL1;
        timer_reg = TIMER_1;
        break;
      case 2:
        controlWord = controlWord | TIMER_SEL2;
        timer_reg = TIMER_2;
        break;
      default:
        return 1;
    }

    // Enviar a control word para o registo de controlo
    if(sys_outb(TIMER_CTRL, controlWord)) {
      return 1;
    }

    uint8_t lsb, msb;

    if(util_get_LSB(frequency, &lsb))
      return 1;

    if(util_get_MSB(frequency, &msb))
      return 1;

    if(sys_outb(timer_reg, lsb))
      return 1;

    if(sys_outb(timer_reg, msb))
      return 1;

    return 0;
}

int(timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hookid;

  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookid))
    return 1;

  return 0;
}

int(timer_unsubscribe_int)() {

  if(sys_irqrmpolicy(&hookid))
    return 1;

  return 0;
}

void(timer_int_handler)() {
  timer_counter ++;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {

  if (timer < 0 || timer > 2)
    return 1;

  uint8_t readBack = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  uint8_t timerReg = TIMER_0;

  switch (timer) {
    case 0:
      timerReg = TIMER_0;
      break;
    case 1:
      timerReg = TIMER_1;
      break;
    case 2:
      timerReg = TIMER_2;
      break;
    default:
      return 1;
  }

  if (sys_outb(TIMER_CTRL, readBack))
    return 1;

  if (util_sys_inb(timerReg, st))
    return 1;

  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {

  if (timer < 0 || timer > 2)
    return 1;

  union timer_status_field_val val;

  uint8_t aux;

  switch (field) {
    case tsf_all:
      val.byte = st;
      break;
    case tsf_initial:
      aux = (st & TIMER_RB_COUNT_ & TIMER_RB_STATUS_);
      val.in_mode = aux;
      break;
    case tsf_mode:
      aux = (st & BIT(1) & BIT(2) & BIT(3));
      val.count_mode = aux;
      break;
    case tsf_base:
      if ((st & TIMER_BCD) == 1)
        val.bcd = true;
      else
        val.bcd = false;
      break;
    default:
      break;
  }

  if (timer_print_config(timer, field, val))
    return 1;

  return 0;
}
