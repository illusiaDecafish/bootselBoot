# bootselBoot
Reset Raspberry Pi Pico to BOOTSEL mode via. USB.

## BOOTSEL

Raspberry Pi Pico has BOOTSEL mode that a Pico board is connect to a host via. USB by device mode. Flash storage on the board is mounted on the host as a USB flash drive.

BOOTSEL mode can be invoked by pressing 'BOOTSEL' button on the board when the board is unplugged and then plugged with USB or 'RUN' pin on right card edge of the board is switched to GND but the procedures are annoying because the board hardware is delicate especially microUSB port. It is better to reset without hardware treatments.

I found a solution to reset Pico via. USB from a host.
