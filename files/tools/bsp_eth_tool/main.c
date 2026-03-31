#include<stdio.h>
#include"libbsp.h"
int main()
{
    int ret = 0;
    unsigned int value = 0;
    link_status_e status =   LINK_UNKNOWN;
    ret = bsp_init();
    if(ret < 0)
    {
        printf("bsp_init error\n");
        return -1;
    }
    ret = bsp_switch_sdk_init(0);
    if(ret < 0)
    {
        printf("bsp_switch_sdk_init error.ret:%d\n",ret);
        return -1;
    }
    ret = bsp_switch_port_link_get(0,5,&status);
    printf("status:%x\n",status);
    ret = bsp_deinit();
    if(ret < 0)
    {
        printf("bsp_deinit error\n");
        return -1;
    }
}
