#include <stddef.h>
#include <stdio.h>
#include <autoconf.h>

void jl3xxx_phy_init(void) {
    #ifdef CONFIG_USR_PHY_JL3XXX
    extern void hw_common_init(void);
    hw_common_init();
    printf("JL3XXX PHY initialized (MT8678)!\n");
    #endif
}
