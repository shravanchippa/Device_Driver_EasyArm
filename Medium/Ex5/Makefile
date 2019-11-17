obj-m :=procfs1.o
obj-m +=procfs2.o


KDIR=/usr/src/linux-headers-2.6.32-41-generic

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf $(wildcard *.o *.ko mo* Mo* *.mod.c )
