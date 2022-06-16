#ifndef LAB3_I8042_H
#define LAB3_I8042_H

#define KBD_IRQ 1
#define MOUSE_IRQ 12

//registos
#define OUT_BUF 0x60
#define STAT_REG 0x64
#define ARGS_REG 0x60

//scancodes
#define ESC_BREAK 0x81

//bits statuscode
#define PARITY_BIT BIT(7)
#define TIMEOUT_BIT BIT(6)
#define AUX_BIT BIT(5)
#define OBF_BIT BIT(0)
#define IBF_BIT BIT(1)
#define MAKE_BIT BIT(7)

//command byte
#define READ_COMMAND 0x20
#define WRITE_COMMAND 0X60
#define ENABLE_KBD BIT(0)

#define DELAY_US 20000

//mouse

#define Y_OVFL BIT(7)
#define X_OVFL BIT(6)
#define Y_SIGN BIT(5)
#define X_SIGN BIT(4)
#define MIDDLE BIT(2)
#define RIGHT BIT(1)
#define LEFT BIT(0)

#define FILLMSB BIT(15) | BIT(14) | BIT(13) | BIT(12) | BIT(11) | BIT(10) | BIT(9) | BIT(8)

//command byte
#define ENABLE_MOUSE 0xF4
#define DISABLE_MOUSE 0xF5
#define ACK 0xFA
#define NACK 0xFE
#define ACK_ERROR 0xFC
#define MOUSE_COMMAND 0xD4


#endif
