#pragma once

#include <cJSON.h>
#include <rtthread.h>
#include <rtservice.h>

/*
 * 频道
 * 网络频道是云端存在数个链接，设备可以主动请求链接获得歌曲列表；也可以切换不同的链接实现不同的频道切换
 * (广义上，频道也可以是本地的概率)
 */

struct player_channel_plugin
{
    char *name;
    int(*play)(cJSON *channel);

    rt_list_t list;
};

 /* 初始化频道（建立起频道列表） */
int player_channel_init(void);
int player_channel_register_plugin(struct player_channel_plugin* plugin);

/* 下一个频道 */
void player_channel_next(void);
/* 上一个频道 */
void player_channel_prev(void);
