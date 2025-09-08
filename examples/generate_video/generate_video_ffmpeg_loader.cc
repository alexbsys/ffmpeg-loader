
#include <avc/ffmpeg-loader.h>
#include <avc/libav_detached_common.h>  // some useful constants from ffmpeg
#include <iostream>

int main() {
  auto avc_loader = avc::CreateAvcModuleProvider3();
  if (!avc_loader->IsAvCodecLoaded() || !avc_loader->IsAvFormatLoaded()) {
    std::cerr << "AVC dynamic libraries were not found. Please place AVC libraries in the same directory of this executable and run again" << std::endl;
    return 254;
  }

  avc_loader->avformat_network_init();

  const char* filename = "output.mp4";
  const int width = 640;
  const int height = 480;
  const int fps = 25;
  const int num_frames = 100;

  avc::AVFormatContext* fmt_ctx = nullptr;
  avc_loader->avformat_alloc_output_context2(&fmt_ctx, nullptr, nullptr, filename);

  avc::AVStream* stream = avc_loader->avformat_new_stream(fmt_ctx, nullptr);
  const avc::AVCodec* codec = avc_loader->avcodec_find_encoder_by_name("libx264");

  avc::AVCodecContext* codec_ctx = avc_loader->avcodec_alloc_context3(codec);
  avc_loader->d()->AVCodecContextSetWidth(codec_ctx, width);       // codec_ctx->width = width;
  avc_loader->d()->AVCodecContextSetHeight(codec_ctx, height);     // codec_ctx->height = height;
  avc_loader->d()->AVCodecContextSetTimeBase(codec_ctx, cmf::MediaTimeBase(1, fps));   // codec_ctx->time_base = { 1, fps };
  avc_loader->d()->AVCodecContextSetFrameRate(codec_ctx, cmf::MediaTimeBase(fps, 1));  // codec_ctx->framerate = { fps, 1 };

  // Replacement for codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P. Because we are use own pixel format constants, conversion is required
  avc_loader->d()->AVCodecContextSetPixFmt(codec_ctx,
    avc_loader->GetVideoPixelFormatConverter()->VideoPixelFormatToAVPixelFormat(cmf::VideoPixelFormat_YUV420P));

  avc_loader->d()->AVCodecContextSetGopSize(codec_ctx, 10); // codec_ctx->gop_size = 10;

  avc_loader->avcodec_open2(codec_ctx, codec, nullptr);
  avc_loader->avcodec_parameters_from_context(
    avc_loader->d()->AVStreamGetCodecPar(stream), codec_ctx);

  // replacement for avio_open(&fmt_ctx->pb, filename, AVIO_FLAG_WRITE). 
  // Because call changes pointer inside structure, we need to get it, then set it back
  {
    avc::AVIOContext* ioctx = avc_loader->d()->AVFormatContextGetPb(fmt_ctx);
    avc_loader->avio_open2(&ioctx, filename, AVIO_FLAG_WRITE, nullptr, nullptr);
    avc_loader->d()->AVFormatContextSetPb(fmt_ctx, ioctx);  // set modified AVIOContext to pb
  }

  avc_loader->avformat_write_header(fmt_ctx, nullptr);

  avc::AVFrame* frame = avc_loader->av_frame_alloc();
  avc_loader->d()->AVFrameSetFormat(frame, avc_loader->d()->AVCodecContextGetPixFmt(codec_ctx)); // frame->format = codec_ctx->pix_fmt;
  avc_loader->d()->AVFrameSetWidth(frame, avc_loader->d()->AVCodecContextGetWidth(codec_ctx));   // frame->width = codec_ctx->width;
  avc_loader->d()->AVFrameSetHeight(frame, avc_loader->d()->AVCodecContextGetHeight(codec_ctx)); // frame->height = codec_ctx->height;

  avc_loader->av_frame_get_buffer(frame, 0);

  avc::AVPacket* pkt = avc_loader->av_packet_alloc();

  for (int i = 0; i < num_frames; i++) {

    // Fill YUV frame: simple gradient
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        uint8_t* data0_ptr = avc_loader->d()->AVFrameGetData(frame, 0);  // frame->data[0]
        int line_size_0 = avc_loader->d()->AVFrameGetLineSize(frame, 0); // frame->linesize[0]
        data0_ptr[y * line_size_0 + x] = x + y + i * 3;
      }
    }

    for (int y = 0; y < height / 2; y++) {
      for (int x = 0; x < width / 2; x++) {
        uint8_t* data1_ptr = avc_loader->d()->AVFrameGetData(frame, 1);  // frame->data[1]
        int line_size_1 = avc_loader->d()->AVFrameGetLineSize(frame, 1); // frame->linesize[1]

        uint8_t* data2_ptr = avc_loader->d()->AVFrameGetData(frame, 2);  // frame->data[2]
        int line_size_2 = avc_loader->d()->AVFrameGetLineSize(frame, 2); // frame->linesize[2]

        data1_ptr[y * line_size_1 + x] = 128 + y + i * 2;
        data2_ptr[y * line_size_2 + x] = 64 + x + i * 5;
      }
    }

    avc_loader->d()->AVFrameSetPts(frame, i); //  frame->pts = i;

    avc_loader->avcodec_send_frame(codec_ctx, frame);
    while (avc_loader->avcodec_receive_packet(codec_ctx, pkt) == 0) {
      avc_loader->av_interleaved_write_frame(fmt_ctx, pkt);
      avc_loader->av_packet_unref(pkt);
    }
  }

  // Finish encoding
  avc_loader->avcodec_send_frame(codec_ctx, nullptr);
  while (avc_loader->avcodec_receive_packet(codec_ctx, pkt) == 0) {
    avc_loader->av_interleaved_write_frame(fmt_ctx, pkt);
    avc_loader->av_packet_unref(pkt);
  }

  avc_loader->av_write_trailer(fmt_ctx);

  avc_loader->av_frame_free(&frame);
  avc_loader->av_packet_free(&pkt);
  avc_loader->avcodec_free_context(&codec_ctx);

  {
    // replace for avio_closep(&fmt_ctx->pb)
    avc::AVIOContext* ioctx = avc_loader->d()->AVFormatContextGetPb(fmt_ctx);
    avc_loader->avio_closep(&ioctx);
    avc_loader->d()->AVFormatContextSetPb(fmt_ctx, ioctx);  // set modified AVIOContext to pb
  }

  avc_loader->avformat_free_context(fmt_ctx);
  return 0;
}
