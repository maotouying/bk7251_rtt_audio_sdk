#ifndef __DRV_I2S_H__
#define __DRV_I2S_H__

#include "typedef.h"
#include "i2s.h"
#include "i2s_pub.h"
#include "gpio_pub.h"
static rt_err_t rt_i2s_init(rt_device_t dev);
static rt_err_t rt_i2s_open(void);
static rt_err_t rt_i2s_close(rt_device_t dev);
static rt_err_t rt_i2s_read(rt_device_t dev, rt_off_t pos, void*buffer, rt_size_t size);
static rt_err_t rt_i2s_write(rt_device_t dev, rt_off_t pos, void*buffer, rt_size_t size );
static rt_err_t rt_i2s_cotrol(rt_device_t dev);


int rt_i2s_hw_init(void);



#endif
