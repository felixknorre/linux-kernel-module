# linux-kernel-module

## Installation

Use Debian Buster V10.3.0 and install additional packages:

```bash
# module-assistant
sudo apt-get install module-assistant
# make
sudo apt-get install make
# look up your linux version 
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
cd HelloWorld

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
print messages to the kernel log (var/log/syslog)
