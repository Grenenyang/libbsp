#ifndef __SWITCH_EXPORT_H
#define __SWITCH_EXPORT_H
#include <stdint.h>

#ifndef WTSWITCH_ENABLE
#define WTSWITCH_ENABLE 1
#endif

#ifndef WTSWITCH_DISABLE
#define WTSWITCH_DISABLE 0
#endif

#ifndef __HANDLE_T_
#define __HANDLE_T_
typedef uintptr_t handle_t;
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

int bsp_switch_sdk_init(handle_t handle);

#endif
