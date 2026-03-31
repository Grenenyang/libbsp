#ifndef WTSWITCH_H
#define WTSWITCH_H
#include "export.h"
typedef struct switch_ops
{
    int (*sdk_init)(struct hw_device *device);
    int (*sdk_deinit)(struct hw_device *device);

    //port
    int (*port_link_get)(struct hw_device *device, uint32_t port,link_status_e *value);
    int (*port_link_set)(uint32_t port,link_status_e value);
    int (*port_sqi_get)(uint32_t port, uint32_t *sqi_value);
    int (*port_mib_dump)(uint32_t port);
    int (*port_mib_clear)(uint32_t port);
    int (*port_speed_get)(uint32_t port, speed_e *speed);
    int (*port_speed_set)(uint32_t port, speed_e speed);
    int (*port_ango_get)(uint32_t port , uint32_t *ango);
    int (*port_ango_set)(uint32_t port , uint32_t ango);
    int (*port_pvid_set)(uint32_t port , uint32_t pvid);
    int (*port_pvid_get)(uint32_t port , uint32_t *pvid);
    int (*port_cable_get)(uint32_t port , cablestatus_e *cable);
    int (*port_vlan_double_tag_get)(uint32_t port,vlan_accept_double_tag_e *vlan_accept_double_tag);
    int (*port_vlan_double_tag_set)(uint32_t port,vlan_accept_double_tag_e vlan_accept_double_tag);
    //vlan
    int (*vlan_member_get)(uint32_t vlan, uint32_t *tagged,uint32_t *untagged);
    int (*vlan_create)(uint32_t vlan, uint32_t tagged,uint32_t untagged);
    int (*vlan_destroy)(uint32_t vid);
    int (*vlan_member_add)(uint32_t vid, uint32_t port, uint32_t is_tagged);
    int (*vlan_member_remove)(uint32_t vid, uint32_t port);

    //mirror

    //acl

    //qos

    //l2
    int (*l2_entry_dump)(void);
    int (*l2_multi_add)(l2_multi_entry_t *entry);
    int (*l2_uni_add)(l2_entry_t *entry);
    int (*l2_entry_del)(l2_entry_t *entry);
    int (*l2_learn_enable)(uint32_t portmask, uint32_t enable);
    //phy
    int (*phy_role_get)(uint32_t port,role_e *role);
    int (*phy_role_set)(uint32_t port,role_e role);
    int (*phy_test_mode_set)(uint32_t port, uint8_t testMode, uint8_t is_master);

    int (*switch_reg_read)(struct hw_device *device, uint32_t regAddr, uint32_t *value);
    int (*switch_reg_write)(struct hw_device *device, uint32_t regAddr, uint32_t value);
    int (*firmware_version_get)(void);
    int (*firmware_version_update)(char *firmware_path);
}switch_ops_t;
#endif
