#include "BevStream.hpp"

namespace BevStream {



class GainStream : public BevStream
{
public:

  GainStream(bool defer_callback, bool print);

  void stopThreadDerivedClass();
  void setBufferOptions(bufferevent* in, bufferevent* out);
  void gotData(struct bufferevent *bev, struct evbuffer *buf, size_t len);


};












}
