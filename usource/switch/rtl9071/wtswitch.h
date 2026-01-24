#ifndef WTSWITCH_H
#define WTSWITCH_H
#include <stdint.h>


#ifndef WTSWITCH_ENABLE
#define WTSWITCH_ENABLE 1
#endif

#ifndef WTSWITCH_DISABLE
#define WTSWITCH_DISABLE 0
#endif

typedef enum
{
    LINK_UP,
    LINK_DOWN,
    LINK_UNKNOWN,
}link_status_e;

typedef enum
{
    ROLE_SLAVE,
    ROLE_MASTER,
    ROLE_UNKNOWN,
}role_e;

typedef enum
{
    PORT_SPEED_10M,          /**< PHY BMCR speed 10M */
    PORT_SPEED_100M,         /**< PHY BMCR speed 100M */
    PORT_SPEED_1000M,       /**< PHY BMCR speed 1000M */
    PORT_SPEED_500M,        /**< PHY BMCR speed 500M */
    PORT_SPEED_UNKNOWN,
}speed_e;

#define MAC_ADDR_LEN 6
typedef struct
{
    uint8_t  mac[MAC_ADDR_LEN];
    uint16_t vid;
    uint8_t  port;
    uint16_t fid;
}l2_entry_t;

typedef struct
{
    uint8_t  mac[MAC_ADDR_LEN];
    uint16_t vid;
    uint32_t  port_mask;
    uint16_t fid;
}l2_multi_entry_t;


typedef enum
{
    CABLE_NORMAL = 0, /**< The cable status is normal */
    CABLE_OPEN,       /**< The cable status is open */
    CABLE_SHORT,      /**< The cable status is short */
    CABLE_ERROR,      /**< Cannot detect the cable status */
} cablestatus_e;


/* Acceptable double tag mode of VLAN */
typedef enum
{
    WT_SWITCH_DOUBLE_TAG_ACCEPT = 0, /**< Accept double tag */
    WT_SWITCH_DOUBLE_TAG_DENY,       /**< Deny double tag */
    WT_SWITCH_DOUBLE_TAG_END,
} vlan_accept_double_tag_e;
typedef struct
{
    int (*sdk_init)(void);
    int (*sdk_deinit)(void);

    //port
    int (*port_link_get)(uint32_t port,link_status_e *value);
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

    int (*switch_reg_read)(uint32_t regAddr, uint32_t *value);
    int (*switch_reg_write)(uint32_t regAddr, uint32_t value);
    int (*firmware_version_get)(void);
    int (*firmware_version_update)(char *firmware_path);
}wtswitch_t;
#endif
