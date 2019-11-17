obj-m :=mini2440_leds.o

KDIR=/home/praneeth/Desktop/ubuntu-praneeth/mini2440/src/kernel/linux-2.6.29

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf $(wildcard *.o *.ko mo* Mo* *.mod.c )
