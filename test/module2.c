#include "init_common.h"

// 模块2初始化函数
static int module2_init(void) {
    printf("[Module2] 初始化成功（module2.so）\n");
    return 0;
}

// 模块2退出函数
static void module2_exit(void) {
    printf("[Module2] 退出完成（module2.so）\n");
}

// 注册到section
my_module_init(module2_init);
my_module_exit(module2_exit);

// 可选：业务接口
__attribute__((visibility("default")))
void module2_do_work(void) {
    printf("[Module2] 执行业务逻辑\n");
}
