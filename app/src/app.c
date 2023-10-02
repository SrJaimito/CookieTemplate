#include "app.h"

#include "app_config.h"

#include "em_emu.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_timer.h"

/**
 * Called once before main infinite loop.
*/
void setup() {
    // External high frequency clock config
    EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
    EMU_DCDCInit(&dcdcInit);

    CMU_HFXOInit_TypeDef hfxo_init_config = CMU_HFXOINIT_DEFAULT;
    CMU_HFXOInit(&hfxo_init_config);

    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);

    // Toggle LED config
    CMU_ClockEnable(cmuClock_GPIO, 1);

    GPIO_PinModeSet(LED_PORT, LED_PIN, gpioModePushPull, 0);

    // Timer interrupt config
    CMU_ClockEnable(cmuClock_TIMER0, 1);

    TIMER_Init_TypeDef timer0_init_config = TIMER_INIT_DEFAULT;
    timer0_init_config.enable = 0;
    timer0_init_config.prescale = timerPrescale512;

    TIMER_Init(TIMER0, &timer0_init_config);
    TIMER_TopSet(TIMER0, 37109);

    TIMER_IntEnable(TIMER0, TIMER_IF_OF);
    NVIC_EnableIRQ(TIMER0_IRQn);

    TIMER_Enable(TIMER0, 1);
}

/**
 * Main infinite loop.
*/
void loop() {

}
