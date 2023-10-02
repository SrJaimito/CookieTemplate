#include "app.h"

#include "app_config.h"

#include "em_device.h"

/**
 * Called once before main infinite loop.
*/
void setup() {
    // External high frequency clock config
    CMU->OSCENCMD = CMU_OSCENCMD_HFXOEN;
    while (!(CMU->STATUS & CMU_STATUS_HFXORDY));

    CMU->HFCLKSEL = CMU_HFCLKSEL_HF_HFXO;
    while (CMU->HFCLKSTATUS != CMU_HFCLKSTATUS_SELECTED_HFXO);

    CMU->OSCENCMD = CMU_OSCENCMD_HFRCODIS;

    // Toggle LED config
    CMU->HFBUSCLKEN0 |= CMU_HFBUSCLKEN0_GPIO;

    if (LED_PIN < 8) {
        GPIO->P[LED_PORT].MODEL |= _GPIO_P_MODEL_MODE0_PUSHPULL << (4 * LED_PIN);
    } else {
        GPIO->P[LED_PORT].MODEH |= _GPIO_P_MODEL_MODE0_PUSHPULL << (4 * (LED_PIN - 8));
    }

    // Timer interrupt config
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_TIMER0;

    TIMER0->CTRL |= TIMER_CTRL_PRESC_DIV512;
    TIMER0->TOP = 37109;
    TIMER0->IEN = TIMER_IEN_OF;

    NVIC_EnableIRQ(TIMER0_IRQn);

    TIMER0->CMD = TIMER_CMD_START;
}

/**
 * Main infinite loop.
*/
void loop() {

}
