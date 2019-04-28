#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "player_system.h"
#include "player.h"
#include "audio_codec.h"
#include "audio_stream.h"
#include "netstream.h"
#include "stream_pipe.h"
#include "netstream_buffer.h"

extern struct stream_pipe * netstream_get_pipe(void);

#include <finsh.h>
int stream_buffer(int argc, char **argv)
{
	struct stream_pipe *pipe = netstream_get_pipe();

    rt_kprintf("read_mirror : %d read_index : %d\n", pipe->ringbuffer.read_mirror, pipe->ringbuffer.read_index);
    rt_kprintf("write_mirror : %d write_index : %d\n", pipe->ringbuffer.write_mirror, pipe->ringbuffer.write_index);
    rt_kprintf("buffer_size : %d\n", pipe->ringbuffer.buffer_size);

    return 0;
}
MSH_CMD_EXPORT(stream_buffer, stream buffer read / write size);

extern rt_uint32_t rb_buffer_data_len(struct rb_buffer *rb);

int stream_pipe_dump(int argc, char *argv[])
{
    rt_uint32_t total_size, used_size, remain_size;
	struct stream_pipe *pipe = netstream_get_pipe();

    total_size = pipe->ringbuffer.buffer_size;
    used_size = rb_buffer_data_len(&pipe->ringbuffer);
    remain_size =  total_size - used_size;

    rt_kprintf("[netstream]: total size = %d \n", total_size);
    rt_kprintf("[netstream]: used size = %d \n", used_size);
    rt_kprintf("[netstream]: remain size = %d \n", remain_size);
    rt_kprintf("[netstream]: read_mirror = %d , read_index = %d\n", pipe->ringbuffer.read_mirror, pipe->ringbuffer.read_index);
    rt_kprintf("[netstream]: write_mirror = %d , write_index = %d\n", pipe->ringbuffer.write_mirror, pipe->ringbuffer.write_index);
    rt_kprintf("[netstream]: reader_ready_wm = %d , writer_resume_wm = %d\n", pipe->reader_ready_wm, pipe->writer_resume_wm);

    return 0;
}
MSH_CMD_EXPORT(stream_pipe_dump, stream pipe info dump);
