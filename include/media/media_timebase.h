
#ifndef MEDIA_TIMEBASE_HEADER
#define MEDIA_TIMEBASE_HEADER

namespace cmf {

struct MediaTimeBase {
  int num_;
  int den_;

  MediaTimeBase() : num_(0), den_(1) {}
  MediaTimeBase(int num, int den) : num_(num), den_(den) {}
};

}//namespace cmf

#endif //MEDIA_TIMEBASE_HEADER
