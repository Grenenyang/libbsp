/*
 * SPDX-License-Identifier: LicenseRef-Realtek-Proprietary
 *
 * Copyright (c) 2024, Realtek Semiconductor Corp. All rights reserved.
 *
 * This software is a confidential and proprietary property of Realtek
 * Semiconductor Corp. Disclosure, reproduction, redistribution, in
 * whole or in part, of this work and its derivatives without express
 * permission is prohibited.
 *
 * Realtek Semiconductor Corp. reserves the right to update, modify, or
 * discontinue this software at any time without notice. This software is
 * provided "as is" and any express or implied warranties, including, but
 * not limited to, the implied warranties of merchantability and fitness for
 * a particular purpose are disclaimed. In no event shall Realtek
 * Semiconductor Corp. be liable for any direct, indirect, incidental,
 * special, exemplary, or consequential damages (including, but not limited
 * to, procurement of substitute goods or services; loss of use, data, or
 * profits; or business interruption) however caused and on any theory of
 * liability, whether in contract, strict liability, or tort (including
 * negligence or otherwise) arising in any way out of the use of this software,
 * even if advised of the possibility of such damage.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include "wtswitch.h"
#include <stdbool.h>
extern wtswitch_t rtk9071_switch;
wtswitch_t *wtswitch = NULL;


/*----------------- 模块参数结构 -----------------*/
typedef struct {
    int port_id;
    const char *action;
    const char *property;
    int value;
    char **argv;
} PortArgs;

typedef struct {
    const char *action;
    const char *property;
    void *value;
} firmware_args_t;

typedef struct {
    int vlan_id;
    const char *action;
    const char *property;
    int argc;
    char **argv;
    int tagged;
    int untagged;
} vlan_args_t;

typedef struct
{
    const char *action;
    const char *property;
    int argc;
    char **argv;
} l2_args_t;

typedef int (*port_handler)(const PortArgs *args);
typedef int (*handler)(void *args);
// 端口操作映射表结构
typedef struct {
    const char *action;
    const char *property;
    port_handler handler;
    const char *help;
} PortOperation;

typedef struct {
    const char *action;
    const char *property;
    handler handler;
    const char *help;
} cmd_operation;


#define WT_SWITCH_FUNC_CHECK(ret,fun)      \
do{\
    if(wtswitch->fun == NULL)\
    {\
        printf("%s not support\n",#fun);            \
         ret = -1; \
         return ret;\
    }\
}while(0)
static int handle_port_link_set(const PortArgs *args)
{
    int ret = 0;

    WT_SWITCH_FUNC_CHECK(ret,port_link_set);
    ret = wtswitch->port_link_set(args->port_id,args->value);
    return ret;
}
static int handle_port_link_get(const PortArgs *args)
{
    int ret = 0;
    link_status_e value = LINK_UNKNOWN;

    WT_SWITCH_FUNC_CHECK(ret,port_link_get);
    ret = wtswitch->port_link_get(args->port_id,&value);
    printf("port %d link status:%s\n",args->port_id,value == LINK_UP?"up":"down");
    return ret;
}
static int handle_port_role_set(const PortArgs *args)
{
    int ret = 0;

    WT_SWITCH_FUNC_CHECK(ret,phy_role_set);
    ret = wtswitch->phy_role_set(args->port_id,args->value);
    return ret;
}
static int handle_port_role_get(const PortArgs *args)
{
    int ret = 0;
    role_e value = ROLE_UNKNOWN;

    WT_SWITCH_FUNC_CHECK(ret,phy_role_set);
    ret = wtswitch->phy_role_get(args->port_id,&value);
    printf("port %d role = %s\n",args->port_id,value  == ROLE_MASTER ? "Master":"Slave");
    return ret;
    
}
static int handle_port_sqi_get(const PortArgs *args)
{
    int ret = 0;
    uint32_t value = 0;
    
    WT_SWITCH_FUNC_CHECK(ret,port_sqi_get);
    ret = wtswitch->port_sqi_get(args->port_id,&value);
    printf("port %d sqi = %d\n",args->port_id,value);
    return ret;
}
static int handle_port_mib_dump(const PortArgs *args)
{
    int ret = 0;

    WT_SWITCH_FUNC_CHECK(ret,port_mib_dump);
    wtswitch->port_mib_dump(args->port_id);
    return 0;
}

static int handle_port_mib_clear(const PortArgs *args)
{
    int ret = 0;

    WT_SWITCH_FUNC_CHECK(ret,port_mib_clear);
    ret =  wtswitch->port_mib_clear(args->port_id);
    return ret;
}


static int handle_port_speed_get(const PortArgs *args)
{
    int ret = 0;
    speed_e speed = 0;
    
    WT_SWITCH_FUNC_CHECK(ret,port_speed_get);
    ret = wtswitch->port_speed_get(args->port_id,&speed);
    switch (speed)
    {
    case  PORT_SPEED_10M:
        printf("port %d speed = 10M\n",args->port_id);
        break;
    case  PORT_SPEED_100M:
        printf("port %d speed = 100M\n",args->port_id);
        break;
    case  PORT_SPEED_1000M:
        printf("port %d speed = 1000M\n",args->port_id);
        break;
    default:
        break; 
    }
    return ret;
}

static int handle_port_speed_set(const PortArgs *args)
{
    int ret = 0;
    
    WT_SWITCH_FUNC_CHECK(ret,port_speed_set);
    ret = wtswitch->port_speed_set(args->port_id,args->value);
    return ret;
}

static int handle_port_ango_get(const PortArgs *args)
{
    int ret = 0;
    uint32_t ango = -1;
    
    WT_SWITCH_FUNC_CHECK(ret,port_ango_get);
    ret = wtswitch->port_ango_get(args->port_id,&ango);
    printf("port %d ango = %s\n",args->port_id,ango == WTSWITCH_ENABLE?"enable":"disable");
    return ret;
}

static int handle_port_ango_set(const PortArgs *args)
{
    int ret = 0;

    WT_SWITCH_FUNC_CHECK(ret,port_ango_set);
    ret = wtswitch->port_ango_set(args->port_id,args->value);

    return ret;
}
static int handle_port_pvid_set(const PortArgs *args)
{
    int ret = 0;

    WT_SWITCH_FUNC_CHECK(ret,port_pvid_set);
    ret = wtswitch->port_pvid_set(args->port_id,args->value);

    return ret;
}

static int handle_port_pvid_get(const PortArgs *args)
{
    int ret = 0;
    uint32_t pvid = 0;

    WT_SWITCH_FUNC_CHECK(ret,port_pvid_get);
    ret = wtswitch->port_pvid_get(args->port_id,&pvid);
    printf("port:%d pvid:%d\n",args->port_id,pvid);
    return ret;
}

static int handle_port_cable_get(const PortArgs *args)
{
    int ret = 0;
    cablestatus_e cable = 0;

    WT_SWITCH_FUNC_CHECK(ret,port_cable_get);
    ret = wtswitch->port_cable_get(args->port_id,&cable);
    switch(cable)
    {
    case CABLE_NORMAL:
        printf("port:%d cable is normal\n",args->port_id);
        break;
    case CABLE_OPEN:
        printf("port:%d cable is open\n",args->port_id);
        break;
    case CABLE_SHORT:
        printf("port:%d cable is short\n",args->port_id);
        break;
    default:
        printf("port:%d cable get error\n",args->port_id);
        break;
    }
    return ret;
}

/*wt_switch port 5 test_mode set 1 1*/
static int handle_port_test_mode_set(const PortArgs *args)
{
    int ret = 0;
    uint8_t testMode = 0;
    uint8_t is_master = 0;

    testMode = atoi(args->argv[5]);
    is_master = atoi(args->argv[6]);
    WT_SWITCH_FUNC_CHECK(ret,phy_test_mode_set);
    ret = wtswitch->phy_test_mode_set(args->port_id,testMode,is_master);
    return ret;
}
/*wt_switch port 5 vlan_double_tag set 1*/
static int handle_port_vlan_double_tag_set(const PortArgs *args)
{
    int ret = 0;
    WT_SWITCH_FUNC_CHECK(ret,port_vlan_double_tag_set);
    ret = wtswitch->port_vlan_double_tag_set(args->port_id,args->value);
    return ret;
}

static int handle_port_vlan_double_tag_get(const PortArgs *args)
{
    int ret = 0;
    vlan_accept_double_tag_e double_tag = WT_SWITCH_DOUBLE_TAG_END;

    WT_SWITCH_FUNC_CHECK(ret,port_vlan_double_tag_get);
    ret = wtswitch->port_vlan_double_tag_get(args->port_id,&double_tag);
    printf("prot %d accept double tag is %s\n",args->port_id,double_tag == WT_SWITCH_DOUBLE_TAG_ACCEPT?"accept":"deny");
    return ret;
}
static PortOperation port_operations[] = {
    {"set", "link", handle_port_link_set, "设置链路状态: link set <0:up|1:down>"},
    {"get", "link", handle_port_link_get, "获取链路状态: link get"},
    {"set", "role", handle_port_role_set, "设置端口角色: role set <0:slave|1:master>"},
    {"get", "role", handle_port_role_get, "获取端口角色: role get"},
    {"get", "sqi", handle_port_sqi_get,  "获取链路信号质量: sqi get"},
    {"dump", "mib", handle_port_mib_dump,  "dump 端口mib计数: mib dump"},
    {"clear", "mib", handle_port_mib_clear,  "清除端口mib计数: mib clear"},
    {"get", "speed", handle_port_speed_get,  "获取端口速率: speed get"},
    {"set", "speed", handle_port_speed_set,  "设置端口速率: speed set <0:10M|1:100M|2:1000M>"},
    {"set", "pvid", handle_port_pvid_set,  "设置端口pvid: pvid set <pvid>"},
    {"get", "pvid", handle_port_pvid_get,  "获取端口pvid: pvid get"},
    {"get", "ango", handle_port_ango_get,  "获取端口自协商状态: ango get"},
    {"set", "ango", handle_port_ango_set,  "获取端口自协商状态: ango set <0:disable|1:enale>"},
    {"get", "cable", handle_port_cable_get,  "获取端口线缆状态: cable get <0:normal|1:open|2:short>"},
    {"set", "test_mode", handle_port_test_mode_set,  "设置phy test mode: test_mode set <0:mode> <0:master>"},
    {"set", "vlan_double_tag", handle_port_vlan_double_tag_set, "set port vlan double tag <0:accept|1:deny>"},
    {"get", "vlan_double_tag", handle_port_vlan_double_tag_get, "get port vlan double tag"},
    {NULL, NULL, NULL, NULL}
};


static int handle_firmware_version_get(void *args)
{
    int ret = 0;

    WT_SWITCH_FUNC_CHECK(ret,firmware_version_get);
    ret = wtswitch->firmware_version_get();
    return ret;
}

static int handle_firmware_version_update(void *args)
{
    int ret = 0;
    firmware_args_t *firmware_args =(firmware_args_t *)args;
    char *firmware_path = (char *)(firmware_args->value);
    WT_SWITCH_FUNC_CHECK(ret,firmware_version_update);
    ret = wtswitch->firmware_version_update(firmware_path);
    return ret;
}
static cmd_operation firmware_operations[] =
{
    {"get", "version", handle_firmware_version_get, "获取firmware version: version get"},
    {"update", "version", handle_firmware_version_update, "更新firmware version: version update"},
    {NULL,NULL,NULL,NULL}
};
void parse_mac_address(const char *mac_str, uint8_t *mac_array) 
{
    // 去除分隔符并验证长度

    char clean_str[13] = {0};
    int j = 0;
    for (int i = 0; mac_str[i] != '\0' && j < 12; i++) {
        if (mac_str[i] == ':' || mac_str[i] == '-') {
            continue;
        }
        clean_str[j++] = mac_str[i];
    }
    
    // 长度校验
    if (j != 12) {
        fprintf(stderr, "错误：MAC地址长度无效\n");
        return;
    }
    
    // 转换为字节
    for (int i = 0; i < 6; i++) {
        char part[3] = { clean_str[2*i], clean_str[2*i + 1], '\0' };
        char *end;
        unsigned long value = strtoul(part, &end, 16);
        
        // 验证转换有效性
        if (*end != '\0' || value > 0xFF) {
            fprintf(stderr, "错误：无效的十六进制字符\n");
            return;
        }
        mac_array[i] = (uint8_t)value;
    }
}
static int handle_l2_entry_dump(void *args)
{
    int ret = 0;

    WT_SWITCH_FUNC_CHECK(ret,l2_entry_dump);
    ret = wtswitch->l2_entry_dump();
    return ret;
}

/*wtswitch l2 uni add mac port vid*/
static int handle_l2_uni_add(void *args)
{
    int ret = 0;
    l2_entry_t l2_entry = {0};

    l2_args_t *l2_args =(l2_args_t *)args;
    l2_entry.port = atoi(l2_args->argv[5]);
    l2_entry.vid = atoi(l2_args->argv[6]);
    parse_mac_address(l2_args->argv[4],l2_entry.mac);
    WT_SWITCH_FUNC_CHECK(ret,l2_uni_add);
    ret = wtswitch->l2_uni_add(&l2_entry);
    return ret;
}

/*wtswitch l2_multi_add mac portmask vid*/
static int handle_l2_multi_add(void *args)
{
    int ret = 0;
    l2_multi_entry_t l2_entry = {0};
    char *end = NULL;

    l2_args_t *l2_args =(l2_args_t *)args;
    l2_entry.port_mask = strtol(l2_args->argv[5],&end,0);
    l2_entry.vid = atoi(l2_args->argv[6]);
    parse_mac_address(l2_args->argv[4],l2_entry.mac);
    WT_SWITCH_FUNC_CHECK(ret,l2_multi_add);
    ret = wtswitch->l2_multi_add(&l2_entry);
    return ret;
}

/*wtswitch l2 entry del mac vid fid*/
static int handle_l2_entry_del(void *args)
{
    int ret = 0;
    l2_entry_t l2_entry = {0};

    l2_args_t *l2_args =(l2_args_t *)args;
    l2_entry.vid = atoi(l2_args->argv[5]);
    l2_entry.fid = atoi(l2_args->argv[6]);
    parse_mac_address(l2_args->argv[4],l2_entry.mac);
    WT_SWITCH_FUNC_CHECK(ret,l2_entry_del);
    ret = wtswitch->l2_entry_del(&l2_entry);
    return ret;
}

/*wtswitch l2 learn enable portmask enable*/
static int handle_l2_learn_enable(void *args)
{
    int ret = 0;
    uint32_t enable = 0;
    uint32_t portmask = 0;
    char *end = NULL;

    l2_args_t *l2_args =(l2_args_t *)args;
    enable = atoi(l2_args->argv[5]);
    portmask = strtol(l2_args->argv[4],&end,0);
    WT_SWITCH_FUNC_CHECK(ret,l2_learn_enable);
    ret = wtswitch->l2_learn_enable(portmask,enable);
    return ret;
}

static cmd_operation l2_operations[] =
{
    {"dump", "entry", handle_l2_entry_dump, "l2 dump entry dump"},
    {"add", "uni", handle_l2_uni_add, "l2 uni add: mac port vid"},
    {"add", "multi", handle_l2_multi_add, "l2 multi add: mac portmask vid"},
    {"del", "entry", handle_l2_entry_del, "l2 entry del:mac vid fid"},
    {"enable", "learn", handle_l2_learn_enable, "l2 learn enable: portmask <0:disable|1:enable>"},
    {NULL,NULL,NULL,NULL}
};
static int handle_vlan_member_get(void *args)
{
    int ret = 0;
    uint32_t untagged = 0;
    uint32_t tagged = 0;

    vlan_args_t *vlan_args =(vlan_args_t *)args;
    WT_SWITCH_FUNC_CHECK(ret,vlan_member_get);
    ret = wtswitch->vlan_member_get(vlan_args->vlan_id, &tagged, &untagged);
    printf("vid:%d, tagged:0x%x, untagged:0x%x\n",vlan_args->vlan_id,tagged,untagged);
    return ret;
}

static int handle_vlan_create(void *args)
{
    int ret = 0;
    uint32_t untagged = 0;
    uint32_t tagged = 0;
    char *end = NULL;

    vlan_args_t *vlan_args =(vlan_args_t *)args;
    tagged = strtol(vlan_args->argv[4], &end, 0);
    untagged = strtol(vlan_args->argv[5],&end, 0);
    WT_SWITCH_FUNC_CHECK(ret,vlan_create);
    ret = wtswitch->vlan_create(vlan_args->vlan_id, tagged, untagged);
    return ret;
}

static int handle_vlan_destroy(void *args)
{
    int ret = 0;

    vlan_args_t *vlan_args =(vlan_args_t *)args;
    WT_SWITCH_FUNC_CHECK(ret,vlan_destroy);
    ret = wtswitch->vlan_destroy(vlan_args->vlan_id);
    return ret;
}

static int handle_vlan_member_add(void *args)
{
    int ret = 0;
    uint32_t port = 0;
    uint32_t is_tagged = 0;

    vlan_args_t *vlan_args =(vlan_args_t *)args;
    port = atoi(vlan_args->argv[5]);
    is_tagged = atoi(vlan_args->argv[6]);
    WT_SWITCH_FUNC_CHECK(ret,vlan_member_add);
    ret = wtswitch->vlan_member_add(vlan_args->vlan_id,port,is_tagged);
    return ret;
}
static int handle_vlan_member_remove(void *args)
{
    int ret = 0;
    uint32_t port = 0;

    vlan_args_t *vlan_args =(vlan_args_t *)args;
    port = atoi(vlan_args->argv[5]);
    WT_SWITCH_FUNC_CHECK(ret,vlan_member_remove);
    ret = wtswitch->vlan_member_remove(vlan_args->vlan_id,port);
    return ret;
}

static cmd_operation vlan_operations[] =
{
    {"get", "member", handle_vlan_member_get, "获取vlan member: member get"},
    {"add", "member", handle_vlan_member_add, "add vlan member: member add port <1:tagged|0:untagged>"},
    {"remove", "member", handle_vlan_member_remove, "remove vlan member: member remove port"},
    {"create", NULL, handle_vlan_create, "create vlan : create tagged untagged"},
    {"destroy", NULL, handle_vlan_destroy, "destroy vlan : destroy"},
    {NULL,NULL,NULL,NULL}
};
typedef struct {
    int acl_id;
    const char *action;
} AclArgs;

typedef struct {
    const char *module;
    union {
        PortArgs port;
        vlan_args_t vlan;
        AclArgs acl;
        firmware_args_t firmware;
        l2_args_t l2;
    } args;
    bool valid;
} Context;

typedef struct {
    const char *name;
    int (*parse)(int argc, char *argv[], Context *ctx);
    int (*execute)(Context *ctx);
    const char *help;
} Module;

static int port_parse(int argc, char *argv[], Context *ctx)
{

    PortArgs *args = &ctx->args.port;
    args->value = -1;

    if (argc < 4)
    {
        fprintf(stderr, "错误: 参数不足\n");
        return -1;
    }

    // 解析基础参数
    args->port_id = atoi(argv[2]);
    args->action = argv[4];

    // 解析属性和值
    if (argc > 4)
    {
        args->property = argv[3];
        if (argc > 5)
        {
            if (!isdigit(argv[5][0]))
            {
                fprintf(stderr, "错误: 无效的数值参数\n");
                return -1;
            }
            args->value = atoi(argv[5]);
        }
    }

    // 参数有效性验证
    if (args->port_id < 1 || args->port_id > 255)
    {
        fprintf(stderr, "错误: 端口号必须在1-255之间\n");
        return -1;
    }

    args->argv = argv;
    return 0;
}
static int port_execute(Context *ctx)
{
    const PortArgs *args = &ctx->args.port;

    for (PortOperation *op = port_operations; op->action; op++)
    {
        if (strcmp(args->action, op->action) == 0 &&
            strcmp(args->property, op->property) == 0)
        {
            // 参数有效性检查
            if (strcmp(op->action, "set") == 0 && args->value == -1) {
                fprintf(stderr, "错误: set操作需要指定值\n");
                return -1;
            }
            // 执行对应操作
            return op->handler(args);
        }
    }

    // 未找到匹配的操作
    fprintf(stderr, "错误: 不支持的端口操作 '%s %s'\n",
            args->property, args->action);

    // 显示帮助信息
    fprintf(stderr, "\n支持的端口操作:\n");
    for (PortOperation *op = port_operations; op->action; op++)
    {
        fprintf(stderr, "  port <ID> %s\n", op->help);
    }

    return -1;
}
/*----------------- VLAN模块实现 -----------------*/
static int vlan_parse(int argc, char *argv[], Context *ctx)
{
    if (argc < 3)
    {
        fprintf(stderr, "错误: 参数不足\n");
        return -1;
    }

    ctx->args.vlan.vlan_id = atoi(argv[2]);
    if(strcmp(argv[3], "create") == 0 ||strcmp(argv[3], "destroy") == 0)
    {
             ctx->args.vlan.action = argv[3];
    }
    else
    {
        ctx->args.vlan.property = argv[3];
        ctx->args.vlan.action = argv[4];
    }
    ctx->args.vlan.argc = argc;
    ctx->args.vlan.argv = argv;
    return 0;
}

static int vlan_execute(Context *ctx)
{
    const vlan_args_t *args = &ctx->args.vlan;
    if(args->action != NULL && args->property != NULL)
    {
        for (cmd_operation *op = vlan_operations; op->action; op++)
        {
            if (strcmp(args->action, op->action) == 0 &&
                strcmp(args->property, op->property) == 0)
            {
                return op->handler((void *)args);
            }
        }
        // 未找到匹配的操作
        fprintf(stderr, "错误: 不支持的vlan操作 '%s %s'\n",
            args->property, args->action);

    }
    else
    {
        for (cmd_operation *op = vlan_operations; op->action; op++)
        {
            if (strcmp(args->action, op->action) == 0)
            {
                return op->handler((void *)args);
            }
        }
        // 未找到匹配的操作
        fprintf(stderr, "错误: 不支持的vlan操作 '%s %s'\n", args->action);

    }
    fprintf(stderr, "\n支持的vlan操作:\n");
    for (cmd_operation *op = vlan_operations; op->action; op++)
    {
        fprintf(stderr, "  vlan %s\n", op->help);
    }
    return -1;
}

/*----------------- ACL模块实现 -----------------*/
static int acl_parse(int argc, char *argv[], Context *ctx) {
    if (argc < 4)
        return -1;
    ctx->args.acl.acl_id = atoi(argv[2]);
    ctx->args.acl.action = argv[3];
    return 0;
}

static int acl_execute(Context *ctx) {
    printf("[ACL模块] 删除规则 %d\n", ctx->args.acl.acl_id);
    return 0;
}


static int l2_parse(int argc, char *argv[], Context *ctx)
{
    l2_args_t *args = &ctx->args.l2;

    if (argc < 3)
    {
        fprintf(stderr, "错误: 参数不足\n");
        return -1;
    }
    args->property = argv[2];
    args->action = argv[3];
    args->argc = argc;
    args->argv = argv;
    return 0;
}

static int l2_execute(Context *ctx)
{
    const l2_args_t *args = &ctx->args.l2;
    for (cmd_operation *op = l2_operations; op->action; op++)
    {
        if (strcmp(args->action, op->action) == 0 &&
            strcmp(args->property, op->property) == 0)
        {
            return op->handler((void *)args);
        }
    }
    // 未找到匹配的操作
    fprintf(stderr, "错误: 不支持的l2操作 '%s %s'\n",
            args->property, args->action);

    fprintf(stderr, "\n支持的l2操作:\n");
    for (cmd_operation *op = l2_operations; op->action; op++)
    {
        fprintf(stderr, "  l2 %s\n", op->help);
    }

    return -1;
}

static int firmware_parse(int argc, char *argv[], Context *ctx)
{
    firmware_args_t *args = &ctx->args.firmware;

    if (argc < 3)
    {
        fprintf(stderr, "错误: 参数不足\n");
        return -1;
    }

    args->action = argv[3];
    // 解析属性和值
    if (argc >= 3)
    {
        args->property = argv[2];
        if (argc > 3)
        {
            args->value = (void *)argv[4];
        }
    }
     return 0;
}

static int firmware_execute(Context *ctx)
{
    const firmware_args_t *args = &ctx->args.firmware;

    for (cmd_operation *op = firmware_operations; op->action; op++)
    {
        if (strcmp(args->action, op->action) == 0 &&
            strcmp(args->property, op->property) == 0)
        {
            return op->handler((void *)args);
        }
    }
    // 未找到匹配的操作
    fprintf(stderr, "错误: 不支持的firmware操作 '%s %s'\n",
            args->property, args->action);

    fprintf(stderr, "\n支持的firmware操作:\n");
    for (cmd_operation *op = firmware_operations; op->action; op++)
    {
        fprintf(stderr, "  firmware %s\n", op->help);
    }

    return -1;
}
/*----------------- 模块注册表 -----------------*/
static Module g_wtswitch_modules[] = {
    {
        .name = "port",
        .parse = port_parse,
        .execute = port_execute,
        .help = "端口配置模块\n"
                "  port <ID>  <link|role|mib|speed|pvid|ango|sqi|cable>\n"
    },
    {
        .name = "vlan",
        .parse = vlan_parse,
        .execute = vlan_execute,
        .help = "VLAN配置模块\n"
                "  vlan <ID> <create|member|destroy>\n"
                "  vlan <ID> delete"
    },
    {
        .name = "acl",
        .parse = acl_parse,
        .execute = acl_execute,
        .help = "ACL配置模块\n"
                "  acl <ID> delete"
    },
    {
        .name = "firmware",
        .parse = firmware_parse,
        .execute = firmware_execute,
        .help = "switch 固件模块\n"
		"  firmware version get\n"
		"  firmware version update <version>"
    },
    {
        .name = "l2",
        .parse = l2_parse,
        .execute = l2_execute,
        .help = "switch l2模块\n"
		"  l2 <entry|learn>"
    },
    {NULL, NULL, NULL, NULL}
};

/*----------------- 核心逻辑 -----------------*/
static void show_help(const char *prog) {
    printf("用法: %s <模块> <参数>\n\n可用模块:\n", prog);
    for (Module *m = g_wtswitch_modules; m->name; m++)
    {
        printf("  %s\n    %s\n\n", m->name, m->help);
    }
}


int main(int argc, char **argv)
{
    int ret = 0;
    wtswitch = &rtk9071_switch;
    if(argc < 2)
    {
        show_help(argv[0]);
	return -1;
    }
    if(wtswitch->sdk_init == NULL)
    {
        printf("sdk_init not support\n");
        return -1;
    }
    ret = wtswitch->sdk_init();
    if(ret < 0)
    {
        printf("sdk init error.ret=%d\n",ret);
        return -1;
    }
	// 查找匹配的模块
    Module *module = NULL;
    for (Module *m = g_wtswitch_modules; m->name; m++)
    {
        if (strcmp(argv[1], m->name) == 0)
        {
            module = m;
            break;
        }
    }

    if (!module)
    {
        fprintf(stderr, "错误: 未知模块 '%s'\n", argv[1]);
        show_help(argv[0]);
        return -1;
    }

    // 解析参数
    Context ctx =
    {
        .module = module->name,
        .valid = true
    };

    if (module->parse(argc, argv, &ctx) != 0)
    {
        fprintf(stderr, "用法: %s %s\n", argv[0], module->help);
        return -1;
    }

    // 执行命令
    ret =  module->execute(&ctx);
    if(ret != 0)
    {
        printf("ret=%d\n",ret);
    }
    if(wtswitch->sdk_deinit == NULL)
    {
        printf("switch_reg_write not support\n");
        return -1;
    }
    ret |= wtswitch->sdk_deinit();
    return ret;
}
