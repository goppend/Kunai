// define pin connections
#define GPIO_L3         0
#define GPIO_R3         1
#define GPIO_START      2
#define GPIO_L_MOTOR    3
#define GPIO_R_MOTOR    4
#define GPIO_LED        23
#define GPIO_SELECT     24
#define GPIO_ANALOG     25

#ifdef DIGITAL_INPUTS
    // define digital inputs
    #define GPIO_FACE_SQUARE    5
    #define GPIO_FACE_CROSS     6
    #define GPIO_FACE_CIRCLE    7
    #define GPIO_FACE_TRIANGLE  8
    #define GPIO_SHOULDER_R2    9
    #define GPIO_SHOULDER_R1    10
    #define GPIO_SHOULDER_L2    17
    #define GPIO_SHOULDER_L1    18
    #define GPIO_DPAD_UP        19
    #define GPIO_DPAD_LEFT      20
    #define GPIO_DPAD_RIGHT     21
    #define GPIO_DPAD_DOWN      22
    #define GPIO_ANA_R_Y        26
    #define GPIO_ANA_R_X        27
    #define GPIO_ANA_L_Y        28
    #define GPIO_ANA_L_X        29
#else
    // define pins for external ADCs
    #define GPIO_LEFT_ADC   9 // adc 1
    #define GPIO_RIGHT_ADC  12 // adc 2
    #define SPI_DEV_RX      8
    #define SPI_DEV_SCK     10
    #define SPI_DEV_TX      11
#endif
