//
//  main.c
//  libusbtest
//
//  Created by decafish on 2021/05/02.
//

#include <stdio.h>
#include <libusb.h>

void    logcallback(libusb_context *ctx, enum libusb_log_level level, const char *str)
{
    fprintf(stderr, "level%1d: %s\n", level, str);
}

// reset to BOOTSEL
#define RESET_REQUEST_BOOTSEL 0x01
// regular flash boot
#define RESET_REQUEST_FLASH 0x02


int main(int argc, const char * argv[])
{
    uint8_t     request = RESET_REQUEST_BOOTSEL;
    if (argc > 1) {
        switch (*argv[1]) {
            case 'B':
            case 'b':
                request = RESET_REQUEST_BOOTSEL;
                break;
            case 'F':
            case 'f':
                request = RESET_REQUEST_FLASH;
                break;
            default:
                fprintf(stderr, "%s: Reset Pi Pico via. USB\n", argv[0]);
                fprintf(stderr, "\t$ %s B[OOTSEL] to reset to BOOTSEL mode.\n", argv[0]);
                fprintf(stderr, "\t$ %s F[LASH] to reset to regular boot mode.\n", argv[0]);
                fprintf(stderr, "\twith no argument means B[OOTSEL] specified.\n");
                return 0;
        }
    }
    int ret = libusb_init(NULL);
    if (ret != 0) {
        return -1;
    }
    //  libusb_set_log_cb(NULL, logcallback, LIBUSB_LOG_CB_GLOBAL);
    
    libusb_device_handle    *handle;
    uint16_t    vendor_id = 0x2E8A;
    uint16_t    product_id = 0x000A;

    handle = libusb_open_device_with_vid_pid(NULL, vendor_id, product_id);
    if (handle == NULL) {
        fprintf(stderr, "No pico device available or a device has no standard pico_stdio_usb module.\n");
        libusb_exit(NULL);
        return(-1);
    }
    
    int interface_number = 2;
    ret = libusb_claim_interface(handle, interface_number);
    if (ret != 0) {
        fprintf(stderr, "Pico device found has no interface#2. Already BOOTSEL mode?\n");
        libusb_exit(NULL);
        return(-1);
    }
    
    uint8_t     reqtype = (0 << 6)  //  standard request host to device
                            | (1);  // to interface
    uint16_t    windex = interface_number;
    uint16_t    wvalue = 0;
    //unsigned char   data[4];
    ret = libusb_control_transfer(handle, reqtype, request, wvalue, windex, NULL, 0, 10);
    
    //printf("return value from control transfer = %d\n", ret);
    libusb_exit(NULL);
    
    
    return 0;
}



