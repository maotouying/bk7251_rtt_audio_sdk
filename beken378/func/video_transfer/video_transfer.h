#ifndef __VIDEO_TRANS_H__
#define __VIDEO_TRANS_H__

typedef enum {
    TVIDEO_SND_NONE         = 0LU,
    TVIDEO_SND_UDP,
    TVIDEO_SND_TCP,
    TVIDEO_SND_INTF,
} TVIDEO_SND_TYPE;

typedef struct tvideo_desc
{
    UINT8 *rxbuf;
    
    void (*node_full_handler)(void *curptr, UINT32 newlen, UINT32 is_eof, UINT32 frame_len);
    void (*data_end_handler)(void);   

    UINT16 rxbuf_len;
    UINT16 rx_read_len;
    UINT32 node_len;
} TVIDEO_DESC_ST, *TVIDEO_DESC_PTR;

typedef struct tvideo_hdr_param
{
    UINT8* ptk_ptr;
    UINT32 ptklen;
    UINT32 frame_id;
    UINT32 is_eof;
    UINT32 frame_len;
}TV_HDR_PARAM_ST, *TV_HDR_PARAM_PTR;

typedef void (*tvideo_add_pkt_header)(TV_HDR_PARAM_PTR param); 
typedef int (*video_transfer_send_func)(UINT8 *data, UINT32 len);
typedef void (*video_transfer_start_cb)(void); 
typedef void (*video_transfer_end_cb)(void); 

typedef struct tvideo_setup_desc
{
    UINT32 send_type;
    video_transfer_send_func send_func;
    video_transfer_start_cb start_cb;
    video_transfer_start_cb end_cb;
    
    UINT32 pkt_header_size;
    tvideo_add_pkt_header add_pkt_header;
} TVIDEO_SETUP_DESC_ST, *TVIDEO_SETUP_DESC_PTR;

#if (CFG_USE_SPIDMA || CFG_USE_CAMERA_INTF)
void tvideo_intfer_send_msg(UINT32 new_msg);
UINT32 video_transfer_init(TVIDEO_SETUP_DESC_PTR setup_cfg);
UINT32 video_transfer_deinit(void);
#endif

#endif // __VIDEO_TRANS_H__
