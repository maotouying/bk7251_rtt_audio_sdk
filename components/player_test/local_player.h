#ifndef LOCAL_PLAYER_H__
#define LOCAL_PLAYER_H__

int local_player_play(const char* filename);
rt_bool_t local_player_is_playing(void);

int play_tone(const char* filename);
int play_wait_done(int timeout_ms);

#endif

