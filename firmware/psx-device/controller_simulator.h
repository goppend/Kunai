#include "pico/stdlib.h"


typedef struct
{
    uint16_t buttons;
    uint8_t dpad_up;
    uint8_t dpad_rt;
    uint8_t dpad_dn;
    uint8_t dpad_lt;
    uint8_t face_tri;
    uint8_t face_cir;
    uint8_t face_crs;
    uint8_t face_squ;
    uint8_t ljoy_x;
    uint8_t ljoy_y;
    uint8_t rjoy_x;
    uint8_t rjoy_y;
    uint8_t shld_l1;
    uint8_t shld_r1;
    uint8_t shld_l2;
    uint8_t shld_r2;
}PSXInputState;

typedef struct __attribute((packed, aligned(1)))
{
    bool analog_led;
    bool small_motor;
    uint8_t large_motor;
}PSXOutputState;

#define MODE_DIGITAL 0x41
#define MODE_ANALOG  0x73
#define MODE_ANALOG_PRESSURE 0x79
#define MODE_CONFIG 0xF3

#define CMD_PRES_CONFIG 0x40
#define CMD_POLL_CONFIG_STATUS 0x41
#define CMD_POLL 0x42
#define CMD_CONFIG 0x43
#define CMD_STATUS 0x45
#define CMD_CONST_46 0x46
#define CMD_CONST_47 0x47
#define CMD_CONST_4C 0x4C
#define CMD_ENABLE_RUMBLE 0x4D
#define CMD_POLL_CONFIG 0x4F
#define CMD_ANALOG_SWITCH 0x44

#define SEL   0
#define L3    1
#define R3    2
#define STA   3
#define UP    4
#define RIGHT 5
#define DOWN  6
#define LEFT  7

#define L2    8
#define R2    9
#define L1    10
#define R1    11
#define TRI   12
#define CIR   13
#define CRS   14
#define SQU   15

void psx_device_init(uint pio, PSXInputState *indata, PSXOutputState *outdata, void (*reset_pio)());
void psx_device_main();
