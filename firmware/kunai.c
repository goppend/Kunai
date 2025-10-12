#include <stdlib.h>

#include "pico/multicore.h"
#include "hardware/clocks.h"

//Device
#include "controller_simulator.h"
#include "portable_device.h"

// Data structures to pass information to/from core 1, responsible for communicating with the host console in a tight loop
static PSXInputState* psxReport;
static PSXOutputState* psxFeedback;

// Handle controller <-> console comms on core 1
void core1_main() {
    psx_device_main();
}

// Handle input polling on core 0
int main(void) {
    // Set sysclock to a multiple of 12MHz.
    set_sys_clock_khz(240000, true);
    stdio_init_all();

    psxReport = (PSXInputState*)calloc(1, sizeof(PSXInputState));
    psxFeedback = (PSXOutputState*)calloc(1, sizeof(PSXOutputState));
    psx_device_init(0, psxReport, psxFeedback, core1_main);
    portable_device_init();

    while(1) {
        portable_device_loop(psxReport, psxFeedback);
    }
    
    return 0;
}
