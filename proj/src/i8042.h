#ifndef LAB3_I8042_H
#define LAB3_I8042_H

#define KBD_IRQ 1 /*!< Interrupt line */

//registos
#define OUT_BUF 0x60  /*!< Output Buffer */
#define STAT_REG 0x64 /*!< Status Register */
#define ARGS_REG 0x60 /*!< Arguments Register */

//scancodes
#define ESC_BREAK 0x81 /*!< Breakcode of the ESC key */

//bits statuscode
#define PARITY_BIT BIT(7) /*!< Parity bit error */
#define TIMEOUT_BIT BIT(6) /*!< Timout error */
#define AUX_BIT BIT(5) /*!< Mouse data bit */
#define OBF_BIT BIT(0) /*!< Output buffer full */
#define IBF_BIT BIT(1) /*!< Input buffer full */

//command byte
#define READ_COMMAND 0x20 /*!< Read Command Byte */
#define WRITE_COMMAND 0X60 /*!< Write Command Byte */

//mouse
#define Y_OVFL BIT(7) /*!< Overflow on the y axis */
#define X_OVFL BIT(6) /*!< Overflow on the x axis */
#define Y_SIGN BIT(5) /*!< Sign of movement on the y axis */
#define X_SIGN BIT(4) /*!< Sign of movement on the x axis */
#define MIDDLE BIT(2) /*!< Middle button pressed */
#define RIGHT BIT(1) /*!< Right button pressed */
#define LEFT BIT(0) /*!< Left button pressed */
#define FILLMSB BIT(15) | BIT(14) | BIT(13) | BIT(12)| BIT(11)| BIT(10) | BIT(9) | BIT(8) /*!< Coverts to complement 2 */

#define ENABLE_MOUSE 0xF4 /*!< Enable mouse Command */
#define DISABLE_MOUSE 0xF5 /*!< Disable mouse Command */
#define ACK 0xFA /*!< Acknowledgement byte : Everything ok */
#define NACK 0xFE /*!< Acknowledgement byte : Invalid byte */
#define ACK_ERROR 0xFC /*!< Acknowledgement byte : Second consecutive invalid byte */
#define MOUSE_COMMAND 0xD4 /*!< Write byte to mouse Command */
#define MOUSE_IRQ 12 /*!< Mouse IRQ line */

#endif
