#include <stddef.h>
#include <stdio.h>
#include <autoconf.h>

void rtl9071_switch_init(void) {
    #ifdef CONFIG_USR_SWITCH_RTL9071
    extern void hw_common_init(void);
    hw_common_init();
    printf("RTL9071 switch initialized (MT8678)!\n");
    #endif
}
