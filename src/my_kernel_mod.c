#include <linux/module.h>
#include <linux/version.h>

// meta info
MODULE_AUTHOR("Felix Knorre");
MODULE_DESCRIPTION("Hello World Module");
MODULE_LICENSE("GPL");


static int __init hello_init(void){
    printk("Hello, world\n");
    return 0;
}

static void __exit hello_exit(void){
    printk("Goodbye...\n");
}

module_init(hello_init);
module_exit(hello_exit);



