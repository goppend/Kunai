
// ALPHA - smoothing factor used in the exponential smoothing algorithm performed on the raw analog inputs
#define ALPHA 0.8
// THRESHOLD - minimum value to for an analog button press to register as a digital "on"
#define ANALOG_THRESHOLD 5

#ifdef DIGITAL_INPUTS
#define ANALOG_CHANNELS 4
#else
#define ANALOG_CHANNELS 16
#endif

// macros to translate input value to the button bitfield
#define report_gpio(input, bit)     psxReport->buttons |= (gpio_get(input) == 0) << bit
#define report_button(input, bit)   psxReport->buttons |= (psxReport->input >= ANALOG_THRESHOLD) << bit

void portable_device_init();
void portable_device_loop(PSXInputState* psxReport, PSXOutputState* psxFeedback);
