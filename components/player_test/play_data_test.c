#include <stdio.h>

#include <rtthread.h>
#include <finsh.h>

#include <player.h>
#include "audio_codec.h"
#include "data_work.h"

#include <player_system.h>
#include "webclient.h"

char* mp3_url = "http://183.193.243.90:9151/mp3/209713.mp3";
#define READ_BUF  (4 * 1024)

/* player 播放结束的回调函数 */
void player_complete_cb(void)
{
    rt_kprintf("player complete\n");
}

void play_data_thread(void *param)
{
    int res = RT_EOK;
    char *data;
    int data_len;
    size_t offset;
    size_t total_length = 0;
    int codec_type = 0;
    struct webclient_session* session = RT_NULL;

    codec_type = CODEC_MP3; 

    //设置解码器及音频总长度, 如果不知道音频总长度，这里填-1
    res = player_play_data(codec_type, (-1));
    if(res != PLAYER_OK)
    {
        rt_kprintf("player data failed\n");
        goto __exit;
    }

    //设置播放结束的回调函数
    player_set_play_complete(player_complete_cb);

    session = webclient_open(mp3_url);
    if(session == NULL)
    {
        rt_kprintf("open website failed.\n");
        goto __exit;
    }
    if(session->response != 200)
    {
        rt_kprintf("wrong response: %d\n", session->response);
        goto __exit;
    }

    data = rt_malloc(READ_BUF);
    if(data == NULL)
    {
        rt_kprintf("out of memory\n");
        goto __exit;
    }

    if(session->content_length == 0)
    {
        while(1)
        {
            data_len = webclient_read(session, data, READ_BUF);
            //todo 加密mp3音频数据解密
            if (data_len > 0)
            {
                //将音频数据写入player，注意：此函数阻塞，直到将所有的数据写入player后，退出
                player_write_data(data, data_len);
                total_length += data_len;
                rt_memset(data, 0x0, READ_BUF);
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        for(offset = 0; offset < session->content_length;)
        {
            data_len = webclient_read(session, data,
                    session->content_length - offset > READ_BUF ?
                            READ_BUF : session->content_length - offset);

            // rt_kprintf("data len : %d\n", data_len);
            if(data_len > 0)
            {
                //将音频数据写入player，注意：此函数阻塞，直到将所有的数据写入player后，退出
                player_write_data(data, data_len);
                total_length += data_len;
                rt_memset(data, 0x0, READ_BUF);
            }
            else
            {
                break;
            }

            offset += data_len;
        }
    }

    if(total_length)
    {
        //下载音频数据结束，已得知数据总长度，将数据总长度设置到player中
        data_work_set_content_length(total_length);
        rt_kprintf("\n download %d bytes\n", total_length);
    }

__exit:
    if(session != NULL)
    {
        webclient_close(session);
    }
    if(data != RT_NULL)
    {
        rt_free(data);
        data = RT_NULL;
    }
    return;
}

int play_data_test(int argc, char** argv)
{
    rt_thread_t tid;
    tid = rt_thread_create("play_data_test", play_data_thread, RT_NULL, 1024 * 4, 10, 5);
    if(tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
}
MSH_CMD_EXPORT_ALIAS(play_data_test, play_data, play_data_test);
