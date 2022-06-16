#ifndef LAB3_I8042_H
#define LAB3_I8042_H

#define KBD_IRQ 1

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

#endif
