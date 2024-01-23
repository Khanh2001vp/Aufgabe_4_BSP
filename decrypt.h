//
//  decrypt.h
//  Aufgabe_4
//
//  Created by Ng Q Khánh on 20/01/2024.
//

#ifndef decrypt_h
#define decrypt_h

#include <stdio.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <string.h>

/*static struct miscdevice decrypt_misc_device = {
    .minor = 1,
    .name = "decrypt",
    .fops = &decrypt_fops, // Con trỏ đến struct file_operations cho giải mã
};
*/

static ssize_t decrypt_read(struct file *file, char __user *buf, size_t count, loff_t *ppos);

static ssize_t decrypt_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos);

static int __init decrypt_init(void);

static void __exit decrypt_exit(void);

static const struct file_operations decrypt_fops = {
    .owner = THIS_MODULE,
    .read = decrypt_read,
    .write = decrypt_write,
};

#endif /* decrypt_h */
