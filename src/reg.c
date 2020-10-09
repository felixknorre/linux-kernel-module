// device driver registration and deregistration

#include <linux/module.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include "./reg.h"

#define KERNEL_MODULE_NAME "MyKernelModule"
#define KM_CLASS_NAME "KernelModuleClass"
#define KM_DEVICE_NAME "my_km"
#define KM_DEBUG

/**********************************************************************/
/*  POSIX Interface                 */
/*********************************************************************/

static int km_open(struct inode* fsdev, struct file * f_entity)
{
    #ifdef KM_DEBUG
        printk("MY_KM APPLICATION INTERFACE: open()...\n");
    #endif
    return 0;
}


static int km_close(struct inode * fsdev, struct file * f_entity)
{
    #ifdef KM_DEBUG
        printk("MY_KM APPLICATION INTERFACE: close()...\n");
    #endif
    return 0;
}

static ssize_t km_write(struct file * f_entity, const char * buffer, size_t count, loff_t * offset)
{
    #ifdef KM_DEBUG
        printk("MY_KM APPLICATION INTERFACE: write()...\n");
    #endif
    return 0;
}

static ssize_t km_read(struct file * f_entity, char * buffer, size_t count, loff_t * offset)
{
    #ifdef KM_DEBUG
        printk("MY_KM APPLICATION INTERFACE: read()...\n");
    #endif
    return 0;
}

static long km_ioctl(struct file * f_entitiy, unsigned int cmd, unsigned long arg)
{
    #ifdef KM_DEBUG
        printk("MY_KM APPLICATION INTERFACE: ioctl()\n");
    #endif
    return 0;
}


/*******************************************/
/*     Character Device Registration       */
/******************************************/

// device number
static dev_t km_dn;
// character device struct
static struct cdev * cdev_obj = NULL;

static struct file_operations km_op = {
    .open = km_open,
    .release = km_close,
    .read = km_read,
    .write = km_write,
    .unlocked_ioctl = km_ioctl,
    .owner = THIS_MODULE
};

// class
static struct class * km_class = NULL;
// device
static struct device * km_device = NULL;

int km_reg(void)
{
	int result;

    // 1. alloc a device number
    if(alloc_chrdev_region(&km_dn, 0, 1, KERNEL_MODULE_NAME))
    {
        printk("MY_KM: Dynamic device number assignment faild...\n");
        return -EIO;
    }

    #ifdef KERNEL_DEBUGGING
        printk("MY_KM: Dynamic device number: DN=%d, Major=%d, Minor=%d", km_dn, MAJOR(km_dn), MINOR(km_dn));
    #endif

    // 2. alloc memory for cdev
    cdev_obj = cdev_alloc();
    if(cdev_obj == NULL)
    {
        printk("MY_KM: Driver object allocation failed...\n");
        goto free_devnumb;
    }
    // 3. init character device object
    cdev_init(cdev_obj, &km_op);
    kobject_set_name(&cdev_obj->kobj, KM_DEVICE_NAME);
    cdev_obj->owner = THIS_MODULE;

    // 4. registration of the character device driver
    result = cdev_add(cdev_obj, km_dn, 1);
    if(result != 0)
    {
        printk("MY_KM fatal error: adding device failed...\n");
        goto free_cdev;
    }

    // 5. create class 
    km_class = class_create(THIS_MODULE, KM_CLASS_NAME);
    if(km_class == NULL)
    {
        printk("MY_KM fatal error: create class failed...\n");
        goto free_cdev;
    }

    // 6. create dyn device in /dev
    km_device = device_create(km_class, NULL, km_dn, NULL, KM_DEVICE_NAME);
    if(km_device == NULL)
    {
        printk("MY_KM fatal error: create device failed...\n");
        goto free_class;
    }
         
    
    return 0;

    free_class:
        class_destroy(km_class);

    free_cdev:
        kobject_put(&cdev_obj->kobj);
        cdev_obj = NULL;

    free_devnumb:
        unregister_chrdev_region(km_dn, 1);
        return -1;
}

void km_dereg(void)
{
    // reverse character device registration

    // free device
    device_destroy(km_class, km_dn);
    
    // free class
    class_destroy(km_class);

    // free cdev
    if(cdev_obj){
        cdev_del(cdev_obj);
    }
    
    // free device number
    unregister_chrdev_region(km_dn, 1);

}

