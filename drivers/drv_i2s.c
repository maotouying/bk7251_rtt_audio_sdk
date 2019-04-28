#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>

#include "typedef.h"
#include "sys_config.h"
#include "rtos_pub.h"


#include "beken378\driver\include\i2s_pub.h"
#include "i2s.h"

#include "drv_i2s.h"


#ifdef  RT_I2S_TRANS_DEBUG
#define i2s_trans_dbg(fmt, ...)   rt_kprintf(fmt, ##__VA_ARGS__)
#else
#define i2s_trans_dbg(fmt, ...)
#endif

rt_size_t rt_i2s_transfer(struct rt_i2s_bus_device *bus,
								struct rt_i2s_msg   msgs[],
								rt_uint32_t          num )
{
	rt_size_t ret;
	if(bus->ops->master_xfer)
	{
		for(ret= 0; ret<num; ret++)
		{
			i2s_trans_dbg("msgs[%d]，len=%d\r\n",ret,msgs[ret].len);

		}
	 rt_mutex_take(&bus->lock, RT_WAITING_FOREVER);
     ret = bus->ops->master_xfer(bus, msgs, num);
     rt_mutex_release(&bus->lock);

        return ret;
	}
	else
	{
		rt_kprintf("---I2S bus ops not supptoted\r\n");
		
		return RT_NULL;
	}

	return ret ;
}
static rt_err_t rt_i2s_init(rt_device_t dev)
{
	i2s_init();
	
	return RT_EOK;

}

static rt_err_t rt_i2s_open(void)
{	

	return RT_EOK;

}

static rt_err_t rt_i2s_close(rt_device_t dev)
{
	return RT_EOK;
}


static rt_err_t rt_i2s_read(rt_device_t dev, rt_off_t pos, void*buffer, rt_size_t size)
{
	return 0;
}


static rt_err_t rt_i2s_write(rt_device_t dev, rt_off_t pos, void*buffer, rt_size_t size )
{
	return 0;
}


static rt_err_t rt_i2s_cotrol(rt_device_t dev)
{
	return RT_EOK;
}

#ifdef RT_USING_DEVICE_OPS
static const struct rt_device_ops i2s_ops =
{
    rt_i2s_init,
    rt_i2s_open,
    rt_i2s_close,
    rt_i2s_read,
    rt_i2s_write,
    rt_i2s_cotrol
};
#endif /* RT_USING_DEVICE_OPS */



static struct rt_device g_i2s_test;

int  rt_i2s_hw_init(void)
{
	struct rt_device *device = & g_i2s_test;


    /* set device type */
    device->type = RT_Device_Class_I2SBUS;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;
    device->user_data   = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    device->ops = &i2s_ops;
#else
    device->control = rt_i2s_cotrol;
    device->init    = rt_i2s_init;
    device->open    = rt_i2s_open;
    device->close   = rt_i2s_close;
    device->read    = rt_i2s_read;
    device->write   = rt_i2s_write;
#endif /* RT_USING_DEVICE_OPS */	

    /* register to device manager */

	rt_device_register(device, "i2s", RT_DEVICE_FLAG_RDWR);

	rt_kprintf("---i2s register over---\r\n");

	return RT_EOK;

}

INIT_DEVICE_EXPORT(rt_i2s_hw_init);





