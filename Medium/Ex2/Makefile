obj-m :=chr_drv_udev.o

KDIR= /lib/modules/`uname -r`/build/

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	rm -rf $(wildcard *.o *.ko mo* Mo* *.mod.c)
