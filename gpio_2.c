/**
 * This file is a copy of gpio.c.
 * The former had been compile-verified.
 */

#include <linux/gpio.h> /* You may want to look at the contents of this file to
                         * understand the data structures shown below. */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define RED_LOFF    27
#define GRN_LOFF    4
#define YLW_LOFF    17
#define BTN_LOFF    11

/* Additional includes omitted in the original handout. */

int main(int argc, char *argv[])
{
    int fd0 = open("/dev/gpiochip0", O_RDWR); // Open the file descriptor.

    struct gpiochip_info cinfo;
    ioctl(fd0, GPIO_GET_CHIPINFO_IOCTL, &cinfo); // Get chip information.
    fprintf(stdout, "GPIO chip 0: %s, \"%s\", %u lines\n",
            cinfo.name, cinfo.label, cinfo.lines);
    fprintf(stdout, "Press 5 seconds to quit.\n");

    struct gpiohandle_request req_RGY;   // Red, Green and Yellow LEDs.
    struct gpiohandle_data data_RGY;     // Data bits.

    req_RGY.lines = 3;              // Three pins in this handle.
    req_RGY.lineoffsets[0] = GRN_LOFF;     // Pin 4: Green LED.
    req_RGY.lineoffsets[1] = YLW_LOFF;    // Pin 17: Yellow LED.
    req_RGY.lineoffsets[2] = RED_LOFF;
    req_RGY.flags = GPIOHANDLE_REQUEST_OUTPUT;

    data_RGY.values[0] = 1;         // Green LED: High (ON).
    data_RGY.values[1] = 0;         // Yellow LED: Low (OFF).
    data_RGY.values[2] = 0;         // Red LED: Low (OFF).

    ioctl(fd0, GPIO_GET_LINEHANDLE_IOCTL, &req_RGY); // Get the line handle.

    struct gpiohandle_request req_btn;
    struct gpiohandle_data data_btn;

    req_btn.lines = 1;
    req_btn.lineoffsets[0] = BTN_LOFF;
    req_btn.flags = GPIOHANDLE_REQUEST_INPUT;

    ioctl(fd0, GPIO_GET_LINEHANDLE_IOCTL, &req_btn);

    int slept = 0;
    int cont_pressed = 0;
    for (;;) {
        ioctl(req_RGY.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data_RGY);
        usleep(10000); // Sleep for 10 millisecond.
        
        // poll each cycle
        ioctl(req_btn.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data_btn);
        if (data_btn.values[0] == 1) {
            cont_pressed += 1;
            data_RGY.values[2] = 1;
        } else {
            cont_pressed = 0;
            data_RGY.values[2] = 0;
        }

        if (cont_pressed > 500) {
            // 5-second press quits the program
            break;
        }

        if (slept % 100 == 0) {
            // do every 100 sleeps (1 second)
            data_RGY.values[0] = !data_RGY.values[0]; // Toggle Green.
            data_RGY.values[1] = !data_RGY.values[1];  // Toggle Yellow.
        }
        
        slept += 1;
    }

    close(req_RGY.fd); // Release the line.
    close(req_btn.fd);
    close(fd0);       // Close the chip descriptor.
    exit(EXIT_SUCCESS);
}
