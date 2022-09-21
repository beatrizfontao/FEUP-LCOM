#include "mouse.h"
#include "utils.h"

int mouse_hookid = 12;
uint8_t mouse_scancode = 0, mouse_statuscode = 0;
int ih_error = 0;

uint16_t Xres;

int (mouse_subscribe_int)(int *bit_no) {
    *bit_no = mouse_hookid;

    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hookid))
        return 1;

    return 0;
}

int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&mouse_hookid))
        return 1;

    return 0;
}

void (mouse_ih)() {
    if(util_sys_inb(STAT_REG, &mouse_statuscode)){
        ih_error = 1;
        printf("sys_inb\n");
        return;
    }

    if(!((mouse_statuscode & (PARITY_BIT | TIMEOUT_BIT)) == 0 && (mouse_statuscode & OBF_BIT))){

        mouse_scancode = 0;
        ih_error = 1;
        printf("cond\n");
        return;
    }
    util_sys_inb(OUT_BUF, &mouse_scancode);

}

void (getMousePacket)(struct packet *pp, uint8_t bytes[3]) {
    // bytes
    pp->bytes[0] = bytes[0];
    pp->bytes[1] = bytes[1];
    pp->bytes[2] = bytes[2];

    // buttons
    pp->lb = bytes[0] & LEFT;
    pp->rb = bytes[0] & RIGHT;
    pp->mb = bytes[0] & MIDDLE;

    // x and y displacement
    if(bytes[0] & Y_SIGN)
        pp->delta_y = bytes[2] | FILLMSB;
    else pp->delta_y = bytes[2];
    if(bytes[0] & X_SIGN)
        pp->delta_x = bytes[1] | FILLMSB;
    else pp->delta_x = bytes[1];

    // x and y overflow
     pp->x_ov = (bytes[0] & X_OVFL) >> 6;
     pp->y_ov = (bytes[0] & Y_OVFL) >> 7;
}


int (send_mouse_command)(uint8_t cmd){
    uint8_t ack;
    do{
        if(util_sys_inb(STAT_REG, &mouse_statuscode))
            return 1;
        if (mouse_statuscode & IBF_BIT) // checks if we can write
            continue;
        if(sys_outb(STAT_REG, MOUSE_COMMAND))
            return 1;

        if(util_sys_inb(STAT_REG, &mouse_statuscode))
            return 1;
        if (mouse_statuscode & IBF_BIT) // checks if we can write
            continue;
        if(sys_outb(ARGS_REG, cmd))
            return 1;

        tickdelay(micros_to_ticks(20000));

        if(util_sys_inb(OUT_BUF, &ack))
            return 1;

        if(ack == ACK_ERROR)
            return 1;

        if(ack == NACK)
            continue;

    }while(ack != ACK);

    return 0;
}

void (updateMouse)(struct packet *pp, Mouse *mouse) {

    if (pp->bytes[0] & X_SIGN) { // checks if x movement is negative
        if (mouse->x + pp->delta_x < 0)
            mouse->x = 0;
        else
            mouse->x = mouse->x + pp->delta_x;
    }
    else{
        if (mouse->x + pp->delta_x > X_RES - mouse->img.width)
            mouse->x = X_RES - mouse->img.width;
        else
            mouse->x = mouse->x + pp->delta_x;
    }

    if (pp->bytes[0] & Y_SIGN) { // checks if y movement is negative
        if (mouse->y + mouse->img.height - pp->delta_y > Y_RES)
            mouse->y = Y_RES - mouse->img.height;
        else
            mouse->y = mouse->y - pp->delta_y;
    }
    else {
        if (mouse->y - pp->delta_y< 0)
            mouse->y = 0;
        else
            mouse->y = mouse->y - pp->delta_y;
    }

    mouse->lb_pressed = pp->lb;
}

Mouse *(createMouse)(int x, int y){
    Mouse *mouse = (Mouse *) malloc(sizeof(Mouse));

    xpm_image_t mouse_img;
    xpm_load(mouse_xpm, XPM_INDEXED, &mouse_img);
    mouse->img = mouse_img;
    mouse->x = x;
    mouse->y = y;
    mouse->lb_pressed = false;

    return mouse;
}

void (removeMouse)(Mouse *mouse){
    free(mouse);
    mouse = NULL;
}
