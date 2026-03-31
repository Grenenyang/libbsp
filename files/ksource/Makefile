include $(PRJ_BUILD_ROOT)/include/config/auto.conf

obj-m += bspio.o
bspio-objs := core/core.o 
bspio-$(CONFIG_KSOURCE_SWITCH_RTL7071) += switch/rtl9071.o
include $(wildcard $(PRJ_ROOT)/ksource/*/Makefile)
bspio-objs += $(bspio-y)
$(info -----------bspio-objs=$(bspio-objs))
EXTRA_CFLAGS += -I$(PRJ_ROOT) -I$(PRJ_BUILD_ROOT)
.PHONY: all clean
a11:
	make -C $(STAGING_KERNEL_BUILDDIR)  M=$(CURDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules
clean:
	make -C $(STAGING_KERNEL_BUILDDIR)  M=$(CURDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean
