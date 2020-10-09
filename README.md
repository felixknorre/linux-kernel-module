# linux-kernel-module

## Installation

Use Debian Buster V10.3.0 and install additional packages:

```bash
# module-assistant
sudo apt-get install module-assistant
# make
sudo apt-get install make
# look up your linux kernel version 
uname -a
# and install matching linux source and header (in my case)
sudo apt-get install linux-source-4.19
sudo apt-get install linux-headers-4.19.0-8-amd64
# fakeroot
sudo apt-get install fakeroot
# libncurses5-dev
sudo apt-get install libncurses5-dev
# bin86
sudo apt-get install bin86
```
Source: Prof. Dr. Stefan Böhmer

## Usage

### Compilation
```bash
cd src

make
```

### Commands for Kernel Modules in Linux

```bash

# list modules
lsmod

# show information about module
modinfo [modulename/filename]

# insert module
insmod [filename] [module options...]

# remove module
# -f(force), -s(send errors to syslog), -v(show version of program and exit)
rmmod [-f] [-s] [-v] [modulename]

# inserting and removing modules
# -r(remove), -q(quit), -a(insert all module names on the command line)
modprobe [-r] [-q] [-a]

```

## Documentation

### printk
print messages to the kernel log (var/log/messages), used for debugging

### __init and __exit
Marker for funktions that are only used at initialization and cleanup
* __init: is executed only once, then swap space/delete
* __exit: only used during unloading, Modules usually run for a long time, therefore swap space

### Registration
1. Allocation of a device number
```c
static dev_t devNumb;
// dev:output para, 
// baseminor: first minor number, 
// count: number of minor numbers required 
// name: name of the device or driver
alloc_chrdev_region(&devNumb, 0, 1, "ModuleName");
```
2. Allocation of memory for a cdev structure
```c
static struct cdev * device_cdev_obj;
device_cdev_obj = cdev_alloc();
```

3. Init cdev
* you need a file_operations structure for calling the POSIXs functions
```c
static struct file_operations km_op = {
    .open = km_open,
    .release = km_close,
    .read = km_read,
    .write = km_write,
    .unlocked_ioctl = km_ioctl,
    .owner = THIS_MODULE
};

cdev_init(device_cdev_obj, &km_op);
kobject_set_name(&device_cdev_obj->kobj, KM_DRIVER_NAME);
device_cdev_obj->owner = THIS_MODULE;
```

4. Add the kernel object to the kernel
```c
result = cdev_add(device_cdev_obj, devNumb, 1);
``` 

5. Create a class
```c
km_class = class_create(THIS_MODULE, KM_CLASS_NAME);
```

6. Create a device
```c
km_device = device_create(km_class, NULL, devNumb, NULL, KM_DEVICE_NAME);
```

### Workqueue

* the variable HZ defines the interrupts per second (100-1000)
* jiffies -> number of interrupts
* T_Future = jiffies + n * Hz, n = seconds

#### Creation delayed workqueue

* struct workqueue_struct -> def. of a workqueue
* struct work_struct -> def. of task

1. Creation of a workqueue
```c 
#define WQ_NAME "my_wq"
static workqueue_struct * wq;

wq = create_workqueue(WQ_NAME);
```

2. Define task
```c
DECLARE_WORK(work_name, funct_ptr);
DECLARE_DELAYED_WORK(work_name, funct_ptr);
```
3. Insert task
```c
queue_work(wq, &work_name, 10*Hz);
queue_delayed_work(wq, &work_name, 10*Hz);
```

#### Removal
```c
ret = cancel_delayed_work(&kmm_wq_elem);

if(ret == 0) {
    flush_delayed_work(&kmm_wq_elem);
}	

destroy_workqueue(wq);
```
