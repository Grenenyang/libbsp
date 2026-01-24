#include "init_common.h"

// 模块1初始化函数
static int module1_init(void) {
    printf("[Module1] 初始化成功（module1.so）\n");
    return 0;
}

// 模块1退出函数
static void module1_exit(void) {
    printf("[Module1] 退出完成（module1.so）\n");
}

// 注册到section（仅需调用宏，无需关心如何遍历）
my_module_init(module1_init);
my_module_exit(module1_exit);

// 可选：业务接口（示例）
__attribute__((visibility("default")))
void module1_do_work(void) {
    printf("[Module1] 执行业务逻辑\n");
}
