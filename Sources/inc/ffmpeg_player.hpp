/*
 * File FFmpegplayer.hpp belongs to miniDart project
 * Copyright Eric Bachard  2018 December 4th , 14:35:03 (UTC+0200)
 * This document is under GPL v2 license
 * See : http://www.gnu.org/licenses/gpl-2.0.html
 */

#ifndef __FFMPEG_PLAYER_HPP
#define __FFMPEG_PLAYER_HPP

#include <iostream>
#include <unistd.h> 

#define __STDC_CONSTANT_MACROS

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
////#include <libavdevice/avdevice.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/avstring.h>
#include <libavutil/time.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>


#ifndef REFRESH_RATE
#define REFRESH_RATE 0.01
//#define REFRESH_RATE 0.005
#endif

#define VIDEO_PICTURE_QUEUE_SIZE 3

/* Minimum SDL audio buffer size, in samples. */
#ifndef SDL_AUDIO_MIN_BUFFER_SIZE
#define SDL_AUDIO_MIN_BUFFER_SIZE 1024
#endif

#define AVCODEC_MAX_AUDIO_FRAME_SIZE 192000

/* SDL custom Event type. Notifies the next video frame has to be displayed */
#define FF_REFRESH_EVENT (SDL_USEREVENT)

#define FF_QUIT_EVENT (SDL_USEREVENT + 1)

/* no AV sync correction is done if below the minimum AV sync threshold */
#define AV_SYNC_THRESHOLD_MIN 0.04
/* AV sync correction is done if above the maximum AV sync threshold */
#define AV_SYNC_THRESHOLD_MAX 0.1
/* If a frame duration is longer than this, it will not be duplicated to compensate AV sync */
#define AV_SYNC_FRAMEDUP_THRESHOLD 0.1
/* no AV correction is done if too big error */
#define AV_NOSYNC_THRESHOLD 3.0

typedef struct _VideoPicture
{
    AVFrame *frame;
    double pts;
    int width;
    int height;
    int format;
    int uploaded;
    int flip_v;
} VideoPicture;

typedef struct _FrameQueue
{
    VideoPicture queue[VIDEO_PICTURE_QUEUE_SIZE];
    int rindex; //read index
    int windex; //write index
    int size;
    int max_size;
    int rindex_shown; //nums of pictures having been readed
    SDL_mutex *mutex;
    SDL_cond *cond;
} FrameQueue;

typedef struct _PacketQueue
{
    AVPacketList * first_pkt;
    AVPacketList * last_pkt;
    int nb_packets;
    int size;
    int abort_request;
    SDL_mutex *mutex;
    SDL_cond *cond;
} PacketQueue;

typedef struct _VideoState
{
    int seek_req;
    int seek_flags;
    int seek_pos;
    int seek_rel;

    AVFormatContext *ic;
    int video_stream;
    int audio_stream;

    FrameQueue pictq;

    double audio_clock;
    AVCodecContext *audio_avctx;
    AVStream *audio_st;
    PacketQueue audioq;
    unsigned int audio_buf_size;
    unsigned int audio_buf_index;

    double frame_timer;
    double video_clock;
    double  video_current_pts;
    int64_t video_current_pts_time;

    /* AV Sync (currently unused) */
    int     av_sync_type;
    double  external_clock;
    int64_t external_clock_time;

    AVCodecContext *video_avctx;
    AVStream *video_st;
    PacketQueue videoq;

    struct SwsContext *img_convert_ctx;

    SDL_Thread *read_tid;
    SDL_Thread *video_tid;

    char *filename;
    int abort_request;
} VideoState;


/* Audio Video Sync Types */
enum
{
    /* Sync to audio clock */
    AV_SYNC_AUDIO_MASTER,

    /* Sync to video clock */
    AV_SYNC_VIDEO_MASTER,

    /* Sync to external clock: the computer clock */
    AV_SYNC_EXTERNAL_MASTER
};

void video_refresh(void *arg, double *remaining_time);
void refresh_loop_wait_event(VideoState*, SDL_Event *);

//double incr, pos;
double get_audio_clock(VideoState *is);
double get_video_clock(VideoState *is);
double get_external_clock(VideoState *is);
double get_master_clock(VideoState *is);

void video_refresh_timer(void * userdata);

void stream_seek(VideoState* is, int64_t pos, int rel, int seek_by_bytes);
//stream_seek(is, (int64_t)(pos * AV_TIME_BASE), (int64_t)(incr * AV_TIME_BASE));
int main2(char * filename);
void event_loop(VideoState *is);
int do_exit(VideoState *);
void frame_queue_destroy(FrameQueue *f);
int packet_queue_init(PacketQueue *q);
void packet_queue_flush(PacketQueue *q);
void packet_queue_start(PacketQueue *q);
int packet_queue_put(PacketQueue *q, AVPacket *pkt);
int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block);
void packet_queue_destroy(PacketQueue *q);
void packet_queue_abort(PacketQueue *q);
int audio_decode_frame(VideoState *is, uint8_t *audio_buf, int buf_size, double *pts_ptr);
void audio_callback(void *userdata, uint8_t *stream, int len);
void saveFrame(AVFrame *pFrame, int w, int h, int iFrame);
double synchronize_video(VideoState * , AVFrame *, double pts);

#endif /* __FFMPEG_PLAYER_HPP */
