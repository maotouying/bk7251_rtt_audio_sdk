#include <stdio.h>

#include <rtthread.h>
#include <finsh.h>

#include <player_app.h>
#include <player.h>
#include <list_player.h>

#include <player_system.h>

struct local_player_sate
{
    char *url;
    int32_t used_time;
    int32_t index;
    music_list_t items;
};

/* 本地缺省播放列表 */
static music_list_t local_list;
/* 云端下发播放列表 */
static music_list_t cloud_list;
/* 本地播放状态 */
static struct local_player_sate local_play_stat;
/* 模拟云端下发事件 */
static int cloud_play_event = 0;

/* 创建本地播放列表 */
void local_list_items_create(void)
{
    list_player_detach_items();
    if (local_list)
    {
        list_player_items_delete(local_list);
        local_list = NULL;
    }

    local_list = list_player_items_create();
}

/* 删除本地播放列表 */
void local_list_items_delete(void)
{
    music_list_t table = RT_NULL;
    table = list_player_detach_items();
    if (table && local_list)
    {
        list_player_items_delete(local_list);
        local_list = NULL;
        rt_kprintf(" <%s:%d> list_player_items_delete\r\n", __func__, __LINE__);
    }
}

/* 创建云端播放列表 */
void cloud_list_items_create(void)
{
    list_player_detach_items();
    if (cloud_list)
    {
        list_player_items_delete(cloud_list);
        cloud_list = NULL;
    }

    cloud_list = list_player_items_create();
}

/* 删除云端播放列表 */
void cloud_list_items_delete(void)
{
    music_list_t table = RT_NULL;
    table = list_player_detach_items();
    if (table && cloud_list)
    {
        list_player_items_delete(cloud_list);
        cloud_list = NULL;
        rt_kprintf(" <%s:%d> list_player_items_delete\r\n", __func__, __LINE__);
    }
}

/* 挂起本地播放 */
int local_list_suspend(void)
{
    local_play_stat.url = (char *)player_get_uri();
    local_play_stat.used_time = player_get_position();
    local_play_stat.index = list_player_current_index();
    local_play_stat.items = list_player_detach_items();

    return RT_EOK;
}

/* 恢复本地播放 */
int local_list_resume(void)
{
    rt_kprintf("local_list_resume %s,%d,%d \n", local_play_stat.url, local_play_stat.used_time, local_play_stat.index);

    if (local_play_stat.url)
    {
        int current_volume = player_app_get_volume(NULL);
        player_app_set_volume(NULL, 0);
        cloud_list_items_delete();
        list_player_mode_set(LISTER_LIST_REPEAT_MODE);
        list_player_play(local_play_stat.items);
        rt_kprintf("list_player_num before\r\n");

        player_app_set_volume(NULL, current_volume);
    }
    else
    {
        rt_kprintf("L%d local_list_play not implement  \n", __LINE__);
    }

    local_play_stat.url = NULL;
    local_play_stat.used_time = 0;
    local_play_stat.index = 0;
}

/* 本地list player 回调处理函数 */
static void local_list_event_handle(music_list_t table, int event, void *arg)
{
    rt_kprintf(" <%s:%d> list_player_items_delete\r\n", __func__, __LINE__);
}

/* 云端 list player 回调处理函数 */
static void cloud_list_event_handle(music_list_t table, int event, void *arg)
{
    /* 播放结束，删除列表，恢复本地播放 */
    if (event == LISTER_END_OF_ITEM)
    {
        rt_kprintf("<%s:%d> list_player_items_delete\r\n", __func__, __LINE__);
        cloud_list_items_delete();
        local_list_resume();
    }
}

void list_player_test_entry(void *parameter)
{
    struct music_item items;

    local_list_items_create();

    items.name = ("73865964.mp3");
    items.URL = ("http://183.193.243.90:9151/mp3/73865964.mp3");
    list_player_item_add(local_list, &items, 0);

    items.name = ("108479485.mp3");
    items.URL = ("http://183.193.243.90:9151/mp3/108479485.mp3");
    list_player_item_add(local_list, &items, 1);

    items.name = ("84986462.mp3");
    items.URL = ("http://183.193.243.90:9151/mp3/84986462.mp3");
    list_player_item_add(local_list, &items, -1);

    list_player_play(local_list);

    while (1)
    {
        if (cloud_play_event == 1)
        {
            cloud_play_event = 0;

            local_list_suspend();

            cloud_list_items_create();
            items.name = ("congcong.mp3");
            items.URL = ("http://183.193.243.90:9151/mp3/test_lr.mp3");
            list_player_item_add(cloud_list, &items, -1);
            list_player_set_table_handler(cloud_list, cloud_list_event_handle, RT_NULL);
            list_player_play(cloud_list);
        }

        rt_thread_delay(1000);
    }
}

int list_player_test(int argc, char *argv[])
{
    rt_thread_t tid = RT_NULL;

    tid = rt_thread_create("list_play_test",
                           list_player_test_entry,
                           RT_NULL,
                           1024 * 8,
                           22,
                           10);
    if (tid != RT_NULL)
        rt_thread_startup(tid);

}
FINSH_FUNCTION_EXPORT_ALIAS(list_player_test, __cmd_list_test, list_player_test);

int list_player_event_send(int argc, char *argv[])
{
    cloud_play_event = 1;

    return RT_EOK;
}
FINSH_FUNCTION_EXPORT_ALIAS(list_player_event_send, __cmd_list_send, list_player_event_send);

#define LISTER_TEST

#ifdef LISTER_TEST

extern struct player_list *_list_player;

static music_list_t music_list;
int lister_play(void)
{
    struct music_item items;

    if (music_list != RT_NULL)
    {
        list_player_play(music_list);
        return 0;
    }
    music_list = list_player_items_create();

#if 1
    items.name = ("1");
    items.URL = ("http://bernard.coding.me/channel/music/10years.mp3");
    list_player_item_add(music_list, &items, 0);

    items.name = ("2");
    items.URL = ("http://bernard.coding.me/channel/music/congcong.mp3");
    list_player_item_add(music_list, &items, 1);

    items.name = ("3");
    items.URL = ("http://bernard.coding.me/channel/music/later.mp3");
    list_player_item_add(music_list, &items, -1);


    items.name = ("4");
    items.URL = ("http://bernard.coding.me/channel/music/xxy.mp3");
    list_player_item_add(music_list, &items, 1);

#else
    /* 创建空表 */
#endif

    list_player_play(music_list);
    return 0;
}
MSH_CMD_EXPORT(lister_play, lister player);

int lister_detach(void)
{
    list_player_detach_items();
    return 0;
}
MSH_CMD_EXPORT(lister_detach, lister stop);


int lister_stop(void)
{
    list_player_stop();
    return 0;
}
MSH_CMD_EXPORT(lister_stop, lister stop);

int lister_suspend(void)
{
    list_player_suspend();
    return 0;
}
MSH_CMD_EXPORT(lister_suspend, lister suspend);

int lister_resume(void)
{
    list_player_resume();
    return 0;
}
MSH_CMD_EXPORT(lister_resume, lister resume);

int lister_next(void)
{
    list_player_next();
    return 0;
}
MSH_CMD_EXPORT(lister_next, lister next);

int lister_prev(void)
{
    list_player_prev();
    return 0;
}
MSH_CMD_EXPORT(lister_prev, lister prev);

static int lister_mode(int argc, char *argv[])
{
    int mode;

    if (argc < 2)
    {
        rt_kprintf("Usage:lister_mode mode\n");
        return -1;
    }
    mode = *argv[1] - '0';
    rt_kprintf("set mode :%d\n", list_player_mode_set(mode));
    return 0;
}
MSH_CMD_EXPORT(lister_mode, lister mode);

static int lister_empty(int argc, char *argv[])
{
    list_player_empty();
    music_list = RT_NULL;
    return 0;
}
MSH_CMD_EXPORT(lister_empty, lister empty);

static int lister_add(int argc, char *argv[])
{
    struct music_item item;
    int index = -1;

    if (!_list_player)
    {
        rt_kprintf("[list_player]: _list_player is null \n");
        return -RT_ERROR;
    }

    if (argc < 3 || argc > 4)
    {
        rt_kprintf("Usage:lister_add name URL\n");
        return -1;
    }
    rt_memset(&item, 0, sizeof(item));

    item.name = rt_strdup(argv[1]);
    item.URL = rt_strdup(argv[2]);
    if (argc == 4)
    {
        index = atoi(argv[3]);
    }

    if ((item.name != RT_NULL) && (item.URL != RT_NULL))
    {
        list_player_item_add(_list_player->table, &item, index);
    }
    if (item.name)
    {
        rt_free(item.name);
    }
    if (item.URL)
    {
        rt_free(item.URL);
    }

    return RT_EOK;
}
MSH_CMD_EXPORT(lister_add, lister add);

static int lister_del(int argc, char *argv[])
{
    struct music_item item;

    if (!_list_player)
    {
        rt_kprintf("[list_player]: _list_player is null \n");
        return -RT_ERROR;
    }

    if (argc < 3)
    {
        rt_kprintf("Usage:lister_del name URL\n");
        return -1;
    }
    rt_memset(&item, 0, sizeof(item));

    item.name = rt_strdup(argv[1]);
    item.URL = rt_strdup(argv[2]);
    if ((item.name != RT_NULL) && (item.URL != RT_NULL))
    {
        list_player_item_del(_list_player->table, &item);
    }
    if (item.name != RT_NULL)
    {
        rt_free(item.name);
    }
    if (item.URL != RT_NULL)
    {
        rt_free(item.URL);
    }

    return 0;
}
MSH_CMD_EXPORT(lister_del, lister delete);

static int lister_del_by_index(int argc, char *argv[])
{
    struct music_item item;

    if (!_list_player)
    {
        rt_kprintf("[list_player]: _list_player is null \n");
        return -RT_ERROR;
    }

    if (argc != 2)
    {
        rt_kprintf("Usage:lister_del index \n");
        return -1;
    }

    list_player_item_del_by_index(_list_player->table, atoi(argv[1]));

    return 0;
}
MSH_CMD_EXPORT(lister_del_by_index, lister delete by index);

static int lister_dump(int argc, char *argv[])
{
    music_item_t item;

    if (!_list_player)
    {
        rt_kprintf("[list_player]: list_player not initialize \n");
        return -RT_ERROR;
    }

    rt_kprintf("[list_player] music table dump:\n");
    rt_kprintf("play status: %d\n", _list_player->status);
    rt_kprintf("item_num: %d\n", _list_player->table->item_num);
    rt_kprintf("item_position: %d\n", _list_player->table->item_position);
    rt_kprintf("current_index: %d\n", list_player_current_index());
    item = list_player_current_item();
    if (item)
    {
        rt_kprintf("current_name: %s\n", item->name);
        rt_kprintf("current_url: %s\n", item->URL);
    }
    rt_kprintf("\n");

    list_player_dump(_list_player->table);

    return RT_EOK;
}
MSH_CMD_EXPORT(lister_dump, lister dump);

#endif

