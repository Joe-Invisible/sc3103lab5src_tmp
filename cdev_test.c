#include "linux/export.h"
#include "linux/fs.h"
#include "linux/kern_levels.h"
#include "linux/moduleparam.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int debug_en = 0;
module_param(debug_en, int, 0);
MODULE_PARM_DESC(debug_en, "Enable debug mode");

static int _open (struct inode *, struct file *);
static int _release (struct inode *, struct file *);
ssize_t _read (struct file *, char __user *, size_t, loff_t *);
ssize_t _write (struct file *, const char __user *, size_t, loff_t *);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = _open,
    .release = _release,
    .read = _read,
    .write = _write,
};

static char cdev_buf[32] = { 0 };
static int cdev_bufptr = 0;

static int __init hello_init(void) {
    printk(KERN_ALERT "Useless Module - Joe (C) 2020\n");
    printk(KERN_ALERT "Debug mode %s\n", debug_en ? "ON" : "OFF");
    return register_chrdev(101, "purposeless dev", &fops);
}

static void __exit hello_exit(void) {
    printk(KERN_ALERT "Goodbye\n");
    unregister_chrdev(101, "purposeless dev");
}

static int _open (struct inode *inode, struct file *file) {
    printk(KERN_ALERT "someone opened purposeless dev\n");
    return 0;
}
static int _release (struct inode *inode, struct file *file) {
    printk(KERN_ALERT "someone closed purposeless dev\n");
    return 0;
}

ssize_t _read (struct file *file, char __user *user_buf, size_t len, loff_t *off) {
    printk(KERN_ALERT "someone reading from purposeless dev\n");

    if (cdev_bufptr == 0) 
        return 0;

    if (len > cdev_bufptr) 
        len = cdev_bufptr;

    int err = copy_to_user(user_buf, cdev_buf, len);

    cdev_bufptr -= len;

    return len;
}

ssize_t _write (struct file *file, const char __user *user_buf, size_t len, loff_t *off) {
    printk(KERN_ALERT "someone writing to purposeless dev\n");
    
    if (len > cdev_bufptr - 32)
        return 0;

    int err = copy_from_user(cdev_buf, user_buf, len);
    cdev_bufptr += len;

    return len;
}


module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joe");
MODULE_DESCRIPTION("Simple Module");


