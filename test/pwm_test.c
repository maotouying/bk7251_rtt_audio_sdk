#include "rtos_pub.h"
#include "BkDriverPwm.h"
#include "error.h"
#include <stdint.h>
#include <stdlib.h>
#include <finsh.h>

#define CONFIG_PWM_TEST

#ifdef CONFIG_PWM_TEST

static pwm_param_t test_pwm[BK_PWM_MAX];
static UINT8 print_cnt[BK_PWM_MAX]={0};
static void test_pwm_isr(unsigned char param)
{
	if(print_cnt[param]++ >= 20)
	{
		rt_kprintf("---pwm isr channel:%d---\r\n",param);
		print_cnt[param] = 0;
	}
}
/*
param:	duty & end-- unit is 1/26MHz = 38ns
   |<---------end-------->|
   |<- duty->|            

    ---------			  |
   |    	 |            |
   |    	 |            |
   |         |            |
	    	  ------------
*/

//test_pwm_init channel int_enable duty end
static void test_pwm_init(int argc,char *argv[])
{
	UINT32 channel,duty,end,int_enable;
	if(argc != 5)
		return;
	channel = atoi(argv[1]);
	int_enable = atoi(argv[2]);
	duty = atoi(argv[3]);
	end = atoi(argv[4]);

	if(end < duty)
		return;	
	
	if(int_enable)
	{
		bk_pwm_initialize(&test_pwm[channel],(pwm_channel_t)channel,test_pwm_isr,1);
	}
	else
	{
		bk_pwm_initialize(&test_pwm[channel],(pwm_channel_t)channel,NULL,1);
	}
	bk_pwm_set_duty_end(&test_pwm[channel],duty,end);
	bk_pwm_enable(&test_pwm[channel],1);
}
MSH_CMD_EXPORT(test_pwm_init,pwm init);


//test_pwm_change_param channel duty value
//test_pwm_change_param channel endvalue value
static void test_pwm_change_param(int argc,char*argv[])
{
	uint32 val,channel;
	if(argc != 4)
		return;
	channel = atoi(argv[1]);
	val = atoi(argv[3]);

	if(0 == strcmp(argv[2],"duty"))
	{
		test_pwm[channel].duty_cycle = val;
	}
	else if(0 == strcmp(argv[2],"endvalue"))
	{
		test_pwm[channel].end_value = val;
	}
	else
		return;

	bk_pwm_set_duty_end(&test_pwm[channel],test_pwm[channel].duty_cycle,test_pwm[channel].end_value);
}
MSH_CMD_EXPORT(test_pwm_change_param,pwm change param);

static void test_pwm_enable(int argc,char*argv[])
{
	uint16 channel;
	uint8 enable;
	if(argc != 3)
		return;
	
	channel = atoi(argv[1]);
	enable = atoi(argv[2]);

	bk_pwm_enable(&test_pwm[channel],enable);
}
MSH_CMD_EXPORT(test_pwm_enable,pwm enable);
#endif