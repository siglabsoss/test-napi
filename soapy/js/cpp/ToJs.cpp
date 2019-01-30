#include "ToJs.hpp"
#include <iostream>
#include <cassert>


namespace BevStream {

using namespace std;

// We have a nice cascasing stream setup within libevent, but now we need to get the data back to js
// The idea is that we will fire a pending callback (From the async rain example) and then block
// on a condition variable as described in https://en.cppreference.com/w/cpp/thread/condition_variable

ToJs::ToJs(bool defer_callback, bool print, std::mutex *m, std::condition_variable *cv, bool *ready, std::vector<tojs_t> *pointers) :
   BevStream(defer_callback,print)
   ,mutfruit(m)
   ,streamOutputReadyCondition(cv)
   ,outputReady(ready)
   ,outputPointers(pointers) {
  this->name = "ToJs";
}


void ToJs::stopThreadDerivedClass() {
  cout << "stopThreadDerivedClass() how to stop?" << endl;

}

// runs from same place (thread) that constructed us
void ToJs::setBufferOptions(bufferevent* in, bufferevent* out) {
  bufferevent_setwatermark(out, EV_READ, (1024)*4, 0);
  bufferevent_set_max_single_read(out, (1024)*4 );
}

// runs on _thread
void ToJs::gotData(struct bufferevent *bev, struct evbuffer *buf, size_t lengthIn) {
  // cout << "Gain got data with " << lengthIn << endl;

  if(next) {
    // cout <
  } else {
    evbuffer_drain(buf, lengthIn);
    return;
  }

  size_t this_read = (lengthIn / 4) * 4;

  unsigned char* temp_read = evbuffer_pullup(buf, this_read);
  uint32_t* temp_read_uint = (uint32_t*)temp_read;

  // cout << temp_read[0] << "," << temp_read[1] << "," << temp_read[2] << endl;
  if(_print) {
    cout << temp_read_uint[0] << "," << temp_read_uint[1] << "," << temp_read_uint[2] << endl;
  }
  // char* badPractice = (char*)malloc(1024*4);
  // bufferevent_write(next->pair->in, badPractice, 1024*4);


  // evbuffer_drain(buf, n_to_add);

  // evbuffer_invoke_callbacks_(next->input);

    // if(name == "gain1") {
    // static int times = 0;
    // if( times < 2 ) {
    // times++;
    // bufferevent_write(next->pair->in, badPractice, 4*1024);
    // }
    // }

}




} // namespace