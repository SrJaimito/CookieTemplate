#include "retarget.h"

#include "em_cmu.h"
#include "em_usart.h"

void retarget_init() {
    CMU_ClockEnable(cmuClock_HFPER, 1);
    CMU_ClockEnable(cmuClock_USART0, 1);
    CMU_ClockEnable(cmuClock_GPIO, 1);

    GPIO_PinModeSet(PRINTF_UART_PORT, PRINTF_UART_PIN, gpioModePushPull, 1);

    USART_InitAsync_TypeDef usart_init = USART_INITASYNC_DEFAULT;
    usart_init.enable = 0;

    USART_InitAsync(USART0, &usart_init);

    USART0->ROUTEPEN |= USART_ROUTEPEN_TXPEN;
    USART0->ROUTELOC0 |= USART_ROUTELOC0_TXLOC_LOC0;

    USART_Enable(USART0, usartEnableTx);
}

int _write(int fd, char *buf, int len) {
    (void) fd;

    for (int i = 0; i < len; i++) {
        USART_Tx(USART0, (uint8_t) (buf[i]));
    }

    return len;
}
