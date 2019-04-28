
#include "error.h"
#include "include.h"
#include "arm_arch.h"
#include "gpio_pub.h"
#include "uart_pub.h"
#include "music_msg_pub.h"
#include "co_list.h"
#include "saradc_pub.h"
#include "temp_detect_pub.h"
#include "sys_rtos.h"
#include "rtos_pub.h"
#include "saradc_intf.h"
#include "pwm_pub.h"
#include "pwm.h"
#include <stdint.h>
#include <stdlib.h>
#include <finsh.h>

#define LED_TIMER_PWM_ID 1
#define LED_TIMER_PERIOD 4 /*4ms*/
#define LED_MATRIX_COLUMN_NUM 5



int hal_pwm_stop(void)
{
	int ret =LED_TIMER_PWM_ID;
	

	ret = pwm_ctrl(CMD_PWM_UINT_DISABLE, (void*)&ret);
	if (ret != 0) {
		rt_kprintf("stop pwm%d failed!\n", LED_TIMER_PWM_ID);
		return -1;
	}
	
	return 0;
}

void all_led_off(void)
{
	hal_pwm_stop();
}

int deep_sleep_mode(void)
{
	uint32_t gpio_index_map = 0x1C;
	uint32_t gpio_edge_map = 0;
	uint32_t gpio_index, gpio_pull;
	rt_kprintf("enter bk_enter_deep_sleep: 0x%08X 0x%08X\n\n", gpio_index_map, gpio_edge_map);
	bk_enter_deep_sleep(gpio_index_map, gpio_edge_map);

	return 0;
}

void key_unconfig(void);

void enter_deep_sleep_mode(void)
{
	rt_thread_sleep(200);
	
	UINT32 gpio_index_map = 0;
	UINT32 gpio_edge_map = 0;
	UINT32 gpio_index, gpio_pull;
	rt_kprintf("enter into sleep mdoe \r\n");
	all_led_off(); 						/*close all led*/
	rt_thread_sleep(200);
	audio_dac_eable_mute(1);
	key_uninitialization();				/*uninitial key*/	
	saradc_disable_vddram_voltage();	/*disable saradc*/
	
	deep_sleep_mode();
}

FINSH_FUNCTION_EXPORT_ALIAS(enter_deep_sleep_mode, __cmd_sleep_mode, test sleep mode);


