/*
 *
 *
 * Nick Miller
 * Dice Driver
 *
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/random.h>

#include <asm/uaccess.h>


static ssize_t dice_read(struct file * file, char * buf,
                         size_t count, loff_t *ppos)
{
    int i;
    get_random_bytes(&i, sizeof(int));
    
    
    i = (i % 6) + 1;
    while(i == 0){
        i = (i % 6) + 1;

    }
    
    if (i < 0){
        i *= -1;
    }
    if (count < sizeof(int)){
        return -EINVAL;
    }
    if (*ppos != 0){
        return 0;
    }
    
    if (copy_to_user(buf, &i, sizeof(int))){
        return -EINVAL;
    }
    *ppos = sizeof(int);
    
    return sizeof(int);
}


static const struct file_operations dice_fops = {
    .owner		= THIS_MODULE,
    .read		= dice_read,
};

static struct miscdevice dice_dev = {

    MISC_DYNAMIC_MINOR,

    "dice_driver",

    &dice_fops
};

static int __init
dice_init(void)
{
    int ret;
    

    ret = misc_register(&dice_dev);
    if (ret)
    printk(KERN_ERR
           "Unable to register \"DiceDriver!\" misc device\n");
    
    return ret;
}

module_init(dice_init);

static void __exit
dice_exit(void)
{
    misc_deregister(&dice_dev);
}

module_exit(dice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nick Miller <nam99@pitt.edu>");
MODULE_DESCRIPTION("\"Random Dice Roll\" minimal module");
MODULE_VERSION("dev");
