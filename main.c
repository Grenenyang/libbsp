#include <stdio.h>
int wt_tools_init(void);
int bsp_deinit();
int bsp_switch_sdk_init(int handle);
int main()
{
	wt_tools_init();
	bsp_deinit();
	return 0;
}
