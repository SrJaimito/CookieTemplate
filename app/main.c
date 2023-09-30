/**
 * This file contains the main function. It calls secondary
 * functions that can be used to handle the MCU application.
*/

#include "app.h"

int main() {
    setup();

    while (1) {
        loop();
    }

    return 0;
}
