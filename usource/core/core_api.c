// 先包含系统核心头文件（解决size_t/stddef.h依赖）
#include <stddef.h>    // 必须：定义size_t
#include <stdlib.h>    // 辅助：标准库
#include <stdio.h>     // 标准IO
#include <autoconf.h>  // 配置头文件

// 公共函数实现
void hw_common_init(void) {
    #ifdef CONFIG_USR_CORE
    printf("User-space core initialized (MT8678 HW_VERSION=%s)!\n", CONFIG_HW_VERSION);
    #endif
}
