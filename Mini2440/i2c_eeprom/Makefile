# Makefile for the mi2c driver


obj-m := mi2c.o
mi2c-objs := mi2c-char.o mi2c-i2c.o  

KERNELDIR=/home/alexander/project/mini2440/linux-2.6.29


KDIR= /home/alexander/project/mini2440/linux-2.6.29

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf $(wildcard *.o *.ko mo* Mo* *.mod.c )
