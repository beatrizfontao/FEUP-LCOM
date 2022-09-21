#ifndef PROJ_RTC_H
#define PROJ_RTC_H

#include <lcom/lcf.h>
#include "util.h"

#define IRQ_RTC 8 /* !< Interrupt line for RTC */

#define RTC_ADDR_REG 0x70 /* !< Address to access the RTC */
#define RTC_DATA_REG 0x71 /* !< Address to transfer data to/from the RTC's register in question */

//Control/Status Register A
#define RS3 BIT(3) /* !< Rate selector for bit 3 of register A */
#define RS2 BIT(2) /* !< Rate selector for bit 2 of register A */
#define RS1 BIT(1) /* !< Rate selector for bit 1 of register A */
#define RS0 BIT(0) /* !< Rate selector for bit 0 of register A */

//Control/Status Register B
#define PIE BIT(6) /* !< Enable Periodic Interrupt */

//Control/Status Register C
#define IRQF BIT(7) /* !< If set to 1, indicates that one of the interrupt sources has triggered */ 
#define PF BIT(6) /* !< If set to 1, if a periodic interrupt occurred */

#define REG_A 0x10 /* !< address of the rtc's register A */
#define REG_B 0x11 /* !< address of the rtc's register B */
#define REG_C 0x12 /* !< address of the rtc's register C */

/**
 * @brief Subscribes the rtc's interrupt
 * 
 * @param bit_no
 * @return 1 on failure or 0 on success
 */
int (rtc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes the rtc's interrupt
 * 
 * @return 1 on failure or 0 on success
 */
int(rtc_unsubscribe_int)();

/**
 * @brief Loads information from register C and handles the interruption
 */
void rtc_ih();

/**
 * @brief Enables the rtc Periodic Interruptions
 * 
 * @return 1 on failure or 0 on success
 */
int (rtc_enable)();  

/**
 * @brief Disables the rtc Periodic Interruptions
 * 
 * @return 1 on failure or 0 on success
 */
int (rtc_disable)();

/**
 * @brief Reads information from the register to the data value
 * 
 * @param data Data to be read to
 * @param reg Register to be read from
 * @return 1 on failure or 0 on success
 */
int rtc_read(uint8_t *data, int reg);

/**
 * @brief Writes the data to the register
 * 
 * @param data Data to be written
 * @param reg Register to be written to
 * @return 1 on failure or 0 on success
 */
int rtc_write(uint8_t data, int reg);

#endif
