export PRJ_ROOT := $(CURDIR)
export PRJ_BUILD_ROOT := $(PRJ_ROOT)/output

export CROSS_COMPILE ?= $(CONFIG_CROSS_PREFIX:%=%)
$(info CROSS_COMPILE=$(CROSS_COMPILE))

all: $(addprefix $(PRJ_BUILD_ROOT)/,libwttools.so wt_tools)

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
	cd $(PRJ_BUILD_ROOT); \
	$(PRJ_ROOT)/build/bin/conf -s --defconfig=$(PRJ_ROOT)/configs/$@ $(PRJ_ROOT)/Kconfig; \
	mkdir -p include/config include/generated; \
	if [ -e .config ]; then $(PRJ_ROOT)/build/bin/conf --olddefconfig $(PRJ_ROOT)/Kconfig; fi

CFLAGS  = -g -fPIC -Wall -Wextra -Wno-unused-parameter -DNO_ENCRYPTION
CFLAGS += -I$(PRJ_BUILD_ROOT)/include/generated -I$(PRJ_ROOT)/include -I$(PRJ_ROOT)/usource/core 
CFLAGS += -I$(PRJ_ROOT)/cjson -I $(PRJ_ROOT)/utils -I $(PRJ_ROOT) -I $(PRJ_ROOT)/../psh
CFLAGS += -g
CFLAGS += -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare -Wformat=2 -Wwrite-strings -Wpointer-arith -Wcast-qual -Wshadow -Wno-missing-field-initializers -Winline -Wbad-function-cast
CFLAGS += -DPOSIX -DMD=2
CFLAGS +=  -Wl,--export-dynamic -Wl,--undefined=__start___wt_tools_modules

export CFLAGS


TARGET_LIB_LFLAGS  = -lm -lpthread -shared -fPIC
generate:
	$(PRJ_ROOT)/build/script/generate_class_h.sh  -c $(PRJ_BUILD_ROOT)/.config -o  $(PRJ_BUILD_ROOT)/include/generated/wt_tools_class.h -t "struct class"
	@echo $(PRJ_BUILD_ROOT)/include/generated/wt_tools_class.h has been built!

$(PRJ_BUILD_ROOT)/libwttools.so: generate  $(PRJ_BUILD_ROOT)/usource/built-in.o 
	$(CROSS_COMPILE)gcc $(TARGET_LIB_LFLAGS) -o $@ $(PRJ_BUILD_ROOT)/usource/built-in.o 
	mkdir -p $(PRJ_BUILD_ROOT)/strip
	$(CROSS_COMPILE)strip $@ -o $(PRJ_BUILD_ROOT)/strip/libupdate.so
	@cp $(PRJ_ROOT)/unpack/update.h $(PRJ_BUILD_ROOT)/update.h
	@echo $(PRJ_BUILD_ROOT)/libupdate.so has been built!

TARGET_MAIN_LFLAGS  = -lm -ldl -lpthread -L$(PRJ_BUILD_ROOT) -lupdate
TARGET_MAIN_LFLAGS += -L$(PRJ_ROOT)/../hikcrypto/output/target_lib -lhikcrypto
TARGET_MAIN_LFLAGS += $(LIBBSP_LDFLAGS_EXTRA)
TARGET_MAIN_LFLAGS += -lbsp -lrt -L$(TOP)/libbsp/output -lbsp
TARGET_MAIN_LFLAGS += -lrt -L$(TOP)/libbsp/output -lbsp
TARGET_MAIN_LFLAGS += -L$(PRO_ROOT)/../psh -lpsh


$(PRJ_BUILD_ROOT)/hikupdate: $(PRJ_BUILD_ROOT)/update/built-in.o
	$(CROSS_COMPILE)gcc -o $@ $^ $(TARGET_MAIN_LFLAGS)
	$(CROSS_COMPILE)strip $@
	@echo $(PRJ_BUILD_ROOT)/hikupdate has been built!
HOST_SOURCES   = $(wildcard pack/*.c)
HOST_OBJS      = $(HOST_SOURCES:%.c=$(PRJ_BUILD_ROOT)/%.o)
HOST_CFLAGS    = -D_REENTRANT -O2 -Wall -fPIC -g -Wextra -D$(PLATFORM) -I ../hikcrypto/include -I cjson
HOST_LFLAGS    = -lm -lpthread -no-pie

$(PRJ_BUILD_ROOT)/hikpack:$(HOST_OBJS)
	gcc -o $(PRJ_BUILD_ROOT)/hikpack $^ $(PRJ_ROOT)/../hikcrypto/output/host_lib/libhikcrypto.a $(PRJ_BUILD_ROOT)/cjson/host_obj/cjson/cJSON.o $(HOST_LFLAGS)
	@chmod 777 $(PRJ_BUILD_ROOT)/hikpack
	@echo $(PRJ_BUILD_ROOT)/hikpack has been built!

$(HOST_OBJS): $(PRJ_BUILD_ROOT)/%.o:%.c
	@mkdir -p $(PRJ_BUILD_ROOT)/pack
	@mkdir -p $(PRJ_BUILD_ROOT)/$(dir $*)
	gcc $(HOST_CFLAGS) -c $^ -o $@

obj-y += usource/
$(PRJ_BUILD_ROOT)/usource/built-in.o:build_all

build_all:
	make -f $(PRJ_ROOT)/build/Makefile.build

clean:
	rm -rf $(PRJ_BUILD_ROOT)

.PHONY: all clean menuconfig savedefconfig %_defconfig
