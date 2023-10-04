/**
 * This file contains the main function. It calls secondary
 * functions that can be used to handle the MCU application.
*/

#include "app.h"

#include "em_emu.h"
#include "em_cmu.h"

int main() {
    // External high frequency clock config
    EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
    EMU_DCDCInit(&dcdcInit);

    CMU_HFXOInit_TypeDef hfxo_init_config = CMU_HFXOINIT_DEFAULT;
    CMU_HFXOInit(&hfxo_init_config);

    CMU_OscillatorEnable(cmuOsc_HFXO, 1, 1);
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    CMU_OscillatorEnable(cmuOsc_HFRCO, 0, 0);

    SystemHFXOClockSet(38400000);

    // Call application setup
    setup();

    // Start application loop
    while (1) {
        loop();
    }

    return 0;
}
