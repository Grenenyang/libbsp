#ifndef __BSP_IO_IOCTL_H__
#define __BSP_IO_IOCTL_H__

#include <linux/ioctl.h>
#include <linux/types.h>

// ========== 1. 全局配置（避免冲突） ==========
#define BSP_IO_MAGIC 'U' // ioctl幻数

// 外设标识（新增外设时，在此添加唯一标识）
typedef enum {
    BSP_PERIPH_SWITCH = 0,  // 交换机外设
    BSP_PERIPH_PHY,         // PHY外设
    BSP_PERIPH_UART,        // UART外设
    BSP_PERIPH_MAX          // 外设数量上限
} bsp_periph_id_t;

// ========== 2. 外设自定义子命令（新增外设时，在此添加对应子命令） ==========
// Switch外设子命令
typedef enum {
    RTL_CMD_REG_GET = 0,
    RTL_CMD_REG_SET,
    SWITCH_CMD_MAX
} switch_ioctl_cmd_t;

// PHY外设子命令（示例，后续可扩展）
typedef enum {
    PHY_CMD_INIT = 0,
    PHY_CMD_GET_LINK,
    PHY_CMD_MAX
} phy_ioctl_cmd_t;

// ========== 3. 统一数据交互结构体（用户态<->内核态） ==========
typedef struct {
    bsp_periph_id_t periph_id;  // 目标外设ID
    __u32 cmd;                  // 外设内部子命令
    __u64 buf;                  // 用户态数据缓冲区地址
    __u32 len;                  // 数据缓冲区长度
    __s32 ret;                  // 内核态处理结果返回值
} bsp_io_ioctl_data_t;

// ========== 4. 统一ioctl命令码 ==========
#define BSP_IO_IOCTL_CMD _IOWR(BSP_IO_MAGIC, 0, bsp_io_ioctl_data_t)

#endif // __BSP_IO_IOCTL_H__
