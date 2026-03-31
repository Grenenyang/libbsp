#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include "core.h"
#include "../switch/rtl9071.h"

// ========== 1. 核心驱动全局变量 ==========
static dev_t bsp_io_devno;        // 字符设备号
static struct cdev bsp_io_cdev;   // 字符设备结构
static struct class *bsp_io_class;// 设备类
static struct device *bsp_io_dev; // 设备节点（/dev/bsp_io_ioctl）

// ========== 2. 函数指针类型定义 ==========
typedef long (*ioctl_handler_t)(bsp_io_ioctl_data_t *data);

// ========== 4. 核心：二级函数指针数组（外设ID + 命令ID） ==========
static ioctl_handler_t g_ioctl_handler[BSP_PERIPH_MAX][SWITCH_CMD_MAX] = {
    [BSP_PERIPH_SWITCH] = {
        [RTL_CMD_REG_GET]       = rtl9071_reg_get,
        [RTL_CMD_REG_SET]       = rtl9071_reg_set,
    },
};
static int bsp_io_open(struct inode *inode, struct file *filp)
{
    return 0;
}

static int bsp_io_release(struct inode *inode, struct file *filp)
{
    return 0;
}
// ========== 5. 真正的 ioctl：直接查表调用 ==========
static long bsp_io_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    bsp_io_ioctl_data_t kernel_data;
    ioctl_handler_t handler;

    // 1. 命令校验
    if (_IOC_TYPE(cmd) != BSP_IO_MAGIC ||
        _IOC_NR(cmd) != 0 ||
        _IOC_SIZE(cmd) != sizeof(bsp_io_ioctl_data_t)) {
        printk(KERN_ERR "[BSP_IO] Invalid ioctl command\n");
        return -ENOTTY;
    }

    // 2. 拷贝用户态数据
    if (copy_from_user(&kernel_data, (void __user *)arg, sizeof(kernel_data))) {
        printk(KERN_ERR "[BSP_IO] Copy from user failed\n");
        return -EFAULT;
    }

    kernel_data.ret = -EINVAL;

    // 3. 边界检查
    if (kernel_data.periph_id >= BSP_PERIPH_MAX)
        goto out;

    if (kernel_data.cmd >= SWITCH_CMD_MAX)
        goto out;

    // 4. 查表 + 直接调用（你要的核心功能！）
    handler = g_ioctl_handler[kernel_data.periph_id][kernel_data.cmd];
    if (handler)
        handler(&kernel_data);

out:
    // 5. 回写结果
    if (copy_to_user((void __user *)arg, &kernel_data, sizeof(kernel_data))) {
        printk(KERN_ERR "[BSP_IO] Copy to user failed\n");
        return -EFAULT;
    }

    return 0;
}

// ========== 6. 文件操作集 ==========
static const struct file_operations bsp_io_fops = {
    .owner = THIS_MODULE,
    .open = bsp_io_open,
    .release = bsp_io_release,
    .unlocked_ioctl = bsp_io_unlocked_ioctl,
    .compat_ioctl = bsp_io_unlocked_ioctl,
};
// ========== 7. 驱动初始化 / 退出 ==========
static int __init bsp_io_ioctl_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&bsp_io_devno, 0, 1, "bsp_io_ioctl");
    if (ret < 0) {
        printk(KERN_ERR "[BSP_IO] Alloc chrdev region failed\n");
        goto err_alloc;
    }

    cdev_init(&bsp_io_cdev, &bsp_io_fops);
    bsp_io_cdev.owner = THIS_MODULE;
    ret = cdev_add(&bsp_io_cdev, bsp_io_devno, 1);
    if (ret < 0) {
        printk(KERN_ERR "[BSP_IO] Cdev add failed\n");
        goto err_cdev;
    }

    bsp_io_class = class_create(THIS_MODULE, "bsp_io_ioctl_class");
    if (IS_ERR(bsp_io_class)) {
        ret = PTR_ERR(bsp_io_class);
        goto err_class;
    }

    bsp_io_dev = device_create(bsp_io_class, NULL, bsp_io_devno, NULL, "bsp_io_ioctl");
    if (IS_ERR(bsp_io_dev)) {
        ret = PTR_ERR(bsp_io_dev);
        goto err_device;
    }

    printk(KERN_INFO "[BSP_IO] Init success\n");
    return 0;

err_device:
    class_destroy(bsp_io_class);
err_class:
    cdev_del(&bsp_io_cdev);
err_cdev:
    unregister_chrdev_region(bsp_io_devno, 1);
err_alloc:
    return ret;
}

static void __exit bsp_io_ioctl_exit(void)
{
    device_destroy(bsp_io_class, bsp_io_devno);
    class_destroy(bsp_io_class);
    cdev_del(&bsp_io_cdev);
    unregister_chrdev_region(bsp_io_devno, 1);
    printk(KERN_INFO "[BSP_IO] Exit\n");
}

module_init(bsp_io_ioctl_init);
module_exit(bsp_io_ioctl_exit);
MODULE_LICENSE("GPL");
