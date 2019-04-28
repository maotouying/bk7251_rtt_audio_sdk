#ifndef __PLAYER_BELL_H__
#define __PLAYER_BELL_H__

/* 提示音播放事件回调函数 */
typedef void (*bell_event_handler)(int event);

enum BELL_PLAYER_EVENT
{
    BELL_PLAYER_COMPLEMENT_EVENT = 0x01,    /* 提示音播放完成事件 */
};

/* 提示音功能初始化 */
int bell_player_init(void);

/* 异步播放 */
int bell_player_play(char *url);

/* 同步播放 */
int bell_player_play_sync(char *url);

/* 设置事件处理函数 */
void bell_player_set_event_handler(bell_event_handler handler);

/* 获取提示音状态 */
int bell_player_get_state(void);

#endif
