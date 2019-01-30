#include "BevStream.hpp"

#include <mutex>
#include <condition_variable>
#include <vector>

namespace BevStream {


typedef struct tojs_t {
  void* mem; // pointer to memory that was allocated with malloc and will be free'd by nodejs
  size_t length; // in bytes
} tojs_t;

class ToJs : public BevStream
{
public:

  ToJs(bool defer_callback, bool print, std::mutex *m, std::condition_variable *cv, bool *ready, std::vector<tojs_t> *pointers);

  void stopThreadDerivedClass();
  void setBufferOptions(bufferevent* in, bufferevent* out);
  void gotData(struct bufferevent *bev, struct evbuffer *buf, size_t len);

  std::mutex *mutfruit = 0;
  std::condition_variable *streamOutputReadyCondition = 0;
  bool *outputReady = 0;
  std::vector<tojs_t> *outputPointers;

};












}
