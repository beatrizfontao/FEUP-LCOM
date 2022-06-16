#include "mouse.h"

int mouse_hookid = 1;
uint8_t statuscode, scancode;
int ih_error = 0;

int (mouse_subscribe_int)(uint8_t *bit_no){
  *bit_no = mouse_hookid;

  if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hookid)){
    return 1;
  }

  return 0;
}

int (mouse_unsubscribe_int)(){

  if(sys_irqrmpolicy(&mouse_hookid))
    return 1;

  return 0;
}

int (utils_sys_inb)(int port, uint8_t *value){
  uint32_t aux;

  if(sys_inb(port, &aux))
    return 1;

  *value = (uint8_t)aux;

  return 0;
}

void(mouse_ih)(){
  if(utils_sys_inb(STAT_REG, &statuscode)){
    ih_error = 1;
  }

  if(!((statuscode & (PARITY_BIT | TIMEOUT_BIT)) == 0 && (statuscode & OBF_BIT))){
    scancode = 0;
    ih_error = 1;
    return;
  }

  utils_sys_inb(OUT_BUF, &scancode);
}

void (mouse_getPacket)(uint8_t bytes[3], struct packet* pp){
  pp->bytes[0] = bytes[0];
  pp->bytes[1] = bytes[1];
  pp->bytes[2] = bytes[2];

  pp->lb = bytes[0] & LEFT;
  pp->rb = (bytes[0] & RIGHT) >> 1;
  pp->mb = (bytes[0] & MIDDLE) >> 2;

  if(bytes[0] & Y_SIGN)
    pp->delta_y = bytes[2] | FILLMSB;
  else
    pp->delta_y = bytes[2];
  if(bytes[0] & X_SIGN)
    pp->delta_x = bytes[1] | FILLMSB;
  else
    pp->delta_x = bytes[1];

  // x and y overflow
  pp->x_ov = (bytes[0] & X_OVFL) >> 6;
  pp->y_ov = (bytes[0] & Y_OVFL) >> 7;
}

int (enable_mouse)(uint8_t cmd){
  uint8_t ack;
  do{
    if(utils_sys_inb(STAT_REG, &statuscode))
      return 1;
    if (statuscode & IBF_BIT) // checks if we can write
      continue;
    if(sys_outb(STAT_REG, MOUSE_COMMAND))
      return 1;

    if(utils_sys_inb(STAT_REG, &statuscode))
      return 1;
    if (statuscode & IBF_BIT) // checks if we can write
      continue;
    if(sys_outb(ARGS_REG, cmd))
      return 1;

    tickdelay(micros_to_ticks(20000));

    if(utils_sys_inb(OUT_BUF, &ack))
      return 1;

    if(ack == ACK_ERROR)
      return 1;

  }while(ack != ACK);

  return 0;
}
