![](http://github.com/goppend/kunai/blob/master/img/logo_kunai.png)

The Kunai is an open source gamepad based on the Raspberry Pi Pico microcontroller.

Its intended use is either as a standalone controller for Playstation and Playstation 2 systems, or integrated into an all-in-one handheld device that uses the aforementioned systems hardware.

Features
---
- A no-compromises approach to emulating the original functions of the Dualshock 2 gamepad.
- 12 inputs that can be configured as either digital or pressure-sensitive analog buttons.
- 2 rumble motor outputs capable of being driven at 256 pulse-width modulated levels.
- Compatible with readily available aftermarket joysticks. Hall-effect joysticks have yet to be tested, but may be compatible.

Compatibility
---
Not enough testing has been performed to discover any major incompatibilities yet, but as the project evolves, this section will be populated with a list of known good and ill-working titles and applications.

To-do
---
Currently, this project is exiting the "functional prototype" phase, and is not especially useful for gaming, but this will change soon. 
- Shrink the prototype PCB down to work as a drop-in replacement for Dualshock controllers.
- Develop a module as small as possible for rapid integration into custom handheld projects.

Attribution
---
[Dangiu/PicoMemcard](https://github.com/dangiu/PicoMemcard/) - Credit for the PIO code responsible for doing gamepad/console communication.

[Loc15/PicoGamepadConverter](https://github.com/Loc15/PicoGamepadConverter/) - Much inspiration for this project, and the multi-core implementation.

[GameSX](https://gamesx.com/controldata/psxcont/psxcont.htm) and [Curious Inventor](https://store.curiousinventor.com/guides/PS2)- Excellent documentation on the protocol and data structures.

Ultipryce - Logo artwork.
___
Disclaimer

This is a passion project by a fan and has no affiliation with Sony or the Playstation brand.