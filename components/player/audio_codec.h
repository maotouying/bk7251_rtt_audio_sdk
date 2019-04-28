#ifndef AUDIO_CODEC_H__
#define AUDIO_CODEC_H__

enum AUDIO_CODEC_TYPE
{
    CODEC_UNKNOWN,
    CODEC_WAV,
    CODEC_PCM,
    CODEC_AMR,
    CODEC_MP3,
    CODEC_AAC,
    FORMAT_M4A,
    FORMAT_TS,
    CODEC_FLAC,
    CODEC_OGG,
    CODEC_WMA,
};

struct audio_codec;
struct audio_codec_ops
{
    int (*run)(struct audio_codec *codec);
    int (*destory)(struct audio_codec *codec);
};

struct audio_codec
{
    const struct audio_codec_ops *ops;
    uint32_t seekable;

    struct audio_stream *stream;
};
#define CODEC_STREAM(codec) (((struct audio_codec*)(codec))->stream)

int audio_codec_get_type(const char *URI);
int audio_codec_get_type_by_mime(const char *mime);

struct audio_codec* audio_codec_create(struct audio_stream *stream);
int audio_codec_destory(struct audio_codec *codec);

int audio_codec_run(struct audio_codec *codec);

#endif
