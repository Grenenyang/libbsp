#include "core.h"

// ====================== 全局变量 ======================
struct hw_driver_module *hw_module_list = NULL;
pthread_mutex_t hw_module_lock = PTHREAD_MUTEX_INITIALIZER;
// ====================== Class 接口实现（修复链表初始化） ======================
int hw_class_init(hw_class_t *cls, const char *name) {
    if (cls == NULL || name == NULL) {
        HW_ERR("hw_class_init: invalid param (cls=%p, name=%s)\n", cls, name);
        return HW_ERR_INVAL;
    }

    // 初始化基础字段
    memset(cls, 0, sizeof(hw_class_t));
    cls->name = strdup(name);
    if (cls->name == NULL) {
        HW_ERR("hw_class_init: strdup failed (errno=%d)\n", errno);
        return HW_ERR_INVAL;
    }

    // 修复：使用函数初始化链表（替代原宏）
    hw_list_head_init(&cls->drv_list);
    hw_list_head_init(&cls->dev_list);

    // 初始化锁
    if (pthread_mutex_init(&cls->lock, NULL) != 0) {
        HW_ERR("hw_class_init: mutex init failed (errno=%d)\n", errno);
        free(cls->name);
        return HW_ERR_LOCK;
    }

    cls->dev_count = 0;
    cls->drv_count = 0;

    HW_INFO("Class %s init success\n", name);
    return HW_OK;
}

void hw_class_deinit(hw_class_t *cls) {
    if (cls == NULL) return;

    pthread_mutex_lock(&cls->lock);

    // 注销所有驱动
    hw_driver_unregister_all_from_class(cls);
    // 解绑所有设备
    hw_device_driver_unbind_all(cls);

    // 释放名称
    if (cls->name) {
        free(cls->name);
        cls->name = NULL;
    }

    // 销毁锁
    pthread_mutex_destroy(&cls->lock);

    HW_INFO("Class %s deinit success\n", cls->name);
}

int hw_class_get_drv_count(hw_class_t *cls) {
    return (cls != NULL) ? cls->drv_count : 0;
}

int hw_class_get_dev_count(hw_class_t *cls) {
    return (cls != NULL) ? cls->dev_count : 0;
}

// ====================== Driver 接口实现 ======================
int hw_driver_register(hw_class_t *cls, hw_driver_t *drv) {
    if (cls == NULL || drv == NULL || drv->name == NULL) {
        HW_ERR("hw_driver_register: invalid param (cls=%p, drv=%p)\n", cls, drv);
        return HW_ERR_INVAL;
    }

    pthread_mutex_lock(&cls->lock);

    // 检查是否已注册
    hw_driver_t *tmp_drv;
    hw_list_for_each_entry(tmp_drv, &cls->drv_list, node) {
        if (strcmp(tmp_drv->name, drv->name) == 0) {
            HW_ERR("hw_driver_register: driver %s already exist in class %s\n",
                   drv->name, cls->name);
            pthread_mutex_unlock(&cls->lock);
            return HW_ERR_EXIST;
        }
    }

    // 初始化驱动节点，尾插法添加到链表
    hw_list_head_init(&drv->node);
    hw_list_add_tail(&drv->node, &cls->drv_list);
    cls->drv_count++;

    pthread_mutex_unlock(&cls->lock);

    HW_INFO("Driver %s register to class %s success (total drv: %d)\n",
            drv->name, cls->name, cls->drv_count);
    return HW_OK;
}

void hw_driver_unregister(hw_class_t *cls, hw_driver_t *drv) {
    if (cls == NULL || drv == NULL) return;

    pthread_mutex_lock(&cls->lock);

    // 从链表删除
    hw_list_del(&drv->node);
    cls->drv_count--;

    // 解绑该驱动绑定的所有设备
    hw_device_t *dev;
    hw_list_for_each_entry(dev, &cls->dev_list, node) {
        if (dev->drv == drv) {
            if (dev->drv->ops && dev->drv->ops->remove) {
                dev->drv->ops->remove(dev);
            }
            dev->drv = NULL;
        }
    }

    pthread_mutex_unlock(&cls->lock);

    HW_INFO("Driver %s unregister from class %s success\n", drv->name, cls->name);
}

void hw_driver_unregister_all_from_class(hw_class_t *cls) {
    if (cls == NULL) return;

    pthread_mutex_lock(&cls->lock);

    hw_driver_t *drv, *tmp;
    hw_list_for_each_entry_safe(drv, tmp, &cls->drv_list, node) {
        hw_driver_unregister(cls, drv);
    }

    cls->drv_count = 0;
    pthread_mutex_unlock(&cls->lock);
}

// ====================== Device 接口实现 ======================
int hw_device_add(hw_class_t *cls, hw_device_t *dev) {
    if (cls == NULL || dev == NULL || dev->name == NULL) {
        HW_ERR("hw_device_add: invalid param (cls=%p, dev=%p)\n", cls, dev);
        return HW_ERR_INVAL;
    }

    pthread_mutex_lock(&cls->lock);

    // 检查设备是否已存在
    hw_device_t *tmp_dev;
    hw_list_for_each_entry(tmp_dev, &cls->dev_list, node) {
        if (strcmp(tmp_dev->name, dev->name) == 0) {
            HW_ERR("hw_device_add: device %s already exist in class %s\n",
                   dev->name, cls->name);
            pthread_mutex_unlock(&cls->lock);
            return HW_ERR_EXIST;
        }
    }

    // 初始化设备节点，尾插法添加
    dev->cls = cls;
    dev->drv = NULL;
    hw_list_head_init(&dev->node);
    hw_list_add_tail(&dev->node, &cls->dev_list);
    cls->dev_count++;

    pthread_mutex_unlock(&cls->lock);

    HW_INFO("Device %s add to class %s success (total dev: %d)\n",
            dev->name, cls->name, cls->dev_count);
    return HW_OK;
}

void hw_device_remove(hw_class_t *cls, hw_device_t *dev) {
    if (cls == NULL || dev == NULL) return;

    pthread_mutex_lock(&cls->lock);

    // 解绑驱动
    if (dev->drv) {
        if (dev->drv->ops && dev->drv->ops->remove) {
            dev->drv->ops->remove(dev);
        }
        dev->drv = NULL;
    }

    // 从链表删除
    hw_list_del(&dev->node);
    cls->dev_count--;

    pthread_mutex_unlock(&cls->lock);

    HW_INFO("Device %s remove from class %s success\n", dev->name, cls->name);
}

int hw_device_driver_match(hw_class_t *cls) {
    if (cls == NULL) {
        HW_ERR("hw_device_driver_match: invalid class (cls=%p)\n", cls);
        return HW_ERR_INVAL;
    }

    pthread_mutex_lock(&cls->lock);

    int match_count = 0;
    hw_device_t *dev;
    hw_driver_t *drv;

    // 遍历所有设备，匹配兼容的驱动
    hw_list_for_each_entry(dev, &cls->dev_list, node) {
        if (dev->compatible == NULL) {
            HW_WARN("Device %s has no compatible field, skip match\n", dev->name);
            continue;
        }

        // 遍历所有驱动，匹配compatible
        hw_list_for_each_entry(drv, &cls->drv_list, node) {
            if (drv->compatible && strcmp(dev->compatible, drv->compatible) == 0) {
                dev->drv = drv;
                match_count++;

                // 执行probe初始化
                if (drv->ops && drv->ops->probe) {
                    int ret = drv->ops->probe(dev);
                    if (ret != HW_OK) {
                        HW_ERR("Device %s probe failed (driver: %s, ret=%d)\n",
                               dev->name, drv->name, ret);
                        dev->drv = NULL;
                        match_count--;
                    } else {
                        HW_INFO("Device %s match driver %s success (compatible: %s)\n",
                                dev->name, drv->name, dev->compatible);
                    }
                }
                break; // 匹配到第一个驱动即退出
            }
        }

        if (!dev->drv) {
            HW_WARN("Device %s (compatible: %s) no matched driver in class %s\n",
                    dev->name, dev->compatible, cls->name);
        }
    }

    pthread_mutex_unlock(&cls->lock);

    if (match_count == 0) {
        HW_ERR("No device-driver match in class %s\n", cls->name);
        return HW_ERR_MATCH;
    }

    HW_INFO("Class %s device-driver match success (matched: %d/%d)\n",
            cls->name, match_count, cls->dev_count);
    return HW_OK;
}

void hw_device_driver_unbind_all(hw_class_t *cls) {
    if (cls == NULL) return;

    pthread_mutex_lock(&cls->lock);

    hw_device_t *dev;
    hw_list_for_each_entry(dev, &cls->dev_list, node) {
        if (dev->drv) {
            if (dev->drv->ops && dev->drv->ops->remove) {
                dev->drv->ops->remove(dev);
            }
            dev->drv = NULL;
            HW_INFO("Device %s unbind driver success\n", dev->name);
        }
    }

    pthread_mutex_unlock(&cls->lock);
}

// ====================== Section版模块管理核心实现） ======================

#include <elf.h>
#include <link.h>

// 新增：ELF Section遍历回调函数（获取.hw_drv_modules Section的地址）
static int find_hw_drv_section(struct dl_phdr_info *info, size_t size, void *data) {
    (void)size;
    void **section_info = (void **)data;
    void *start = NULL;
    void *end = NULL;

    // 遍历ELF Program Header
    for (int i = 0; i < info->dlpi_phnum; i++) {
        const Elf64_Phdr *phdr = &info->dlpi_phdr[i];
        if (phdr->p_type != PT_LOAD) continue;

        // 打开SO文件，读取Section头
        FILE *fp = fopen(info->dlpi_name, "rb");
        if (!fp) continue;

        Elf64_Ehdr ehdr;
        fread(&ehdr, sizeof(ehdr), 1, fp);
        fseek(fp, ehdr.e_shoff, SEEK_SET);

        Elf64_Shdr shdr;
        for (int j = 0; j < ehdr.e_shnum; j++) {
            fread(&shdr, sizeof(shdr), 1, fp);
            // 查找.hw_drv_modules Section
            if (shdr.sh_type == SHT_PROGBITS) {
                fseek(fp, ehdr.e_shoff + ehdr.e_shentsize * j + shdr.sh_name, SEEK_SET);
                char sh_name[256] = {0};
                fread(sh_name, 1, sizeof(sh_name)-1, fp);
                if (strcmp(sh_name, ".hw_drv_modules") == 0) {
                    // 计算Section的实际内存地址
                    start = (void *)(info->dlpi_addr + shdr.sh_addr);
                    end = (void *)((uint8_t *)start + shdr.sh_size);
                    break;
                }
            }
        }
        fclose(fp);
        if (start) break;
    }

    if (start && end) {
        section_info[0] = start;
        section_info[1] = end;
        return 1; // 找到后停止遍历
    }
    return 0;
}

// 重写：扫描.hw_drv_modules Section（不依赖__start/__stop符号）
int hw_module_scan_section(void) {
    HW_INFO("Start scan .hw_drv_modules section (ELF parse mode)...\n");

    // 解析ELF，获取Section起止地址
    void *section_info[2] = {NULL, NULL};
    dl_iterate_phdr(find_hw_drv_section, section_info);
    
    if (!section_info[0] || !section_info[1]) {
        HW_ERR("Failed to find .hw_drv_modules section\n");
        return HW_ERR_SECTION;
    }

    struct hw_driver_module *mod = (struct hw_driver_module *)section_info[0];
    struct hw_driver_module *stop = (struct hw_driver_module *)section_info[1];
    int scan_count = 0;

    // 遍历Section内的驱动模块
    for (; mod < stop; mod++) {
        if (mod->name == NULL) {
            HW_WARN("Skip invalid module (name is NULL)\n");
            continue;
        }
        hw_module_register(mod);
        scan_count++;
        HW_DEBUG("Scan module: %s (desc: %s, ver: %s)\n",
                 mod->name, mod->description ? mod->description : "none",
                 mod->version ? mod->version : "none");
    }

    if (scan_count == 0) {
        HW_WARN("No module found in .hw_drv_modules section\n");
        return HW_ERR_SECTION;
    }

    HW_INFO("Scan section success (found %d modules)\n", scan_count);
    return HW_OK;
}

void hw_module_register(hw_driver_module_t *mod) {
    if (mod == NULL || mod->name == NULL) {
        HW_ERR("hw_module_register: invalid module (mod=%p)\n", mod);
        return;
    }

    pthread_mutex_lock(&hw_module_lock);

    // 头插法加入全局链表
    mod->next = hw_module_list;
    hw_module_list = mod;

    pthread_mutex_unlock(&hw_module_lock);

    HW_INFO("Module %s register success [Section]\n", mod->name);
}

int hw_core_init_all_modules(void) {
    HW_INFO("Start init all driver modules...\n");

    pthread_mutex_lock(&hw_module_lock);
    struct hw_driver_module *mod = hw_module_list;
    int ret = HW_OK;
    int init_count = 0;

    while (mod) {
        if (mod->init) {
            ret = mod->init();
            if (ret != HW_OK) {
                HW_ERR("Module %s init failed (ret=%d)\n", mod->name, ret);
                pthread_mutex_unlock(&hw_module_lock);
                return ret;
            }
            init_count++;
            HW_INFO("Module %s init success\n", mod->name);
        }
        mod = mod->next;
    }

    pthread_mutex_unlock(&hw_module_lock);

    HW_INFO("All modules init success (total: %d)\n", init_count);
    return HW_OK;
}

void hw_core_exit_all_modules(void) {
    HW_INFO("Start exit all driver modules...\n");

    pthread_mutex_lock(&hw_module_lock);
    struct hw_driver_module *mod = hw_module_list;
    int exit_count = 0;

    while (mod) {
        if (mod->exit) {
            mod->exit();
            exit_count++;
            HW_INFO("Module %s exit success\n", mod->name);
        }
        mod = mod->next;
    }

    pthread_mutex_unlock(&hw_module_lock);
    HW_INFO("All modules exit success (total: %d)\n", exit_count);
}
struct hw_class g_class_switch;
