#include "core.h"
#include "interface.h"
#include "switch.h"
#include "autoconf.h"
#ifdef CONFIG_CLASS_SWITCH
struct hw_class class_switch = {
	.name = "class_switch"
};
int bsp_switch_sdk_init(handle_t handle)
{
    return BSP_INTERFACE_UNLOCKED(switch,sdk_init,handle);
}

int bsp_switch_reg_read(handle_t handle,uint32_t regAddr, uint32_t *value)
{
    return BSP_INTERFACE_UNLOCKED(switch,switch_reg_read,handle,regAddr, value);
}

int bsp_switch_reg_write(handle_t handle,uint32_t regAddr, uint32_t value)
{
    return BSP_INTERFACE_UNLOCKED(switch,switch_reg_write,handle,regAddr, value);
}

int bsp_switch_port_link_get(handle_t handle,uint32_t port,link_status_e *value)
{
    return BSP_INTERFACE_UNLOCKED(switch,port_link_get,handle,port,value);
}
#else
int bsp_switch_sdk_init(handle_t handle)
{
    HW_WARN("%s not support\n",__FUNCTION__);
    return 0;
}

int bsp_switch_reg_read(handle_t handle,uint32_t regAddr, uint32_t *value)
{
      HW_WARN("%s not support\n",__FUNCTION__);
      return 0;
}

int bsp_switch_reg_write(handle_t handle,uint32_t regAddr, uint32_t value)
{
      HW_WARN("%s not support\n",__FUNCTION__);
      return 0;
}

int bsp_switch_port_link_get(handle_t handle,uint32_t port,link_status_e *value)
{
      HW_WARN("%s not support\n",__FUNCTION__);
      return 0;
}
#endif
