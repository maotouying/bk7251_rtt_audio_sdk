
#ifndef __I2S_H__
#define __I2S_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif
#define RT_I2S_DEBUG


struct rt_i2s_msg
{
    rt_uint16_t len;
    rt_uint8_t  *buf;
};

struct rt_i2s_bus_device;

struct rt_i2s_bus_device_ops
{
    rt_size_t (*master_xfer)(struct rt_i2s_bus_device *bus,
                             struct rt_i2s_msg msgs[],
                             rt_uint32_t num);
    rt_size_t (*slave_xfer)(struct rt_i2s_bus_device *bus,
                            struct rt_i2s_msg msgs[],
                            rt_uint32_t num);
    rt_err_t (*i2c_bus_control)(struct rt_i2s_bus_device *bus,
                                rt_uint32_t,
                                rt_uint32_t);
};

/*for i2c bus driver*/
struct rt_i2s_bus_device
{
    struct rt_device parent;
    const struct rt_i2s_bus_device_ops *ops;
    rt_uint16_t  flags;
    rt_uint16_t  addr;
    struct rt_mutex lock;
    rt_uint32_t  timeout;
    rt_uint32_t  retries;
    void *priv;
};

#ifdef RT_I2S_DEBUG
#define i2s_dbg(fmt, ...)   rt_kprintf(fmt, ##__VA_ARGS__)
#else
#define i2s_dbg(fmt, ...)
#endif

rt_err_t rt_i2s_bus_device_register(struct rt_i2s_bus_device *bus,
                                    const char               *bus_name);
struct rt_i2s_bus_device *rt_i2s_bus_device_find(const char *bus_name);
rt_size_t rt_i2s_transfer(struct rt_i2s_bus_device *bus,
                          struct rt_i2s_msg         msgs[],
                          rt_uint32_t               num);
rt_size_t rt_i2s_master_send(struct rt_i2s_bus_device *bus,
                             rt_uint16_t               addr,
                             rt_uint16_t               flags,
                             const rt_uint8_t         *buf,
                             rt_uint32_t               count);
rt_size_t rt_i2s_master_recv(struct rt_i2s_bus_device *bus,
                             rt_uint16_t               addr,
                             rt_uint16_t               flags,
                             rt_uint8_t               *buf,
                             rt_uint32_t               count);
int rt_i2s_core_init(void);

#ifdef __cplusplus
}
#endif

#endif
