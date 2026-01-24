#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "wtswitch.h"
#include "rtkas_api_ext.h"
#include "rtkas_api_system.h"
#include "rtkas_api_phy.h"
#include "rtkas_api_phy_test.h"
#include "rtkas_api_mib.h"
#include "rtkas_api_sys_boot.h"
#include "rtkas_api_spi_flash.h"
#include "rtkas_api_vlan.h"
#include "rtkas_api_l2.h"
#include "core.h"

#define WTSWITCH_DBG

#define SWC_CMD_REG_GET 70
#define SWC_CMD_REG_SET 71
#define SWC_CMD_TYPE_8  0
#define SWC_CMD_TYPE_16 1
#define SWC_CMD_TYPE_32 2

#define SWC_VERSION_REG 0x1b004c
/* RTCORE device */
#ifndef RTASE_SWC_NAME
#define RTASE_SWC_NAME "/dev/rtcore"
#endif

#define PAGE_SWITCH_REG (0xFFFF01u)
#define MAX_PORT 10
static int32_t  g_pcieFd   = 0;
static uint32_t g_pciePage = 0x4A000000;
UnitChip_t     g_unitChip  = {.chip = CHIP_907XC_VB, .unit = 1u};


static int cvb_spi_flash_update_image(UnitChip_t unitChip,const char *image_path) {
    // 临时实现（根据实际逻辑补充）
    HW_INFO("cvb_spi_flash_update_image: dummy implementation\n");
    return HW_OK;
}

typedef struct
{
    uint8_t  type;
    uint32_t regAddr;
    uint32_t regValue;
} rtase_swc_cmd_t;


static int  pcie_open(void)
{
    if(0 == g_pcieFd)
    {
        g_pcieFd = open(RTASE_SWC_NAME, O_RDWR);
        if(0 > g_pcieFd)
        {
#ifdef DEBUG_PCIE_INTERFACE
            printf("PCIe: Can't open device.\n");
#endif
            g_pcieFd = 0;
        }
    }
    else
    {
#ifdef DEBUG_PCIE_INTERFACE
        printf("PCIe: device has been open.\n");
#endif
        return g_pcieFd;
    }
    return 0;
}
static void pcie_close(void)
{
    if(0 < g_pcieFd)
    {
        (void)close(g_pcieFd);
        g_pcieFd = 0;
    }
}

static int rtk9071_port_link_get(uint32_t port,link_status_e *link_status)
{
    int ret = 0;
    PhyLinkStatus_t PhyLinkStatus = {0};
    
    if(port > MAX_PORT || link_status == NULL)
    {
        printf("param error\n");
        return -1;
    }
    PhyLinkStatus.port = port;
    ret = rtk_phy_bmsr_link_status_get(g_unitChip,&PhyLinkStatus);
    if(PhyLinkStatus.linkSts == 0)
    {
        *link_status = LINK_DOWN;
    }else
    {
        *link_status = LINK_UP;
    }
#ifdef WTSWITCH_DBG     
    printf("port %d link status:%d\n",port,*link_status);
#endif 
    return ret;
}

static int rtk9071_port_link_set(uint32_t port, link_status_e link_status)
{
    int ret = 0;
    
    if(port > MAX_PORT || link_status >LINK_UNKNOWN)
    {
        printf("param error\n");
        return -1;
    }

    PhyPwd_t pPhyPwd = {0};
    pPhyPwd.portMap =(1 << port);
    pPhyPwd.pwdCfg = link_status;
    ret = rtk_phy_bmcr_power_down_set(g_unitChip,&pPhyPwd);
    return ret;
}

static RtkApiRet platform_pcie_read(uint32_t regAddr, uint32_t *value)
{
    RtkApiRet       ret = SUCCESS;
    rtase_swc_cmd_t cmd;

    if(value == NULL )
    {
        return -1;
    }
#ifdef DEBUG_PCIE_INTERFACE
    printf("regaddr:0x%x valueaddr:0x%x.\n",regAddr,value);
#endif

    memset(&cmd, 0, sizeof(rtase_swc_cmd_t));
    if(0 == g_pcieFd)
    {
#ifdef DEBUG_PCIE_INTERFACE
        printf("PCIe: device has not been open.\n");
#endif
        ret = -RT_ERR_PCIE_FILE_NOT_OPEN;
    }
    else
    {
        if(regAddr == PAGE_SWITCH_REG)
        {
            *value = (g_pciePage >> 24u) & 0xFFu;
        }
        else
        {
            cmd.type    = SWC_CMD_TYPE_32;
            cmd.regAddr = regAddr | g_pciePage;
            if(ioctl(g_pcieFd, SWC_CMD_REG_GET, &cmd) != 0)
            {
#ifdef DEBUG_PCIE_INTERFACE
                printf("PCIe: ioctl failed.\n");
#endif
                ret = -RT_ERR_PCIE_IOCTL_FAIL;
            }
            *value = cmd.regValue;
        }
    }

    return ret;
}

static RtkApiRet platform_pcie_write(uint32_t regAddr, uint32_t value)
{
    RtkApiRet       ret = RT_ERR_OK;
    rtase_swc_cmd_t cmd;

    if(0 == g_pcieFd)
    {
#ifdef DEBUG_PCIE_INTERFACE
        printf("PCIe: device has not been open.");
#endif
        ret = -RT_ERR_PCIE_FILE_NOT_OPEN;
    }
    else
    {
        if(regAddr == PAGE_SWITCH_REG)
        {
            g_pciePage = ((value & 0xFFu) << 24u);
        }
        else
        {
            cmd.type     = SWC_CMD_TYPE_32;
            cmd.regAddr  = regAddr | g_pciePage;
            cmd.regValue = value;
            if(ioctl(g_pcieFd, SWC_CMD_REG_SET, &cmd) != 0)
            {
#ifdef DEBUG_PCIE_INTERFACE
                printf("PCIe: ioctl failed.\n");
#endif
                ret = -RT_ERR_PCIE_IOCTL_FAIL;
            }
        }
    }
    return ret;
}
static RtkApiRet platform_pcie_write_byte(uint32_t regAddr, uint32_t value)
{
    RtkApiRet       ret = RT_ERR_OK;
    rtase_swc_cmd_t cmd;

    if(0 == g_pcieFd)
    {
#ifdef DEBUG_PCIE_INTERFACE
        printf("PCIe: device has not been open.");
#endif
        ret = -RT_ERR_PCIE_FILE_NOT_OPEN;
    }
    else
    {
        if(regAddr == PAGE_SWITCH_REG)
        {
            g_pciePage = ((value & 0xFFu) << 24u);
        }
        else
        {
            cmd.type     = SWC_CMD_TYPE_8;
            cmd.regAddr  = regAddr | g_pciePage;
            cmd.regValue = value;
            if(ioctl(g_pcieFd, SWC_CMD_REG_SET, &cmd) != 0)
            {
#ifdef DEBUG_PCIE_INTERFACE
                printf("PCIe: ioctl failed.\n");
#endif
                ret = -RT_ERR_PCIE_IOCTL_FAIL;
            }
        }
    }

    return ret;
}

static int32 my_usecond_delay_wrapper(uint32 delayTime)
{
    return usleep(delayTime);
}
static int32 my_timer_get_cur_wrapper(uint32 *us)
{
    int32 retVal;

#if 1
    struct timespec curTime;
#endif

    if(NULL == us)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        retVal = RT_ERR_OK;
    }

#if 1
    CHK_FUN_RET(retVal, clock_gettime(CLOCK_REALTIME, &curTime));
    if(RT_ERR_OK == retVal)
    {
        *us = (uint32)(curTime.tv_sec * 1000000 + (curTime.tv_nsec / 1000u));
    }
#endif

    return retVal;
}

static int rtk9071_sys_porting_stdlib(void)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* register c standard library API */
        CHK_FUN_CONTINUE(ret, rtk_memcmp_init(&memcmp));
        CHK_FUN_CONTINUE(ret, rtk_memset_init(&memset));
        CHK_FUN_CONTINUE(ret, rtk_memcpy_init(&memcpy));
        CHK_FUN_CONTINUE(ret, rtk_strcmp_init(&strcmp));

        /* register customer own library API */
        CHK_FUN_CONTINUE(ret, rtk_printf_init(&printf));
        CHK_FUN_CONTINUE(ret, rtk_fopen_init((fopen_func_ptr)fopen));
        CHK_FUN_CONTINUE(ret, rtk_fclose_init((fclose_func_ptr)fclose));
        CHK_FUN_CONTINUE(ret, rtk_fread_init((fread_func_ptr)fread));
        CHK_FUN_CONTINUE(ret, rtk_fseek_init((fseek_func_ptr)fseek));
        CHK_FUN_CONTINUE(ret, rtk_usecond_delay_init(&my_usecond_delay_wrapper));
        CHK_FUN_CONTINUE(ret, rtk_timer_get_current_init(&my_timer_get_cur_wrapper));

    } while(false);

    return ret;
}
/**@}*/ /* SYS_PORTING_STDLIB */
static int rtk9071_sdk_init(void)
{
    RtkApiRet ret = RT_ERR_OK;
    RegifCallback_t regifCallback = {
        .readFunc       = &platform_pcie_read,
        .writeFunc      = &platform_pcie_write,
        .writeBytesFunc = &platform_pcie_write_byte};

    ret = rtk_regif_init(REGIF_PCIE, (const RegifCallback_t *)&regifCallback);
    if (ret != 0)
    {
        printf("rtk_reg_init error/n");
        return -1;
    }
    ret = pcie_open();
    UnitChip_t     unitChip  = {.chip = CHIP_907XC_VB, .unit = 1u};
    RtkSysStatus_e sysStatus = EM_SYS_STATUS_UNKNOWN;

    do
    {
        /* Step 1. Call system init function */
        CHK_FUN_CONTINUE(ret, rtk_sys_init(unitChip));

        /* Step 2. Polling system status until it is ready */
        while((ret == RT_ERR_OK) && (sysStatus != EM_SYS_STATUS_READY))
        {
            ret = rtk_sys_status_get(unitChip, &sysStatus);
        }

    } while(0u == 1u);
   
    ret |= rtk9071_sys_porting_stdlib();
    
    //start mib
    ret |= rtk_stat_start_all(g_unitChip);

    
    return ret;
}

static int rtk9071_sdk_deinit(void)
{
    pcie_close();
    return 0;
}

#define GET_STAT_PORT(unit,stat, port, type) \
    do { \
        stat.port = (port); \
        stat.counterType = (type); \
	stat.counterValue = (0);\
        rtk_stat_port_get((unit), &stat); \
    } while(0)

typedef struct
{
    RtkMibCounterType_e type;
    const char* desc;
} StatItem;

static const StatItem statItems[] = {
    {EM_IFOUTUCASTPKTS,   "Tx unicast pkt"},
    {EM_IFOUTMULTICASTPKTS,"Tx multicast pkt"},
    {EM_IFOUTBROADCASTPKTS,"Tx broadcast pkt"},
    {EM_IFOUTDISCARDS, "Tx drop Pkt"},
    {EM_IFINUCASTPKTS, "Rx unicast pk"},
    {EM_ETHERSTATSMULTICASTPKTS,"Rx multicast pkt"},
    {EM_ETHERSTATSBROADCASTPKTS,"Rx broadcast pkt"},
    {EM_ETHERSTATSDROPEVENTS,"Rx drop Pkt"}
};


static int rtk9071_port_mib_dump(uint32_t port)
{
    StatPortGet_t statPortGetLo = {0};
    StatPortGet_t statPortGetHi = {0};
    int i = 0;
    
    if(port > MAX_PORT)
    {
        printf("param error\n");
        return -1;
    }
    
    GET_STAT_PORT(g_unitChip,statPortGetLo,port,EM_IFOUTOCTETS_LSB);
    GET_STAT_PORT(g_unitChip,statPortGetHi,port,EM_IFOUTOCTETS_MSB);
    printf("Tx Bytes : %lu\n",((uint64_t)(statPortGetHi.counterValue) << 32) |statPortGetLo.counterValue);
    
    GET_STAT_PORT(g_unitChip,statPortGetLo,port,EM_IFINOCTETS_LSB);
    GET_STAT_PORT(g_unitChip,statPortGetHi,port,EM_IFINOCTETS_MSB);
    printf("Rx Bytes : %lu\n",((uint64_t)(statPortGetHi.counterValue) << 32) |statPortGetLo.counterValue);

    for ( i = 0; i < sizeof(statItems)/sizeof(StatItem); i++)
    {
        StatPortGet_t stat = {0}; 
        GET_STAT_PORT(g_unitChip, stat, port, statItems[i].type);
        printf("%-20s: %10u\n",   statItems[i].desc, stat.counterValue);  
    }
    
    return 0;
}

static int rtk9071_port_mib_reset(uint32_t port)
{
    int ret = 0;
    StatPortConfig_t statPortReset = {0};
    
    if(port > MAX_PORT)
    {
        printf("param error\n");
        return -1;
    }
    statPortReset.port = port;
    ret = rtk_stat_port_reset(g_unitChip,&statPortReset);
    return  ret;
}

int rtk9071_port_sqi_get(uint32_t port, uint32_t *sqi_value)
{
    int ret = 0;
    SqiGetInfo_t sqiGetInfo = {0};

    if(port > MAX_PORT || sqi_value == NULL)
    {
        printf("param error\n");
        return -1;
    }
    
    if(port < 5)
    {
        sqiGetInfo.speed = (uint8_t)0u;
    }else
    {
        sqiGetInfo.speed = (uint8_t)1u;
    }
    sqiGetInfo.port = port;
    ret = rtk_port_sqi_get(g_unitChip,&sqiGetInfo);
#ifdef WTSWITCH_DBG 
    printf("port %d sqi value = %u\n",port,(unsigned int)sqiGetInfo.sqi);
#endif
    *sqi_value = (uint32_t)sqiGetInfo.sqi;
    return ret;
}

static int rtk9071_phy_reg_read(uint32_t port, uint32_t reg, uint32_t page, uint32_t *value)
{
    
    return 0;
}
static int rtk9071_phy_role_set(uint32_t port,role_e role)
{
    int ret = 0;
    PhyRoleCfgInfo_t pPhyRoleCfgInfo = {0};
    
    if(port > MAX_PORT || role >= ROLE_UNKNOWN)
    {
        printf("param error\n");
        return -1;
    }
    pPhyRoleCfgInfo.port = port;
    pPhyRoleCfgInfo.isMaster = role;
    ret = rtk_phy_phycr_role_set(g_unitChip, &pPhyRoleCfgInfo);
    return ret;
}

static int rtk9071_phy_role_get(uint32_t port,role_e *role)
{
    int ret = 0;
    PhyRoleCfgInfo_t pPhyRoleCfgInfo = {0};
    
    if(port > MAX_PORT || role == NULL)
    {
        printf("param error\n");
        return -1;
    }
    pPhyRoleCfgInfo.port = port;
    pPhyRoleCfgInfo.isMaster = ROLE_UNKNOWN;
    ret = rtk_phy_phycr_role_get(g_unitChip, &pPhyRoleCfgInfo);
#ifdef WTSWITCH_DBG 
    printf("port %d role = %s\n",port,pPhyRoleCfgInfo.isMaster == 1 ? "Master":"Slave");
#endif
    if(pPhyRoleCfgInfo.isMaster == 1)
    {
        *role = ROLE_MASTER;
    }else
    {
        *role = ROLE_SLAVE;
    }
    return ret;
}

static int rtk9071_port_speed_get(uint32_t port, speed_e *speed)
{
    int ret = 0;
    PhySpd_t pPhySpd = {0};

    pPhySpd.port = port;
    ret = rtk_phy_bmcr_speed_get(g_unitChip, &pPhySpd);
#ifdef WTSWITCH_DBG 
    printf("port %d speed = %d\n",port,pPhySpd.speed);
#endif
    *speed = pPhySpd.speed;
    return ret;
}

static int rtk9071_port_speed_set(uint32_t port, speed_e speed)
{
    int ret = 0;
    PhySpd_t pPhySpd = {0};

    pPhySpd.port = port;
    pPhySpd.speed = speed;
    ret = rtk_phy_bmcr_speed_set(g_unitChip, &pPhySpd);    
    return ret;
}

static int rtk9071_port_ango_get(uint32_t port , uint32_t *ango)
{
    int ret = 0;
    PhyAnMode_t pPhyAnMode=  {0};

    pPhyAnMode.portMap = port;
    ret = rtk_phy_bmcr_an_mode_get(g_unitChip, &pPhyAnMode);
#ifdef WTSWITCH_DBG 
    printf("port %d ango = %d\n",port,pPhyAnMode.anEnableCfg);
#endif
    *ango = pPhyAnMode.anEnableCfg;
    return ret;
}

static int rtk9071_port_ango_set(uint32_t port , uint32_t ango)
{
    int ret = 0;
    PhyAnMode_t pPhyAnMode=  {0};

    pPhyAnMode.portMap = port;
    pPhyAnMode.anEnableCfg = ango;
    ret = rtk_phy_bmcr_an_mode_set(g_unitChip, &pPhyAnMode);
    return ret;
}

static int rtk9071_port_duplex_get()
{
    int ret = 0;
    
    return ret;
}

static int rtk9071_firmware_version_get(void)
{
    uint32_t version = 0;
    int ret = 0;
    
    ret = platform_pcie_read(SWC_VERSION_REG,&version);
    if(ret != 0)
    {
        return ret;
    }
    printf("rtk9071 version is 0x%x\n",version);
    return 0;
}

int cvb_spi_flash_update_image(UnitChip_t unitChip,const char *firmware);
static int rtk9071_firmware_version_update(char *firmware_path)
{
    int ret = 0;
    
    if(firmware_path == NULL)
    {
        printf("param error\n");
        return -1;
    }
    ret = cvb_spi_flash_update_image(g_unitChip,firmware_path);
   
    return ret;
}

#define VLAN_VID_MAX (0xfffu)

static int rtk9071_port_pvid_set(uint32_t port , uint32_t pvid)
{
    int ret = 0;
    VlanPortPvidInfo_t vlanPortPvidInfo = {0};
    
    if(port > MAX_PORT || pvid >VLAN_VID_MAX)
    {
        printf("param error\n");
        return -1;
    }

    vlanPortPvidInfo.port = port;
    vlanPortPvidInfo.pvid = pvid;
    vlanPortPvidInfo.priority = 0u;
    ret = rtk_vlan_port_pvid_set(g_unitChip,&vlanPortPvidInfo);
    return ret;
}

static int rtk9071_port_pvid_get(uint32_t port , uint32_t *pvid)
{
    int ret = 0;
    VlanPortPvidInfo_t vlanPortPvidInfo = {0};
    
    if(port > MAX_PORT || pvid == NULL)
    {
        printf("param error\n");
        return -1;
    }

    vlanPortPvidInfo.port = port;
    ret = rtk_vlan_port_pvid_get(g_unitChip,&vlanPortPvidInfo);
    *pvid = vlanPortPvidInfo.pvid;
    return ret;
}
static int rtk9071_vlan_member_get(uint32_t vlan, uint32_t *tagged,uint32_t *untagged)
{
    int ret = 0;
    VlanInfo_t vlanInfo = {0};
        
    if(vlan > VLAN_VID_MAX || tagged == NULL || untagged == NULL)
    {
        printf("param error\n");
        return -1;
    }
    vlanInfo.vid = vlan;
    ret = rtk_vlan_get(g_unitChip, &vlanInfo);
 
    *untagged = vlanInfo.vlanEntry.utagMsk;
    *tagged = vlanInfo.vlanEntry.mbrMsk &(~(vlanInfo.vlanEntry.utagMsk));
    return ret;    
}

static int rtk9071_vlan_create(uint32_t vlan, uint32_t tagged,uint32_t untagged)
{
    int ret = 0;
     VlanInfo_t vlanInfo = {0};
        
    if(vlan > VLAN_VID_MAX)
    {
        printf("param error\n");
        return -1;
    }
    vlanInfo.vid = vlan;
    vlanInfo.vlanEntry.utagMsk = untagged;
    vlanInfo.vlanEntry.mbrMsk = untagged | tagged;
    ret = rtk_vlan_set(g_unitChip, &vlanInfo);
    return ret;
}
static int rtk9071_vlan_destroy(uint32_t vid)
{
    int ret = 0;
    VlanDestroyInfo_t vlanDestroyInfo = {0};

    if(vid > VLAN_VID_MAX)
    {
        printf("param error\n");
        return -1;
    }
    vlanDestroyInfo.vid = vid;
    ret =  rtk_vlan_destroy(g_unitChip, &vlanDestroyInfo);
    return ret;
}

static int rtk9071_vlan_member_add(uint32_t vid, uint32_t port, uint32_t is_tagged)
{
    int ret = 0;
    VlanInfo_t vlanInfo = {0};
    
    if(vid > VLAN_VID_MAX || port > MAX_PORT)
    {
        printf("param error\n");
        return -1;
    }
    vlanInfo.vid = vid;
    ret = rtk_vlan_get(g_unitChip, &vlanInfo);
    if(ret < 0)
    {
        printf("vlan find error.\n");
        return ret;
    }
    if(is_tagged == 1)
    {
        vlanInfo.vlanEntry.mbrMsk |= (1 << port); 
    }else
        vlanInfo.vlanEntry.utagMsk |= port;
  
    ret = rtk_vlan_set(g_unitChip, &vlanInfo);
    
    return ret;
}

static int rtk9071_vlan_member_remove(uint32_t vid, uint32_t port)
{
    int ret = 0;
    VlanInfo_t vlanInfo = {0};
    
    if(vid > VLAN_VID_MAX || port > MAX_PORT)
    {
        printf("param error\n");
        return -1;
    }

    vlanInfo.vid = vid;
    ret = rtk_vlan_get(g_unitChip, &vlanInfo);
    if(ret < 0)
    {
        printf("vlan find error.\n");
        return ret;
    }
    vlanInfo.vlanEntry.mbrMsk &= ~(1 << port);
    vlanInfo.vlanEntry.utagMsk &= ~(1 << port);
    ret = rtk_vlan_set(g_unitChip, &vlanInfo);
    
    return ret;
}
#define L2_TABLE_MAX 4096
static int rtk9071_l2_dump(void)
{
    LutGetInfo_t lutGetInfo = {0};
    int index = 0;

    printf("index\ttype\tmac\t\t\tulfid\tvlanId\tfilterId\tport\tage\n");
    while(index < L2_TABLE_MAX)
    {
        lutGetInfo.index = index;
        rtk_lut_entry_get(g_unitChip, &lutGetInfo);
        switch(lutGetInfo.lutEntry.type)
        {
        case EM_LUT_UNI:
            printf("%4d\t%s\t%02X:%02X:%02X:%02X:%02X:%02X\t%d\t%d\t%d\t\t%d\t%d\n",lutGetInfo.lutEntry.index,"unicast",lutGetInfo.lutEntry.uni.mac[0],lutGetInfo.lutEntry.uni.mac[1],lutGetInfo.lutEntry.uni.mac[2],lutGetInfo.lutEntry.uni.mac[3],lutGetInfo.lutEntry.uni.mac[4],lutGetInfo.lutEntry.uni.mac[5],lutGetInfo.lutEntry.uni.ulfid,lutGetInfo.lutEntry.uni.vid,lutGetInfo.lutEntry.uni.fid,lutGetInfo.lutEntry.uni.spa,lutGetInfo.lutEntry.uni.age);
            break;
        case EM_LUT_MULTI:
            printf("%4d\t%s\t%02X:%02X:%02X:%02X:%02X:%02X\t%s\t%d\t%d\t\t0x%x\t%s\n",lutGetInfo.lutEntry.index,"multicast",lutGetInfo.lutEntry.multi.mac[0],lutGetInfo.lutEntry.multi.mac[1],lutGetInfo.lutEntry.multi.mac[2],lutGetInfo.lutEntry.multi.mac[3],lutGetInfo.lutEntry.multi.mac[4],lutGetInfo.lutEntry.multi.mac[5],"-",lutGetInfo.lutEntry.multi.vid,lutGetInfo.lutEntry.multi.fid,lutGetInfo.lutEntry.multi.mbr,"-");
        default:
            break;
        }
        index++;
    }
    return 0;
}

static int rtk9071_l2_uni_add(l2_entry_t *entry)
{
    int ret = 0;
    LutUniAddInfo_t lutUniAddInfo = {0};

    lutUniAddInfo.port = entry->port;
    lutUniAddInfo.vid = entry->vid;
    lutUniAddInfo.fid = entry->fid;
    memcpy((char *)lutUniAddInfo.mac ,(char *)entry->mac, MAC_ADDR_LEN);
    ret = rtk_lut_uni_entry_add(g_unitChip, &lutUniAddInfo);

    return ret;
}

static int rtk9071_l2_multi_add(l2_multi_entry_t *entry)
{
    int ret = 0;
    LutMultiAddInfo_t lutMultiAddInfo = {0};

    lutMultiAddInfo.vid = entry->vid;
    lutMultiAddInfo.portmask = entry->port_mask;
    lutMultiAddInfo.fid = entry->fid;
    memcpy((char *)lutMultiAddInfo.mac ,(char *)entry->mac, MAC_ADDR_LEN);

    ret = rtk_lut_multi_entry_add(g_unitChip, &lutMultiAddInfo);

    return ret;
}

static int rtk9071_l2_del(l2_entry_t *entry)
{
    int ret = 0;
    LutDeleteInfo_t lutDeleteInfo = {0};

    lutDeleteInfo.vid = entry->vid;
    lutDeleteInfo.fid = entry->fid;
    strncpy((char *)lutDeleteInfo.mac ,(char *)entry->mac, MAC_ADDR_LEN);

    ret = rtk_lut_entry_delete(g_unitChip, &lutDeleteInfo);

    return ret;
}

static int rtk9071_l2_learn_enable(uint32_t portmask, uint32_t enable)
{
    int ret = 0;
    LutMacLrnInfo_t lutMacLrnInfo = {0};
    lutMacLrnInfo.enable = !!enable;
    lutMacLrnInfo.portmask = portmask;
    ret = rtk_lut_mac_lrn_en(g_unitChip,&lutMacLrnInfo);

    return ret;
}

static int rtk9071_cable_get(uint32_t port , cablestatus_e *cable)
{
    int ret = 0;
    RTCTInfo_t rtctInfo = {0};

    rtctInfo.port = port;
    rtk_rtct_run_cable_diag(g_unitChip,&rtctInfo);
    sleep(1);
    ret = rtk_rtct_get_cable_status(g_unitChip,&rtctInfo);
    *cable =(cablestatus_e) rtctInfo.status;

    return ret;
}

static int rtk9071_phy_test_mode_set(uint32_t port, uint8_t testMode, uint8_t is_master)
{
    int ret = 0;
    PhyTestModeInfo_t phyTMInfo = {0};
    phyTMInfo.port = port;
    phyTMInfo.speed = 1;
    phyTMInfo.testMode = testMode;
    phyTMInfo.isMaster = is_master;
    ret = rtk_phy_testmode_set(g_unitChip,&phyTMInfo);
    return ret;
}
static int rtk9071_port_vlan_double_tag_set(uint32_t port,vlan_accept_double_tag_e vlan_accept_double_tag)
{
    int ret = 0;
    VlanPortAcptDoubleTagInfo_t vlanPortAcptDoubleTagInfo = {0};

    vlanPortAcptDoubleTagInfo.acceptDoubleTag = (VlanAcceptDoubleTag_e)vlan_accept_double_tag;
    vlanPortAcptDoubleTagInfo.port = port;
    ret = rtk_vlan_port_acpt_double_tag_set(g_unitChip, &vlanPortAcptDoubleTagInfo);
    return ret;
}
static int rtk9071_port_vlan_double_tag_get(uint32_t port,vlan_accept_double_tag_e *vlan_accept_double_tag)
{
    int ret = 0;
    VlanPortAcptDoubleTagInfo_t vlanPortAcptDoubleTagInfo = {0};

    vlanPortAcptDoubleTagInfo.port = port;
    ret = rtk_vlan_port_acpt_double_tag_get(g_unitChip, &vlanPortAcptDoubleTagInfo);
    *vlan_accept_double_tag =(vlan_accept_double_tag_e)vlanPortAcptDoubleTagInfo.acceptDoubleTag;
    return ret;
}
static tswitch_t rtk9071_switch =
{
    .sdk_init = rtk9071_sdk_init,
    .sdk_deinit = rtk9071_sdk_deinit,
    .port_link_set = rtk9071_port_link_set,
    .port_link_get = rtk9071_port_link_get,
    .port_sqi_get = rtk9071_port_sqi_get,
    .port_mib_dump = rtk9071_port_mib_dump,
    .port_mib_clear = rtk9071_port_mib_reset,
    .port_speed_get = rtk9071_port_speed_get,
    .port_speed_set = rtk9071_port_speed_set,
    .port_ango_get = rtk9071_port_ango_get,
    .port_ango_set = rtk9071_port_ango_set,
    .port_pvid_set = rtk9071_port_pvid_set,
    .port_pvid_get = rtk9071_port_pvid_get,
    .port_cable_get = rtk9071_cable_get,
    .port_vlan_double_tag_set = rtk9071_port_vlan_double_tag_set,
    .port_vlan_double_tag_get = rtk9071_port_vlan_double_tag_get,
    .vlan_member_get = rtk9071_vlan_member_get,
    .vlan_create = rtk9071_vlan_create,
    .vlan_destroy = rtk9071_vlan_destroy,
    .vlan_member_add = rtk9071_vlan_member_add,
    .vlan_member_remove = rtk9071_vlan_member_remove,
    .phy_role_get = rtk9071_phy_role_get,
    .phy_role_set = rtk9071_phy_role_set,
    .phy_test_mode_set = rtk9071_phy_test_mode_set,
    .switch_reg_read = platform_pcie_read,
    .firmware_version_get = rtk9071_firmware_version_get,
    .firmware_version_update = rtk9071_firmware_version_update,
    .l2_entry_dump = rtk9071_l2_dump,
    .l2_uni_add = rtk9071_l2_uni_add,
    .l2_multi_add = rtk9071_l2_multi_add,
    .l2_entry_del = rtk9071_l2_del,
    .l2_learn_enable = rtk9071_l2_learn_enable,
};
