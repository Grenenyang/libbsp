#include "core.h"
#include "interface.h"
#include "switch.h"
#ifdef CONFIG_CLASS_SWITCH
struct hw_class class_switch = {
	.name = "class_switch"
};
int bsp_switch_sdk_init(handle_t handle)
{
    return BSP_INTERFACE_UNLOCKED(switch,sdk_init,handle);
}
#else
int bsp_switch_sdk_init(handle_t handle)
{
    HW_WARN("%s not support\n",__FUNCTION__);
    return 0;
}
#endif
