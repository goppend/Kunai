// Uncomment this line to use digital-only inputs
//#define DIGITAL_INPUTS

#ifndef MODULE
	// direct pin connections for main
	#define GPIO_R_MOTOR    0
	#define GPIO_R3         2
	#define GPIO_START      7
    #define PIO_HOST_CMD    16
    #define PIO_HOST_ATN    17
    #define PIO_HOST_CLK    18
    #define PIO_HOST_DAT    19
    #define PIO_HOST_ACK    20
	#define GPIO_ANALOG     22
	#define GPIO_LED        25
	#define GPIO_SELECT     26
	#define GPIO_L_MOTOR    27
	#define GPIO_L3         28
#else
	// direct pin connections for module
	#define GPIO_L3         0
	#define GPIO_R3         1
	#define GPIO_START      2
	#define GPIO_L_MOTOR    3
	#define GPIO_R_MOTOR    4
    #define PIO_HOST_ACK    11
    #define PIO_HOST_CLK    12
    #define PIO_HOST_ATN    13
    #define PIO_HOST_CMD    14
    #define PIO_HOST_DAT    15
	#define GPIO_LED        23
	#define GPIO_SELECT     24
	#define GPIO_ANALOG     25
#endif

#ifndef DIGITAL_INPUTS
    // define pins for external ADCs
    #define GPIO_LEFT_ADC   9 // adc 1
    #define GPIO_RIGHT_ADC  12 // adc 2
    #define SPI_DEV_RX      8
    #define SPI_DEV_SCK     10
    #define SPI_DEV_TX      11
#else
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
#endif
