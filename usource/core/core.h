#ifndef __CORE_H__
#define __CORE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stddef.h>
#include <errno.h>
#include <dlfcn.h>
#include <link.h>

// ====================== 1. 通用基础定义 ======================
#define HW_OK          0
#define HW_ERR_INVAL   -1
#define HW_ERR_NOENT   -2
#define HW_ERR_EXIST   -3
#define HW_ERR_JSON    -4
#define HW_ERR_LOCK    -5
#define HW_ERR_MATCH   -6
#define HW_ERR_SECTION -7

// 调试宏
#ifdef RTL_907X_D_DEV
#define HW_DEBUG(fmt, ...) printf("[CORE][DEBUG] " fmt, ##__VA_ARGS__)
#define HW_WARN(fmt, ...)  printf("[CORE][WARN] " fmt, ##__VA_ARGS__)
#else
#define HW_DEBUG(fmt, ...)
#define HW_WARN(fmt, ...)
#endif
#define HW_INFO(fmt, ...)  printf("[CORE][INFO] " fmt, ##__VA_ARGS__)
#define HW_ERR(fmt, ...)   printf("[CORE][ERROR] " fmt, ##__VA_ARGS__)

// ====================== 2. 通用链表操作（修复初始化逻辑） ======================
typedef struct hw_list_node {
    struct hw_list_node *next;
    struct hw_list_node *prev;
} hw_list_node_t;

// 修复：链表初始化函数（替代原宏，避免语句执行错误）
static inline void hw_list_head_init(hw_list_node_t *head) {
    if (head) {
        head->next = head;
        head->prev = head;
    }
}

#define HW_LIST_HEAD(name) \
    hw_list_node_t name = {&(name), &(name)}

// 头插法添加节点
static inline void hw_list_add(hw_list_node_t *new_node, hw_list_node_t *head) {
    if (new_node == NULL || head == NULL) return;
    new_node->next = head->next;
    new_node->prev = head;
    head->next->prev = new_node;
    head->next = new_node;
}

// 尾插法添加节点
static inline void hw_list_add_tail(hw_list_node_t *new_node, hw_list_node_t *head) {
    if (new_node == NULL || head == NULL) return;
    new_node->prev = head->prev;
    new_node->next = head;
    head->prev->next = new_node;
    head->prev = new_node;
}

// 删除节点
static inline void hw_list_del(hw_list_node_t *node) {
    if (node == NULL) return;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = node;
    node->prev = node;
}

// 判断链表是否为空
static inline int hw_list_empty(const hw_list_node_t *head) {
    return (head != NULL) && (head->next == head);
}

// ====================== 3. container_of 宏（修复const警告） ======================
#define container_of(ptr, type, member) ({          \
    typeof(((type *)0)->member) *__mptr = (typeof(((type *)0)->member) *)(ptr); \
    (type *)((char *)__mptr - offsetof(type, member)); \
})

#define hw_list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define hw_list_for_each_entry(pos, head, member)                \
    for (pos = container_of((head)->next, typeof(*pos), member);  \
         &pos->member != (head);                                 \
         pos = container_of(pos->member.next, typeof(*pos), member))

#define hw_list_for_each_entry_safe(pos, n, head, member)          \
    for (pos = container_of((head)->next, typeof(*pos), member),    \
         n = container_of(pos->member.next, typeof(*pos), member); \
         &pos->member != (head);                                    \
         pos = n, n = container_of(n->member.next, typeof(*n), member))

// ====================== 4. Class/Device/Driver 核心结构体 ======================
struct hw_device;
struct hw_class;

typedef struct hw_driver_ops {
    int (*probe)(struct hw_device *dev);
    void (*remove)(struct hw_device *dev);
    int (*read_reg)(struct hw_device *dev, unsigned int reg, unsigned int *val);
    int (*write_reg)(struct hw_device *dev, unsigned int reg, unsigned int val);
} hw_driver_ops_t;

typedef struct hw_driver {
    char *name;
    char *compatible;
    hw_driver_ops_t *ops;
    void *priv;
    hw_list_node_t node;
} hw_driver_t;

typedef struct hw_device {
    char *name;
    char *compatible;
    unsigned int reg_base;
    unsigned int irq;
    struct hw_driver *drv;
    struct hw_class *cls;
    void *priv;
    hw_list_node_t node;
} hw_device_t;

typedef struct hw_class {
    char *name;
    hw_list_node_t drv_list;
    hw_list_node_t dev_list;
    pthread_mutex_t lock;
    int dev_count;
    int drv_count;
    void *priv;
} hw_class_t;

// ====================== 5. 驱动模块管理（修复Section宏定义） ======================
typedef struct hw_driver_module {
    const char *name;
    int (*init)(void);
    void (*exit)(void);
    const char *description;
    const char *version;
    struct hw_driver_module *next;
} hw_driver_module_t;

// 修复：Section名直接用字符串，避免宏拼接错误
#define HW_DRV_MODULE_SECTION_STR ".hw_drv_modules"
// 链接器自动生成的Section起止符号（关键修复：3个下划线）
#define HW_DRV_MODULE_START_SYM __start___hw_drv_modules
#define HW_DRV_MODULE_STOP_SYM  __stop___hw_drv_modules

// 修复：module_init宏（直接指定Section字符串，无宏拼接）
#define module_init(init_func)                                                                 \
    static struct hw_driver_module __mod_##init_func __attribute__((section(HW_DRV_MODULE_SECTION_STR), aligned(8), used)) = { \
        .name = #init_func,                                                                    \
        .init = init_func,                                                                     \
        .exit = NULL,                                                                          \
        .description = "RTL907X " #init_func " driver module",                                \
        .version = "1.0",                                                                      \
    };

#define module_exit(exit_func)                                                                 \
    static void __exit_##exit_func(void) {                                                     \
        if (exit_func) exit_func();                                                            \
    }

#define MODULE_DESCRIPTION(desc) static const char *__mod_desc = desc;
#define MODULE_VERSION(ver) static const char *__mod_version = ver;

// 全局模块链表
extern struct hw_driver_module *hw_module_list;
extern pthread_mutex_t hw_module_lock;

// ====================== 6. 核心接口声明 ======================
// Class相关
int hw_class_init(hw_class_t *cls, const char *name);
void hw_class_deinit(hw_class_t *cls);
int hw_class_get_drv_count(hw_class_t *cls);
int hw_class_get_dev_count(hw_class_t *cls);

// Driver相关
int hw_driver_register(hw_class_t *cls, hw_driver_t *drv);
void hw_driver_unregister(hw_class_t *cls, hw_driver_t *drv);
void hw_driver_unregister_all_from_class(hw_class_t *cls);

// Device相关
int hw_device_add(hw_class_t *cls, hw_device_t *dev);
void hw_device_remove(hw_class_t *cls, hw_device_t *dev);
int hw_device_driver_match(hw_class_t *cls);
void hw_device_driver_unbind_all(hw_class_t *cls);

// Section相关核心接口
int hw_module_scan_section(void);
void hw_module_register(hw_driver_module_t *mod);
int hw_core_init_all_modules(void);
void hw_core_exit_all_modules(void);

#ifndef __init
#define __init __attribute__((unused))
#endif
#endif // __CORE_H__
