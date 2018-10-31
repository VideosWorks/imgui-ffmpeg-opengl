#include <assert.h>

//#ifdef NATIVE_BUILD
#include <GL/glew.h>
#include <GL/gl.h>
//#endif
#include <stdlib.h>

#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h> // so yuv2rgb works

#define IMAGE_ALIGN 1

#include "sdl2_ffmpeg/kiterror.h"
#include "sdl2_ffmpeg/internal/kitlibstate.h"
#include "sdl2_ffmpeg/internal/kitdecoder.h"
#include "sdl2_ffmpeg/internal/utils/kithelpers.h"
#include "sdl2_ffmpeg/internal/utils/kitbuffer.h"
#include "sdl2_ffmpeg/internal/video/kitvideo.h"
#include "sdl2_ffmpeg/internal/utils/kitlog.h"

//#include <opencv2/opencv.hpp>
/*
// gcc has no __mm_malloc(size_t, size, sise_t align) nor __free(size_t size)
static inline void* aligned_malloc(size_t size, size_t align) {
    void *result;
    #ifdef _MSC_VER 
    result = _aligned_malloc(size, align);
    #else 
     if(posix_memalign(&result, align, size)) result = 0;
    #endif
    return result;
}
*/
static inline void aligned_free(void *ptr) {
    #ifdef _MSC_VER 
        _aligned_free(ptr);
    #else 
      free(ptr);
    #endif

}

static struct SwsContext * convert_ctx = NULL;
static struct SwsContext * img_convert_ctx2 = NULL;


#define KIT_VIDEO_SYNC_THRESHOLD 0.01

static int count = 0;
static int first_time = 1;
static int err = 0;

enum AVPixelFormat supported_list[] = {
    AV_PIX_FMT_YUV420P,
    AV_PIX_FMT_YUYV422,
    AV_PIX_FMT_UYVY422,
    AV_PIX_FMT_NV12,
    AV_PIX_FMT_NV21,
    AV_PIX_FMT_RGB24,
    AV_PIX_FMT_BGR24,
    AV_PIX_FMT_RGB555,
    AV_PIX_FMT_BGR555,
    AV_PIX_FMT_RGB565,
    AV_PIX_FMT_BGR565,
    AV_PIX_FMT_BGRA,
    AV_PIX_FMT_RGBA,
    AV_PIX_FMT_NONE
};

#ifdef DEBUG

static void yuvToRgb24(uint8_t* _src, uint8_t* _dst, int _width,int _height)
{
    uint8_t* _src_planes[3] = {_src,_src + _width*_height, _src + (_width * _height * 3/2) };

    int _src_stride[3] = {_width, _width/2, _width/2};

    uint8_t* _dest_planes[3] = {_dst,NULL,NULL};

    int _dest_stride[3] = {_width*3,0,0};

    struct SwsContext *_img_convert_ctx = sws_getContext(
                                                             _width,_height, AV_PIX_FMT_YUV420P,
                                                             _width, _height,AV_PIX_FMT_BGR24,
                                                             SWS_POINT,NULL,NULL,NULL
                                                        );
    sws_scale(  _img_convert_ctx,
                (const unsigned char * const *)_src_planes,
                _src_stride,
                0,
                _height,
                _dest_planes,
                _dest_stride
             );

//    if(_img_convert_ctx != NULL)
//        sws_freeContext(_img_convert_ctx);
}

static int une_fois = 1;

static int once = 1;

char * supported_list_names[] = {
    "AV_PIX_FMT_YUV420P",
    "AV_PIX_FMT_YUYV422",
    "AV_PIX_FMT_UYVY422",
    "AV_PIX_FMT_NV12",
    "AV_PIX_FMT_NV21",
    "AV_PIX_FMT_RGB24",
    "AV_PIX_FMT_BGR24",
    "AV_PIX_FMT_RGB555",
    "AV_PIX_FMT_BGR555",
    "AV_PIX_FMT_RGB565",
    "AV_PIX_FMT_BGR565",
    "AV_PIX_FMT_BGRA",
    "AV_PIX_FMT_RGBA",
    "AV_PIX_FMT_NONE"
};


char * getAvPixelStringName( unsigned int AVPixelFormat)
{
   char * toReturn = supported_list_names[3]; // default
   switch( AVPixelFormat)
   {
     // MUST match with supported list_names[]
     case AV_PIX_FMT_YUV420P: toReturn = supported_list_names[0]; break;
     case AV_PIX_FMT_YUYV422: toReturn = supported_list_names[1]; break;
     case AV_PIX_FMT_UYVY422: toReturn = supported_list_names[2]; break;
     case AV_PIX_FMT_NV12:    toReturn = supported_list_names[3]; break;
     case AV_PIX_FMT_NV21:    toReturn = supported_list_names[4]; break;
     case AV_PIX_FMT_RGB24:   toReturn = supported_list_names[5]; break;
     case AV_PIX_FMT_BGR24:   toReturn = supported_list_names[6]; break;
     case AV_PIX_FMT_RGB555:  toReturn = supported_list_names[7]; break;
     case AV_PIX_FMT_BGR555:  toReturn = supported_list_names[8]; break;
     case AV_PIX_FMT_RGB565:  toReturn = supported_list_names[9]; break;
     case AV_PIX_FMT_BGR565:  toReturn = supported_list_names[10]; break;
     case AV_PIX_FMT_BGRA:    toReturn = supported_list_names[11]; break;
     case AV_PIX_FMT_RGBA:    toReturn = supported_list_names[12]; break;
     case AV_PIX_FMT_NONE:    toReturn = supported_list_names[13]; break;

     default:                 toReturn = supported_list_names[13]; break;
   }

   return toReturn;
}

char * supported_list_SDLnames[] = {
    "SDL_PIXELFORMAT_YV12",
    "SDL_PIXELFORMAT_YUY2",
    "SDL_PIXELFORMAT_UYVY",
    "SDL_PIXELFORMAT_NV12",
    "SDL_PIXELFORMAT_NV21",
    "SDL_PIXELFORMAT_RGBA32",
    "SDL_PIXELFORMAT_DONT_KNOW"
};

char * getSDLPixelStringName( unsigned int SDLPixelFormat)
{
   char * toReturn = supported_list_names[3]; // default
   switch( SDLPixelFormat)
   {
     // MUST match with supported_list_SDLnames[]
     case SDL_PIXELFORMAT_YV12:   toReturn = supported_list_SDLnames[0]; break;
     case SDL_PIXELFORMAT_YUY2:   toReturn = supported_list_SDLnames[1]; break;
     case SDL_PIXELFORMAT_UYVY:   toReturn = supported_list_SDLnames[2]; break;
     case SDL_PIXELFORMAT_NV12:   toReturn = supported_list_SDLnames[3]; break;
     case SDL_PIXELFORMAT_NV21:   toReturn = supported_list_SDLnames[4]; break;
     case SDL_PIXELFORMAT_RGBA32: toReturn = supported_list_SDLnames[5]; break;

     default: toReturn = supported_list_names[6]; break;
   }

   return toReturn;
}
#endif

typedef struct Kit_VideoDecoder {
    struct SwsContext *sws;
    AVFrame *scratch_frame;
} Kit_VideoDecoder;

typedef struct Kit_VideoPacket {
    double pts;
    AVFrame *frame;
} Kit_VideoPacket;


static Kit_VideoPacket* _CreateVideoPacket(AVFrame *frame, double pts) {
    Kit_VideoPacket *p = calloc(1, sizeof(Kit_VideoPacket));
    p->frame = frame;
    p->pts = pts;
    return p;
}

static unsigned int _FindSDLPixelFormat(enum AVPixelFormat fmt) {
    switch(fmt) {
        case AV_PIX_FMT_YUV420P:
            return SDL_PIXELFORMAT_YV12;
        case AV_PIX_FMT_YUYV422:
            return SDL_PIXELFORMAT_YUY2;
        case AV_PIX_FMT_UYVY422:
            return SDL_PIXELFORMAT_UYVY;
        case AV_PIX_FMT_NV12:
            return SDL_PIXELFORMAT_NV12;
        case AV_PIX_FMT_NV21:
            return SDL_PIXELFORMAT_NV21;
        default:
            return SDL_PIXELFORMAT_RGBA32;
    }
}

static enum AVPixelFormat _FindAVPixelFormat(unsigned int fmt) {
    switch(fmt) {
        case SDL_PIXELFORMAT_YV12: return AV_PIX_FMT_YUV420P;
        case SDL_PIXELFORMAT_YUY2: return AV_PIX_FMT_YUYV422;
        case SDL_PIXELFORMAT_UYVY: return AV_PIX_FMT_UYVY422;
        case SDL_PIXELFORMAT_NV12: return AV_PIX_FMT_NV12;
        case SDL_PIXELFORMAT_NV21: return AV_PIX_FMT_NV21;
        case SDL_PIXELFORMAT_ARGB32: return AV_PIX_FMT_ARGB;
        case SDL_PIXELFORMAT_RGBA32: return AV_PIX_FMT_RGBA;
        case SDL_PIXELFORMAT_BGR24: return AV_PIX_FMT_BGR24;
        case SDL_PIXELFORMAT_RGB24: return AV_PIX_FMT_RGB24;
        case SDL_PIXELFORMAT_RGB555: return AV_PIX_FMT_RGB555;
        case SDL_PIXELFORMAT_BGR555: return AV_PIX_FMT_BGR555;
        case SDL_PIXELFORMAT_RGB565: return AV_PIX_FMT_RGB565;
        case SDL_PIXELFORMAT_BGR565: return AV_PIX_FMT_BGR565;
        default:
            return AV_PIX_FMT_NONE;
    }
}

static void free_out_video_packet_cb(void *packet) {
    Kit_VideoPacket *p = packet;
    av_freep(&p->frame->data[0]);
    av_frame_free(&p->frame);
    free(p);
}

static void dec_decode_video_cb(Kit_Decoder *dec, AVPacket *in_packet) {
    assert(dec != NULL);
    assert(in_packet != NULL);
    
    Kit_VideoDecoder *video_dec = dec->userdata;
    AVFrame *out_frame;
    int frame_finished;
    int len;
    double pts;
    Kit_VideoPacket *out_packet;

    while(in_packet->size > 0) {
        // source : https://blogs.gentoo.org/lu_zero/2016/03/29/new-avcodec-api/
        // It took me some times to make it work, and not sure I'm using it as explained though / ericb
        len = avcodec_send_packet(dec->codec_ctx, in_packet);
        if (len == AVERROR(EAGAIN)) {
            len = 0;
        }

        len = avcodec_receive_frame(dec->codec_ctx, video_dec->scratch_frame);
        if (len == AVERROR_EOF) {
            frame_finished = 1;
            len = 0;
        }
        else if(len < 0) {
            return;
        }
        else {
            len = in_packet->size;
        }


        if(frame_finished) {
            // Target frame
            out_frame = av_frame_alloc();
            av_image_alloc(
                    out_frame->data,
                    out_frame->linesize,
                    dec->codec_ctx->width,
                    dec->codec_ctx->height,
                    _FindAVPixelFormat(dec->output.format),
                    1);

            // Scale from source format to target format, don't touch the size
            sws_scale(
                video_dec->sws,
                (const unsigned char * const *)video_dec->scratch_frame->data,
                video_dec->scratch_frame->linesize,
                0,
                dec->codec_ctx->height,
                out_frame->data,
                out_frame->linesize);

            // Get presentation timestamp
#ifndef FF_API_FRAME_GET_SET
            pts = av_frame_get_best_effort_timestamp(video_dec->scratch_frame);
#else
            pts = video_dec->scratch_frame->best_effort_timestamp;
#endif
            pts *= av_q2d(dec->format_ctx->streams[dec->stream_index]->time_base);

            // Lock, write to audio buffer, unlock
            out_packet = _CreateVideoPacket(out_frame, pts);
            Kit_WriteDecoderOutput(dec, out_packet);
        }
        in_packet->size -= len;
        in_packet->data += len;
    }
}

static void dec_close_video_cb(Kit_Decoder *dec) {
    if(dec == NULL) return;

    Kit_VideoDecoder *video_dec = dec->userdata;
    if(video_dec->scratch_frame != NULL) {
        av_frame_free(&video_dec->scratch_frame);
    }
    if(video_dec->sws != NULL) {
        sws_freeContext(video_dec->sws);
    }
    free(video_dec);
}

Kit_Decoder* Kit_CreateVideoDecoder(const Kit_Source *src, int stream_index, Kit_RendererType rType) {
    assert(src != NULL);
    if(stream_index < 0) {
        return NULL;
    }

    Kit_LibraryState *state = Kit_GetLibraryState();

    // First the generic decoder component ...
    Kit_Decoder *dec = Kit_CreateDecoder(
        src,
        stream_index,
        state->video_buf_frames,
        free_out_video_packet_cb,
        state->thread_count);
    if(dec == NULL) {
        goto exit_0;
    }

    // ... then allocate the video decoder
    Kit_VideoDecoder *video_dec = calloc(1, sizeof(Kit_VideoDecoder));
    if(video_dec == NULL) {
        goto exit_1;
    }

    // Create temporary video frame
    video_dec->scratch_frame = av_frame_alloc();
    if(video_dec->scratch_frame == NULL) {
        Kit_SetError("Unable to initialize temporary video frame");
        goto exit_2;
    }

    // Find best output format for us
    enum AVPixelFormat output_format = avcodec_find_best_pix_fmt_of_list(
        supported_list, dec->codec_ctx->pix_fmt, 1, NULL);

    // Set format configs
    Kit_OutputFormat output;
    memset(&output, 0, sizeof(Kit_OutputFormat));
    output.width = dec->codec_ctx->width;
    output.height = dec->codec_ctx->height;

    switch (rType)
    {
        case KIT_OPENGL_RENDERER:
            output.format = SDL_PIXELFORMAT_BGR24;
        break;

        case KIT_OPENCV_RENDERER:
        case KIT_DEFAULT_RENDERER:
        case KIT_SDL_RENDERER:
        default:
            output.format = _FindSDLPixelFormat(output_format);
        break;
    }

#ifdef DEBUG
    if (once)
    {
        fprintf(stdout, "\n");
        fprintf(stdout, "Source format (ffmpeg output) : \n");
        fprintf(stdout, "(AVPixel source fmt) dec->codec_ctx->pix_fmt           : %d\n", dec->codec_ctx->pix_fmt);
        fprintf(stdout, "AVPixel  name getAvPixelStringName(dec->codec_ctx->pix_fmt)  : %s\n", getAvPixelStringName(dec->codec_ctx->pix_fmt));
        fprintf(stdout, "SDLPixel name associated to _FindSDLPixelFormat(dec->codec_ctx->pix_fmt)  : %s\n\n", getSDLPixelStringName(_FindSDLPixelFormat(dec->codec_ctx->pix_fmt)));
        fprintf(stdout, "Original video size : \n");
        fprintf(stdout, "dec->codec_ctx->width             : %d\n", dec->codec_ctx->width);
        fprintf(stdout, "dec->codec_ctx->height            : %d\n", dec->codec_ctx->height);
        fprintf(stdout, "Target format (used as input for SDL_Texture creation) : \n");
        fprintf(stdout, "Best output format for us, _FindSDLPixelFormat(output_format) = output.format : %u\n", output.format);
        fprintf(stdout, "SDLPixel name associated to output.format : %s\n", getSDLPixelStringName(output.format));
        fprintf(stdout, "AVPixel name associated  to output.format SDLPixel name : %s\n\n", getAvPixelStringName(_FindAVPixelFormat(output.format)));
    }
#endif

    // FIXME
    //http://www.aieq.xyz/?questions/46159322/ffmpeg-poor-yuv-to-rgb-in-sws-scale-was-ffmpeg-blocky-chroma-decoding
    // SWS_FULL_CHR_H_INT | SWS_ACCURATE_RND|SWS_BILINEAR,

    // Create scaler for handling format changes
    video_dec->sws = sws_getContext(
        dec->codec_ctx->width, // Source w
        dec->codec_ctx->height, // Source h
        dec->codec_ctx->pix_fmt, // Source fmt
        dec->codec_ctx->width, // Target w
        dec->codec_ctx->height, // Target h
        _FindAVPixelFormat(output.format), // Target fmt
        SWS_BILINEAR,
        //SWS_FULL_CHR_H_INT | SWS_ACCURATE_RND|SWS_BILINEAR,
        NULL, NULL, NULL);
    if(video_dec->sws == NULL) {
        Kit_SetError("Unable to initialize video converter context");
        goto exit_3;
    }
#ifdef DEBUG
    else
        fprintf(stdout, "videoconverter context initialized \n");
#endif

    // Set callbacks and userdata, and we're go
    dec->dec_decode = dec_decode_video_cb;
    dec->dec_close = dec_close_video_cb;
    dec->userdata = video_dec;
    dec->output = output;
    return dec;

exit_3:
    av_frame_free(&video_dec->scratch_frame);
exit_2:
    free(video_dec);
exit_1:
    Kit_CloseDecoder(dec);
exit_0:
    return NULL;
}

double Kit_GetVideoDecoderPTS(Kit_Decoder *dec) {
    Kit_VideoPacket *packet = Kit_PeekDecoderOutput(dec);
    if(packet == NULL) {
        return -1.0;
    }
    return packet->pts;
}

int Kit_GetVideoDecoderSDLData(Kit_Decoder *dec, SDL_Texture *texture) {
    assert(dec != NULL);
    assert(texture != NULL);

    Kit_VideoPacket *packet = Kit_PeekDecoderOutput(dec);
    if(packet == NULL) {
        return 0;
    }

    double sync_ts = _GetSystemTime() - dec->clock_sync;

    // Check if we want the packet
    if(packet->pts > sync_ts + KIT_VIDEO_SYNC_THRESHOLD) {
        // Video is ahead, don't show yet.
        return 0;
    } else if(packet->pts < sync_ts - KIT_VIDEO_SYNC_THRESHOLD) {
        // Video is lagging, skip until we find a good PTS to continue from.
        while(packet != NULL) {
            Kit_AdvanceDecoderOutput(dec);
            free_out_video_packet_cb(packet);
            packet = Kit_PeekDecoderOutput(dec);
            if(packet == NULL) {
                break;
            } else {
                dec->clock_pos = packet->pts;
            }
            if(packet->pts > sync_ts - KIT_VIDEO_SYNC_THRESHOLD) {
                break;
            }
        }
    }

    // If we have no viable packet, just skip
    if(packet == NULL) {
        return 0;
    }

    // Update output texture with current video data.
    // Take formats into account.
    switch(dec->output.format) {
/**/
        case SDL_PIXELFORMAT_YV12: // most of the tested videos use YUV420P / SDL YV12
        case SDL_PIXELFORMAT_IYUV:
            SDL_UpdateYUVTexture(
                texture, NULL, 
                packet->frame->data[0], packet->frame->linesize[0],
                packet->frame->data[1], packet->frame->linesize[1],
                packet->frame->data[2], packet->frame->linesize[2]);
            break;

        default:
            SDL_UpdateTexture(
                texture, NULL,
                packet->frame->data[0],
                packet->frame->linesize[0]);
            break;
    }

    // Advance buffer, and free the decoded frame.
    Kit_AdvanceDecoderOutput(dec);
    dec->clock_pos = packet->pts;
    free_out_video_packet_cb(packet);

    return 0;
}

int Kit_GetVideoDecoderOpenGLData(Kit_Decoder *dec, unsigned int gltexture) {
    assert(dec != NULL);

    Kit_VideoPacket *packet = Kit_PeekDecoderOutput(dec);
    if(packet == NULL) {
        return 0;
    }

    double sync_ts = _GetSystemTime() - dec->clock_sync;

    // Check if we want the packet
    if(packet->pts > sync_ts + KIT_VIDEO_SYNC_THRESHOLD) {
        // Video is ahead, don't show yet.
        return 0;
    } else if(packet->pts < sync_ts - KIT_VIDEO_SYNC_THRESHOLD) {
        // Video is lagging, skip until we find a good PTS to continue from.
        while(packet != NULL) {
            Kit_AdvanceDecoderOutput(dec);
            free_out_video_packet_cb(packet);
            packet = Kit_PeekDecoderOutput(dec);
            if(packet == NULL) {
                break;
            } else {
                dec->clock_pos = packet->pts;
            }
            if(packet->pts > sync_ts - KIT_VIDEO_SYNC_THRESHOLD) {
                break;
            }
        }
    }

    // If we have no viable packet, just skip
    if(packet == NULL) {
        fprintf(stdout, "Packet is null, return ... \n");
        return 0;
    }

#ifdef DEBUG
    if (une_fois) {
        fprintf(stdout, "Packet : \n");
        fprintf(stdout, "(frame start address) packet->frame->data[0] : %p\n", packet->frame->data[0]);
        fprintf(stdout, "(line size) packet->frame->linesize[0] : %d\n", packet->frame->linesize[0]);
        fprintf(stdout, "packet->frame->data[1] : %p\n", packet->frame->data[1]);
        fprintf(stdout, "packet->frame->linesize[1] : %d\n", packet->frame->linesize[1]);
        fprintf(stdout, "packet->frame->data[2] : %p\n", packet->frame->data[2]);
        fprintf(stdout, "packet->frame->linesize[2] : %d\n\n", packet->frame->linesize[2]);
        une_fois = 0;
    }
#endif

    int textureWidth  = dec->codec_ctx->width;
    int textureHeight = dec->codec_ctx->height;

    // OpenGL solution ...
/*
    glPixelStorei( GL_UNPACK_SWAP_BYTES, GL_FALSE );
    glPixelStorei( GL_UNPACK_LSB_FIRST,  GL_TRUE  );
    glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei( GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
*/

    if(first_time == 1)
    {
        glGenTextures(1, &gltexture);
        glBindTexture(GL_TEXTURE_2D, gltexture);
        glTexImage2D( GL_TEXTURE_2D,
                      0,
                      GL_BGR,
                      textureWidth,
                      textureHeight,
                      0,
                      GL_BGR,
                      GL_UNSIGNED_BYTE,
                      packet->frame->data[0]);

        err = glGetError();

        if ( GL_NO_ERROR != err ) {
            fprintf(stdout, "glTexImage2D setting problem is : %d\n", err);
        }


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if ( GL_NO_ERROR != err ) {
            fprintf(stdout, "glTexParameteri problem is : %d\n", err);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        glBindTexture(GL_TEXTURE_2D, 0);

        // unneeded
        // glGenerateMipmap(GL_TEXTURE_2D);

        err = glGetError();

        if ( GL_NO_ERROR != err ) {
            fprintf(stdout, "glBindTexture problem is : %d\n", err);
        }
//        first_time = 0;
    }

    else {
        // Update output texture with current video data.
        glTexSubImage2D(
                        GL_TEXTURE_2D, 0, 0, 0, textureWidth, textureHeight, GL_RGB, GL_UNSIGNED_BYTE, 
                        packet->frame->data[0]);

        err = glGetError();
        if ( (GL_NO_ERROR != err) ) {
                fprintf(stdout, "glTexSubImage2D problem is : %d\n ", err);
        }
    }

    // Advance buffer, and free the decoded frame.
    Kit_AdvanceDecoderOutput(dec);
    dec->clock_pos = packet->pts;
    free_out_video_packet_cb(packet);

    return 0;
}


int Kit_GetVideoDecoderOpenCVData(Kit_Decoder *dec, uint8_t * pData) {
    assert(dec != NULL);

    Kit_VideoPacket *packet = Kit_PeekDecoderOutput(dec);
    if(packet == NULL) {
        return 0;
    }

    double sync_ts = _GetSystemTime() - dec->clock_sync;

    // Check if we want the packet
    if(packet->pts > sync_ts + KIT_VIDEO_SYNC_THRESHOLD) {
        // Video is ahead, don't show yet.
        return 0;
    } else if(packet->pts < sync_ts - KIT_VIDEO_SYNC_THRESHOLD) {
        // Video is lagging, skip until we find a good PTS to continue from.
        while(packet != NULL) {
            Kit_AdvanceDecoderOutput(dec);
            free_out_video_packet_cb(packet);
            packet = Kit_PeekDecoderOutput(dec);
            if(packet == NULL) {
                break;
            } else {
                dec->clock_pos = packet->pts;
            }
            if(packet->pts > sync_ts - KIT_VIDEO_SYNC_THRESHOLD) {
                break;
            }
        }
    }

    // If we have no viable packet, just skip
    if(packet == NULL) {
        fprintf(stdout, "Packet is null, return ... \n");
        return 0;
    }

    //OPENCV SOLUTION : convert FFMPEG frame into cv::Mat
    //Link : https://stackoverflow.com/questions/29263090/ffmpeg-avframe-to-opencv-mat-conversion
    int textureWidth  = dec->codec_ctx->width;
    int textureHeight = dec->codec_ctx->height;

    AVFrame dst;

    av_image_fill_arrays( dst.data,
                          dst.linesize, packet->frame->data[0], AV_PIX_FMT_BGR24,
                          textureWidth, textureHeight, IMAGE_ALIGN);
    dst.data[0] = pData;

    convert_ctx= sws_getContext( textureWidth, textureHeight, dec->codec_ctx->pix_fmt,
                                 textureWidth, textureHeight, AV_PIX_FMT_BGR24,
                                 SWS_FAST_BILINEAR, NULL, NULL, NULL
                               );
    if(convert_ctx == NULL) {
        fprintf(stderr, "Cannot initialize the conversion context!\n");
        exit(1);
    }

    sws_scale(convert_ctx, (const unsigned char * const *)packet->frame->data, packet->frame->linesize, 0, textureHeight,
                        dst.data, dst.linesize);

    if(convert_ctx != NULL)
        sws_freeContext(convert_ctx);

    // Advance buffer, and free the decoded frame.
    Kit_AdvanceDecoderOutput(dec);
    dec->clock_pos = packet->pts;
    free_out_video_packet_cb(packet);

    dst.data[0] = 0;
    dst.linesize[0] = 0;

    return 0;
}

int Kit_GetVideoDecoderOpenCVData2(Kit_Decoder *dec, uint8_t * pYUVData) {
    assert(dec != NULL);

    Kit_VideoPacket *packet = Kit_PeekDecoderOutput(dec);
    if(packet == NULL) {
        return 0;
    }

    if (count < 10)
        count++;

    double sync_ts = _GetSystemTime() - dec->clock_sync;

    // Check if we want the packet
    if(packet->pts > sync_ts + KIT_VIDEO_SYNC_THRESHOLD) {
        // Video is ahead, don't show yet.
        return 0;
    } else if(packet->pts < sync_ts - KIT_VIDEO_SYNC_THRESHOLD) {
        // Video is lagging, skip until we find a good PTS to continue from.
        while(packet != NULL) {
            Kit_AdvanceDecoderOutput(dec);
            free_out_video_packet_cb(packet);
            packet = Kit_PeekDecoderOutput(dec);
            if(packet == NULL) {
                break;
            } else {
                dec->clock_pos = packet->pts;
            }
            if(packet->pts > sync_ts - KIT_VIDEO_SYNC_THRESHOLD) {
                break;
            }
        }
    }

    // If we have no viable packet, just skip
    if(packet == NULL) {
        fprintf(stdout, "Packet2 is null, return ... \n");
        return 0;
    }
#ifdef DEBUG
    if (une_fois) {
        fprintf(stdout, "Packet : \n");
        fprintf(stdout, "(frame start address) packet->frame->data[0] : %p\n", packet->frame->data[0]);
        fprintf(stdout, "(line size) packet->frame->linesize[0] : %d\n", packet->frame->linesize[0]);
        fprintf(stdout, "packet->frame->data[1] : %p\n", packet->frame->data[1]);
        fprintf(stdout, "packet->frame->linesize[1] : %d\n", packet->frame->linesize[1]);
        fprintf(stdout, "packet->frame->data[2] : %p\n", packet->frame->data[2]);
        fprintf(stdout, "packet->frame->linesize[2] : %d\n\n", packet->frame->linesize[2]);
    }
#endif
    // try to convert YUV FFMPEG frame into BGR frame (as cv:Mat)
    // https://wiki.videolan.org/YUV/#NV12
    // other link : https://stackoverflow.com/questions/20494132/ffmpeg-dumping-yuv-data-into-avframe-structure
    AVFrame * dst2 = av_frame_alloc();
    dst2->width = dec->codec_ctx->width;
    dst2->height = dec->codec_ctx->height;
    dst2->format = dec->codec_ctx->pix_fmt;
//    dst2->format = AV_PIX_FMT_YUV420P;

    long int src_size = av_image_fill_arrays( dst2->data,
                          dst2->linesize, packet->frame->data[0], dst2->format,
                          dst2->width , dst2->height, IMAGE_ALIGN);

    if (src_size == 0)
        fprintf(stdout, "pb with the src frame\n");

#ifdef DEBUG
    if (une_fois)
    {
        fprintf(stdout, "src_size = %ld\n", src_size);
        fprintf(stdout, "dst2->format = %d\n", dst2->format);
        fprintf(stdout, "AV_PIX_FMT_NV12 = %d\n", AV_PIX_FMT_NV12);
    }
        une_fois = 0;
#endif
    dst2->data[0] = pYUVData;
//    dst2->data[1] = dst2->data[0] + dst2->width * dst2->height;
//    dst2->data[2] = dst2->data[1] + dst2->width * dst2->height/4;

//!    dst2->data[0] = pYUVData;
//    dst2->linesize[0] = packet->frame->linesize[0];
//    dst2->linesize[1] = packet->frame->linesize[1];
//    dst2->linesize[2] = packet->frame->linesize[2];

//    dst2->data[1] = dst2->data[0] + packet->frame->linesize[0] * dst2->height/2;
//    dst2->data[2] = dst2->data[1] + packet->frame->linesize[1] * dst2->height/2;
    dst2->data[1] = packet->frame->data[0] + dst2->width * dst2->height;
    dst2->data[2] = packet->frame->data[1] + dst2->width * dst2->height/4;
/*
    dst2->data[1] = packet->frame->data[0] + packet->frame->linesize[0] * dst2->height/4;
    dst2->data[2] = packet->frame->data[1] + packet->frame->linesize[1] * dst2->height/4;


    // Copy data from the 3 input buffers

    memcpy(dst2->data[0], packet->frame->data[0], dst2->linesize[0] * dst2->height );
    memcpy(dst2->data[1], packet->frame->data[1], dst2->linesize[1] * dst2->height/2);
    memcpy(dst2->data[2], packet->frame->data[2], dst2->linesize[2] * dst2->height/2);
*/

//#define V1
#ifdef V1
    int size = packet->frame->linesize[0] * dst2->height;
    memcpy(dst2->data[0]             , packet->frame->data[0], size);
    memcpy(dst2->data[0] + size      , packet->frame->data[2], size/4);
    memcpy(dst2->data[0] + size * 5/4, packet->frame->data[1], size/4);
#else
//#define V2
#ifdef V2
    int pitch = packet->frame->linesize[0];
    register unsigned char *y1, *y2, *y3, *i1, *i2, *i3;
    int i;
    y1 = dst2->data[0];
    y3 = dst2->data[0] + pitch * dst2->height;
    y2 = dst2->data[0] + pitch * dst2->height * 5 / 4;
    i1 = packet->frame->data[0];
    i2 = packet->frame->data[1];
    i3 = packet->frame->data[2];

    for (i = 0; i<(dst2->height/2); i++)
    {
        memcpy(y1, i1, pitch);
        i1 += packet->frame->linesize[0];
        y1 += pitch;

        memcpy(y1, i1, pitch);
        memcpy(y2, i2, pitch/2);
        memcpy(y3, i3, pitch/2);

        y1 += pitch;
        y2 += pitch / 2;
        y3 += pitch / 2;

        i1 += packet->frame->linesize[0];
        i2 += packet->frame->linesize[1];
        i3 += packet->frame->linesize[2];
    }
#else
//    yuvToRgb24(packet->frame->data[0], dst2->data[0], dst2->width, dst2->height);

    uint8_t* src_planes[3] = {  packet->frame->data[0],
//!                                packet->frame->data[0] +  packet->frame->linesize[0]/2/*dst2->width*/ * dst2->height,
//!                                packet->frame->data[0] + (packet->frame->linesize[0]/2/*dst2->width*/ * dst2->height * 3/2)
                                packet->frame->data[0] +  dst2->width/2 * dst2->height,
                                packet->frame->data[0] + (dst2->width/2 * dst2->height * 3/2)
                              };

    int src_stride[3] = { dst2->width,
                          dst2->width * 3/2,
                          dst2->width * 3/2};

    uint8_t* dest_planes[3] = { dst2->data[0], NULL, NULL};

    int dest_stride[3] = { dst2->width * 3 , 0 , 0 };

    if(img_convert_ctx2 == NULL)
    {
        img_convert_ctx2 = sws_getContext(
                                           dst2->width, dst2->height, dst2->format,
                                           dst2->width, dst2->height, AV_PIX_FMT_BGR24,
                                           SWS_POINT,
                                           NULL,
                                           NULL,
                                           NULL
                                         );
    }
    else
    {
        img_convert_ctx2 = sws_getCachedContext( img_convert_ctx2,
                                                 dst2->width, dst2->height, dst2->format,
                                                 dst2->width, dst2->height, AV_PIX_FMT_BGR24,
                                                 SWS_POINT,
                                                 NULL,
                                                 NULL,
                                                 NULL);
        sws_scale(  img_convert_ctx2,
                    (const unsigned char * const *)src_planes, src_stride,
                    0,
                    dst2->height, dest_planes, dest_stride
                 );
    }
#endif
#endif

//    av_frame_free(&dst2);

    // Advance buffer, and free the decoded frame.
    Kit_AdvanceDecoderOutput(dec);
    dec->clock_pos = packet->pts;
    free_out_video_packet_cb(packet);

    dst2->data[0] = 0;
    dst2->linesize[0] = 0;

    return 0;
}


int Kit_GetVideoDecoderYUV2OpenGLData(Kit_Decoder *dec, uint8_t * pData) {
    assert(dec != NULL);

    Kit_VideoPacket *packet = Kit_PeekDecoderOutput(dec);
    if(packet == NULL) {
        return 0;
    }

    double sync_ts = _GetSystemTime() - dec->clock_sync;

    // Check if we want the packet
    if(packet->pts > sync_ts + KIT_VIDEO_SYNC_THRESHOLD) {
        // Video is ahead, don't show yet.
        return 0;
    } else if(packet->pts < sync_ts - KIT_VIDEO_SYNC_THRESHOLD) {
        // Video is lagging, skip until we find a good PTS to continue from.
        while(packet != NULL) {
            Kit_AdvanceDecoderOutput(dec);
            free_out_video_packet_cb(packet);
            packet = Kit_PeekDecoderOutput(dec);
            if(packet == NULL) {
                break;
            } else {
                dec->clock_pos = packet->pts;
            }
            if(packet->pts > sync_ts - KIT_VIDEO_SYNC_THRESHOLD) {
                break;
            }
        }
    }

    // If we have no viable packet, just skip
    if(packet == NULL) {
        fprintf(stdout, "Packet is null, return ... \n");
        return 0;
    }

    //OPENCV SOLUTION : convert FFMPEG frame into cv::Mat
    //Link : https://stackoverflow.com/questions/29263090/ffmpeg-avframe-to-opencv-mat-conversion
    int textureWidth  = dec->codec_ctx->width;
    int textureHeight = dec->codec_ctx->height;

    // cv::Mat cvMat = cv::Mat(dst2->width, textureHeight, CV_8UC3);
    AVFrame dst;

    av_image_fill_arrays( dst.data,
                          dst.linesize, packet->frame->data[0], AV_PIX_FMT_BGR24,
                          textureWidth, textureHeight, IMAGE_ALIGN);
/*
    dst.data[0] = pData;
    dst.

    packet->frame->data[0], packet->frame->linesize[0],
                packet->frame->data[1], packet->frame->linesize[1],
                packet->frame->data[2], packet->frame->linesize[2]);



    frame->data[0] = plane[0]
*/

    return 0;
}


