# kbuild part of makefile
ifneq ($(KERNELRELEASE),)
obj-m :=  cdrv.o
obj-m :=  cdrv_dynamic.o
obj-m :=  cdrv_dynamic_two_minor_nums.o
obj-m :=  cdrv_automated_file_creation.o
EXTRA_CFLAGS += -DDEBUG
else

KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

# CONFIG_MODULE_SIG=n
# command to convert input files into desired target 
# this command uses kbuild scripts of kernel-header/source folder specified
build:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

endif
