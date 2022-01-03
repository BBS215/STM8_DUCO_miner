#include "peripheral.h"

////////// LEDS /////////////////////

//											    LED0
const GPIO_Pin_TypeDef	LED_pin[LEDS_CNT] = {   GPIO_PIN_5  };
GPIO_TypeDef* const		LED_port[LEDS_CNT] = {  GPIOB	    };

uint8_t g_LEDS_mode[LEDS_CNT];

void Leds_init(void)
{
	uint8_t i;
	for(i=0;i<LEDS_CNT;i++)
	{
		GPIO_Init(LED_port[i], LED_pin[i], GPIO_MODE_OUT_PP_HIGH_SLOW);
		Led_setmode(i, LED_OFF);
	}
}

void Led_setmode(uint8_t num, uint8_t mode)
{
	if (num >= LEDS_CNT) return;
	g_LEDS_mode[num] = mode;
	if (mode == LED_ON) {
		GPIO_WriteLow(LED_port[num], LED_pin[num]);
	} else
	if (mode == LED_OFF) {
		GPIO_WriteHigh(LED_port[num], LED_pin[num]);
	}
}

/// UART ///////////////////////////////////////////

uint32_t g_UART_timeout;
uint8_t g_UART_RX_buf[256];
volatile uint8_t g_UART_RX_buf_WR_ptr;
uint8_t g_UART_RX_buf_RD_ptr;

void UART_init(void)
{
	UART1_DeInit();
	UART1_Init((uint32_t)UART_SPEED, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
				UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
	GPIOD->CR1 |= (uint8_t)GPIO_PIN_6; // UART RX Pull up
	g_UART_RX_buf_WR_ptr = 0;
	g_UART_RX_buf_RD_ptr = 0;
	g_UART_timeout = 10000; // 10 sec
	UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
	UART1_Cmd(ENABLE);
}

uint8_t UART_send(uint8_t *p_buf, uint16_t size)
{
	uint16_t i;
	if (!p_buf) return -1;
	if (!size) return -2;
	for(i=0;i<size;i++)
	{
		while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET); // Loop until the end of transmission
		UART1_SendData8(p_buf[i]);
	}
	return 0;
}

void UART_RX_INTR(void)
{
    g_UART_RX_buf[g_UART_RX_buf_WR_ptr] = UART1_ReceiveData8();
    g_UART_RX_buf_WR_ptr++;
}

void UART_Flush(void)
{
    g_UART_RX_buf_RD_ptr = g_UART_RX_buf_WR_ptr;
}

bool UART_available(void)
{
    return ((g_UART_RX_buf_RD_ptr == g_UART_RX_buf_WR_ptr)?FALSE:TRUE);
}

uint8_t UART_readStringUntil(uint8_t *p_buf, uint8_t buf_max_size, uint8_t c)
{
    uint8_t size = 0;
    uint32_t tickstart = HAL_GetTick_ms();
    while(size < buf_max_size) {
        if (g_UART_RX_buf_RD_ptr != g_UART_RX_buf_WR_ptr) {
            p_buf[size] = g_UART_RX_buf[g_UART_RX_buf_RD_ptr];
            g_UART_RX_buf_RD_ptr++;
            if (p_buf[size] == c) return size;
            size++;
        } else {
            if ((HAL_GetTick_ms() - tickstart) > g_UART_timeout) return 0;
        }
    }
    return size;
}

uint32_t UART_strtoul(uint8_t *p_buf, uint8_t buf_size)
{
    uint32_t ret = 0;
    uint8_t size = 0;
    if (buf_size > 10) buf_size = 10;
    while(size < buf_size) {
        ret = ret * 10;
        ret += (p_buf[size] - '0');
        size++;
    }
    return ret;
}

