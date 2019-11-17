//obj-m :=chr_drv_reentrant.o
obj-m +=chr_drv_mutex.o
//obj-m +=chr_drv_spinlock.o


KDIR=/usr/src/linux-headers-2.6.32-41-generic

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	rm -rf $(wildcard *.o *.ko mo* Mo* *.mod.c)
