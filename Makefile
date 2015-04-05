KERNELDIR := /usr/lib/modules/$(shell uname -r)/build

obj-m := cpucycles4ns.o

all:
	$(MAKE) -C $(KERNELDIR) M=`pwd` EXTRAVERSION=-omap1 modules

.PHONY: clean
clean:
	- rm -fr *.mod.* *.o *.ko modules.order Module.symvers .cpu* .tmp*
