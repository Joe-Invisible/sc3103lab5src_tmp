#include <linux/gpio.h> /* You may want to look at the contents of this file to
                         * understand the data structures shown below. */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* Additional includes omitted in the original handout. */

int main(int argc, char *argv[])
{
    int fd0 = open("/dev/gpiochip0", O_RDWR); // Open the file descriptor.

    struct gpiochip_info cinfo;
    ioctl(fd0, GPIO_GET_CHIPINFO_IOCTL, &cinfo); // Get chip information.
    fprintf(stdout, "GPIO chip 0: %s, \"%s\", %u lines\n",
            cinfo.name, cinfo.label, cinfo.lines);

    struct gpiohandle_request req_GY;   // Green and Yellow LEDs.
    struct gpiohandle_data data_GY;     // Data bits.

    req_GY.lines = 2;              // Two pins in this handle.
    req_GY.lineoffsets[0] = 4;     // Pin 4: Green LED.
    req_GY.lineoffsets[1] = 17;    // Pin 17: Yellow LED.
    req_GY.flags = GPIOHANDLE_REQUEST_OUTPUT;

    data_GY.values[0] = 1;         // Green LED: High (ON).
    data_GY.values[1] = 0;         // Yellow LED: Low (OFF).

    ioctl(fd0, GPIO_GET_LINEHANDLE_IOCTL, &req_GY); // Get the line handle.

    for (int i = 0; i < 5; ++i) {
        ioctl(req_GY.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data_GY);
        usleep(1000000); // Sleep for one second.

        data_GY.values[0] = !data_GY.values[0]; // Toggle Green.
        data_GY.values[1] = !data_GY.values[1];  // Toggle Yellow (as printed).
    }

    close(req_GY.fd); // Release the line.
    close(fd0);       // Close the chip descriptor.
    exit(EXIT_SUCCESS);
}
