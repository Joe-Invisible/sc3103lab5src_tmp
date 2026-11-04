#include <linux/module.h>

/* Additional includes omitted in the original handout. */

#include <linux/gpio.h>
#include <linux/interrupt.h>

static unsigned int ledGreen = 4;     // GPIO4 (header pin 7): Green LED.
static unsigned int pushButton = 11; // GPIO11 (header pin 23): Push button.
static unsigned int irqNumber;       // IRQ number shared within this file.
static bool ledOn = 0;               // Used to toggle the LED state.

// GPIO interrupt handler.
static irq_handler_t rpi_gpio_isr(unsigned int irq, void *dev_id,
                                  struct pt_regs *regs)
{
    /* TODO (original handout): Toggle the LED state. */
    /* TODO (original handout): Set the LED accordingly. */
    printk(KERN_ALERT "GPIO Interrupt!\n");
    return (irq_handler_t)IRQ_HANDLED; // Announce that IRQ was handled.
}

// LKM exit function.
static void __exit rpi_gpio_exit(void)
{
    gpio_set_value(ledGreen, 0); // Turn the LED off.
    gpio_free(ledGreen);         // Free the LED GPIO (as printed).
    gpio_free(pushButton);       // Free the button GPIO (as printed).
    free_irq(irqNumber, NULL);   // Free the IRQ; no dev_id.
    printk(KERN_ALERT "Goodbye from the GPIO LKM!\n");
}

// LKM initialization function.
static int __init rpi_gpio_init(void)
{
    int result = 0;
    printk(KERN_ALERT " Initializing the GPIO LKM\n");

    ledOn = true;                           // LED is initially ON.
    gpio_request(ledGreen, "sysfs");       // Request the LED GPIO.
    gpio_direction_output(ledGreen, ledOn); // Set output and turn on LED.

    gpio_request(pushButton, "sysfs"); // Request the button GPIO.
    gpio_direction_input(pushButton);   // Set as input.
    gpio_set_debounce(pushButton, 1000); // Debounce delay (handout: 1000 ms).

    irqNumber = gpio_to_irq(pushButton); // Map button to IRQ number.
    printk(KERN_ALERT "Button mapped to IRQ: %d\n", irqNumber);

    // Request an interrupt line.
    result = request_irq(irqNumber,          // Requested interrupt number.
                         (irq_handler_t)rpi_gpio_isr, // ISR (as printed).
                         IRQF_TRIGGER_RISING, // Trigger on rising edge.
                         "rpi_gpio_handler",  // Name in /proc/interrupts.
                         NULL);              // No shared dev_id.
    return result;
}
