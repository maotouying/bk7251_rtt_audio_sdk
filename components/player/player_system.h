#ifndef PLAYER_SYSTEM_H__
#define PLAYER_SYSTEM_H__

#include <stdint.h>
#include <stdlib.h>
#include <player_config.h>

#if CONFIG_PLAYER_MEMALLOC
void* player_malloc(size_t size);
void  player_free(void *ptr);
void* player_realloc(void *ptr, size_t size);

void* player_tcm_malloc(size_t size);
void  player_tcm_free(void *ptr);

char *player_strdup(const char *s1);
#else
#ifdef _WIN32
    #include <stdlib.h>
    #define player_malloc       malloc
    #define player_free         free
    #define player_realloc      realloc

    #define player_tcm_malloc   malloc
    #define player_tcm_free     free

    char *player_strdup(const char *s1);

#else
    #define player_malloc       rt_malloc
    #define player_free         rt_free
    #define player_realloc      rt_realloc

    #define player_tcm_malloc   rt_malloc
    #define player_tcm_free     rt_free

    #define player_strdup       rt_strdup
#endif

#endif

#endif
