# WiiLightController
A simple C++ homebrew app for the Nintendo Wii that allows you to control the DVD light. Allows you to set the Wii DVD light to any brightness that is supported by the LEDs themselves.

## Controller compatibility
WiiLightController has *not* been designed for use with any other controller except the Wiimote by itself. Others may work, but they haven't been tested to work with it.

## Wii compatibility
| Model                                        | Compatible?                |
|----------------------------------------------|----------------------------|
| Wii with GameCube Backwards Compatibility    | YES                        |
| Wii without GameCube Backwards Compatibility | YES                        |
| Wii U's Virtual Wii                          | YES BUT DOES NOTHING       |
| Wii Mini                                     | CANNOT RUN ANY HOMEBREW    |

Basically, if your Wii has a blue LED outline around the disc drive and can run homebrew, this will work.

## Dolphin compatibility
WiiLightController runs correctly in Dolphin, but because Dolphin is just software, the program does nothing as there is drive light to control.

## Requirements for compiling yourself
- LibWiiLight (http://wiibrew.org/wiki/Libwiilight)
- DevkitPro (https://devkitpro.org)
- DevkitPPC (https://devkitpro.org)
