# bootselBoot
A small command line application to reset Raspberry Pi Pico to BOOTSEL mode via. USB.

## BOOTSEL

Raspberry Pi Pico has BOOTSEL mode that a Pico board is connect to a host via. USB by device mode. Flash storage on the board is mounted on the host as a USB flash drive.

BOOTSEL mode can be invoked by pressing 'BOOTSEL' button on the board when the board is plugged with USB or 'RUN' pin on the right card edge of the board is switched to GND. But these procedures are annoying because the board is rather delicate and there is a possibility to break a microUSB port. It is better to handle without hardware treatments.

I found a solution to reset Pico via. USB from a host with some limitations:

1. An executable code with pico_stdio_usb module is installed and executed on the board.
2. The board is connected to a host computer via. USB.

Default mode of pico_stdio_usb module has three intefaces on its USB descriptor. Interface#0 and interface#1 are for CDC (Communication data class) device and interface#2 is for resetting the device. The interface#2 can receive a control transfer with bRequest field set 0x01 or 0x02. 0x01 request invokes BOOTSEL boot and 0x02 regular flash boot.

I wrote a small code using libusb-1.0 to send the control transfers to Pi Pico board.
