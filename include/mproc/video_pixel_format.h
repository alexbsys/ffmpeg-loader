#ifndef VIDE_OPIXEL_FORMAT_HEADER
#define VIDE_OPIXEL_FORMAT_HEADER

namespace cmf {

/**
 * @brief The VideoPixelFormat enum.
 *        Implements video pixel format codes
 */
enum VideoPixelFormat {
  VideoPixelFormat_NONE = 0,  ///< Invalid value for pixel format
  VideoPixelFormat_YUV420P,  ///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per   1
                             ///< 2x2 Y samples)
  VideoPixelFormat_YUYV422,  ///< packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr             2   YUY2
  VideoPixelFormat_RGB24,    ///< packed RGB 8:8:8, 24bpp, RGBRGB...               3
  VideoPixelFormat_BGR24,    ///< packed RGB 8:8:8, 24bpp, BGRBGR...               4
  VideoPixelFormat_YUV422P,  ///< planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per   5
                             ///< 2x1 Y samples)
  VideoPixelFormat_YUV444P,  ///< planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per   6
                             ///< 1x1 Y samples)
  VideoPixelFormat_YUV410P,  ///< planar YUV 4:1:0,  9bpp, (1 Cr & Cb sample per   7
                             ///< 4x4 Y samples)
  VideoPixelFormat_YUV411P,  ///< planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per   8
                             ///< 4x1 Y samples)
  VideoPixelFormat_GRAY8,    ///<        Y        ,  8bpp                          9
  VideoPixelFormat_MONOWHITE,  ///<        Y        ,  1bpp, 0 is white, 1 is      10
                               ///<        black, in each byte pixels are
                               ///<        ordered from the msb to the lsb
  VideoPixelFormat_MONOBLACK,  ///<        Y        ,  1bpp, 0 is black, 1 is      11
                               ///<        white, in each byte pixels are
                               ///<        ordered from the msb to the lsb
  VideoPixelFormat_PAL8,  ///< 8 bit with VideoVideoPixelFormat_RGB32 palette      12
  VideoPixelFormat_YUVJ420P,  ///< planar YUV 4:2:0, 12bpp, full scale (JPEG),     13
                              ///< deprecated in favor of
                              ///< VideoVideoPixelFormat_YUV420P and setting
                              ///< color_range
  VideoPixelFormat_YUVJ422P,  ///< planar YUV 4:2:2, 16bpp, full scale (JPEG),     14
                              ///< deprecated in favor of
                              ///< VideoVideoPixelFormat_YUV422P and setting
                              ///< color_range
  VideoPixelFormat_YUVJ444P,  ///< planar YUV 4:4:4, 24bpp, full scale (JPEG),     15
                              ///< deprecated in favor of
                              ///< VideoVideoPixelFormat_YUV444P and setting
                              ///< color_range
  VideoPixelFormat_XVMC_MPEG2_MC,  ///< XVideo Motion Acceleration via common      16
                                   ///< packet passing
  VideoPixelFormat_XVMC_MPEG2_IDCT,      //                                        17
  VideoPixelFormat_UYVY422,    ///< packed YUV 4:2:2, 16bpp, Cb Y0 Cr Y1           18
  VideoPixelFormat_UYYVYY411,  ///< packed YUV 4:1:1, 12bpp, Cb Y0 Y1 Cr Y2 Y3     19
  VideoPixelFormat_BGR8,       ///< packed RGB 3:3:2,  8bpp, (msb)2B 3G 3R(lsb)    20
  VideoPixelFormat_BGR4,  ///< packed RGB 1:2:1 bitstream,  4bpp, (msb)1B 2G       21
                          ///< 1R(lsb), a byte contains two pixels, the first
                          ///< pixel in the byte is the one composed by the 4
                          ///< msb bits
  VideoPixelFormat_BGR4_BYTE,  ///< packed RGB 1:2:1,  8bpp, (msb)1B 2G 1R(lsb)    22
  VideoPixelFormat_RGB8,       ///< packed RGB 3:3:2,  8bpp, (msb)2R 3G 3B(lsb)    23
  VideoPixelFormat_RGB4,  ///< packed RGB 1:2:1 bitstream,  4bpp, (msb)1R 2G       24
                          ///< 1B(lsb), a byte contains two pixels, the first
                          ///< pixel in the byte is the one composed by the 4
                          ///< msb bits
  VideoPixelFormat_RGB4_BYTE,  ///< packed RGB 1:2:1,  8bpp, (msb)1R 2G 1B(lsb)    25
  VideoPixelFormat_NV12,       ///< planar YUV 4:2:0, 12bpp, 1 plane for Y and 1   26
                               ///< plane for the UV components, which are
  ///< interleaved (first byte U and the following byte
  ///< V)
  VideoPixelFormat_NV21,  ///< as above, but U and V bytes are swapped             27

  VideoPixelFormat_ARGB,  ///< packed ARGB 8:8:8:8, 32bpp, ARGBARGB...             28
  VideoPixelFormat_RGBA,  ///< packed RGBA 8:8:8:8, 32bpp, RGBARGBA...             29
  VideoPixelFormat_ABGR,  ///< packed ABGR 8:8:8:8, 32bpp, ABGRABGR...
  VideoPixelFormat_BGRA,  ///< packed BGRA 8:8:8:8, 32bpp, BGRABGRA...

  VideoPixelFormat_GRAY16BE,  ///<        Y        , 16bpp, big-endian
  VideoPixelFormat_GRAY16LE,  ///<        Y        , 16bpp, little-endian
  VideoPixelFormat_YUV440P,   ///< planar YUV 4:4:0 (1 Cr & Cb sample per 1x2 Y
                              ///< samples)
  VideoPixelFormat_YUVJ440P,  ///< planar YUV 4:4:0 full scale (JPEG),
                              ///< deprecated in favor of
                              ///< VideoVideoPixelFormat_YUV440P and setting
                              ///< color_range
  VideoPixelFormat_YUVA420P,  ///< planar YUV 4:2:0, 20bpp, (1 Cr & Cb sample
                              ///< per 2x2 Y & A samples)
  VideoPixelFormat_VDPAU_H264,   ///< H.264 HW decoding with VDPAU, data[0]
                                 ///< contains a vdpau_render_state struct which
                                 ///< contains the bitstream of the slices as
                                 ///< well as various fields extracted from
                                 ///< headers
  VideoPixelFormat_VDPAU_MPEG1,  ///< MPEG-1 HW decoding with VDPAU, data[0]
                                 ///< contains a vdpau_render_state struct which
                                 ///< contains the bitstream of the slices as
                                 ///< well as various fields extracted from
                                 ///< headers
  VideoPixelFormat_VDPAU_MPEG2,  ///< MPEG-2 HW decoding with VDPAU, data[0]
                                 ///< contains a vdpau_render_state struct which
                                 ///< contains the bitstream of the slices as
                                 ///< well as various fields extracted from
                                 ///< headers
  VideoPixelFormat_VDPAU_WMV3,   ///< WMV3 HW decoding with VDPAU, data[0]
                                 ///< contains a vdpau_render_state struct which
                                 ///< contains the bitstream of the slices as
                                 ///< well as various fields extracted from
                                 ///< headers
  VideoPixelFormat_VDPAU_VC1,    ///< VC-1 HW decoding with VDPAU, data[0]
                                 ///< contains a vdpau_render_state struct which
  ///< contains the bitstream of the slices as well
  ///< as various fields extracted from headers
  VideoPixelFormat_RGB48BE,  ///< packed RGB 16:16:16, 48bpp, 16R, 16G, 16B, the
                             ///< 2-byte value for each R/G/B component is
                             ///< stored as big-endian
  VideoPixelFormat_RGB48LE,  ///< packed RGB 16:16:16, 48bpp, 16R, 16G, 16B, the
                             ///< 2-byte value for each R/G/B component is
                             ///< stored as little-endian

  VideoPixelFormat_RGB565BE,  ///< packed RGB 5:6:5, 16bpp, (msb)   5R 6G
                              ///< 5B(lsb), big-endian
  VideoPixelFormat_RGB565LE,  ///< packed RGB 5:6:5, 16bpp, (msb)   5R 6G
                              ///< 5B(lsb), little-endian
  VideoPixelFormat_RGB555BE,  ///< packed RGB 5:5:5, 16bpp, (msb)1A 5R 5G
                              ///< 5B(lsb), big-endian, most significant bit to
                              ///< 0
  VideoPixelFormat_RGB555LE,  ///< packed RGB 5:5:5, 16bpp, (msb)1A 5R 5G
                              ///< 5B(lsb), little-endian, most significant bit
                              ///< to 0

  VideoPixelFormat_BGR565BE,  ///< packed BGR 5:6:5, 16bpp, (msb)   5B 6G
                              ///< 5R(lsb), big-endian
  VideoPixelFormat_BGR565LE,  ///< packed BGR 5:6:5, 16bpp, (msb)   5B 6G
                              ///< 5R(lsb), little-endian
  VideoPixelFormat_BGR555BE,  ///< packed BGR 5:5:5, 16bpp, (msb)1A 5B 5G
                              ///< 5R(lsb), big-endian, most significant bit to
                              ///< 1
  VideoPixelFormat_BGR555LE,  ///< packed BGR 5:5:5, 16bpp, (msb)1A 5B 5G
                              ///< 5R(lsb), little-endian, most significant bit
                              ///< to 1

  VideoPixelFormat_VAAPI_MOCO,  ///< HW acceleration through VA API at motion
                                ///< compensation entry-point, Picture.data[3]
                                ///< contains a vaapi_render_state struct which
                                ///< contains macroblocks as well as various
                                ///< fields extracted from headers
  VideoPixelFormat_VAAPI_IDCT,  ///< HW acceleration through VA API at IDCT
                                ///< entry-point, Picture.data[3] contains a
                                ///< vaapi_render_state struct which contains
                                ///< fields extracted from headers
  VideoPixelFormat_VAAPI_VLD,   ///< HW decoding through VA API, Picture.data[3]
                                ///< contains a vaapi_render_state struct which
  ///< contains the bitstream of the slices as well
  ///< as various fields extracted from headers

  VideoPixelFormat_YUV420P16LE,  ///< planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample
                                 ///< per 2x2 Y samples), little-endian
  VideoPixelFormat_YUV420P16BE,  ///< planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample
                                 ///< per 2x2 Y samples), big-endian
  VideoPixelFormat_YUV422P16LE,  ///< planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample
                                 ///< per 2x1 Y samples), little-endian
  VideoPixelFormat_YUV422P16BE,  ///< planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample
                                 ///< per 2x1 Y samples), big-endian
  VideoPixelFormat_YUV444P16LE,  ///< planar YUV 4:4:4, 48bpp, (1 Cr & Cb sample
                                 ///< per 1x1 Y samples), little-endian
  VideoPixelFormat_YUV444P16BE,  ///< planar YUV 4:4:4, 48bpp, (1 Cr & Cb sample
                                 ///< per 1x1 Y samples), big-endian
  VideoPixelFormat_VDPAU_MPEG4,  ///< MPEG4 HW decoding with VDPAU, data[0]
                                 ///< contains a vdpau_render_state struct which
                                 ///< contains the bitstream of the slices as
                                 ///< well as various fields extracted from
                                 ///< headers
  VideoPixelFormat_DXVA2_VLD,    ///< HW decoding through DXVA2, Picture.data[3]
                                 ///< contains a LPDIRECT3DSURFACE9 pointer

  VideoPixelFormat_RGB444LE,  ///< packed RGB 4:4:4, 16bpp, (msb)4A 4R 4G
                              ///< 4B(lsb), little-endian, most significant bits
                              ///< to 0
  VideoPixelFormat_RGB444BE,  ///< packed RGB 4:4:4, 16bpp, (msb)4A 4R 4G
                              ///< 4B(lsb), big-endian, most significant bits to
                              ///< 0
  VideoPixelFormat_BGR444LE,  ///< packed BGR 4:4:4, 16bpp, (msb)4A 4B 4G
                              ///< 4R(lsb), little-endian, most significant bits
                              ///< to 1
  VideoPixelFormat_BGR444BE,  ///< packed BGR 4:4:4, 16bpp, (msb)4A 4B 4G
                              ///< 4R(lsb), big-endian, most significant bits to
                              ///< 1
  VideoPixelFormat_GRAY8A,    ///< 8bit gray, 8bit alpha
  VideoPixelFormat_BGR48BE,  ///< packed RGB 16:16:16, 48bpp, 16B, 16G, 16R, the
                             ///< 2-byte value for each R/G/B component is
                             ///< stored as big-endian
  VideoPixelFormat_BGR48LE,  ///< packed RGB 16:16:16, 48bpp, 16B, 16G, 16R, the
                             ///< 2-byte value for each R/G/B component is
                             ///< stored as little-endian

  // the following 10 formats have the disadvantage of needing 1 format for each
  // bit depth, thus If you want to support multiple bit depths, then using
  // VideoVideoPixelFormat_YUV420P16* with the bpp stored seperately is better
  VideoPixelFormat_YUV420P9BE,   ///< planar YUV 4:2:0, 13.5bpp, (1 Cr & Cb
                                 ///< sample per 2x2 Y samples), big-endian
  VideoPixelFormat_YUV420P9LE,   ///< planar YUV 4:2:0, 13.5bpp, (1 Cr & Cb
                                 ///< sample per 2x2 Y samples), little-endian
  VideoPixelFormat_YUV420P10BE,  ///< planar YUV 4:2:0, 15bpp, (1 Cr & Cb sample
                                 ///< per 2x2 Y samples), big-endian
  VideoPixelFormat_YUV420P10LE,  ///< planar YUV 4:2:0, 15bpp, (1 Cr & Cb sample
                                 ///< per 2x2 Y samples), little-endian
  VideoPixelFormat_YUV422P10BE,  ///< planar YUV 4:2:2, 20bpp, (1 Cr & Cb sample
                                 ///< per 2x1 Y samples), little-endian
  VideoPixelFormat_YUV422P10LE,  ///< planar YUV 4:2:2, 20bpp, (1 Cr & Cb sample
                                 ///< per 2x1 Y samples), big-endian
  VideoPixelFormat_YUV444P9BE,   ///< planar YUV 4:4:4, 27bpp, (1 Cr & Cb sample
                                 ///< per 1x1 Y samples), little-endian
  VideoPixelFormat_YUV444P9LE,   ///< planar YUV 4:4:4, 27bpp, (1 Cr & Cb sample
                                 ///< per 1x1 Y samples), big-endian
  VideoPixelFormat_YUV444P10BE,  ///< planar YUV 4:4:4, 30bpp, (1 Cr & Cb sample
                                 ///< per 1x1 Y samples), little-endian
  VideoPixelFormat_YUV444P10LE,  ///< planar YUV 4:4:4, 30bpp, (1 Cr & Cb sample
                                 ///< per 1x1 Y samples), big-endian

  VideoPixelFormat_MJPEG,
  //VideoPixelFormat_YUY2,
  VideoPixelFormat_YVYU422,
  VideoPixelFormat_YUVJ411P
};

static VideoPixelFormat VideoPixelFormat_ToBase(const VideoPixelFormat& fmt) {
  switch(fmt) {
    case VideoPixelFormat_YUVJ420P:
      return VideoPixelFormat_YUV420P;
    case VideoPixelFormat_YUVJ411P:
      return VideoPixelFormat_YUV411P;
    case VideoPixelFormat_YUVJ422P:
      return VideoPixelFormat_YUV422P;
    case VideoPixelFormat_YUVJ440P:
      return VideoPixelFormat_YUV440P;
    case VideoPixelFormat_YUVJ444P:
      return VideoPixelFormat_YUV444P;
    default:
      break;
  }

  return fmt;
}

static VideoPixelFormat VideoPixelFormat_ToJpeg(const VideoPixelFormat& fmt) {
  switch(fmt) {
    case VideoPixelFormat_YUV420P:
      return VideoPixelFormat_YUVJ420P;
    case VideoPixelFormat_YUV411P:
      return VideoPixelFormat_YUVJ411P;
    case VideoPixelFormat_YUV422P:
      return VideoPixelFormat_YUVJ422P;
    case VideoPixelFormat_YUV440P:
      return VideoPixelFormat_YUVJ440P;
    case VideoPixelFormat_YUV444P:
      return VideoPixelFormat_YUVJ444P;
    default:
      break;
  }

  return fmt;
}

static bool VIdeoPixelFormat_IsJpeg(const VideoPixelFormat& fmt) {
  switch(fmt) {
    case VideoPixelFormat_YUVJ420P:
    case VideoPixelFormat_YUVJ411P:
    case VideoPixelFormat_YUVJ422P:
    case VideoPixelFormat_YUVJ440P:
    case VideoPixelFormat_YUVJ444P:
      return true;
    default:
      return false;
  }
}

static bool VideoPixelFormat_IsSame(VideoPixelFormat fmt1, VideoPixelFormat fmt2) {
  if (fmt1 == fmt2)
    return true;

  if (fmt1 == VideoPixelFormat_YUV420P || fmt1 == VideoPixelFormat_YUVJ420P) {
    if (fmt2 == VideoPixelFormat_YUV420P || fmt2 == VideoPixelFormat_YUVJ420P) {
      return true;
    }
  }

  if (fmt1 == VideoPixelFormat_YUV422P || fmt1 == VideoPixelFormat_YUVJ422P) {
    if (fmt2 == VideoPixelFormat_YUV422P || fmt2 == VideoPixelFormat_YUVJ422P) {
      return true;
    }
  }

  if (fmt1 == VideoPixelFormat_YUV411P || fmt1 == VideoPixelFormat_YUVJ411P) {
    if (fmt2 == VideoPixelFormat_YUV411P || fmt2 == VideoPixelFormat_YUVJ411P) {
      return true;
    }
  }

  if (fmt1 == VideoPixelFormat_YUV440P || fmt1 == VideoPixelFormat_YUVJ440P) {
    if (fmt2 == VideoPixelFormat_YUV440P || fmt2 == VideoPixelFormat_YUVJ440P) {
      return true;
    }
  }

  if (fmt1 == VideoPixelFormat_YUV444P || fmt1 == VideoPixelFormat_YUVJ444P) {
    if (fmt2 == VideoPixelFormat_YUV444P || fmt2 == VideoPixelFormat_YUVJ444P) {
      return true;
    }
  }

  return false;
}

}//namespace cmf

#endif  // VIDE_OPIXEL_FORMAT_HEADER
