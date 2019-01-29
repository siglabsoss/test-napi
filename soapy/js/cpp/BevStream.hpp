#pragma once

#include <stdint.h>
#include <thread>
#include <atomic>

#include <event2/event.h>
#include <event2/thread.h>
#include <event2/bufferevent.h>

#include "BevPair2.hpp"


namespace BevStream {

class BevStream {
public:
    BevStream(bool defer_callbacks, bool print);

    bool _print;
    bool _defer_callbacks;

    struct event_base *evbase;
    void stopThread();
    virtual void stopThreadDerivedClass() = 0;

    void threadMain();


    // struct event *_example_event;

    std::thread _thread;


    // std::atomic<bool> _thread_can_terminate = false; // set by inside when ready??

    // HiggsDriver* soapy;

    std::atomic<bool> _thread_should_terminate; // set by outside
    
    // thread and ev base stuff above
    // buffer and pipe stuff below
    BevPair2* bev = 0;
    void setupBuffers();

    virtual void setBufferOptions(bufferevent* in, bufferevent* out) = 0;
    virtual void gotData(struct bufferevent *bev, struct evbuffer *buf, size_t len) = 0;

    void init(); // finishes buffer alloc, gets derived buffer settins, fires thread
};

}