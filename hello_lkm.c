#include <linux/init.h>   // Needed for module_init and module_exit.
#include <linux/module.h> // Needed by all modules.
#include <linux/kernel.h> // Needed for KERN_ definitions.

static int __init hello_init(void)
{
    printk(KERN_ALERT "Hello from kernel world\n");
    return 0; // Zero for success; negative for failure.
}

static void __exit hello_exit(void)
{
    printk(KERN_ALERT "Goodbye from kernel world\n");
}

module_init(hello_init); // Module initialization routine (placeholder).
module_exit(hello_exit); // Module exit routine (placeholder).

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CE3103");
MODULE_DESCRIPTION("Simple Hello module");
MODULE_VERSION("V1");
