#include "error.h"
#include "include.h"

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include <stdint.h>
#include <stdlib.h>
#include <finsh.h>
#include <rtdef.h>

#include "manual_ps_pub.h"
#include "bk_rtos_pub.h"

static void enter_deep_sleep_test(int argc,char *argv[])
{
	bk_rtos_delay_milliseconds(10);
	PS_DEEP_CTRL_PARAM deep_sleep_param;

	deep_sleep_param.wake_up_way			= 0;
	
	deep_sleep_param.gpio_index_map      	= atoi(argv[1]);
	deep_sleep_param.gpio_edge_map       	= atoi(argv[2]);	
	deep_sleep_param.gpio_last_index_map 	= atoi(argv[3]);
	deep_sleep_param.gpio_last_edge_map  	= atoi(argv[4]);
	deep_sleep_param.sleep_time     		= atoi(argv[5]);
	deep_sleep_param.wake_up_way     		= atoi(argv[6]);

	if(argc == 7)
	{		
		rt_kprintf("---deep sleep test param : 0x%0X 0x%0X 0x%0X 0x%0X %d %d\r\n", 
					deep_sleep_param.gpio_index_map, 
					deep_sleep_param.gpio_edge_map,
					deep_sleep_param.gpio_last_index_map, 
					deep_sleep_param.gpio_last_edge_map,
					deep_sleep_param.sleep_time,
					deep_sleep_param.wake_up_way);
		
		bk_enter_deep_sleep_mode(&deep_sleep_param);
	}
	else
	{
		rt_kprintf("---argc error!!! \r\n");
	}
}

FINSH_FUNCTION_EXPORT_ALIAS(enter_deep_sleep_test, __cmd_sleep_mode, test sleep mode);
