/**
 ******************************************************************************
 * @file    BkDriverPwm.h
 * @brief   This file provides all the headers of PWM operation functions.
 ******************************************************************************
 *
 *  The MIT License
 *  Copyright (c) 2017 BEKEN Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is furnished
 *  to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 */
#include "include.h"
#include "rtos_pub.h"
#include "BkDriverPwm.h"
#include "drv_model_pub.h"
#include "error.h"
#include "pwm_pub.h"


OSStatus bk_pwm_initialize(pwm_param_t *pwm, pwm_channel_t channel,PFUNC p_int_func,UINT8 src_clk)
{
	UINT32 ret = kGeneralErr;
	if(channel >= BK_PWM_MAX)
	{
		rt_kprintf("pwm channel invalid!\r\n");
		return ret;
	}
	if(pwm == RT_NULL)
	{
		rt_kprintf("no pwm param!\r\n");
		return ret;
	}
	
	/*init pwm*/
	pwm->channel = (UINT8)channel;
	pwm->cfg.bits.en = PWM_INT_DIS;
	pwm->cfg.bits.mode   = PMODE_PWM;
	pwm->p_Int_Handler = p_int_func;
	pwm->cfg.bits.int_en = PWM_INT_DIS;
	
	if(src_clk)
		pwm->cfg.bits.clk	  = PWM_CLK_26M;
	else
		pwm->cfg.bits.clk	  = PWM_CLK_32K;

	ret = sddev_control(PWM_DEV_NAME, CMD_PWM_INIT_PARAM, pwm);
	return ret;
}

OSStatus bk_pwm_set_duty_end(pwm_param_t *pwm,UINT32 duty_cycle,UINT32 end_value)
{
	UINT32 ret = kGeneralErr;
	if(pwm == RT_NULL)
	{
		rt_kprintf("no pwm param!\r\n");
		return ret;
	}
	if(duty_cycle > end_value)
	{
		rt_kprintf("param invalid!\r\n");
		return ret;
	}
	pwm->duty_cycle = duty_cycle;
	pwm->end_value = end_value;
	ret = sddev_control(PWM_DEV_NAME, CMD_PWM_INIT_PARAM, pwm);
	return ret;
}

OSStatus bk_pwm_enable(pwm_param_t *pwm,UINT8 enable)
{
	UINT32 ret = kGeneralErr;
	if(pwm == RT_NULL)
	{
		rt_kprintf("no pwm param!\r\n");
		return ret;
	}
	
    if (enable)
    {
    	pwm->cfg.bits.en     = PWM_ENABLE;
		if(pwm->p_Int_Handler != NULL)
			pwm->cfg.bits.int_en = PWM_INT_EN;
    }
    else
    {
    	pwm->cfg.bits.int_en = PWM_INT_DIS;
        pwm->cfg.bits.en     = PWM_DISABLE;
    }
	ret = sddev_control(PWM_DEV_NAME, CMD_PWM_INIT_PARAM, pwm);
	return ret;
}

// eof

