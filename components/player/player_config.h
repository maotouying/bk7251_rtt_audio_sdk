#ifndef PLAYER_CONFIG_H__
#define PLAYER_CONFIG_H__

#include <rtthread.h>

/* Privated Packages of RealThread */

#define PKG_USING_CODEC
#define PKG_USING_CODEC_LATEST_VERSION
#define PKG_USING_PLAYER
#define PKG_PLAYER_MP3
#define PKG_PLAYER_WAV
#define PKG_PLAYER_AMR
#define PKG_PLAYER_M4A
#define PKG_USING_PLAYER_LATEST_VERSION

#ifdef PKG_PLAYER_APP
#define CONFIG_APP  1
#endif
#ifdef PKG_PLAYER_WAV
#define CONFIG_WAV  1
#endif
#ifdef PKG_PLAYER_AMR
#define CONFIG_AMR  1
#endif 
#ifdef PKG_PLAYER_MP3
#define CONFIG_MP3  1
#endif
#ifdef PKG_PLAYER_HELIX_AAC
#define CONFIG_AAC  1
#define RT_USING_HELIX_AAC
#endif
#ifdef PKG_PLAYER_M4A
#define CONFIG_M4A  1
#endif
#ifdef PKG_PLAYER_FLAC
#define CONFIG_FLAC 1
#endif
#ifdef PKG_PLAYER_TS
#define CONFIG_TS   1
#endif
#ifdef PKG_PLAYER_OGG
#define CONFIG_OGG	0
#endif

#define CONFIG_NET_STREAM       1
#define CONFIG_HTTP_STREAM      1
#define CONFIG_HLS_STREAM       1
#define CONFIG_SESSION_STREAM   1

#define CONFIG_PLAYER_CHANNEL   0
#define CONFIG_PLAYER_APP       1

#define CONFIG_PLAYER_MEMALLOC  1

/* 512kB buffer size for netstream if no airplay */
#define NETSTREAM_BUFSZ         (128 * 1024)

/* 720kB buffer size for netstream if airplay */
//#define NETSTREAM_BUFSZ       (720 * 1024)

/* 720kB buffer size for netstream if airplay */
//#define NETSTREAM_BUFSZ       (128 * 1024)


#endif

