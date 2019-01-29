#include "GainStream.hpp"
#include <iostream>
#include <cassert>


namespace BevStream {

using namespace std;

GainStream::GainStream(bool defer_callback, bool print) : BevStream(defer_callback,print) {

}


void GainStream::stopThreadDerivedClass() {
  cout << "stopThreadDerivedClass() how to stop?" << endl;

}

// runs from same place (thread) that constructed us
void GainStream::setBufferOptions(bufferevent* in, bufferevent* out) {
  bufferevent_setwatermark(out, EV_READ, (1024)*4, 0);
  bufferevent_set_max_single_read(out, (1024)*4 );
}

// runs on _thread
void GainStream::gotData(struct bufferevent *bev, struct evbuffer *buf, size_t len) {
  cout << "Gain got data with " << len << endl;
}





} // namespace