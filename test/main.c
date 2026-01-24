#include <dlfcn.h>
#include <stdio.h>

// 仅声明模块管理器的接口（无需包含完整头文件，降低耦合）
typedef int (*module_manager_init_t)(void);
typedef void (*module_manager_exit_t)(void);

int main(int argc, char *argv[]) {
    // 1. 加载所有需要的so（先加载业务so，再加载管理器so）
    void *mod1_handle = dlopen("./module1.so", RTLD_NOW | RTLD_GLOBAL);
    void *mod2_handle = dlopen("./module2.so", RTLD_NOW | RTLD_GLOBAL);
    void *manager_handle = dlopen("./module_manager.so", RTLD_NOW | RTLD_GLOBAL);

    if (!mod1_handle || !mod2_handle || !manager_handle) {
        fprintf(stderr, "加载SO失败: %s\n", dlerror());
        return -1;
    }

    // 2. 获取模块管理器的导出接口
    module_manager_init_t do_init = (module_manager_init_t)dlsym(manager_handle, "module_manager_init");
    module_manager_exit_t do_exit = (module_manager_exit_t)dlsym(manager_handle, "module_manager_exit");

    if (!do_init || !do_exit) {
        fprintf(stderr, "获取管理器接口失败: %s\n", dlerror());
        goto clean;
    }

    // 3. 调用管理器接口，触发所有模块初始化（核心调用）
    int ret = do_init();
    if (ret != 0) {
        fprintf(stderr, "模块初始化失败，退出程序\n");
        goto clean;
    }

    // 4. 调用业务模块接口（示例）
    void (*mod1_work)(void) = (void (*)(void))dlsym(mod1_handle, "module1_do_work");
    void (*mod2_work)(void) = (void (*)(void))dlsym(mod2_handle, "module2_do_work");
    if (mod1_work) mod1_work();
    if (mod2_work) mod2_work();

    // 5. 主程序业务逻辑
    printf("\n===== 主程序核心逻辑运行中 =====\n");

    // 6. 调用管理器接口，触发所有模块退出
    do_exit();

clean:
    // 7. 卸载所有so
    dlclose(manager_handle);
    dlclose(mod2_handle);
    dlclose(mod1_handle);
    return ret;
}
