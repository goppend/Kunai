// replace with #undef to use digital-only inputs
#define PRESSURE_INPUT

// direct pin connections
#define GPIO_R_MOTOR    0
#define GPIO_R3         2
#define GPIO_START      7
#define GPIO_ANALOG     22
#define GPIO_LED        25
#define GPIO_SELECT     26
#define GPIO_L_MOTOR    27
#define GPIO_L3         28

#ifdef PRESSURE_INPUT
    // ADCs for pressure sensors
    #define GPIO_LEFT_ADC   9 // adc 1
    #define GPIO_RIGHT_ADC  12 // adc 2
    #define SPI_DEV_RX      8
    #define SPI_DEV_SCK     10
    #define SPI_DEV_TX      11
#else
    // define digital inputs in place of DS2 pressure sensors
    #define GPIO_DPAD_UP        0
    #define GPIO_DPAD_DOWN      0
    #define GPIO_DPAD_LEFT      0
    #define GPIO_DPAD_RIGHT     0
    #define GPIO_FACE_TRIANGLE  0
    #define GPIO_FACE_SQUARE    0
    #define GPIO_FACE_CIRCLE    0
    #define GPIO_FACE_CROSS     0
    #define GPIO_SHOULDER_L1    0
    #define GPIO_SHOULDER_L2    0
    #define GPIO_SHOULDER_R1    0
    #define GPIO_SHOULDER_R2    0
#endif
