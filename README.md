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

## On a Raspberry Pi as a host

Install pico-sdk with TinyUSB module.

```
$ git clone -b master https://github.com/raspberrypi/pico-sdk.git
$ cd pico-sdk
$ git submodule update --init --recursive
$ cd ..
$ git clone -b master https://github.com/raspberrypi/pico-examples.git
```

libusb-1.0 library should be installed.

```
$ sudo apt update
$ sudo apt install libusb-1.0
```

copy bootselBoot.c file to your directory and compile.

```
$ cc bootselBoot.c -o bootselBoot -I/usr/include/libusb-1.0 -L/usr/lib/arm-linux-gnueabihf -lusb-1.0
```

Install a serial terminal emulator. At htis time, 

```
$ apt install screen
```

Plug a Pi Pico board to a USB connector of the host. Install a executable code to the board. For example, install hello_world example from pico-example.

## for Pi Pico to test bootselBoot

Install any executable that includes pico_stdio_usb. For example, here is a test code named pwmled.c The pwmled is a small code and blinks on-board LED continuously using PWM capability of Pi Pico and reads a character from USB port to change blink period.

To build pwmled, create a direcory named 'pwmled' neighbouring pico-sdk directory.

```
$ cd ${PICO_SDK_PATH}/..
$ git clone https://github.com/illusiaDecafish/bootselBoot
$ cd bootselBoot/pwmled/
$ mkdir build
$ cd build/
$ cmake ..
$ make -j4
$ ls
```

If you can find pwmled.uf2 build was successful.

Plug Pi Pico via. USB pressing BOOTSEL button and  copy pwmled.uf2 to flash storage on the board.

```
$ sudo mount /dev/sda1 /mnt/pico
$ sudo cp pwmled.uf2 /mnt/pico/
$ sudo sync
```

Then the LED blinks continuously and slowly (period is may be about 1sec). Invoke the screen command

```
$ screen /dev/ttyACM0 9600
```

device file '/dev/ttyACM0' is a CDC (Communication Data Class) port of the connected Pi Pico. When you key in a character '1', LED blinks faster and '2' .. '9' LED blinks slower. To exit screen command, type C-a (a with control-key) -> k -> y.

## invoke bootselBoot

Again return bootselBoot directory and compile cootselBoot.c.

```
$ cc bootselBoot.c -o bootselBoot -I/usr/include/libusb-1.0 -L/usr/lib/arm-linux-gnueabihf -lusb-1.0
```

And invoke bootselBoot command to reset the Pi Pico

```
$ sudo ./bootselBoot b
```

Then you can see the LED on the board stopping and off and you can find a device file of the flash storage.

```
$  ls /dev/sd*
```

You can mount and copy other uf2 file onto the flash storage and don't need the procedure 'unplug USB/pressing BOOTSEL/plug USB again'.

It is handy and useful.

