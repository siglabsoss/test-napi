#pragma once

#include <stdint.h>
#include <thread>
#include <atomic>

#include <event2/event.h>
#include <event2/thread.h>
#include <event2/bufferevent.h>




class HiggsEvent {
public:
    HiggsEvent(HiggsDriver*, bool _run_timer = false);

    struct event_base *evbase;
    int stopThread();
    virtual void stopThreadDerivedClass() = 0;

    void threadMain();

    struct event *_example_event;

    std::thread _thread;


    // std::atomic<bool> _thread_can_terminate = false; // set by inside when ready??

    HiggsDriver* soapy;

    std::atomic<bool> _thread_should_terminate; // set by outside
    
    bool run_timer;
};