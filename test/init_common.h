#ifndef __INIT_COMMON_H__
#define __INIT_COMMON_H__

#include <stdio.h>
#include <stddef.h>

typedef int (*user_initcall_t)(void);
typedef void (*user_exitcall_t)(void);

// 增强宏：加aligned(8)确保section地址对齐，强制保留变量
#define my_module_init(initfn) \
    static inline user_initcall_t __check_##initfn(void) { return initfn; } \
    static user_initcall_t __init_##initfn __attribute__((section(".my_initcall"), used, aligned(8))) = initfn;

#define my_module_exit(exitfn) \
    static inline user_exitcall_t __check_##exitfn(void) { return exitfn; } \
    static user_exitcall_t __exit_##exitfn __attribute__((section(".my_exitcall"), used, aligned(8))) = exitfn;

#ifdef __cplusplus
extern "C" {
#endif
int module_manager_init(void);
void module_manager_exit(void);
#ifdef __cplusplus
}
#endif

#endif // __INIT_COMMON_H__
