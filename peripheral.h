#ifndef __PERIPHERAL_H
#define __PERIPHERAL_H

#include "main.h"

/// LEDS ///////////////////////////////////////////


#define LED_0		0
#define LEDS_CNT    1

#define LED_OFF 0
#define LED_ON 	1

void Leds_init(void);
void Led_setmode(uint8_t num, uint8_t mode);


/// UART ///////////////////////////////////////////

#define UART_SPEED 115200

void UART_init(void);
uint8_t UART_send(uint8_t *p_buf, uint16_t size);
void UART_Flush(void);
bool UART_available(void);
uint8_t UART_readStringUntil(uint8_t *p_buf, uint8_t buf_max_size, uint8_t c);
uint32_t UART_strtoul(uint8_t *p_buf, uint8_t buf_size);

#endif // __PERIPHERAL_H


