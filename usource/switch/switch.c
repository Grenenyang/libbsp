#include "core.h"
#include "interface.h"
#include "switch.h"
struct hw_class class_switch = {
	.name = "class_switch"
};
int bsp_switch_sdk_init(handle_t handle)
{
    BSP_INTERFACE_UNLOCKED(switch,sdk_init,handle);
}
