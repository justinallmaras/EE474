export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-
ccflags-y := -std=gnu99

obj-m += new_char.o motor_driva.o

# Kernel source directory
KDIR =/tmp/guest-ymfp4q/group5/Lab3/KERNEL

PWD = $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
