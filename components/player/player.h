/*
 * File      : player.h
 * COPYRIGHT (C) 2012-2018, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-05     liu2guang    Perfect the description and format.
 * 2018-12-18     liu2guang    Add user data stream playback interface. 
 */

#ifndef PLAYER_H__
#define PLAYER_H__

#include "player_config.h" 
#include "player_system.h" 

#define PLAYER_VERSION           (1L)
#define PLAYER_SUBVERSION        (2L)
#define PLAYER_REVISION          (1L)

/* play state */
enum PLAYER_STAT
{
    PLAYER_STAT_STOPPED = 0,
    PLAYER_STAT_PLAYING,
    PLAYER_STAT_PAUSED,
};

/* play api result */
enum PLAYER_RESULT_CODE
{
    PLAYER_OK = 0,
    PLAYER_FAILED = -1,
};

/* player event result */ 
enum PLAYER_RESULT
{
    PLAYER_CONTINUE = 0,
    PLAYER_BREAKOUT,
    PLAYER_DO_PLAY,
    PLAYER_DO_SEEK,
};

/* player event */ 
enum PLAYER_EVENT
{
    PLAYER_AUDIO_PLAYBACK = 0,
    PLAYER_AUDIO_CLOSED,

    PLYAER_STATE_CHANGED,
    PLAYER_VOLUME_CHANGED,

    PLAYER_APP_SUSPENDED,
    PLAYER_APP_RESUMED,
};

struct rt_work;
struct webclient_session;

/**
 * Initialization player 
 *
 * @return PLAYER_FAILED: initialization failed
 *         PLAYER_OK    : initialization successful
 */
int player_init(void);

/**
 * Play music 
 *
 * @return PLAYER_FAILED: play failed
 *         PLAYER_OK    : play successful
 * 
 * @note: 
 * 1. calling this function directly returns a failure when the music is played
 * 2. Resume play when the play is paused and the uri has not changed
 * 3. Switch play when the play is paused but the uri has changed
 */
int player_play(void);
int player_play_position(int position);

#if CONFIG_SESSION_STREAM
int player_play_websession(struct webclient_session* session);
#endif

/**
 * Stop music 
 *
 * @return PLAYER_OK: stop successful
 *         PLAYER_FAILED: stop failed
 * 
 * @note: The stop state call with this function returns success directly
 */
int player_stop(void);

/**
 * Pause music 
 *
 * @return PLAYER_FAILED: pause failed
 *         PLAYER_OK    : pause successful
 * 
 * @note: The pause state call with this function returns success directly
 */
int player_pause(void);

/**
 * Seek music 
 *
 * @return PLAYER_FAILED: seek failed
 *         PLAYER_OK    : seek successful
 */
int player_do_seek(int sec); 

/**
 * Set the music uri to play
 *
 * @return PLAYER_FAILED: seek failed
 *         PLAYER_OK    : seek successful
 */
int player_set_uri(const char *uri);

/**
 * Get the uri that is currently playing
 *
 * @return uri that is currently playing
 */
const char* player_get_uri(void);

/**
 * Get the current state being played
 *
 * @return PLAYER_STAT_STOPPED: stop state
 *         PLAYER_STAT_PLAYING: playing state
 *         PLAYER_STAT_PAUSED : pause state
 */
int player_get_state(void);
const char* player_get_state_str(void);

/* Data stream playback interface */ 

/**
 * Ready to start playing user data stream 
 * 
 * @param codec_type decoder type of user data stream @see audio_codec.h "enum AUDIO_CODEC_TYPE"
 * @param len the length of user data stream, the unknown length is set to -1
 *
 * @return PLAYER_FAILED: ready failed
 *         PLAYER_OK    : ready successful
 * 
 * @note: 1. The user data playback mode must specify the length of the music, otherwise the player 
 *           will not automatically stop playing, data stream length can be set after playback!!!, 
 *           To set the total length of the data stream, use the player_set_content_length function
 *        2. The player_play_data function must be followed by player_play!!!
 * 
 */
int player_play_data(int codec_type, int len); 

/**
 * Write user data stream to player
 * 
 * @param data user data stream
 * @param len the length of the data stream to be written
 *
 * @return PLAYER_FAILED: write failed
 *         PLAYER_OK    : write successful
 */
int player_write_data(char *data, int len); 

/**
 * Sets the total length of the data stream 
 * 
 * @param len the total length of the data stream 
 *
 * @return PLAYER_FAILED: set failed
 *         PLAYER_OK    : set successful
 */
int player_set_content_length(int len); 

/**
 * Set the playback completion callback
 * 
 * @param play_complete_cb callback function
 *
 * @return PLAYER_FAILED: set failed
 *         PLAYER_OK    : set successful
 */
int player_set_play_complete(void (*play_complete_cb)(void)); 

/* Internal use of player */
int player_event_handle(int timeout);
int player_fetch_seek(void);
int player_set_duration(int duration);
int player_get_duration(void);
int player_set_position(int position);
int player_get_position(void);
void player_do_work(struct rt_work* work);

#endif
