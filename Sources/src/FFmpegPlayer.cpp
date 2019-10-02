/*
 * File FFmpegplayer.cpp belongs to miniDart project
 * Copyright Eric Bachard  2018 December 4th , 14:35:03 (UTC+0200)
 * This document is under GPL v2 license
 * See : http://www.gnu.org/licenses/gpl-2.0.html
 */

#include <iostream>

using namespace std;

#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif
#include <unistd.h> 
#include <iostream>
#include <cstring>

#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <opencv2/opencv.hpp>

#include "ffmpeg_player.hpp"

#ifdef BUILDING_FRENCH
#include "miniDart_fr.hpp"
#else
#include "miniDart_en-US.hpp"
#endif

static const struct TextureFormatEntry {
    enum AVPixelFormat format;
    int texture_fmt;
} sdl_texture_format_map[] = {
    { AV_PIX_FMT_RGB8,           SDL_PIXELFORMAT_RGB332 },
    { AV_PIX_FMT_RGB444,         SDL_PIXELFORMAT_RGB444 },
    { AV_PIX_FMT_RGB555,         SDL_PIXELFORMAT_RGB555 },
    { AV_PIX_FMT_BGR555,         SDL_PIXELFORMAT_BGR555 },
    { AV_PIX_FMT_RGB565,         SDL_PIXELFORMAT_RGB565 },
    { AV_PIX_FMT_BGR565,         SDL_PIXELFORMAT_BGR565 },
    { AV_PIX_FMT_RGB24,          SDL_PIXELFORMAT_RGB24 },
    { AV_PIX_FMT_BGR24,          SDL_PIXELFORMAT_BGR24 },
    { AV_PIX_FMT_0RGB32,         SDL_PIXELFORMAT_RGB888 },
    { AV_PIX_FMT_0BGR32,         SDL_PIXELFORMAT_BGR888 },
    { AV_PIX_FMT_NE(RGB0, 0BGR), SDL_PIXELFORMAT_RGBX8888 },
    { AV_PIX_FMT_NE(BGR0, 0RGB), SDL_PIXELFORMAT_BGRX8888 },
    { AV_PIX_FMT_RGB32,          SDL_PIXELFORMAT_ARGB8888 },
    { AV_PIX_FMT_RGB32_1,        SDL_PIXELFORMAT_RGBA8888 },
    { AV_PIX_FMT_BGR32,          SDL_PIXELFORMAT_ABGR8888 },
    { AV_PIX_FMT_BGR32_1,        SDL_PIXELFORMAT_BGRA8888 },
//    { AV_PIX_FMT_YUV420P,        SDL_PIXELFORMAT_YV12 },
    { AV_PIX_FMT_YUV420P,        SDL_PIXELFORMAT_IYUV },
    { AV_PIX_FMT_YUYV422,        SDL_PIXELFORMAT_YUY2 },
    { AV_PIX_FMT_UYVY422,        SDL_PIXELFORMAT_UYVY },
    { AV_PIX_FMT_NV12,           SDL_PIXELFORMAT_NV12 },
    { AV_PIX_FMT_NV21,           SDL_PIXELFORMAT_NV21 },
    { AV_PIX_FMT_NONE,           SDL_PIXELFORMAT_RGBA32 },
};

extern SDL_Window *window;
extern AVPacket flush_pkt;
extern VideoState * is;
extern double video_duration;
extern cv::Mat videoMat;
extern SDL_AudioDeviceID audio_dev;

#define DEFAULT_VIDEO_FILENAME_SIZE 16

char default_video_filename[DEFAULT_VIDEO_FILENAME_SIZE] = "./MyVideo01.avi";

static char * old_video_filename = default_video_filename;
static char * current_video_filename = nullptr;

static cv::Mat picture;
static bool cvMat_initialized = false;
static uint8_t *out_data[4];
static int out_linesize[4];

// costly, but allows to convert the AVFrame (most of the time YUV420p) into cv::Mat (BGR24 in this case)
static cv::Mat avframe_to_cvmat(AVFrame *frame)
{
    int in_w = frame->width;
    int in_h = frame->height;
    enum AVPixelFormat in_pix_fmt = (enum AVPixelFormat)frame->format;

    // cv::Mat understands BGR24 by default
    int out_w = frame->width;
    int out_h = frame->height;

    //// SOON, next version
    ////    std::vector<uint8_t> imgbuf(out_h * out_w * 4 + 16);
    enum AVPixelFormat out_pix_fmt = AV_PIX_FMT_BGR24;

    // only once, to save cycles
    if (!cvMat_initialized) {
        //SOON, next version to come, will create .mp4 or avi using ffmpeg, not the buggy OpenCV
        ////        picture = cv::Mat(out_h, out_w, CV_8UC4, imgbuf.data(), out_w * 4);
        picture = cv::Mat(out_h, out_w, CV_8UC3);
        cvMat_initialized = true;
    }

    av_image_fill_arrays(out_data, out_linesize, picture.data, out_pix_fmt, out_w, out_h, 1);// 16);

    // sws_getCachedContext() ?
    is->img_convert_ctx = sws_getContext(in_w, in_h, in_pix_fmt, out_w, out_h, out_pix_fmt, SWS_FAST_BILINEAR, NULL, NULL, NULL);

    if(is->img_convert_ctx != nullptr)
    {
        sws_scale(is->img_convert_ctx, frame->data, frame->linesize, 0, out_h, out_data, out_linesize);
        sws_freeContext(is->img_convert_ctx);
    }
    else
    {
        std::cout << "Cannot initialize the convertion context" << std::endl;
        exit (EXIT_FAILURE);
    }
    return picture;
}


void stream_component_close(VideoState * is, int stream_index)
{
    AVFormatContext *ic = is->ic;
    AVCodecParameters *codecpar;

    if (stream_index < 0 || (unsigned int)stream_index >= ic->nb_streams)
        return;

    codecpar = ic->streams[stream_index]->codecpar;

    switch (codecpar->codec_type)
    {
        case AVMEDIA_TYPE_AUDIO:
            SDL_CloseAudioDevice(audio_dev);
            packet_queue_abort(&is->audioq);

        break;

        case AVMEDIA_TYPE_VIDEO:
            packet_queue_abort(&is->videoq);
        break;

        default:
        break;
    }
}

int do_exit(VideoState * is)
{
        std::cout << " entering in do_exit(is) " << std::endl;

        is->abort_request = 1;

        if (is->read_tid)
            SDL_WaitThread(is->read_tid, NULL);

//#ifdef NATIVE_BUILD
        // important under Linux
        SDL_Delay(20);
//#endif

        std::cout << " is->tead_tid : " << is->read_tid << std::endl;

        /* close each stream */
        if (is->audio_stream >= 0)
            stream_component_close(is, is->audio_stream);

        if (is->video_stream >= 0)
            stream_component_close(is, is->video_stream);

        if (&is->ic)
            avformat_close_input(&is->ic);

        if (&is->videoq)
            packet_queue_destroy(&is->videoq);

        if (&is->audioq)
            packet_queue_destroy(&is->audioq);

        /* free all pictures */
        if (&is->pictq)
            frame_queue_destroy(&is->pictq);

        if (is->img_convert_ctx)
            sws_freeContext(is->img_convert_ctx);

        // is->filename = nullptr;

        av_free(is->filename);

        if (!picture.empty())
            picture.release();

        cvMat_initialized = false;

        if (is)
            av_free(is);

        old_video_filename = current_video_filename;
        avformat_network_deinit();

    return (EXIT_SUCCESS);
}

int audio_open(void *arg, int64_t wanted_channel_layout, int wanted_nb_channels, int wanted_sample_rate)
{
    // define audio specs containers
    SDL_AudioSpec wanted_specs;
    SDL_AudioSpec specs;

    // set audio settings
    wanted_specs.freq = wanted_sample_rate;
    wanted_specs.format = AUDIO_S16SYS;
    wanted_specs.channels = wanted_nb_channels;
    wanted_specs.silence = 0;
    wanted_specs.samples = SDL_AUDIO_MIN_BUFFER_SIZE * 2;
    wanted_specs.callback = audio_callback;
    wanted_specs.userdata = arg;

    // https://trac.ffmpeg.org/ticket/6891
    #ifdef _WIN32
    CoInitialize(NULL);
    #endif

    if (!(audio_dev = SDL_OpenAudioDevice(NULL, 0, &wanted_specs, &specs, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_CHANNELS_CHANGE))) {
        cout<<"[audio]could not open audio, error:"<<SDL_GetError()<<endl;
        return -1;
    }
    return  specs.size;
}

double get_audio_clock(VideoState * is)
{
    double pts;
    int hw_buf_size, bytes_per_sec;
    pts = is->audio_clock;
    hw_buf_size = is->audio_buf_size - is->audio_buf_index;
    bytes_per_sec = 0;

    int n = 2;

    if(is->audio_st)
        bytes_per_sec = n * is->audio_avctx->channels * is->audio_avctx->sample_rate;

    if(bytes_per_sec > 0)
        pts -= (double)hw_buf_size / bytes_per_sec;

    return pts;
}

int video_decode_frame(VideoState * is, AVFrame *frame) {
    // WIP, using :  https://ffmpeg.org/doxygen/3.1/libavformat_2utils_8c_source.html#l02838
    int got_picture = 0;
    AVPacket pkt1, *pkt = &pkt1;
    AVCodecContext *avctx = is->video_avctx;

    if(is->abort_request == 1)
        return -1;

    if(packet_queue_get(&is->videoq, pkt, 1) < 0)
        return -1;

    //flush codec
    if(pkt->data == flush_pkt.data) {
        cout<<"[video] codec flush"<<endl;
        avcodec_flush_buffers(avctx);
        return 0;
    }

    //decode video frame
    if(avcodec_send_packet(avctx, pkt) == AVERROR(EAGAIN) )
        cout<<"[video] could not send the packet to decoder! error:"<<AVERROR(EAGAIN)<<endl;

    av_packet_unref(pkt);
    int ret = AVERROR(EAGAIN);

    do {
        ret = avcodec_receive_frame(avctx, frame);

        if(ret >= 0)
            got_picture = 1;

        if (ret == AVERROR_EOF) {
            cout<<"eof"<<endl;
            avcodec_flush_buffers(avctx);
            break;
        }
        if(ret >= 0)
            break;

    } while(ret != AVERROR(EAGAIN));

    if(got_picture)
    {
        frame->pts = frame->best_effort_timestamp;

        if(frame->pts == AV_NOPTS_VALUE)
            frame->pts = 0;
    }
    return got_picture;
}

int frame_queue_init(FrameQueue *f, int max_size) {
    memset(f, 0, sizeof(FrameQueue));

    if(!(f->mutex = SDL_CreateMutex())) {
        return AVERROR(ENOMEM);
    }

    if(!(f->cond = SDL_CreateCond())) {
        return AVERROR(ENOMEM);
    }

    f->max_size = max_size;
    for(int i = 0; i<max_size; i++) {
        if(!(f->queue[i].frame = av_frame_alloc())) {
            return AVERROR(ENOMEM);
        }
    }

    return 0;
}

void frame_queue_destroy(FrameQueue *f)
{
    for(int i = 0; i<f->max_size; i++) {
        av_frame_unref(f->queue[i].frame);
        av_frame_free(&f->queue[i].frame);
    }
    SDL_DestroyMutex(f->mutex);
    SDL_DestroyCond(f->cond);
}

void frame_queue_push(FrameQueue *f) {
    f->windex++;
    if(f->windex == f->max_size)
        f->windex = 0;

    SDL_LockMutex(f->mutex);
    f->size++;
    SDL_CondSignal(f->cond);
    SDL_UnlockMutex(f->mutex);
}

void frame_queue_next(FrameQueue *f) {
    if(!f->rindex_shown) {
        f->rindex_shown = 1;
        return;
    }

    av_frame_unref(f->queue[f->rindex].frame);

    f->rindex++;
    if (f->rindex == f->max_size)
        f->rindex = 0;

    SDL_LockMutex(f->mutex);
    // 8 mai
    if (f->size > 0)
        f->size--;
    SDL_CondSignal(f->cond);
    SDL_UnlockMutex(f->mutex);
}

int frame_queue_nb_remaining(FrameQueue *f) {
    return f->size - f->rindex_shown;
}

VideoPicture *frame_queue_peek(FrameQueue *f) {
    return &f->queue[(f->rindex + f->rindex_shown) % f->max_size];
}

VideoPicture *frame_queue_peek_next(FrameQueue *f) {
    return &f->queue[(f->rindex + f->rindex_shown + 1) % f->max_size];
}

VideoPicture *frame_queue_peek_last(FrameQueue *f) {
    return &f->queue[f->rindex];
}

VideoPicture *frame_queue_peek_writable(FrameQueue *f) {
    /* wait until we have space to put a new frame */
    SDL_LockMutex(f->mutex);
    while(f->size >= f->max_size) {
        SDL_CondWait(f->cond, f->mutex);
    }
    SDL_UnlockMutex(f->mutex);

    return &f->queue[f->windex];
}

VideoPicture *frame_queue_peek_readable(FrameQueue *f) {
    /* wait until we have a readable new frame */
    SDL_LockMutex(f->mutex);
    while(f->size - f->rindex_shown <= 0) {
        SDL_CondWait(f->cond, f->mutex);
    }
    SDL_UnlockMutex(f->mutex);

    return &f->queue[(f->rindex + f->rindex_shown) % f->max_size];
}

int queue_picture(VideoState * is, double pts, AVFrame *src_frame) {
    VideoPicture *vp;
    double frame_delay;

    //compute the exact PTS for the picture
    if(pts != 0)
        is->video_clock = pts;
    else {
        pts = is->video_clock;
    }
    //update video clock for next frame
    frame_delay = av_q2d(is->video_avctx->time_base);
    frame_delay += src_frame->repeat_pict * (frame_delay * 0.5);
    is->video_clock += frame_delay;

    if(!(vp = frame_queue_peek_writable(&is->pictq)))
        return -1;

    vp->uploaded = 0;
    vp->width = src_frame->width;
    vp->height = src_frame->height;
    vp->format = src_frame->format;
    vp->pts = pts;

    av_frame_move_ref(vp->frame, src_frame);
    frame_queue_push(&is->pictq);

    return 0;
}

int video_thread(void *arg)
{
    is = (VideoState *)arg;

#define VIDEO_SYNC
#ifdef VIDEO_SYNC
    // set this when we are done decoding an entire frame
    int frameFinished = 0;
#endif
    AVFrame *frame = av_frame_alloc();
    double pts = 0;
    AVRational tb = is->video_st->time_base;
    int ret = 0;

    if(!frame) {
        cout<<"video_thread : AVERROR(ENOMEM)"<<endl;
        return AVERROR(ENOMEM);
    }

    for(;;){
        ret = video_decode_frame(is, frame);
        if(ret < 0)
            goto end;
        else if(ret == 0)
            continue;
#ifdef VIDEO_SYNC
        else
            frameFinished = 1;

        // Did we get a video frame?
        if(frameFinished)
        {
            pts = synchronize_video(is, frame, pts);

        }
#endif
        pts = frame->pts * av_q2d(tb);
        ret = queue_picture(is, pts, frame);

        av_frame_unref(frame);

        if(ret < 0)
            goto end;
    }

end:
    av_frame_free(&frame);
    return 0;
}

int stream_component_open(VideoState * is, int stream_index) {
    AVFormatContext *pFormatCtx = is->ic;
    AVCodecContext *avctx;
    AVCodec *codec;
    int sample_rate, nb_channels;
    int64_t channel_layout;
    int ret = 0;

    if(stream_index < 0 || (unsigned int)stream_index >= pFormatCtx->nb_streams) {
        return -1;
    }

    avctx = avcodec_alloc_context3(NULL);
    if(!avctx)
        return AVERROR(ENOMEM);

    if(avcodec_parameters_to_context(avctx, pFormatCtx->streams[stream_index]->codecpar) < 0) {
        cout<<"avcodec_parameters_to_context() failed"<<endl;
        goto fail;
    }

    codec = avcodec_find_decoder(avctx->codec_id);
    if(!codec) {
        cout<<"avcodec_find_decoder() failed"<<endl;
        ret = AVERROR(EINVAL);
        goto fail;
    }

    if((ret = avcodec_open2(avctx, codec, NULL)) < 0) {
        cout<<"open2 failed"<<endl;
        goto fail;
    }

    switch(avctx->codec_type)
    {
        case AVMEDIA_TYPE_AUDIO:
            sample_rate = avctx->sample_rate;
            nb_channels = avctx->channels;
            channel_layout = avctx->channel_layout;

            //prepare audio output
            if((ret = audio_open(is, channel_layout, nb_channels, sample_rate)) < 0) {
                cout<<"audio open failed"<<endl;
                goto fail;
            }

            is->audio_stream = stream_index;
            is->audio_st = pFormatCtx->streams[stream_index];
            is->audio_avctx = avctx;

            //start audio packet queue
            packet_queue_start(&is->audioq);

            //SDL_PauseAudio(0);
            SDL_PauseAudioDevice(audio_dev, 0);

        break;

        case AVMEDIA_TYPE_VIDEO:
            is->video_stream = stream_index;
            is->video_st = pFormatCtx->streams[stream_index];
            is->video_avctx = avctx;

            //start video packet queue
            packet_queue_start(&is->videoq);
            is->video_tid = SDL_CreateThread(video_thread, "video_thread", is);

            if(!is->video_tid)
            {
                cout<<"video tid error"<<endl;
                ret = -1;
                goto fail;
            }
            else
                ret = 0;
        break;

        default:
        break;
    }


    goto out;

fail:
    avcodec_free_context(&avctx);

out:
    return ret;
}

int read_thread(void * arg) {

    is = (VideoState *)arg;

    AVFormatContext *pFormatCtx = NULL;
    AVPacket pkt1, *pkt = &pkt1;
    int video_stream = -1;
    int audio_stream = -1;
    int ret = 0;
    int eof = 0;

    is->abort_request = 0;
    is->audio_stream = -1;
    is->video_stream = -1;

    int count = 0;

    while ((!pFormatCtx)||(count<10))
    {
        pFormatCtx = avformat_alloc_context();
        count++;
    }

    if(!pFormatCtx) {
        ret = AVERROR(ENOMEM);
        cout<<"AVERROR(ENOMEM)"<<endl;
        goto fail;
    }

    // open file. keep that for a little while
    // if(avformat_open_input(&pFormatCtx, static_cast <const char * >( m_FilePath.c_str()), NULL, NULL) < 0)

    if(avformat_open_input(&pFormatCtx, is->filename, NULL, NULL) < 0)
    {
        ret = -1;
                cout<<"(read_thread) filename  = "<<   is->filename << endl;
                cout<<"(read_thread) current_video_filename  = "<<   current_video_filename << endl;
                cout<<"(read_thread) &pFormatCtx = " << &pFormatCtx <<endl;
        goto fail;
    }

    //retrieve stream information
    if(avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        ret = -1;
                cout<<"(read_thread) no stream information"<<endl;
        goto fail;
    }

    av_dump_format(pFormatCtx, 0, is->filename, 0);
    is->ic = pFormatCtx;

    if ((is) && (is->ic != 0))
    {
        video_duration = is->ic->duration/(double)AV_TIME_BASE;
        fprintf(stdout, "video_duration = %f (currently in %s) \n", video_duration, __FUNCTION__);
    }
    else
        fprintf(stdout, "is->ic is null\n");

    //find video/audio stream
    for(unsigned int i = 0; i<pFormatCtx->nb_streams; i++) {
        AVStream *st = pFormatCtx->streams[i];
        enum AVMediaType type = st->codecpar->codec_type;
        if(type == AVMEDIA_TYPE_VIDEO && video_stream < 0) {
            video_stream = i;
        }
        else if(type == AVMEDIA_TYPE_AUDIO && audio_stream < 0) {
            audio_stream = i;
        }
    }

    //open video stream
    if(video_stream >= 0)
        stream_component_open(is, video_stream);

    //open audio stream
    if(audio_stream >= 0)
        stream_component_open(is, audio_stream);

    if(video_stream < 0 && audio_stream < 0) {
        ret = -1;
        cout<<"video_stream <0 && audio_stream <0 !"<<endl;
        goto fail;
    }

    for(;;) {
        if(is->abort_request)
            break;

        if(is->seek_req) {

            cout << "is->seek_req = " << is->seek_req << endl;
            cout << "is->seek_rel = " << is->seek_rel << endl;

            int64_t seek_target = is->seek_pos;
            cout << "seek_target = " << seek_target << endl;

//#define NEW_SEEK_TEST
#ifndef NEW_SEEK_TEST
            int64_t seek_min    = is->seek_rel > 0 ? seek_target - is->seek_rel + 2: INT64_MIN;
            int64_t seek_max    = is->seek_rel < 0 ? seek_target - is->seek_rel - 2: INT64_MAX;

            cout << "seek_min = "    << seek_min    << endl;
            cout << "seek_max = "    << seek_max    << endl;

            ret = avformat_seek_file(is->ic, -1, seek_min, seek_target, seek_max, is->seek_flags);

//////////////////////////////////////
#else
            if((video_stream >= 0) && (audio_stream >= 0))
            {
                seek_target = av_rescale_q(seek_target, AV_TIME_BASE_Q, is->ic->streams[video_stream]->time_base);

                seek_target = av_rescale_q(seek_target, AV_TIME_BASE_Q, is->ic->streams[audio_stream]->time_base);
            }

            ret = av_seek_frame(is->ic, video_stream, seek_target, is->seek_flags);

            ret &= av_seek_frame(is->ic, audio_stream, seek_target, is->seek_flags);
#endif
            if(ret <0)
            {
                cout<<"ret <0, error while seeking"<<endl;
            }
            else
            {
                cout<<"ret <0, error while seeking"<<endl;

                if(is->audio_stream >= 0)
                {

                    cout<<"is->audio_stream vaut : "<<  is-> audio_stream << endl;

                    packet_queue_flush(&is->audioq);
                    packet_queue_put(&is->audioq, &flush_pkt);
                }

                if(is->video_stream >= 0) {

                    cout<<"is->video_stream vaut : "<<  is-> audio_stream << endl;

                    packet_queue_flush(&is->videoq);
                    packet_queue_put(&is->videoq, &flush_pkt);
                }
            }

            is->seek_req = 0;
            eof = 0;
        }

        if(eof)
        {
            //play end
            if(is->audioq.size + is->videoq.size == 0) {
                ret = AVERROR_EOF;
                goto fail;
            }

            eof = 0;
            continue;
        }

        if((ret = av_read_frame(pFormatCtx, pkt)) < 0) {
            if(ret == AVERROR_EOF) {
                eof = 1;
            }

            if(pFormatCtx->pb && pFormatCtx->pb->error)
                break;

            SDL_Delay(10);
            continue;
        }
        else {
            eof = 0;
        }

        if(pkt->stream_index == is->audio_stream) {
            //cout<<"put audio packet"<<endl;
            packet_queue_put(&is->audioq, pkt);
        }
        else if(pkt->stream_index == is->video_stream) {
            //cout<<"put video packet"<<endl;
            packet_queue_put(&is->videoq, pkt);
        }
        else {
            av_packet_unref(pkt);
        }
    }

    ret = 0;
fail:
    if(pFormatCtx && !is->ic) {
        avformat_close_input(&pFormatCtx);
    }
    return 0;
}

void video_image_display(VideoState * is) 
{
    VideoPicture *vp = frame_queue_peek_last(&is->pictq);

    if(!vp->uploaded) 
    {
        vp->uploaded = 1;
        vp->flip_v = vp->frame->linesize[0] < 0;
    }
    videoMat = avframe_to_cvmat(vp->frame);
}

double compute_target_delay(double last_duration, VideoState * is, double pts)
{
    double sync_threshold, diff = 0;
    double delay_av = 0;

    diff = pts - get_audio_clock(is); //diff<0 => video slow, diff>0 => video quick
    sync_threshold = FFMAX(AV_SYNC_THRESHOLD_MIN, FFMIN(AV_SYNC_THRESHOLD_MAX, last_duration));

    if(fabs(diff) < AV_NOSYNC_THRESHOLD)
    {
        if(diff <= -sync_threshold) { //video slow
            delay_av = FFMAX(0, last_duration + diff);
        }
        else if(diff >= sync_threshold && last_duration > AV_SYNC_FRAMEDUP_THRESHOLD) { //video fast
            delay_av = last_duration + diff;
        }
        else if(diff >= sync_threshold) { //video goes too fast
            delay_av = 2 * last_duration;
        }
    }
    return delay_av;
}

//display each frame
void video_refresh(void *arg, double *remaining_time)
{
    is = (VideoState *)arg;

    if(is->video_st)
    {
        if (frame_queue_nb_remaining(&is->pictq) == 0) 
        {
            // nothing to do, no picture to display in the queue
        }
        else {
            double last_duration, delay_av;
            double time;
            VideoPicture *vp, *lastvp;

            //dequeue the picture
            lastvp = frame_queue_peek_last(&is->pictq);
            vp = frame_queue_peek(&is->pictq);

            last_duration = vp->pts - lastvp->pts;

            if(last_duration <= 0 || last_duration >= 1.0)
                last_duration = lastvp->pts;

            delay_av = compute_target_delay(last_duration, is, vp->pts);

            time= av_gettime_relative()/1000000.0;
            if (time < is->frame_timer + delay_av) {
                *remaining_time = FFMIN(is->frame_timer + delay_av - time, *remaining_time);
                goto display;
            }

            is->frame_timer += delay_av;
            if (delay_av > 0 && time - is->frame_timer > AV_SYNC_THRESHOLD_MAX)
                is->frame_timer = time;

            frame_queue_next(&is->pictq);
display:
            if ((is->pictq.rindex_shown) && (is->video_st))
                video_image_display(is);
        }
    }

}


void stream_seek(VideoState* is, int64_t pos, int rel, int seek_by_bytes)
{
    if(!is->seek_req)
    {
        is->seek_pos = pos;
        is->seek_rel = rel;
        is->seek_flags &= ~AVSEEK_FLAG_BYTE;

        if (seek_by_bytes)
            is->seek_flags |= AVSEEK_FLAG_BYTE;
        is->seek_req = 1;
    }
}

int main2(char * filename) {
    is = NULL;
#if LIBAVFORMAT_VERSION_INT < AV_VERSION_INT(58, 9, 100)
    av_register_all();
#endif
    SDL_Init(SDL_INIT_EVERYTHING);

    avformat_network_init();

    av_init_packet(&flush_pkt);
    flush_pkt.data = (uint8_t *)&flush_pkt;

    is = (VideoState *)av_mallocz(sizeof(VideoState));

    if(!is)
        return -1;

    is->filename = av_strdup(filename);

    old_video_filename = av_strdup(default_video_filename);
    current_video_filename = av_strdup(is->filename);

    std::cout <<  " av_sync_type =  " << is->av_sync_type << std::endl;

    if(!is->filename) {
        av_free(is);
        return -1;
    }

    /* start video display */
    if(frame_queue_init(&is->pictq, VIDEO_PICTURE_QUEUE_SIZE) < 0)
    {
        fprintf(stdout, "&is->pictq, VIDEO_PICTURE_QUEUE_SIZE) < 0");
        goto fail;
    }

    if (packet_queue_init(&is->audioq) < 0 || packet_queue_init(&is->videoq) < 0)
    {
        cout << "packet_queue_init(&is->audioq) < 0 || packet_queue_init(&is->videoq) < 0" << endl;
        goto fail;
    }

    is->read_tid = SDL_CreateThread(read_thread, "read_thread", is);

    if(!is->read_tid) {

fail:
        cout << "read_thread failed :-/" << endl;
        is->abort_request = 1;
        SDL_WaitThread(is->read_tid, NULL);

         //close each stream 
        if (is->audio_stream >= 0)
            stream_component_close(is, is->audio_stream);

        if (is->video_stream >= 0)
            stream_component_close(is, is->video_stream);

        avformat_close_input(&is->ic);
        packet_queue_destroy(&is->videoq);
        packet_queue_destroy(&is->audioq);

        // free all pictures 
        frame_queue_destroy(&is->pictq);

        if (is->img_convert_ctx != nullptr)
            sws_freeContext(is->img_convert_ctx);

        is->filename = nullptr;

        av_free(is->filename);

        av_free(is);
        return -1;
    }
    return 0;
}

void audio_callback(void *arg, uint8_t *stream, int len){

    is = (VideoState *)arg;
    int n, audio_size;

    uint8_t audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];

    double pts;
    SDL_memset(stream, 0, len);

    while(len > 0)
    {
        if(is->audio_buf_index >= is->audio_buf_size)
        {
            /*we have already set all our data, get more*/
            audio_size = audio_decode_frame(is, audio_buf, sizeof(audio_buf), &pts);

            if(audio_size < 0) {
                /*error output silence*/
                is->audio_buf_size = 1024;
                memset(audio_buf, 0, is->audio_buf_size);
            }
            else {
                is->audio_buf_size = audio_size;
            }
            is->audio_buf_index = 0;
        }

        n = is->audio_buf_size - is->audio_buf_index;

        if(n > len)
            n = len;

        memcpy(stream, (uint8_t *)audio_buf + is->audio_buf_index, n);
        //SDL_MixAudio(stream, audio_buf + audio_buf_index, len, SDL_MIX_MAXVOLUME);
        len -= n;
        stream += n;
        is->audio_buf_index += n;
    }
}

int audio_decode_frame(VideoState * is, uint8_t *audio_buf, int buf_size, double *pts_ptr)
{
    AVPacket pkt1, *pkt = &pkt1;

    AVFrame *pFrame = av_frame_alloc();
    AVCodecContext *avctx = is->audio_avctx;
    int data_size = -1;
    //int size = 0;
    int64_t dec_channel_layout;
    SwrContext *swrCtx = NULL;
    //AVRational tb;
    double pts;

    if(is->abort_request == 1)
        return -1;

    if(packet_queue_get(&is->audioq, pkt, 1) < 0) {
        return -1;
    }

    //flush codec
    if(pkt->data == flush_pkt.data) {
        cout<<"[audio] codec flush"<<endl;
        avcodec_flush_buffers(avctx);
        return 0;
    }

    //if update, update the audio clock with pts
    if(pkt->pts != AV_NOPTS_VALUE) {
        is->audio_clock = av_q2d(is->audio_st->time_base) * pkt->pts;
    }

    //decode audio frame
    if(avcodec_send_packet(avctx, pkt) == AVERROR(EAGAIN) ) {
        cout<<"[audio] could not send the packet to decoder! error:"<<AVERROR(EAGAIN)<<endl;
    }
    av_packet_unref(pkt);

    int ret = AVERROR(EAGAIN);

    do {
        ret = avcodec_receive_frame(avctx, pFrame);
        if(ret >= 0) {
            //get audio channel layout
            if(pFrame->channel_layout && pFrame->channels == av_get_channel_layout_nb_channels(pFrame->channel_layout)) {
                dec_channel_layout = pFrame->channel_layout;
            }
            else {
                dec_channel_layout = av_get_default_channel_layout(pFrame->channels);
            }

            AVSampleFormat dec_format = AV_SAMPLE_FMT_S16;
            swrCtx = swr_alloc_set_opts(NULL, dec_channel_layout, dec_format, pFrame->sample_rate, pFrame->channel_layout, (AVSampleFormat)pFrame->format, pFrame->sample_rate, 0, NULL);
            if(!swrCtx || swr_init(swrCtx) < 0) {
                break;
            }

            int dec_nb_samples = av_rescale_rnd(swr_get_delay(swrCtx, pFrame->sample_rate) + pFrame->nb_samples, pFrame->sample_rate, pFrame->sample_rate, AVRounding(1));
            int nb = swr_convert(swrCtx, &audio_buf, dec_nb_samples, (const uint8_t **)pFrame->data, pFrame->nb_samples);
            data_size = pFrame->channels * nb * av_get_bytes_per_sample(dec_format);
        }

        if (ret == AVERROR_EOF) {
            cout<<"eof"<<endl;
            avcodec_flush_buffers(avctx);
            break;
        }

        if(ret >= 0)
        {
            pts = is->audio_clock;
            *pts_ptr = pts;
            is->audio_clock += (double)data_size / (double)(2 * avctx->channels * avctx->sample_rate);
            break;
        }

    } while(ret != AVERROR(EAGAIN));

    av_frame_free(&pFrame);
    if(swrCtx)
        swr_free(&swrCtx);

    return data_size;
}

void packet_queue_destroy(PacketQueue *q)
{
    packet_queue_flush(q);
    SDL_DestroyMutex(q->mutex);
    SDL_DestroyCond(q->cond);
}

void packet_queue_abort(PacketQueue *q)
{
    SDL_LockMutex(q->mutex);

    q->abort_request = 1;
    SDL_CondSignal(q->cond);

    SDL_UnlockMutex(q->mutex);
}

int packet_queue_init(PacketQueue *q) {
    memset(q, 0, sizeof(PacketQueue));
    q->mutex = SDL_CreateMutex();
    if(!q->mutex) {
        return AVERROR(ENOMEM);
    }

    q->cond = SDL_CreateCond();
    if(!q->cond) {
        return AVERROR(ENOMEM);
    }

    // FIXME : really needed ?
    q->abort_request = 1;

    return 0;
}

void packet_queue_flush(PacketQueue *q) {
    AVPacketList *pkt, *pktl;
   
    SDL_LockMutex(q->mutex);
    for(pkt = q->first_pkt; pkt != NULL; pkt = pktl) {
        pktl = pkt->next;
        av_packet_unref(&pkt->pkt);
        av_freep(&pkt);
    }

    q->last_pkt = NULL;
    q->first_pkt = NULL;
    q->nb_packets = 0;
    q->size = 0;
 
    SDL_UnlockMutex(q->mutex);
}

void packet_queue_start(PacketQueue *q) {
    SDL_LockMutex(q->mutex);
    q->abort_request = 0;
    SDL_UnlockMutex(q->mutex);
}

int packet_queue_put(PacketQueue *q, AVPacket *pkt) {
    AVPacketList *pktl;
    AVPacket copy = {0};

    if(pkt != &flush_pkt && av_packet_ref(&copy, pkt) < 0) {
        return -1;
    }

    SDL_LockMutex(q->mutex);

    if(q->abort_request) {
        return -1;
    }

    pktl = (AVPacketList *)av_malloc(sizeof(AVPacketList));
    if(!pktl) {
        return -1;
    }

    pktl->pkt = *pkt;
    pktl->next = NULL;

    if(!q->last_pkt)
        q->first_pkt = pktl;
    else
        q->last_pkt->next = pktl;

    q->last_pkt = pktl;
    q->nb_packets++;
    q->size += pktl->pkt.size;
    
    SDL_CondSignal(q->cond);
    SDL_UnlockMutex(q->mutex);

    return 0;
}

int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block)
{
    AVPacketList *pktl;
    int ret = -1;
    
    SDL_LockMutex(q->mutex);
 
    for(;;) {
        if(q->abort_request) {
            ret = -1;
            break;
        }

        pktl = q->first_pkt;
        if(pktl) {
            q->first_pkt = pktl->next;

            if(!q->first_pkt) {
                q->last_pkt = NULL;
            }

            q->nb_packets--;
            q->size -= pktl->pkt.size;
            *pkt = pktl->pkt;
            av_free(pktl);
            
            ret = 1;
            break;
        }
        else if(!block) {
            ret = 0;
            break;
        }
        else {
            //SDL_CondWaitTimeout(q->cond, q->mutex, 10);
            SDL_CondWait(q->cond, q->mutex);
        }
    }
    SDL_UnlockMutex(q->mutex);

    return ret;
}

// inspired from dranger tutorial07.c
double synchronize_video(VideoState * is, AVFrame * src_frame, double pts)
{
    double av_frame_delay;
    // either we have pts, or we have clock
    if (pts != 0)
        is->video_clock = pts;
    else
        pts = is->video_clock;

    // now update the frame delay
    av_frame_delay = av_q2d(is->video_avctx->time_base);

    // if we are repeating a frame, let's adjust tclock accordingly
    av_frame_delay += src_frame->repeat_pict * (av_frame_delay * 0.5);

    return pts;
}

/**
 * taken from tutorial07.c dranger
 * Calculates and returns the current video clock reference value.
 *
 * @param   is the global VideoState reference.
 *
 * @return              the current video clock reference value.
 */
double get_video_clock(VideoState * videoState)
{
    double delta = (av_gettime() - videoState->video_current_pts_time) / 1000000.0;

    return videoState->video_current_pts + delta;
}

/**
 * taken from tutorial07.c dranger
 * Checks the VideoState global reference av_sync_type variable and then calls
 * get_audio_clock, get_video_clock, or get_external_clock accordingly.
 *
 * @param   is  the global VideoState reference.
 *
 * @return              the reference clock according to the chosen AV sync type.
 */
double get_master_clock(VideoState * is)
{
    if (is->av_sync_type == AV_SYNC_VIDEO_MASTER)
    {
        return get_video_clock(is);
    }
    else if (is->av_sync_type == AV_SYNC_AUDIO_MASTER)
    {
        return get_audio_clock(is);
    }
    else if (is->av_sync_type == AV_SYNC_EXTERNAL_MASTER)
    {
        return get_external_clock(is);
    }
    else
    {
        fprintf(stderr, "Error: Undefined A/V sync type.");
        return -1;
    }
}

/**
 * taken from tutorial07.c dranger
 * Calculates and returns the current external clock reference value: the computer
 * clock.
 *
 * @return  the current external clock reference value.
 */
double get_external_clock(VideoState * is)
{
    is->external_clock_time = av_gettime();
    is->external_clock = is->external_clock_time / 1000000.0;

    return is->external_clock;
}

void saveFrame(AVFrame *pFrame, int w, int h, int iFrame) {
    FILE *pFile = NULL;
    char szFileName[32];

    //open file
    sprintf(szFileName, "frame%d.ppm", iFrame);
    pFile = fopen(szFileName, "wb");

    if(pFile == NULL)
        return;

    //write header
    fprintf(pFile, "P6\n%d %d\n255\n", w, h);

    //write pixel data
    fwrite(pFrame->data[0], 1, w * h, pFile);
    fwrite(pFrame->data[1], 1, w * h /4, pFile);
    fwrite(pFrame->data[2], 1, w * h /4, pFile);
    fclose(pFile);
}

