![](https://github.com/goppend/Kunai/blob/main/img/logo_kunai.png)

## Kunai Firmware
This is the first release of the Kunai Firmware. Only one of its operating modes is mostly complete, and needs more and better testing.

### Building
---
- I recommend using VSCodium with the official Raspberry Pi Pico extension and the latest SDK.
- Open this folder.
- Optionally, edit config.h to suit the hardware.
- Compile.
- Two UF2 artifacts are created in the `build/` directory, `kunai.uf2` for the main PCB, and `kunai-mod.uf2` for the Kunai module.

### Configuration
---
In `config.h`, there are some defines that can be changed to alter the behavior of the Kunai:
- `DIGITAL_INPUTS` If defined, the Kunai will treat inputs as digital button presses rather than analog values from pressure sensors, akin to a Dualshock 2.
- `GPIO_*` The digital I/O pins connected to the Pico. By default, these are tailored to suit the version 0 of either hardware implementation.
- `SPI_*` The pins connected to an SPI analog-to-digital converter, to gather the analog pressure inputs. When configured to be digital-only, these pins are undefined.
- If so desired, changing the host `ACK`, `CLK`, `ATN`, `CMD`, and `DAT` pins requires modification of either of the two `psx-device/psxSPI*.pio` files.

### Future Development
---
- Add proper digital-only input methods. (testing in progress)
- Add a pseudo-analog mode for digital-only hardware to support software that requires analog input.
- Analog/digital mode switching is unimplemented. Not entirely sure how to do this.
- Extensive testing will be needed once the hardware is ready.