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

#ifndef __BEKENDRIVERPWM_H__
#define __BEKENDRIVERPWM_H__

#pragma once
#include "pwm_pub.h"
#include "include.h"

typedef enum
{
	PWM_CHANNEL0_P6= 0,
	PWM_CHANNEL1_P7,
	PWM_CHANNEL2_P8,
	PWM_CHANNEL3_P9,
	PWM_CHANNEL4_P24,
	PWM_CHANNEL5_P26,
    BK_PWM_MAX, /* Denotes the total number of PWM port aliases. Not a valid PWM alias */
} pwm_channel_t;


OSStatus bk_pwm_initialize(pwm_param_t *pwm, pwm_channel_t channel,PFUNC p_int_func,UINT8 src_clk);
OSStatus bk_pwm_set_duty_end(pwm_param_t *pwm,UINT32 duty_cycle,UINT32 end_value);
OSStatus bk_pwm_enable(pwm_param_t *pwm,UINT8 enable);

#endif
