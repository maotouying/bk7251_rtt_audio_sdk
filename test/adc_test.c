#include "error.h"
#include "include.h"

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include <stdint.h>
#include <stdlib.h>
#include <finsh.h>
#include <rtdef.h>

#include "saradc_intf.h"
#include "sys_ctrl_pub.h"

#define CONFIG_ADC_TEST

#ifdef CONFIG_ADC_TEST

#define ADC_CHANNEL0_VBAT	0
#define ADC_CHANNEL1_P4 	1
#define ADC_CHANNEL2_P5 	2
#define ADC_CHANNEL3_P23 	3
#define ADC_CHANNEL4_P2		4
#define ADC_CHANNEL5_P3 	5
#define ADC_CHANNEL6_P12 	6
#define ADC_CHANNEL7_P13 	7


#define VBAT_COUNT_NUMBER                   (10)

typedef struct _vbat_detect_ {
    int tmp_vol;
	int mean_vol;
	int detect_cnt;
} VBAT_DETECT_ST;


static ADC_OBJ vbat;
static VBAT_DETECT_ST vbat_det_st;

static ADC_OBJ other_channel_adc;

int adkey_adc_get_value(void)
{
	int mv;
	mv= (int)other_channel_adc.user_data;
	return mv;
}

/****channel 1 - 7***/
void other_channel_detect_callback(int new_mv, void *user_data)
{
	static int cnt = 0;
	other_channel_adc.user_data = (void*)new_mv;

	if(cnt++ >= 100)
	{	
		cnt = 0;
    	rt_kprintf("-----adc channel%d voltage:%d,%x-----\r\n",other_channel_adc.channel,new_mv,other_channel_adc.user_data);
	}
}

void other_channel_detect_config(UINT32 channel)
{
	adc_obj_init(&other_channel_adc, other_channel_detect_callback, channel, &other_channel_adc);
    adc_obj_start(&other_channel_adc);
}

void adc_channel_test(int argc,char *argv[])
{
	int channel;
	if(argc == 2)
	{
		channel = atoi(argv[1]);
		rt_kprintf("---adc channel:%d---\r\n",channel);
		saradc_config_vddram_voltage(PSRAM_VDD_1_8V);
		other_channel_detect_config(channel);
	}
}

MSH_CMD_EXPORT(adc_channel_test,adc test);


//channe vbat
static void vbat_adc_detect_callback(int new_mv, void *user_data)
{
    VBAT_DETECT_ST *vbat = (VBAT_DETECT_ST *)user_data;
	static int cnt = 0;
    new_mv = new_mv << 1;//for vbat channel,need to *2
	int tmp;
	vbat_det_st.tmp_vol += new_mv;
	if(++vbat_det_st.detect_cnt < VBAT_COUNT_NUMBER)
	{
		return ;
	}
	
	vbat->mean_vol = vbat_det_st.tmp_vol/VBAT_COUNT_NUMBER;
	if(cnt ++ >=50)
	{
		cnt = 0;
		rt_kprintf("---vbat voltage:%d---\r\n",vbat->mean_vol);
	}
	vbat_det_st.tmp_vol = 0;
	vbat_det_st.detect_cnt = 0;
}

static void vbat_detect_config(void)
{
    vbat_det_st.tmp_vol = 0;
    vbat_det_st.mean_vol = 0;
	vbat_det_st.detect_cnt = 0;

    adc_obj_init(&vbat, vbat_adc_detect_callback,ADC_CHANNEL0_VBAT, &vbat_det_st);
    adc_obj_start(&vbat);
}

int start_adc_test(void)
{
	saradc_work_create(20);
	vbat_detect_config();
	return 0;
}
INIT_APP_EXPORT(start_adc_test);
#endif
