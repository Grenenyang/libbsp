# 自动获取当前目录所有 .c 文件
BSP_ETH_TOOL_SOURCES   = $(wildcard ./*.c)

# 生成 .o 目标路径
BSP_ETH_TOOL_OBJS      = $(BSP_ETH_TOOL_SOURCES:./%.c=$(PRJ_BUILD_ROOT)/tools/bsp_eth_tool/%.o)

# 编译参数（头文件路径）
BSP_ETH_TOOL_CFLAGS    = -I$(PRJ_BUILD_ROOT)/include

# 链接参数（库路径 + 链接库）
BSP_ETH_TOOL_LDFLAGS   = -L$(PRJ_BUILD_ROOT) -lbsp -lm -pthread

# 生成最终工具
$(PRJ_BUILD_ROOT)/bsp_eth_tool: $(BSP_ETH_TOOL_OBJS)
	$(CC) -o $@ $^ $(BSP_ETH_TOOL_LDFLAGS)
	@chmod 777 $@

# 正确的 .c -> .o 规则
$(PRJ_BUILD_ROOT)/tools/bsp_eth_tool/%.o: ./%.c
	@mkdir -p $(dir $@)
	$(CC) $(BSP_ETH_TOOL_CFLAGS) -c $< -o $@
