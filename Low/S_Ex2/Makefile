obj-m := simple_module_export.o usage.o


KDIR=/home/alexander/project/mini2440/linux-2.6.29

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf $(wildcard *.o *.ko mo* Mo* *.mod.c a.out)

