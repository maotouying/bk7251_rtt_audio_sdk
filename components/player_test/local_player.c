/*
 * simple Local Player for file playing
 */

#include <rtthread.h>
#include "player.h"
#include "player_app.h"
#include <finsh.h>

#include <stdio.h>
#include <stdlib.h>

int play_wait_done(int timeout_ms)
{
    rt_tick_t tick, timeout_tick;

    timeout_tick = rt_tick_from_millisecond(timeout_ms);

    tick = rt_tick_get();
    while (1)
    {
        if (player_get_state() != PLAYER_STAT_PLAYING ) break;

        rt_thread_delay(rt_tick_from_millisecond(100));

        if ((rt_tick_get() - tick) > timeout_tick) return -RT_ETIMEOUT;
    }

    return RT_EOK;
}

int play_tone(const char* filename)
{
    player_stop();
    player_set_uri(filename);
    player_play();

    while (1)
    {
        if (player_get_state() != PLAYER_STAT_PLAYING) break;

        rt_thread_delay(rt_tick_from_millisecond(100));
    }

    return 0;
}

int local_player_play(const char* filename)
{
    player_stop();
    player_set_uri(filename);
    player_play();

    return 0;
}

rt_bool_t local_player_is_playing(void)
{
    if (player_get_state() == PLAYER_STAT_PLAYING) return RT_TRUE;

    return RT_FALSE;
}

void usage(void)
{
    printf("player filename\n");
    printf("player URI\n");
    printf("player stop\n");
    printf("player pause\n");
    printf("player resume\n");
    printf("player seek second\n");
    printf("player volume value, 0 - 99\n");
    printf("player status");
}

int player(int argc, char** argv)
{
    if (argc < 2)
    {
        usage();
        return 0;
    }

    if (strcmp(argv[1], "stop") == 0)
    {
        player_app_stop();
    }
    else if (strcmp(argv[1], "pause") == 0)
    {
        player_pause();
    }
    else if (strcmp(argv[1], "resume") == 0)
    {
        player_play();
    }
    else if (strcmp(argv[1], "seek") == 0)
    {
        int second;

        if (argc != 3) {usage(); return 0;}

        second = atoi(argv[2]);
        player_do_seek(second);
    }
    else if (strcmp(argv[1], "volume") == 0)
    {
        int volume;

        if (argc != 3) {usage(); return 0;}
        volume = atoi(argv[2]);

        if (volume >= 0 && volume < 100)
            player_app_set_volume(player_app_get_activated(), volume);
    }
    else if (strcmp(argv[1], "status") == 0)
    {
        struct player_app *app = player_app_get_activated();
        const char *state_str[] =
        {
            "STOPPED",
            "PLAYING",
            "PAUSED"
        };

        /* dump player stutus */
        printf("Player Status:\n");
        printf("status   - %s\n", state_str[player_get_state()]);
        printf("URI      - %s\n", player_get_uri() != NULL? player_get_uri() : "NULL");
        printf("volume   - %d\n", player_app_get_volume(app));
        if (app)
        {
            printf("play app - %s\n", app->name);
        }

        if (player_get_state() != PLAYER_STAT_STOPPED)
        {
            int value;

            value = player_get_duration();
            rt_kprintf("duration - %02d:%02d\n", value / 60, value % 60);

            value = player_get_position() / 1000;
            rt_kprintf("position - %02d:%02d\n", value /60, value % 60);
        }
    }
    else
    {
        /* stop player */
        player_stop();
        player_set_uri(argv[1]);
        player_play();
    }

    return 0;
}

MSH_CMD_EXPORT(player, player test);
