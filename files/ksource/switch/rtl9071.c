#include <linux/printk.h>
#include "../core/core.h"
#include <linux/uaccess.h>
typedef struct
{
    u8 type;
    u32 regAddr;
    u32 regValue;
} rtase_swc_cmd_t;
extern void rtase_swc_reg_get(rtase_swc_cmd_t *cmd);
extern void rtase_swc_reg_set(rtase_swc_cmd_t *cmd);
long rtl9071_reg_get(bsp_io_ioctl_data_t *data)
{
    rtase_swc_cmd_t cmd_data = {0};
    if (copy_from_user(&cmd_data, (void __user *)data->buf, sizeof(rtase_swc_cmd_t)))
    {
        printk(KERN_ERR "[SWITCH] copy_from_user failed\n");
        return -EFAULT;
    }
    rtase_swc_reg_get(&cmd_data);
    if (copy_to_user((void __user*)data->buf, &cmd_data, sizeof(cmd_data))) {
        printk(KERN_ERR "[BSP_IO] Copy to user failed\n");
        return -EFAULT;
    }
    data->ret = 0;
    return 0;
}
long rtl9071_reg_set(bsp_io_ioctl_data_t *data)
{
    rtase_swc_cmd_t cmd_data = {0};
    if (copy_from_user(&cmd_data, (void __user *)data->buf, sizeof(rtase_swc_cmd_t)))
    {
        printk(KERN_ERR "[SWITCH] copy_from_user failed\n");
        return -EFAULT;
    }
    rtase_swc_reg_set(&cmd_data);
    data->ret = 0;
    return 0;
}
