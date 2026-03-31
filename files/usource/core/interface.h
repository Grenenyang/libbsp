#ifndef __BSP_INTERFACE_H_
#define __BSP_INTERFACE_H_
#define BSP_INTERFACE_UNLOCKED(cls, op, handle, ...)                     \
    ({                                                                   \
        int ret = -1;                                                    \
        int id = 0;                                                      \
        struct hw_device* dev = NULL;                                    \
        cls##_ops_t *ops = NULL;                                         \
        handle_t h = (handle);                                         \
        if (__IS_HANDLE(h)) {                                         \
            id = (int)h;                                          \
            _bsp_device_get(&class_##cls,&dev,id);                        \
        } else {                                                         \
            dev = __HANDLE_TO_DEV(_h);              \
        }                                           \
        if (NULL == dev) {                                            \
            HW_ERR("id=%d, device = NULL\n", id);                 \
            ret = -1;                                          \
        } else if (NULL == dev->drv) {                             \
            HW_ERR("id=%d, device->driver == NULL\n", id);        \
            ret = -2;                                          \
        } else if (NULL == (ops = dev->drv->ops)) {                \
            HW_ERR("id=%d, device->driver->ops == NULL\n", id);   \
            ret = -3;                                             \
        } else if (NULL == ops->op) {                                    \
            HW_ERR("id=%d, device->driver->ops->op == NULL\n", id);\
            ret = -4;                                             \
        } else {                                                         \
            ret = ops->op(dev, ##__VA_ARGS__);                        \
        }                                                                \
        ret;                                                             \
    })

#endif
