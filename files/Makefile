export PRJ_ROOT := $(CURDIR)
export PRJ_BUILD_ROOT := $(PRJ_ROOT)/output

ARCH            ?= arm64
CROSS_COMPILE   ?=

export CC
export LD
export CLANG
export CROSS_COMPILE
$(info ======================================)
$(info CC          = $(CC))
$(info CROSS_COMPILE=$(CROSS_COMPILE))
$(info CLANG       = $(CLANG))
$(info ======================================)

all: $(addprefix $(PRJ_BUILD_ROOT)/,libbsp.so bspio.ko bsp_eth_tool)

# rules for config
menuconfig: $(PRJ_ROOT)/build/bin/mconf
	test -d $(PRJ_BUILD_ROOT) || mkdir -p $(PRJ_BUILD_ROOT)
	cd $(PRJ_BUILD_ROOT); \
	$(PRJ_ROOT)/Kconfig; \
	mkdir -p include/config include/generated; \
	$(PRJ_ROOT)/build/bin/mconf $(PRJ_ROOT)/Kconfig; \
	if [ -e .config ]; then $(PRJ_ROOT)/build/bin/conf --olddefconfig $(PRJ_ROOT)/Kconfig; fi

savedefconfig: $(PRJ_ROOT)/build/bin/conf
	cd $(PRJ_BUILD_ROOT); \
	defconfig_name=$$(cat defconfig_name | tr -d '\n' | tr -d ' '); \
	$(PRJ_ROOT)/build/bin/conf -s --savedefconfig=$(PRJ_ROOT)/configs/$$defconfig_name $(PRJ_ROOT)/Kconfig; \
	echo "Saved defconfig to: $(PRJ_ROOT)/configs/$$defconfig_name"

%_defconfig: $(PRJ_ROOT)/build/bin/conf
	test -d $(PRJ_BUILD_ROOT) || mkdir -p $(PRJ_BUILD_ROOT)
	echo $@ > $(PRJ_BUILD_ROOT)/defconfig_name
	echo $(PROJECT_NAME)> $(PRJ_BUILD_ROOT)/project
	cd $(PRJ_BUILD_ROOT); \
	$(PRJ_ROOT)/build/bin/conf -s --defconfig=$(PRJ_ROOT)/configs/$@ $(PRJ_ROOT)/Kconfig; \
	mkdir -p include/config include/generated; \
	if [ -e .config ]; then $(PRJ_ROOT)/build/bin/conf --olddefconfig $(PRJ_ROOT)/Kconfig; fi

# ========================
# 【自动加 sysroot，解决头文件找不到】
# ========================
CFLAGS  = -g -fPIC -Wall -Wextra -Wno-unused-parameter -DNO_ENCRYPTION
CFLAGS += -I$(PRJ_BUILD_ROOT)/include/generated -I$(PRJ_ROOT)/include -I$(PRJ_ROOT)/usource/core
CFLAGS += -I$(PRJ_ROOT)/cjson -I $(PRJ_ROOT)/utils -I $(PRJ_ROOT) -I $(PRJ_ROOT)/../psh
CFLAGS += -Wall -Wno-sign-compare -Wformat=2 -Wwrite-strings -Wpointer-arith -Wcast-qual -Wshadow
CFLAGS += -DPOSIX -DMD=2
CFLAGS += -Wno-error=unused-function -Wno-error=unused-variable -w

# Clang 额外选项
ifeq ($(CLANG),1)
CFLAGS += -Qunused-arguments
endif

export CFLAGS

define ksource_make_cmd
	set -e; \
	test -d $(PRJ_BUILD_ROOT)/ksource || mkdir -p $(PRJ_BUILD_ROOT)/ksource;
	cp -a $(PRJ_ROOT)/ksource/* $(PRJ_BUILD_ROOT)/ksource/ 2>/dev/null || true
	cd $(PRJ_BUILD_ROOT)/ksource && \
	$(MAKE) CC=$(CC) LD=$(LD) CLANG=$(CLANG)
endef

TARGET_LIB_LFLAGS  = -lm -lpthread -shared -fPIC
generate:
	$(PRJ_ROOT)/build/script/generate_class_h.sh  -c $(PRJ_BUILD_ROOT)/.config -o  $(PRJ_BUILD_ROOT)/include/generated/wt_tools_class.h -t "struct hw_class"
	@echo $(PRJ_BUILD_ROOT)/include/generated/wt_tools_class.h has been built!
	$(PRJ_ROOT)/build/script/generate_driver_h.sh  -c $(PRJ_BUILD_ROOT)/.config -o  $(PRJ_BUILD_ROOT)/include/generated/wt_tools_driver.h -t "struct hw_driver"
	@echo $(PRJ_BUILD_ROOT)/include/generated/wt_tools_driver.h has been built!
	PRJ_ROOT=$(PRJ_ROOT) PRJ_BUILD_ROOT=$(PRJ_BUILD_ROOT) $(PRJ_ROOT)/build/script/export_header.sh

$(PRJ_BUILD_ROOT)/libbsp.so: $(PRJ_BUILD_ROOT)/usource/built-in.o
	$(CC) $(TARGET_LIB_LFLAGS) -o $@ $^ -lm -lpthread
	mkdir -p $(PRJ_BUILD_ROOT)/strip

$(PRJ_BUILD_ROOT)/bspio.ko:
	$(ksource_make_cmd)


$(PRJ_BUILD_ROOT)/bsp_eth_tool:
	make -C $(PRJ_ROOT)/tools/bsp_eth_tool

HOST_SOURCES   = $(wildcard pack/*.c)
HOST_OBJS      = $(HOST_SOURCES:%.c=$(PRJ_BUILD_ROOT)/%.o)
HOST_CFLAGS    = -D_REENTRANT -O2 -Wall -fPIC -g -Wextra -D$(PLATFORM) -I ../hikcrypto/include -I cjson
HOST_LFLAGS    = -lm -lpthread -no-pie

$(PRJ_BUILD_ROOT)/hikpack:$(HOST_OBJS)
	gcc -o $(PRJ_BUILD_ROOT)/hikpack $^ $(PRJ_ROOT)/../hikcrypto/output/host_lib/libhikcrypto.a $(PRJ_BUILD_ROOT)/cjson/host_obj/cjson/cJSON.o $(HOST_LFLAGS)
	@chmod 777 $(PRJ_BUILD_ROOT)/hikpack

$(HOST_OBJS): $(PRJ_BUILD_ROOT)/%.o:%.c
	@mkdir -p $(dir $@)
	gcc $(HOST_CFLAGS) -c $< -o $@

obj-y += usource/
$(PRJ_BUILD_ROOT)/usource/built-in.o: build_all

build_all:generate
	make -f $(PRJ_ROOT)/build/Makefile.build

clean:
	rm -rf $(PRJ_BUILD_ROOT)

.PHONY: all clean menuconfig savedefconfig %_defconfig generate
