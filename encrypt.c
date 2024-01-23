//
//  encrypt.c
//  Aufgabe_4
//
//  Created by Ng Q Khánh on 20/01/2024.
//

#include "encrypt.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
//#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <string.h>

#define TRANSLATE_SHIFT 3 // Số lượng dịch chuyển (Caesar shift)
#define BUFFER_SIZE 40 // Kích thước của bộ đệm

//static struct miscdevice encrypt_misc_device;
static char encrypt_buffer[BUFFER_SIZE];
static int buffer_used = 0;
static int major_number_encrypt;

static ssize_t encrypt_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    if (count > buffer_used){
        count = buffer_used;
    }
    if (copy_to_user(buf, encrypt_buffer, count)){
        return -EFAULT;
    }

    // Di chuyển dữ liệu còn lại trong bộ đệm lên đầu
    memmove(encrypt_buffer, encrypt_buffer + count, buffer_used - count);
    buffer_used -= count;

    return count;
}

static ssize_t encrypt_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    if (count > BUFFER_SIZE - buffer_used){
        return -ENOSPC; // Không đủ không gian trong bộ đệm
    }

    if (copy_from_user(encrypt_buffer + buffer_used, buf, count)){
        return -EFAULT;
    }

    // Mã hóa dữ liệu theo thuật toán Caesar
    int i;
    for (i = buffer_used; i < buffer_used + count; i++) {
        if (encrypt_buffer[i] >= 'A' && encrypt_buffer[i] <= 'Z') {
            encrypt_buffer[i] = 'A' + ((encrypt_buffer[i] - 'A' + TRANSLATE_SHIFT) % 26);
        } else if (encrypt_buffer[i] >= 'a' && encrypt_buffer[i] <= 'z') {
            encrypt_buffer[i] = 'a' + ((encrypt_buffer[i] - 'a' + TRANSLATE_SHIFT) % 26);
        }
    }

    buffer_used += count;

    return count;
}


static int __init encrypt_init(void)
{
   /*int ret;

    ret = misc_register(&encrypt_misc_device);
    if (ret) {
        pr_err("Failed to register the encrypt device\n");
        return ret;
    }

    pr_info("Encrypt device registered\n");

    return 0;
    */
    
    major_number_encrypt = register_chrdev(0, "encrypt", &encrypt_fops);
        if (major_number_encrypt != 0) {
            pr_err("Failed to register /dev/encrypt\n");
            return major_number_encrypt;
        }
    pr_info("Encrypt device registered\n");

}

static void __exit encrypt_exit(void)
{
    //misc_deregister(&encrypt_misc_device);
    unregister_chrdev(major_number_encrypt, "encrypt");
    pr_info("Encrypt device unregistered\n");
}

module_init(encrypt_init);
module_exit(encrypt_exit);

MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple Caesar Encryption Device");
MODULE_LICENSE("GPL");

