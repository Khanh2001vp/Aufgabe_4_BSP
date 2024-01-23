//
//  encrypt.h
//  Aufgabe_4
//
//  Created by Ng Q Khánh on 20/01/2024.
//

#ifndef encrypt_h
#define encrypt_h
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
//#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <stdio.h>
#include <string.h>

/*static struct miscdevice encrypt_misc_device = {
    .minor = 0,
    .name = "encrypt",
    .fops = &encrypt_fops, // Con trỏ đến struct file_operations
};
 */

static ssize_t encrypt_read(struct file *file, char __user *buf, size_t count, loff_t *ppos);

static ssize_t encrypt_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos);

static int __init encrypt_init(void);

static void __exit encrypt_exit(void);

static const struct file_operations encrypt_fops = {
    .owner = THIS_MODULE,
    .read = encrypt_read,
    .write = encrypt_write,
};

#endif /* encrypt_h */
