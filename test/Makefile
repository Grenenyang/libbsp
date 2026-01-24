# 编译器与选项
CC = gcc
CFLAGS = -fPIC -Wall -g -D_GNU_SOURCE
LDFLAGS = -shared -ldl -lelf  # 加-lelf：链接ELF解析库

# 目标：管理器so + 业务so + 主程序
TARGETS = module_manager.so module1.so module2.so main

# 编译所有目标
all: $(TARGETS)

# 核心：模块管理器so（包含遍历逻辑）
module_manager.so: module_manager.c init_common.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

# 业务模块1
module1.so: module1.c init_common.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

# 业务模块2
module2.so: module2.c init_common.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

# 主程序（仅链接dl库，运行时加载so）
main: main.c
	$(CC) $(CFLAGS) -o $@ $< -ldl

# 清理
clean:
	rm -f $(TARGETS) *.o core*  # 清理core dump文件
