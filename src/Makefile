#
# Makefile for a kernel module
#
# 2012, Stefan Böhmer (stefan.boehmer@brsu.de)
#

ifneq ($(KERNELRELEASE),) 
obj-m	:= my_kernel_mod.o
my_kernel_mod-objs  := my_km.o reg.o


else
KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)

default:
	$(MAKE)	-C $(KDIR)	M=$(PWD) modules

#clean: 
#	$(MAKE)	-C $(KDIR)	M=$(PWD) clean

endif
