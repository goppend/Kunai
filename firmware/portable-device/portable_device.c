#include "pico/stdio.h"
#include "hardware/spi.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

#include "controller_simulator.h"
#include "portable_device.h"
#include "config.h"

// Table correlating pins to their function or I/O direction
static uint8_t gpio_table[] = {
#ifdef DIGITAL_INPUTS
    GPIO_DPAD_UP,       GPIO_FUNC_NULL, GPIO_IN,
    GPIO_DPAD_DOWN,     GPIO_FUNC_NULL, GPIO_IN,
    GPIO_DPAD_LEFT,     GPIO_FUNC_NULL, GPIO_IN,
    GPIO_DPAD_RIGHT,    GPIO_FUNC_NULL, GPIO_IN,
    GPIO_FACE_TRIANGLE, GPIO_FUNC_NULL, GPIO_IN,
    GPIO_FACE_SQUARE,   GPIO_FUNC_NULL, GPIO_IN,
    GPIO_FACE_CIRCLE,   GPIO_FUNC_NULL, GPIO_IN,
    GPIO_FACE_CROSS,    GPIO_FUNC_NULL, GPIO_IN,
    GPIO_SHOULDER_L1,   GPIO_FUNC_NULL, GPIO_IN,
    GPIO_SHOULDER_L2,   GPIO_FUNC_NULL, GPIO_IN,
    GPIO_SHOULDER_R1,   GPIO_FUNC_NULL, GPIO_IN,
    GPIO_SHOULDER_R2,   GPIO_FUNC_NULL, GPIO_IN,
    GPIO_ANA_R_Y,       GPIO_FUNC_SIO,  GPIO_IN,
    GPIO_ANA_R_X,       GPIO_FUNC_SIO,  GPIO_IN,
    GPIO_ANA_L_Y,       GPIO_FUNC_SIO,  GPIO_IN,
    GPIO_ANA_L_X,       GPIO_FUNC_SIO,  GPIO_IN,
#else
    GPIO_LEFT_ADC,      GPIO_FUNC_NULL, GPIO_OUT,
    GPIO_RIGHT_ADC,     GPIO_FUNC_NULL, GPIO_OUT,
    SPI_DEV_RX,         GPIO_FUNC_SPI, 0,
    SPI_DEV_TX,         GPIO_FUNC_SPI, 0,
    SPI_DEV_SCK,        GPIO_FUNC_SPI, 0,
#endif
    GPIO_L_MOTOR,       GPIO_FUNC_PWM, 0,
    GPIO_R3,            GPIO_FUNC_NULL, GPIO_IN,
    GPIO_START,         GPIO_FUNC_NULL, GPIO_IN,
    GPIO_ANALOG,        GPIO_FUNC_NULL, GPIO_IN,
    GPIO_LED,           GPIO_FUNC_NULL, GPIO_OUT,
    GPIO_SELECT,        GPIO_FUNC_NULL, GPIO_IN,
    GPIO_L_MOTOR,       GPIO_FUNC_NULL, GPIO_OUT,
    GPIO_L3,            GPIO_FUNC_NULL, GPIO_IN
};

// static variables
static uint pwm_slice_num;
static bool mode_state_change_flag = false;

// the array containing the last smoothed analog input
// Each index is for a different input.
static uint8_t analog_smooth[ANALOG_CHANNELS];

// read from an ADC on the SPI bus.
// adc_select - which ADC to select, ignored when DIGITAL_INPUTS is defined
// channel - the channel to read from
// returns the resultant byte read from the ADC
uint8_t read_adc(uint8_t adc_select, uint8_t channel) {
#ifdef DIGITAL_INPUTS
    adc_select_input(channel);
    return adc_read() >> 4;
#else
    uint8_t address = channel << 3;
    uint8_t result[2];

    // select an ADC
    gpio_put(GPIO_LEFT_ADC, adc_select != 1); 
    gpio_put(GPIO_RIGHT_ADC, adc_select != 2);

    // set the channel
    spi_write_blocking(spi1, &address, 2);

    // deselect both ADCs
    gpio_put(GPIO_LEFT_ADC, 1);
    gpio_put(GPIO_RIGHT_ADC, 1);

    // select an ADC
    gpio_put(GPIO_LEFT_ADC, adc_select != 1); 
    gpio_put(GPIO_RIGHT_ADC, adc_select != 2);

    // read the value from it
    spi_read_blocking(spi1, 0, result, 2);
    
    // deselect both ADCs
    gpio_put(GPIO_LEFT_ADC, 1);
    gpio_put(GPIO_RIGHT_ADC, 1);

    return result[0]<<4 | result[1]>>4;
#endif
}

// Perform an exponential smoothing algorithm on raw ADC values
// next - the next contributing value
// button_ix - the input index
// returns a smoothed value for this input
uint8_t exponential_smooth(uint8_t next, uint8_t button_ix) {
    analog_smooth[button_ix] = ALPHA*next + (1 - ALPHA)*analog_smooth[button_ix];
    return analog_smooth[button_ix];
}

// Init everything needed for the portable
void portable_device_init() {
    // init digital IO
    spi_init(spi1, 1000 * 1000); // init device SPI bus at 1 MHz
    adc_init();

    gpio_set_function(GPIO_R_MOTOR, GPIO_FUNC_PWM);
    uint pwm_slice_num = pwm_gpio_to_slice_num(GPIO_R_MOTOR);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.f);
    pwm_init(pwm_slice_num, &config, false);

    for (int gpio_table_index = 0; gpio_table_index < sizeof(gpio_table) / 3; gpio_table_index++ )
    {
        switch(gpio_table[gpio_table_index * 3 + 1])
        {
            case GPIO_FUNC_NULL:
            {
                // Init GPIOs with no hardware function
                gpio_init(gpio_table[gpio_table_index * 3]);
                gpio_set_dir(gpio_table[gpio_table_index * 3], gpio_table[gpio_table_index * 3 + 2]);

                if (gpio_table[gpio_table_index * 3 + 2] == GPIO_OUT)
                {
                    // set the initial state for outputs to be low
                    gpio_put(gpio_table[gpio_table_index * 3], 0);
                }
                else
                {
                    // set pull up resistor on inputs
                    gpio_pull_up(gpio_table[gpio_table_index * 3]);
                }
                break;
            }
            case GPIO_FUNC_SIO:
            {
                // init analog inputs
                gpio_set_dir(gpio_table[gpio_table_index * 3], gpio_table[gpio_table_index * 3 + 2]);
                gpio_set_function(gpio_table[gpio_table_index * 3], GPIO_FUNC_SIO);
                gpio_disable_pulls(gpio_table[gpio_table_index * 3]);
                gpio_set_input_enabled(gpio_table[gpio_table_index * 3], false);
                break;
            }
            case GPIO_FUNC_SPI:
            {
                // init SPI
                gpio_set_function(gpio_table[gpio_table_index * 3], GPIO_FUNC_SPI);
                break;
            }
            case GPIO_FUNC_PWM:
            {
                // init PWM output for e.g. rumble
                gpio_set_function(gpio_table[gpio_table_index * 3], GPIO_FUNC_PWM);
                pwm_slice_num = pwm_gpio_to_slice_num(gpio_table[gpio_table_index * 3]);
                pwm_config config = pwm_get_default_config();
                pwm_config_set_clkdiv(&config, 4.f);
                pwm_init(pwm_slice_num, &config, false);
                break;
            }
        }
    }
}

// Gather and process inputs and ouputs
void portable_device_loop(PSXInputState* psxReport, PSXOutputState* psxFeedback) {
#ifdef DIGITAL_INPUTS
	psxReport->dpad_rt  = !gpio_get(GPIO_DPAD_RIGHT)    ? 0xFF : 0x00;
	psxReport->dpad_up  = !gpio_get(GPIO_DPAD_UP)       ? 0xFF : 0x00;
	psxReport->dpad_lt  = !gpio_get(GPIO_DPAD_LEFT)     ? 0xFF : 0x00;
	psxReport->dpad_dn  = !gpio_get(GPIO_DPAD_DOWN)     ? 0xFF : 0x00;
	psxReport->shld_l1  = !gpio_get(GPIO_SHOULDER_L1)   ? 0xFF : 0x00;
	psxReport->shld_l2  = !gpio_get(GPIO_SHOULDER_L2)   ? 0xFF : 0x00;
	psxReport->face_cir = !gpio_get(GPIO_FACE_CIRCLE)   ? 0xFF : 0x00;
	psxReport->face_tri = !gpio_get(GPIO_FACE_TRIANGLE) ? 0xFF : 0x00;
	psxReport->face_squ = !gpio_get(GPIO_FACE_SQUARE)   ? 0xFF : 0x00;
	psxReport->face_crs = !gpio_get(GPIO_FACE_CROSS)    ? 0xFF : 0x00;
	psxReport->shld_r1  = !gpio_get(GPIO_SHOULDER_R1)   ? 0xFF : 0x00;
	psxReport->shld_r2  = !gpio_get(GPIO_SHOULDER_R2)   ? 0xFF : 0x00;

    // Use the onboard ADC for joystick inputs
    psxReport->rjoy_y = exponential_smooth(read_adc(-1, 0), 0);
    psxReport->rjoy_x = exponential_smooth(read_adc(-1, 1), 1);
    psxReport->ljoy_y = exponential_smooth(read_adc(-1, 2), 2);
    psxReport->ljoy_x = exponential_smooth(read_adc(-1, 3), 3);
#else
    psxReport->dpad_rt = exponential_smooth(read_adc(1, 0), 0);
    psxReport->dpad_up = exponential_smooth(read_adc(1, 1), 1);
    psxReport->dpad_lt = exponential_smooth(read_adc(1, 2), 2);
    psxReport->shld_l1 = exponential_smooth(read_adc(1, 3), 3);
    psxReport->shld_l2 = exponential_smooth(read_adc(1, 4), 4);
    psxReport->ljoy_y = read_adc(1, 5);
    psxReport->ljoy_x = read_adc(1, 6);
    psxReport->dpad_dn = exponential_smooth(read_adc(1, 7), 7);

    psxReport->face_cir = exponential_smooth(read_adc(2, 0), 8);
    psxReport->face_tri = exponential_smooth(read_adc(2, 1), 9);
    psxReport->face_squ = exponential_smooth(read_adc(2, 2), 10);
    psxReport->rjoy_y = read_adc(2, 3);
    psxReport->rjoy_x = read_adc(2, 4);
    psxReport->face_crs = exponential_smooth(read_adc(2, 5), 13);
    psxReport->shld_r1  = exponential_smooth(read_adc(2, 6), 14);
    psxReport->shld_r2  = exponential_smooth(read_adc(2, 7), 15);
#endif

    psxReport->buttons = 0x0000;
    report_gpio(GPIO_SELECT, 0);
    report_gpio(GPIO_L3, 1);
    report_gpio(GPIO_R3, 2);
    report_gpio(GPIO_START, 3);
    report_button(dpad_up, 4);
    report_button(dpad_rt, 5);
    report_button(dpad_dn, 6);
    report_button(dpad_lt, 7);
    report_button(shld_l2, 8);
    report_button(shld_r2, 9);
    report_button(shld_l1, 10);
    report_button(shld_r1, 11);
    report_button(face_tri, 12);
    report_button(face_cir, 13);
    report_button(face_crs, 14);
    report_button(face_squ, 15);
    psxReport->buttons = ~psxReport->buttons;

    bool analog_button = gpio_get(GPIO_ANALOG);
    if (mode_state_change_flag && !analog_button)
    {
        mode_state_change_flag = false;
    }
    else if (analog_button)
    {
        mode_state_change_flag = true;
        // try to change state
    }

    gpio_put(GPIO_LED, psxFeedback->analog_led);
    gpio_put(GPIO_L_MOTOR, psxFeedback->small_motor);

    pwm_set_enabled(pwm_slice_num, psxFeedback->large_motor > 0);
    pwm_set_gpio_level(GPIO_R_MOTOR, psxFeedback->large_motor * psxFeedback->large_motor);
}