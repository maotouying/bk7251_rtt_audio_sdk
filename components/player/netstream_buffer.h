#ifndef NETSTREAM_BUFFER_H__
#define NETSTREAM_BUFFER_H__

#include "player_config.h"

int player_netstream_buffer_init(void);
void player_netstream_buffer_deinit(void);

uint8_t* player_netstream_get_buffer(void);
int player_netstream_get_bufsz(void);

#endif

