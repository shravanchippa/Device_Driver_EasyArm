obj-m :=chr_drv_skel.o
obj-m += chr_drv_dynamic.o

KDIR=/lib/modules/`uname -r`/build/



all:
	make -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf $(wildcard *.o *.ko mo* Mo* *.mod.c )
