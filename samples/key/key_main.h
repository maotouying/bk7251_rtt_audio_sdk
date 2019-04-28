#ifndef _KEY_MAIN_H_
#define _KEY_MAIN_H_

#include "gpio_pub.h"
#include "uart_pub.h"

#define KEY_DEBUG

#ifdef KEY_DEBUG
#define KEY_PRT                 os_printf
#define KEY_WPRT                warning_prf
#else
#define KEY_PRT                 os_null_printf
#define KEY_WPRT                os_null_printf
#endif

typedef enum AD_KEY_ITEM_T
{
	AD_KEY_TEST0 = 0,
	AD_KEY_TEST1 = 1,
}AD_KEYITEM;

#endif // 
