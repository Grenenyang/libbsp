#ifndef __CORE_EXPORT_H
#define __CORE_EXPORT_H
#include <stdint.h>
#ifndef __HANDLE_T_
#define __HANDLE_T_
typedef uintptr_t handle_t;
#endif
/**
 * @brief bsp_init must be init
 * @return 0:success
 *         other:failed
 */
int bsp_init(void);

/**
 * @brief bsp_deinit must be deinit
 * @return 0:success
 *         other:failed
 */
int bsp_deinit(void);
#endif
