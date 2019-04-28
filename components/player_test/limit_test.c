#include <stdio.h>

#include <rtthread.h>
#include <finsh.h>

#include <player_app.h>
#include <player.h>
#include <list_player.h>

#include <player_system.h>

static music_list_t local_list;
static rt_sem_t _list_sem;
static rt_sem_t _item_end_sem;  /* 用于同步 */
static int flag = 0;
static int num = 0;

static int player_list_limit(music_list_t list, int max, int dir);

static void local_list_test_entry(void *para)
{
    struct music_item item;
    char _name[12];

    while (1)
    {
        rt_sem_take(para, RT_WAITING_FOREVER);

        sprintf(_name, "%d", num++);
        item.name = (_name);
        item.URL = ("http://192.168.10.151/mp3/test_pal_320Kbps.mp3");
        //在头部插入一首歌曲
        if (flag)
        {
            list_player_item_add(local_list, &item, 0);
            player_list_limit(local_list, 3, -1);
        }

    }
}

static void local_list_event_handle(music_list_t list, int event, void *arg)
{
    char _name[12];

    rt_kprintf("<%s:%d>lister event = %d \n", __func__, __LINE__, event);
    if (event == LISTER_END_OF_ITEM)
    {
        struct music_item *item;

        item = list_player_current_item();
        rt_kprintf("[end of item]: current name = %s \n", item->name);
        rt_kprintf("[end of item]: current URL = %s \n", item->URL);
    }

    if (event == LISTER_END_OF_LIST)
        /* 播放完成，添加一首歌曲，继续播放 */
    {
        struct music_item item;

        if (!flag)
        {
            sprintf(_name, "%d", num++);
            item.name = (_name);
            item.URL = ("http://192.168.10.151/mp3/test_pal_320Kbps.mp3");
            //在头部插入一首歌曲
            list_player_item_add(local_list, &item, 0);
            //限定列表最大歌曲数为5首，多余的歌从尾部删除
            player_list_limit(local_list, 3, -1);
            list_player_next();
        }
        else
        {
            rt_sem_release(_list_sem);
        }

        //限定列表最大歌曲数为5首，多余的歌从头部删除
        // player_list_limit(local_list, 2, 0);
    }
}

static int list_limit_play(int argc, char *argv[])
{
    struct music_item item;
    rt_thread_t tid = RT_NULL;

    if (argc == 2)
    {
        flag = atoi(argv[1]);
    }
    else
    {
        flag = 0;
    }
    /* 创建播放列表 */
    if (!local_list)
    {
        local_list = list_player_items_create();

        item.name = ("16k_2ch.wav");
        item.URL = ("http://192.168.10.151/wav/16k_2ch.wav");
        list_player_item_add(local_list, &item, -1);
        _list_sem = rt_sem_create("local_l", 2, RT_IPC_FLAG_FIFO);
        /* 创建end of item同步信号量 */
        _item_end_sem = rt_sem_create("item_end", 0, RT_IPC_FLAG_FIFO);
        tid = rt_thread_create("local_l", local_list_test_entry, _list_sem, 2048, 25, 10);
        if (tid)
            rt_thread_startup(tid);
    }
    list_player_play(local_list);
    list_player_set_table_handler(local_list, local_list_event_handle, RT_NULL);
    list_player_mode_set(LISTER_LIST_ONCE_MODE);

    return RT_EOK;
}
FINSH_FUNCTION_EXPORT_ALIAS(list_limit_play, __cmd_limit_test, list_limit_play);


static int player_list_limit(music_list_t list, int max, int dir)
{
    if (list->item_num <= max)
    {
        return 0;
    }
    while (list->item_num - max)
    {
        list_player_item_del_by_index(list, dir);
    }
    return 0;
}

