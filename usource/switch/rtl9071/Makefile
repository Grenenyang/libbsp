obj-y += rtl9071.o
obj-y += src/



# 芯片版本配置（保留原有逻辑）
TARGET_CHIP_VERSION ?= cvb
ESM_ENABLE ?= 0
RTK_DEV ?= 1
RTK_EXAMPLE_CODE ?= 1  # 关闭示例代码（SO不需要，按需开启）

##############################################################################
## 保留：芯片版本宏定义（dv1/dva/cvb/cva）
##############################################################################
RTL906X_TARGET_CHIP_VERSION :=
ifneq (,$(findstring dva, $(TARGET_CHIP_VERSION)))
RTL906X_TARGET_CHIP_VERSION += dva
endif
ifneq (,$(findstring dv1, $(TARGET_CHIP_VERSION)))
RTL906X_TARGET_CHIP_VERSION += dv1
endif
ifneq (,$(findstring cvb, $(TARGET_CHIP_VERSION)))
RTL906X_TARGET_CHIP_VERSION += cvb
obj-y += RTL907XC_vB/
endif
ifneq (,$(findstring cva, $(TARGET_CHIP_VERSION)))
RTL906X_TARGET_CHIP_VERSION += cva
endif
ifneq (,$(findstring all, $(TARGET_CHIP_VERSION)))
RTL906X_TARGET_CHIP_VERSION += dv1 dva cvb cva
endif

##############################################################################
## 保留：SDK路径配置 + 新增SO必需的-fPIC参数
##############################################################################
SDK_DIR = $(CURDIR)
SDK_GENERAL_SOURCE_DIR = ${SDK_DIR}/src
SDK_GENERAL_HEADER_DIR = ${SDK_DIR}/include
GENERAL_EXAMPLES_SOURCE_DIR = ${SDK_DIR}/examples
SDK_CHIP_DIR_SDK_DV1 = ${SDK_DIR}/RTL907XD_v1
SDK_CHIP_DIR_SDK_DVA = ${SDK_DIR}/RTL907XD_vA
SDK_CHIP_DIR_SDK_CVB = ${SDK_DIR}/RTL907XC_vB
SDK_CHIP_DIR_SDK_CVA = ${SDK_DIR}/RTL907XC_vA

# 适配SO：添加-fPIC（位置无关代码），保留原有CFLAGS
CFLAGS += -fPIC -DRTL_907X_C -DRTL_907X_D
ifneq (,$(findstring dv1, $(RTL906X_TARGET_CHIP_VERSION)))
CFLAGS += -DRTL_907XD_V1
SDK_SOURCE_DIR += ${SDK_CHIP_DIR_SDK_DV1}/src
ASICDRV_SOURCE_DIR += ${SDK_CHIP_DIR_SDK_DV1}/src/asicdrv
BASIC_SOURCE_DIR += ${SDK_CHIP_DIR_SDK_DV1}/src/asicdrv/basic
endif
ifneq (,$(findstring dva, $(RTL906X_TARGET_CHIP_VERSION)))
CFLAGS += -DRTL_907XD_VA
SDK_SOURCE_DIR += ${SDK_CHIP_DIR_SDK_DVA}/src
ASICDRV_SOURCE_DIR += ${SDK_CHIP_DIR_SDK_DVA}/src/asicdrv
BASIC_SOURCE_DIR += ${SDK_CHIP_DIR_SDK_DVA}/src/asicdrv/basic
endif
ifneq (,$(findstring cvb, $(RTL906X_TARGET_CHIP_VERSION)))
CFLAGS += -DRTL_907XC_VB
SDK_SOURCE_DIR += ${SDK_CHIP_DIR_SDK_CVB}/src
ASICDRV_SOURCE_DIR += ${SDK_CHIP_DIR_SDK_CVB}/src/asicdrv
BASIC_SOURCE_DIR += ${SDK_CHIP_DIR_SDK_CVB}/src/asicdrv/basic
endif
ifneq (,$(findstring cva, $(RTL906X_TARGET_CHIP_VERSION)))
CFLAGS += -DRTL_907XC_VA
SDK_SOURCE_DIR += ${SDK_CHIP_DIR_SDK_CVA}/src
ASICDRV_SOURCE_DIR += ${SDK_CHIP_DIR_SDK_CVA}/src/asicdrv
BASIC_SOURCE_DIR += ${SDK_CHIP_DIR_SDK_CVA}/src/asicdrv/basic
endif

##############################################################################
## 保留：头文件路径（适配Makefile.build的INCLUDE_FLAGS → cflags-y）
##############################################################################
cflags-y += -I${SDK_GENERAL_HEADER_DIR}
cflags-y += -I${SDK_CHIP_DIR_SDK_DV1}/include -I${SDK_CHIP_DIR_SDK_DV1}/include/asicdrv -I${SDK_CHIP_DIR_SDK_DV1}/include/asicdrv/basic
cflags-y += -I${SDK_CHIP_DIR_SDK_DVA}/include -I${SDK_CHIP_DIR_SDK_DVA}/include/asicdrv -I${SDK_CHIP_DIR_SDK_DVA}/include/asicdrv/basic
cflags-y += -I${SDK_CHIP_DIR_SDK_CVB}/include -I${SDK_CHIP_DIR_SDK_CVB}/include/asicdrv -I${SDK_CHIP_DIR_SDK_CVB}/include/asicdrv/basic
cflags-y += -I${SDK_CHIP_DIR_SDK_CVA}/include -I${SDK_CHIP_DIR_SDK_CVA}/include/asicdrv -I${SDK_CHIP_DIR_SDK_CVA}/include/asicdrv/basic

##############################################################################
## 适配：源文件列表 → 映射为obj-y（移除main.c，SO不需要入口）
##############################################################################
SOURCE_DIR = ${SDK_GENERAL_SOURCE_DIR} \
             ${SDK_SOURCE_DIR} \
             ${ASICDRV_SOURCE_DIR} \
             ${BASIC_SOURCE_DIR}



# 核心：原有SRCS转为obj-y，移除main.c
rtl9071_core-y += rtkas_regif.c rtkas_system_porting.c rtkas_common.c rtk_swtich.c
# 按芯片版本追加源文件
ifneq (,$(findstring dv1, $(RTL906X_TARGET_CHIP_VERSION)))
rtl9071_core-y += rtl907xd_v1_reg_list.c rtl907xd_v1_table_list.c rtl907xd_v1_asicdrv_mem_ctrl.c rtl907xd_v1_asicdrv.c
endif
ifneq (,$(findstring dva, $(RTL906X_TARGET_CHIP_VERSION)))
rtl9071_core-y += rtl907xd_va_reg_list.c rtl907xd_va_table_list.c rtl907xd_va_asicdrv_mem_ctrl.c rtl907xd_va_asicdrv_timer.c rtl907xd_va_asicdrv.c
ifeq ($(ESM_ENABLE), 1)
rtl9071_core-y += rtl907xd_va_api_esm.c
CFLAGS += -DESM_ENABLE
endif
endif
ifneq (,$(findstring cvb, $(RTL906X_TARGET_CHIP_VERSION)))
rtl9071_core-y += rtl907xc_vb_reg_list.c rtl907xc_vb_table_list.c rtl907xc_vb_asicdrv_mem_ctrl.c rtl907xc_vb_asicdrv.c rtl907xc_vb_asicdrv_ethernet_access.c rtl907xc_vb_api_ethernet_access.c
endif
ifneq (,$(findstring cva, $(RTL906X_TARGET_CHIP_VERSION)))
rtl9071_core-y += rtl907xc_va_reg_list.c rtl907xc_va_table_list.c rtl907xc_va_asicdrv_mem_ctrl.c rtl907xc_va_asicdrv.c rtl907xc_va_asicdrv_ethernet_access.c rtl907xc_va_api_ethernet_access.c
endif

##############################################################################
## 保留：开发模式/模块配置（按需精简）
##############################################################################
ifeq ($(RTK_DEV), 1)
CFLAGS += -DRTL_907X_D_DEV -DRTL_907X_C_DEV -DEN_DBG_REG
endif
ifeq ($(RTL906X_ASICDRV_FOR_INTERNAL), 1)
CFLAGS += -DRTL_907X_ASICDRV_FOR_INTERNAL
endif

# 模块配置（保留原有逻辑，按需启用）
-include makefile.modules
ifeq ($(ESM_ENABLE), 1)
RTL906X_MODULES += reset opfsm
endif
ifneq (,$(findstring all, $(TARGET_CHIP_VERSION)))
-include makefile.rtl907x_all_modules
-include makefile.rtl907xc_va_modules_src
-include makefile.rtl907xc_vb_modules_src
-include makefile.rtl907xd_v1_modules_src
-include makefile.rtl907xd_va_modules_src
else
ifneq (,$(findstring cvb, $(RTL906X_TARGET_CHIP_VERSION)))
-include makefile.rtl907xc_vb_modules
-include makefile.rtl907xc_vb_modules_src
endif
ifneq (,$(findstring cva, $(RTL906X_TARGET_CHIP_VERSION)))
-include makefile.rtl907xc_va_modules
-include makefile.rtl907xc_va_modules_src
endif
ifneq (,$(findstring dv1, $(RTL906X_TARGET_CHIP_VERSION)))
-include makefile.rtl907xd_v1_modules
-include makefile.rtl907xd_v1_modules_src
endif
ifneq (,$(findstring dva, $(RTL906X_TARGET_CHIP_VERSION)))
-include makefile.rtl907xd_va_modules
-include makefile.rtl907xd_va_modules_src
endif
endif

# 示例代码（默认关闭，SO不需要，如需调试可开启RTK_EXAMPLE_CODE=1）
ifeq ($(RTK_EXAMPLE_CODE), 1)
rtl9071_core-y += system_porting.c system_register_interface.c system_register_interface_pcie.c system_register_interface_ethernet_access.c
ifneq (,$(findstring dv1, $(RTL906X_TARGET_CHIP_VERSION)))
-include makefile.rtl907xd_v1_examples
endif
ifneq (,$(findstring dva, $(RTL906X_TARGET_CHIP_VERSION)))
-include makefile.rtl907xd_va_examples
endif
ifneq (,$(findstring cvb, $(RTL906X_TARGET_CHIP_VERSION)))
-include makefile.rtl907xc_vb_examples
endif
ifneq (,$(findstring cva, $(RTL906X_TARGET_CHIP_VERSION)))
-include makefile.rtl907xc_va_examples
endif
endif

##############################################################################
## 适配：传递编译参数到Makefile.build（无需修改）
##############################################################################
export CFLAGS cflags-y SOURCE_DIR TARGET_CHIP_VERSION ESM_ENABLE
