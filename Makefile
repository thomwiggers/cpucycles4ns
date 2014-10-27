KERNELDIR := /usr/lib/modules/3.12.29-1-ARCH/build

obj-m := cpucycles4ns.o

all:
	$(MAKE) -C $(KERNELDIR) M=`pwd` EXTRAVERSION=-omap1 modules

.PHONY: clean
clean:
	- rm -fr *.mod.* *.o *.ko modules.order Module.symvers .cpu* .tmp*
