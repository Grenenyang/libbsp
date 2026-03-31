#ifndef __CORE_H__
#define __CORE_H__
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stddef.h>
#include <errno.h>
#include <stdint.h>

#define DENUG 1
// 调试宏
#ifdef DEBUG
#define HW_DEBUG(fmt, ...) printf("[CORE][DEBUG] " fmt, ##__VA_ARGS__)
#define HW_WARN(fmt, ...)  printf("[CORE][WARN] " fmt, ##__VA_ARGS__)
#else
#define HW_DEBUG(fmt, ...)
#define HW_WARN(fmt, ...)
#endif
#define HW_INFO(fmt, ...)  printf("[CORE][INFO] " fmt, ##__VA_ARGS__)
#define HW_ERR(fmt, ...)   printf("[CORE][ERROR] " fmt, ##__VA_ARGS__)
#define HW_WARN(fmt, ...)  printf("[CORE][WARN] " fmt, ##__VA_ARGS__)



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


typedef struct hw_driver {
    char *name;
    int (*probe)(struct hw_device *dev);
    int (*remove)(struct hw_device *dev);
    void *ops;
    hw_list_node_t node;
} hw_driver_t;

typedef struct hw_device {
    char *name;
    char *compatible;
    struct hw_driver *drv;
    struct hw_class *cls;
    cJSON *root;
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
// ====================== 内部配置（不对外暴露） ======================
#define BSP_MAX_DEVICE_ID    1023    // 最大设备ID（内部限定）
#define BSP_INVALID_HANDLE   ((handle_t)-1)  // 无效句柄（内部使用）
#define BSP_NULL_HANDLE      ((handle_t)NULL) // 空句柄（内部使用）

// ====================== 内部核心宏（不对外暴露） ======================
/**
 * @brief 内部：判断handle_t是否为整数ID形态
 * @param h 输入的handle_t句柄
 * @return 1：是ID；0：是指针/无效句柄
 */
#define __IS_HANDLE(h)                                                    \
    ({                                                                   \
        handle_t _h = (h);                                               \
        (_h >= 0 && _h <= BSP_MAX_DEVICE_ID) ? 1 : 0;                    \
    })

/**
 * @brief 内部：将handle_t（指针形态）转为device指针
 * @param h 输入的handle_t句柄
 * @return 设备指针；非指针形态返回NULL
 */
#define __HANDLE_TO_DEV(h)                                                \
    ({                                                                   \
        struct device* _dev = NULL;                                      \
        handle_t _h = (h);                                               \
        if (_h != BSP_INVALID_HANDLE && _h != BSP_NULL_HANDLE && !__IS_HANDLE(_h)) { \
            _dev = (struct hw_device*)(_h);                                 \
        }                                                                \
        _dev;                                                            \
    })


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

int _bsp_device_get(struct hw_class *cls,struct hw_device **out_dev , int id);
#endif // __CORE_H__
