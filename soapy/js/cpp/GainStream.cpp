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
void GainStream::gotData(struct bufferevent *bev, struct evbuffer *buf, size_t lengthIn) {
  cout << "Gain got data with " << lengthIn << endl;

  if(next) {
    cout << "next: " << next->name << endl;
  } else {
    evbuffer_drain(buf, lengthIn);
    return;
  }

  unsigned char* temp_read = evbuffer_pullup(buf, 1024*4);

  char* badPractice = (char*)malloc(1024*4);
  // bufferevent_write(next->pair->in, badPractice, 1024*4);

  // see
  // http://www.wangafu.net/~nickm/libevent-book/Ref7_evbuffer.html

  struct evbuffer_iovec v[2];
  int n, i;
  size_t n_to_add = 1024*4*2;

// #define DDD next->input
#define DDD bufferevent_get_output(next->pair->in) 

  // Reserve bytes
  n = evbuffer_reserve_space(DDD, n_to_add, v, 2);
  if (n<=0) {
    cout << "fatal in GainStream::evbuffer_reserve_space "<< n << endl;
    return; /* Unable to reserve the space for some reason. */
  }

  cout << "iovec had " << n << " extents" << endl;

  for (i=0; i<n && n_to_add > 0; ++i) {
    size_t len = v[i].iov_len;
    cout << "iovec v had len " << len << endl;
    if (len > n_to_add) /* Don't write more than n_to_add bytes. */
       len = n_to_add;

       ((char*)v[i].iov_base)[0] =0xff;
       ((char*)v[i].iov_base)[1] =0x0f;
       ((char*)v[i].iov_base)[2] =0x03;
       ((char*)v[i].iov_base)[4] =0x04;

    if (
      false
      // generate_data(v[i].iov_base, len) < 0
      ) {
       /* If there was a problem during data generation, we can just stop
          here; no data will be committed to the buffer. */
       return;
    }
    /* Set iov_len to the number of bytes we actually wrote, so we
       don't commit too much. */
     v[i].iov_len = len;
     // v[i].iov_len = 1024*4;



  }

  /* We commit the space here.  Note that we give it 'i' (the number of
     vectors we actually used) rather than 'n' (the number of vectors we
     had available. */
  if (evbuffer_commit_space(DDD, v, i) < 0) {
    cout << "fatal in GainStream::evbuffer_commit_space" << endl;
    return; /* Error committing */
  }

  evbuffer_drain(buf, n_to_add);

  // evbuffer_invoke_callbacks_(next->input);

    // if(name == "gain1") {
    // static int times = 0;
    // if( times < 2 ) {
    // times++;
    // bufferevent_write(next->pair->in, badPractice, 4*1024);
    // }
    // }

}

// // runs on _thread
// void GainStream::gotData(struct bufferevent *bev, struct evbuffer *buf, size_t lengthIn) {
//   cout << "Gain got data with " << lengthIn << endl;

//   if(next) {
//     cout << "next: " << next->name << endl;
//   }

//   unsigned char* temp_read = evbuffer_pullup(buf, 1024*4);

//   // see
//   // http://www.wangafu.net/~nickm/libevent-book/Ref7_evbuffer.html

//   struct evbuffer_iovec v[2];
//   int n, i;
//   size_t n_to_add = 1024*4;

//   // Reserve bytes
//   n = evbuffer_reserve_space(bufferevent_get_input(next->pair->in), n_to_add, v, 2);
//   if (n<=0) {
//     cout << "fatal in GainStream::evbuffer_reserve_space "<< n << endl;
//     return; /* Unable to reserve the space for some reason. */
//   }

//   for (i=0; i<n && n_to_add > 0; ++i) {
//     size_t len = v[i].iov_len;
//     cout << "iovec v had len " << len << endl;
//     if (len > n_to_add) /* Don't write more than n_to_add bytes. */
//        len = n_to_add;
//     if (
//       false
//       // generate_data(v[i].iov_base, len) < 0
//       ) {
//        /* If there was a problem during data generation, we can just stop
//           here; no data will be committed to the buffer. */
//        return;
//     }
//     /* Set iov_len to the number of bytes we actually wrote, so we
//        don't commit too much. */
//      v[i].iov_len = len;
//   }

//   /* We commit the space here.  Note that we give it 'i' (the number of
//      vectors we actually used) rather than 'n' (the number of vectors we
//      had available. */
//   if (evbuffer_commit_space(buf, v, i) < 0) {
//     cout << "fatal in GainStream::evbuffer_commit_space" << endl;
//     return; /* Error committing */
//   }

// }





} // namespace