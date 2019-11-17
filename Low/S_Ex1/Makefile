obj-m := simple_module.o

KDIR= /home/alexander/project/mini2440/linux-2.6.29

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf $(wildcard *.o *.ko mo* Mo* *.mod.c a.out)

