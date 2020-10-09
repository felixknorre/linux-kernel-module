#include <linux/module.h>
#include <linux/version.h>
#include <linux/workqueue.h>

#include "./reg.h"

#define WQ_NAME "km_wq"

// meta info
MODULE_AUTHOR("Felix Knorre");
MODULE_DESCRIPTION("Frist Kernel Module");
MODULE_LICENSE("GPL");

/***************************************************************/
/* workqueue */
/**************************************************************/

// activate wq
static unsigned char sched_set = 0x00;


static struct workqueue_struct * wq;

void sched_print(struct work_struct * f);

static DECLARE_DELAYED_WORK(print_work, sched_print);

void sched_print(struct work_struct * f){
	static int count = 0;
	int res = 0;

	count++;
	

	printk("MY_KM: WQ print %d\n", count);
	
	if(sched_set == 0x00){
		res = queue_delayed_work(wq, &print_work, 20 * HZ);
		if(res != 1){
			printk("MY_KM fatal error: add worl to queue failed....\n");
		}
	}
	
}


/***************************************************************/
/* init and exit functions */
/**************************************************************/

static int __init km_init(void){
    int ret_val;        

    printk("MY_KM: Initialization...\n");
    //Module registratiom
    ret_val = km_reg();
    if(ret_val != 0)
    {
        printk("MY_KM: Initialization failed...\n");
        goto clear_km;
    }

	// create workqueu with work
	wq = create_workqueue(WQ_NAME);
	if(wq == NULL){
		printk("MY_KM fatal error: create workqueue failed....\n");	
	}
	// add work
	ret_val = queue_delayed_work(wq, &print_work, 20 * HZ);
	if(ret_val != 1){
		printk("MY_KM fatal error: add worl to queue failed....\n");
	}

    return 0;

    clear_km:
        km_dereg();
        return -EIO;
}

static void __exit km_exit(void){
	int res;

    km_dereg();
    printk("MY_KM: Terminating...\n");
	
	// disable wq
    sched_set = 0x01;

    res = cancel_delayed_work(&print_work);
    
    // when work in process
    if(res == 0)
    {
        flush_delayed_work(&print_work);
    }    

    destroy_workqueue(wq);
}

module_init(km_init);
module_exit(km_exit);



