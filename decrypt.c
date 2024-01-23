//
//  decrypt.c
//  Aufgabe_4
//
//  Created by Ng Q Khánh on 20/01/2024.
//

#include "decrypt.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
//#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <string.h>

#define TRANSLATE_SHIFT 3 // Số lượng dịch chuyển (Caesar shift)
#define BUFFER_SIZE 40 // Kích thước của bộ đệm

//static struct miscdevice decrypt_misc_device;
static char decrypt_buffer[BUFFER_SIZE];
static int buffer_used = 0;
static int major_number_decrypt;

static ssize_t decrypt_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    if (count > buffer_used){
        count = buffer_used;
    }
    if (copy_to_user(buf, decrypt_buffer, count)){
        return -EFAULT;
    }

    // Di chuyển dữ liệu còn lại trong bộ đệm lên đầu
    memmove(decrypt_buffer, decrypt_buffer + count, buffer_used - count);
    buffer_used -= count;

    return count;
}

static ssize_t decrypt_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    if (count > BUFFER_SIZE - buffer_used){
        return -ENOSPC; // Không đủ không gian trong bộ đệm
    }

    if (copy_from_user(decrypt_buffer + buffer_used, buf, count)){
        return -EFAULT;
    }

    // Giải mã dữ liệu theo thuật toán Caesar (đảo ngược dịch chuyển)
    int i;
    for (i = buffer_used; i < buffer_used + count; i++) {
        if (decrypt_buffer[i] >= 'A' && decrypt_buffer[i] <= 'Z') {
            decrypt_buffer[i] = 'A' + ((decrypt_buffer[i] - 'A' - TRANSLATE_SHIFT + 26) % 26);
        } else if (decrypt_buffer[i] >= 'a' && decrypt_buffer[i] <= 'z') {
            decrypt_buffer[i] = 'a' + ((decrypt_buffer[i] - 'a' - TRANSLATE_SHIFT + 26) % 26);
        }
    }

    buffer_used += count;

    return count;
}


static int __init decrypt_init(void)
{
    /*int ret;

    ret = misc_register(&decrypt_misc_device);
    if (ret) {
        pr_err("Failed to register the decrypt device\n");
        return ret;
    }

    pr_info("Decrypt device registered\n");

    return 0;
     */
    
    major_number_decrypt = register_chrdev(1, "decrypt", &decrypt_fops);
        if (major_number_decrypt != 1) {
            pr_err("Failed to register /dev/decrypt\n");
            unregister_chrdev(major_number_encrypt, "encrypt");
            return major_number_decrypt;
        }
    pr_info("Decrypt device registered\n");

}
static void __exit decrypt_exit(void)
{
    //misc_deregister(&decrypt_misc_device);
    
    unregister_chrdev(major_number_decrypt, "decrypt");
    pr_info("Decrypt device unregistered\n");
}

module_init(decrypt_init);
module_exit(decrypt_exit);

MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple Caesar Decryption Device");
MODULE_LICENSE("GPL");

