obj-m := switch.o
K_SRC =/home/ashu/work/am335x/linux-3.2
all:
	make -C ${K_SRC} M=${PWD} modules
clean:
	rm -f *.o *.ko *.mod.c *.mod.o *.order *.symvers *~
