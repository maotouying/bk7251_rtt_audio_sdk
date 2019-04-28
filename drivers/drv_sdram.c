#include <string.h>

#include <rtthread.h>
#include <board.h>

static struct rt_memheap sdram_heap;

void rt_sdram_heap_init(void)
{
    rt_uint32_t begin = (rt_uint32_t)(RT_HW_SDRAM_BEGIN);
    rt_uint32_t size = RT_HW_SDRAM_END - RT_HW_SDRAM_BEGIN;

    rt_memheap_init(&sdram_heap, "SDRAM",
        (void *)begin, size);
}

void *sdram_malloc(unsigned long size)
{
    return rt_memheap_alloc(&sdram_heap, size);
}

void sdram_free(void *ptr)
{
    rt_memheap_free(ptr);
}

void *sdram_calloc(unsigned int n, unsigned int size)
{
    void* ptr = NULL;
    
    ptr = sdram_malloc(n * size);
    if (ptr)
    {
        memset(ptr, 0, n * size);
    }
    
    return ptr;
}

void *sdram_realloc(void *ptr, unsigned long size)
{
    return rt_memheap_realloc(&sdram_heap, ptr, size);
}
