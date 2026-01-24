// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
/*
 *  rtase_app is the user-space application to configure Realtek Automotive
 *  Switch controllers with PCI-Express interface.
 *
 *  Copyright(c) 2020 Realtek Semiconductor Corp. All rights reserved.
 *
 *  Author:
 *  Realtek ARD software team
 *  No. 2, Innovation Road II, Hsinchu Science Park, Hsinchu 300, Taiwan
 *
 */

/******************************************************************************
 * Include
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/sockios.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>

/******************************************************************************
 * Defines
 ******************************************************************************/
#define MAGIC_NUMBER    'P'
#define MAGIC_NUM_SHIFT 24u
#define FEATURE_SHIFT   8u
#define CMD_SHIFT       0u

#define RTASE_PRIV_COMMAND(feature, cmd) ((MAGIC_NUMBER << MAGIC_NUM_SHIFT) | \
                                          ((feature) << FEATURE_SHIFT) | \
                                          ((cmd) << CMD_SHIFT))

#define TXARB      0x1u
#define TXQOS      0x2u
#define RXQOS      0x3u
#define VLANFILTER 0x4u
#define PTMSYNC    0x5u

#define RTASESTXARB      RTASE_PRIV_COMMAND(TXARB, 0x1)
#define RTASEGTXARB      RTASE_PRIV_COMMAND(TXARB, 0x2)
#define RTASESTXQOS      RTASE_PRIV_COMMAND(TXQOS, 0x1)
#define RTASEGTXQOS      RTASE_PRIV_COMMAND(TXQOS, 0x2)
#define RTASESRXQOS      RTASE_PRIV_COMMAND(RXQOS, 0x1)
#define RTASEGRXQOS      RTASE_PRIV_COMMAND(RXQOS, 0x2)
#define RTASESVLANFILTER RTASE_PRIV_COMMAND(VLANFILTER, 0x1)
#define RTASEGVLANFILTER RTASE_PRIV_COMMAND(VLANFILTER, 0x2)
#define RTASEEPTMSYNC    RTASE_PRIV_COMMAND(PTMSYNC, 0x1)
#define RTASEDPTMSYNC    RTASE_PRIV_COMMAND(PTMSYNC, 0x2)
#define RTASEPPTMSYNC    RTASE_PRIV_COMMAND(PTMSYNC, 0x3)

#define SIO_PRIV_RTK_CMD  (SIOCDEVPRIVATE + 6)
#define SIO_PRIV_RTK_INFO (SIOCDEVPRIVATE + 7)

#define PTP_MASTER_TIME 0u
#define PTP_LOCAL_TIME  1u
#define PTP_SLAVE0_TIME 2u
#define PTP_SLAVE1_TIME 3u
#define PTP_SLAVE2_TIME 4u

/******************************************************************************
 * Structure
 ******************************************************************************/
struct txarb
{
    uint8_t arbitrator;
};

struct txqos
{
    uint8_t tc_num;
    uint32_t bandwith;
};

struct vlanfilter
{
    uint8_t entry;
    uint32_t vid;
    uint16_t function_mask;
};

struct ptmsync
{
    uint8_t clock_ptp;
    uint16_t preload_time;
};

struct rxqos
{
    uint8_t pcp_value;
    uint8_t tc_num;
};


struct cmd_pattern
{
    uint32_t cmd_id;
    void *cmd_data;
};

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
static int txarb(int argc, char **argv);
static int txqos(int argc, char **argv);
static int rxqos(int argc, char **argv);
static int ptmsync(int argc, char **argv);
static int vlanfilter(int argc, char **argv);
static int rtk_info(int argc, char **argv);

/******************************************************************************
 * Global Variables
 ******************************************************************************/
static struct
{
    const char *name;
    int (*act_func)(int argc, char **argv);
} g_cmdInfo[] = {
    {"txarb", txarb},
    {"txqos", txqos},
    {"rxqos", rxqos},
    {"vlanfilter", vlanfilter},
    {"ptmsync", ptmsync},
    {"log", rtk_info},
    /* end of command */
    {NULL, NULL},
};

static struct
{
    const char *crdt;
    uint32_t bw;
} g_txqCrdt[] = {
    {"0", 0x0},
    {"10", 0x06666666}, /* 10% */
    {"20", 0x0CCCCCCC}, /* 20% */
    {"25", 0x10000000}, /* 25% */
    {"30", 0x13333333}, /* 30% */
    {"40", 0x19999999}, /* 40% */
    {"50", 0x20000000}, /* 50% */
    {"60", 0x26666666}, /* 60% */
    {"70", 0x2CCCCCCC}, /* 70% */
    {"75", 0x30000000}, /* 75% */
    {"80", 0x33333333}, /* 80% */
    {"90", 0x39999999}, /* 90% */
    /* end of command */
    {NULL, 0x0},
};

/******************************************************************************
 * Functions
 ******************************************************************************/
static int txarb(int argc, char **argv)
{
    struct cmd_pattern cmd_info;
    struct txarb txarb_info;
    struct ifreq ifIdx;
    int sockfd;

    if (argc < 4)
    {
        printf("Command is not complete.\n");
        return -EINVAL;
    }

    memset(&ifIdx, 0, sizeof(struct ifreq));
    strncpy(ifIdx.ifr_name, argv[1], IFNAMSIZ - 1);

    ifIdx.ifr_data = (char *)&cmd_info;
    cmd_info.cmd_data = (void *)&txarb_info;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("socket");

    if (!strcmp(argv[3], "set"))
    {
        cmd_info.cmd_id = RTASESTXARB;
        if (!strcmp(argv[4], "rr"))
        {
            txarb_info.arbitrator = 0;
        }
        else if (!strcmp(argv[4], "sp"))
        {
            txarb_info.arbitrator = 1;
        }
        else
        {
            printf("Command is not correct.\n");
            return -EINVAL;
        }
    }
    else if (!strcmp(argv[3], "get"))
    {
        cmd_info.cmd_id = RTASEGTXARB;
    }
    else
    {
        printf("Command is not correct.\n");
        return -EINVAL;
    }

    if (ioctl(sockfd, SIO_PRIV_RTK_CMD, &ifIdx) < 0)
            perror("SIO_PRIV_RTK_CMD");

    if (cmd_info.cmd_id == RTASEGTXARB)
    {
        if (txarb_info.arbitrator == 1)
            printf("Tx arbitration mode: Strict Priority.\n");
        else
            printf("Tx arbitration mode: Round Robin.\n");
    }
    else
    {
        printf("Access was successful.\n");
    }

    return 0;
}

static int txqos(int argc, char **argv)
{
    struct cmd_pattern cmd_info;
    struct txqos txqos_info;
    struct ifreq ifIdx;
    bool flag = false;
    int sockfd;
    int tc_num;
    int i = 4;
    int j;

    if (argc < 5)
    {
        printf("Command is not complete.\n");
        return -EINVAL;
    }

    memset(&ifIdx, 0, sizeof(struct ifreq));
    strncpy(ifIdx.ifr_name, argv[1], IFNAMSIZ - 1);

    tc_num = strtol(argv[4], NULL, 10);
    if (tc_num > 7)
    {
        printf("tc number is not correct.\n");
        return -EINVAL;
    }
    else
    {
        txqos_info.tc_num = tc_num;
    }


    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("socket");

    ifIdx.ifr_data = (char *)&cmd_info;
    cmd_info.cmd_data = (void *)&txqos_info;

    if (!strcmp(argv[3], "set"))
    {
        cmd_info.cmd_id = RTASESTXQOS;

        for (j = 0; g_txqCrdt[j].crdt != NULL; j++)
        {
            if (!strcmp(argv[6], g_txqCrdt[j].crdt))
            {
                txqos_info.bandwith = g_txqCrdt[j].bw;
                break;
            }
        }
    }
    else if (!strcmp(argv[3], "get"))
    {
        cmd_info.cmd_id = RTASEGTXQOS;
    }
    else
    {
        printf("Command is not correct.\n");
        return -EINVAL;
    }

    if (ioctl(sockfd, SIO_PRIV_RTK_CMD, &ifIdx) < 0)
        perror("SIO_PRIV_RTK_CMD");

    if (cmd_info.cmd_id == RTASEGTXQOS)
    {
        for (i = 0; g_txqCrdt[i].crdt != NULL; i++)
        {
            if (txqos_info.bandwith == g_txqCrdt[i].bw)
            {
                printf("TC%d credit is %s%%\n", tc_num, g_txqCrdt[i].crdt);
                flag = true;
                break;
            }
        }

        if(!flag)
        {
            printf("Warning: Credit is %08x.\n", txqos_info.bandwith);
            return -EINVAL;
        }
    }
    else
    {
        printf("Access was successful.\n");
    }

    return 0;
}

static int rxqos(int argc, char **argv)
{
    struct cmd_pattern cmd_info;
    struct rxqos rxqos_info;
    struct ifreq ifIdx;
    int sockfd;
    int i = 4;

    if (argc < 6)
    {
        printf("Command is not complete.\n");
        return -EINVAL;
    }

    memset(&ifIdx, 0, sizeof(struct ifreq));
    strncpy(ifIdx.ifr_name, argv[1], IFNAMSIZ - 1);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("socket");

    ifIdx.ifr_data = (char *)&cmd_info;
    cmd_info.cmd_data = (void *)&rxqos_info;

    rxqos_info.pcp_value = strtol(argv[5], NULL, 10);

    if (!strcmp(argv[3], "set"))
    {
        cmd_info.cmd_id = RTASESRXQOS;
        rxqos_info.tc_num = strtol(argv[7], NULL, 10);
    }
    else if (!strcmp(argv[3], "get"))
    {
        cmd_info.cmd_id = RTASEGRXQOS;
    }
    else
    {
        printf("Command is not correct.\n");
        return -EINVAL;
    }

    if (ioctl(sockfd, SIO_PRIV_RTK_CMD, &ifIdx) < 0)
        perror("SIO_PRIV_RTK_CMD");

    if (cmd_info.cmd_id == RTASEGRXQOS)
        printf("pcp %u is assigned to tc %u\n", rxqos_info.pcp_value, rxqos_info.tc_num);
    else
        printf("Access was successful.\n");

    return 0;
}

static int vlanfilter(int argc, char **argv)
{
    struct vlanfilter vlanfilter_info;
    struct cmd_pattern cmd_info;
    struct ifreq ifIdx;
    int sockfd, entry;
    uint16_t fmask;
    uint32_t vid;
    int i = 4;

    if (argc < 5)
    {
        printf("Command is not complete.\n");
        return -EINVAL;
    }

    memset(&ifIdx, 0, sizeof(struct ifreq));
    strncpy(ifIdx.ifr_name, argv[1], IFNAMSIZ - 1);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("socket");

    ifIdx.ifr_data = (char *)&cmd_info;
    cmd_info.cmd_data = (void *)&vlanfilter_info;

    entry = strtol(argv[4], NULL, 10);
    if (entry >= 32)
    {
        printf("entry number is not correct.\n");
        return -EINVAL;
    }

    vlanfilter_info.entry = entry;

    if (!strcmp(argv[3], "set"))
    {
        cmd_info.cmd_id = RTASESVLANFILTER;
        vid = strtol(argv[5], NULL, 10);
        if ((vid < 1) || (vid > 4094))
        {
            printf("invalid vid.\n");
            return -EINVAL;
        }

        vlanfilter_info.vid = vid;
        vlanfilter_info.function_mask = strtol(argv[6], NULL, 16);
    }
    else if (!strcmp(argv[3], "get"))
    {
        cmd_info.cmd_id = RTASEGVLANFILTER;
    }
    else
    {
        printf("Command is not correct.\n");
        return -EINVAL;
    }

    if (ioctl(sockfd, SIO_PRIV_RTK_CMD, &ifIdx) < 0)
        perror("SIO_PRIV_RTK_CMD");

    if (cmd_info.cmd_id == RTASEGVLANFILTER)
        printf("VLAN entry %d: vid = %u, function_mask = %x\n", vlanfilter_info.entry, vlanfilter_info.vid, vlanfilter_info.function_mask);
    else
        printf("Access was successful.\n");

    return 0;
}

static int ptmsync_get_ptp_number(char *ptp_timer)
{
    int ret;

    if (!strcmp(ptp_timer, "PTP_MASTER_TIME"))
    {
        ret = PTP_MASTER_TIME;
    }
    else if (!strcmp(ptp_timer, "PTP_LOCAL_TIME"))
    {
        ret = PTP_LOCAL_TIME;
    }
    else if (!strcmp(ptp_timer, "PTP_SLAVE0_TIME"))
    {
        ret = PTP_SLAVE0_TIME;
    }
    else if (!strcmp(ptp_timer, "PTP_SLAVE1_TIME"))
    {
        ret = PTP_SLAVE1_TIME;
    }
    else if (!strcmp(ptp_timer, "PTP_SLAVE2_TIME"))
    {
        ret = PTP_SLAVE2_TIME;
    }
    else
    {
        printf("Command is not correct.\n");
        ret = -EINVAL;
    }

    return ret;
}

static int ptmsync(int argc, char **argv)
{
    struct cmd_pattern cmd_info;
    struct ptmsync ptmsync_info;
    uint32_t clock_sel;
    struct ifreq ifIdx;
    int sockfd;
    int i = 4;

    if (argc < 4)
    {
        printf("Command is not complete.\n");
        return -EINVAL;
    }

    memset(&ifIdx, 0, sizeof(struct ifreq));
    strncpy(ifIdx.ifr_name, argv[1], IFNAMSIZ - 1);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("socket");

    ifIdx.ifr_data = (char *)&cmd_info;
    cmd_info.cmd_data = (void *)&ptmsync_info;

    if (!strcmp(argv[3], "enable"))
    {
        cmd_info.cmd_id = RTASEEPTMSYNC;
        clock_sel = ptmsync_get_ptp_number(argv[4]);
        if (clock_sel == -EINVAL)
        {
            printf("ptmsync_get_ptp_number failed.\n");
            return -EINVAL;
        }

        ptmsync_info.clock_ptp = clock_sel;
    }
    else if (!strcmp(argv[3], "disable"))
    {
        cmd_info.cmd_id = RTASEDPTMSYNC;
    }
    else if (!strcmp(argv[3], "preload"))
    {
        cmd_info.cmd_id = RTASEPPTMSYNC;
        ptmsync_info.preload_time = strtol(argv[4], NULL, 10);
    }
    else
    {
        printf("Command is not correct.\n");
        return -EINVAL;
    }

    if (ioctl(sockfd, SIO_PRIV_RTK_CMD, &ifIdx) < 0)
        perror("SIO_PRIV_RTK_CMD");

    return 0;
}

static int rtk_info(int argc, char **argv)
{
    struct ifreq ifIdx;
    int sockfd;

    memset(&ifIdx, 0, sizeof(struct ifreq));
    strncpy(ifIdx.ifr_name, argv[1], IFNAMSIZ - 1);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("socket");

    if (ioctl(sockfd, SIO_PRIV_RTK_INFO, &ifIdx) < 0)
        perror("SIO_PRIV_RTK_INFO");
    else
        printf("The device information has been recorded in kernel message.\n");

    return 0;
}

int main(int argc, char **argv)
{
    int ret = 0;
    int i;

    if (argc > 2)
    {
        for (i = 0; g_cmdInfo[i].name != NULL; i++)
        {
            if (!strcmp(argv[2], g_cmdInfo[i].name))
                ret = g_cmdInfo[i].act_func(argc, argv);
        }
    }
    else
    {
        ret = -EINVAL;
    }

    return ret;
}
