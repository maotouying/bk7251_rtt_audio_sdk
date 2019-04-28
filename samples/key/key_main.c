#include "include.h"
#include "typedef.h"
#include "arm_arch.h"
#include "gpio_pub.h"
#include "key_main.h"
#include "multi_button.h"
#include "rtos_pub.h"
#include "error.h"
#include "sys_ctrl_pub.h"

#define KEY_FUNC_TEST

#ifdef KEY_FUNC_TEST
#define TEST_KEY_GPIO1 4
#define TEST_KEY_GPIO2 5

static beken_timer_t g_key_timer;
BUTTON_S gpio_button_test[2];
BUTTON_S ad_button_test[2];

/////----gpio key----//////
static void gpiokey0_short_press(void *param)
{
	KEY_PRT("gpiokey0_short_press\r\n");
}

static void gpiokey0_double_press(void *param)
{
	KEY_PRT("gpiokey0_double_press\r\n");
}

static void gpiokey0_long_press_hold(void *param)
{
	KEY_PRT("gpiokey1_long_press_hold\r\n");
}
static void gpiokey1_short_press(void *param)
{
	KEY_PRT("gpiokey1_short_press\r\n");
}

static void gpiokey1_double_press(void *param)
{
	KEY_PRT("gpiokey1_double_press\r\n");
}

static void gpiokey1_long_press_hold(void *param)
{
	KEY_PRT("gpiokey0_long_press_hold\r\n");
}

static uint8_t key_get_gpio_level(BUTTON_S*handle)
{
	return bk_gpio_input((uint32_t)handle->user_data);
}
/*gpio key config:input && pull up*/
void gpio_key_item_configure(int gpio1,int gpio2)
{
	gpio_config(GPIO4,GMODE_INPUT_PULLUP);
	button_init(&gpio_button_test[0], key_get_gpio_level, 0,(void*)gpio1);
	button_attach(&gpio_button_test[0], SINGLE_CLICK,     gpiokey0_short_press);
	button_attach(&gpio_button_test[0], DOUBLE_CLICK,     gpiokey0_double_press);
	button_attach(&gpio_button_test[0], LONG_PRESS_HOLD,  gpiokey0_long_press_hold);
	button_start(&gpio_button_test[0]);
	
	gpio_config(GPIO5,GMODE_INPUT_PULLUP);
	button_init(&gpio_button_test[1], key_get_gpio_level, 0,(void*)gpio2);
	button_attach(&gpio_button_test[1], SINGLE_CLICK,     gpiokey1_short_press);
	button_attach(&gpio_button_test[1], DOUBLE_CLICK,     gpiokey1_double_press);
	button_attach(&gpio_button_test[1], LONG_PRESS_HOLD,  gpiokey1_long_press_hold);
	button_start(&gpio_button_test[1]);
}
/////////////////////////////


/////----adc key----//////
static void adkey0_short_press(void *param)
{
	KEY_PRT("adkey0_short_press\r\n");
}

static void adkey0_double_press(void *param)
{
	KEY_PRT("adkey0_double_press\r\n");
}

static void adkey0_long_press_hold(void *param)
{
	KEY_PRT("adkey0_long_press_hold\r\n");
}
static void adkey1_short_press(void *param)
{
	KEY_PRT("adkey1_short_press\r\n");
}

static void adkey1_double_press(void *param)
{
	KEY_PRT("adkey1_double_press\r\n");
}

static void adkey1_long_press_hold(void *param)
{
	KEY_PRT("adkey1_long_press_hold\r\n");
}

extern int adkey_adc_get_value(void);
extern void saradc_config_vddram_voltage(UINT32 vol);
extern void other_channel_detect_config(UINT32 channel);

static uint8_t key_adc_get_level(BUTTON_S*handle)
{
	AD_KEYITEM key;
	int adc_val;
	uint8 level = 1;
	
	key = (AD_KEYITEM)handle->user_data;
	adc_val = adkey_adc_get_value();
	switch(key)
	{
		case AD_KEY_TEST0:
			if((adc_val >1000)&&(adc_val < 1400))
			{
				level = 0;
			}
			break;
		case AD_KEY_TEST1:
			if((adc_val >1500)&&(adc_val<1900))
			{
				level = 0;
			}
			break;
			
		default:
			break;
	}
	return level;
}

void adc_key_item_configure(uint32_t channel)
{
	saradc_config_vddram_voltage(PSRAM_VDD_1_8V);
	other_channel_detect_config(channel);

	button_init(&ad_button_test[0], key_adc_get_level, 0,(void*)AD_KEY_TEST0);
	button_attach(&ad_button_test[0], SINGLE_CLICK,     adkey0_short_press);
	button_attach(&ad_button_test[0], DOUBLE_CLICK,     adkey0_double_press);
	button_attach(&ad_button_test[0], LONG_PRESS_HOLD,  adkey0_long_press_hold);
	button_start(&ad_button_test[0]);

	button_init(&ad_button_test[1], key_adc_get_level, 0,(void*)AD_KEY_TEST1);
	button_attach(&ad_button_test[1], SINGLE_CLICK,     adkey1_short_press);
	button_attach(&ad_button_test[1], DOUBLE_CLICK,     adkey1_double_press);
	button_attach(&ad_button_test[1], LONG_PRESS_HOLD,  adkey1_long_press_hold);
	button_start(&ad_button_test[1]);
}
/////////////////////////////////////////////


void key_configure(int adc_or_gpio,uint32_t adc_channel,uint32_t gpio1,uint32_t gpio2)
{
	OSStatus result;
	if(adc_or_gpio)
	{
		if((gpio1 >=40)||(gpio1 >= 40))
		{
			rt_kprintf("gpio error\r\n");
			return;
		}
		gpio_key_item_configure(gpio1,gpio2);
	}
	else
	{
		if((adc_channel == 0)|| (adc_channel > 7))
		{
			rt_kprintf("channel error\r\n");
			return;
		}
		adc_key_item_configure(adc_channel);
	}

    result = rtos_init_timer(&g_key_timer,
                            TICKS_INTERVAL,
                            button_ticks,
                            (void *)0);
    ASSERT(kNoErr == result);
	
    result = rtos_start_timer(&g_key_timer);
    ASSERT(kNoErr == result);
}

void key_unconfig(void)
{
	OSStatus ret;
	
	if(rtos_is_timer_init(&g_key_timer))
	{
	    if (rtos_is_timer_running(&g_key_timer)) 
		{
	        ret = rtos_stop_timer(&g_key_timer);
			ASSERT(kNoErr == ret);
	    }

	    ret = rtos_deinit_timer(&g_key_timer);
		ASSERT(kNoErr == ret);
	}
}
/*
		    cmd parameter
GPIO KEY: 	0 adc_channel 0
ADC KEY: 	1 gpio1 gpio2
*/
void key_initialization(int argc,char *argv[])
{
	int flag = 0;
	uint32_t adc_channel,gpio1,gpio2;
	if(argc == 4)
	{
		rt_kprintf("=== key  initailization ===\r\n");
		flag = atoi(argv[1]);
		adc_channel = atoi(argv[2]);
		gpio1 = atoi(argv[2]);
		gpio2 = atoi(argv[3]);
		key_configure(flag,adc_channel,gpio1,gpio2);
	}
}
void gpio_key_init(void)
{
	gpio_key_item_configure(TEST_KEY_GPIO1,TEST_KEY_GPIO2);
}

void adc_key_init(void)
{
	adc_key_item_configure(2);// channel2 --- gpio5
}
void key_uninitialization(void)
{
	key_unconfig();
}
MSH_CMD_EXPORT(key_initialization,adc or gpio key);
//INIT_APP_EXPORT(gpio_key_init);
//INIT_APP_EXPORT(adc_key_init);

// eof
#endif
