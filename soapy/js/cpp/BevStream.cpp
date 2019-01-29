#include "BevStream.hpp"

#include <math.h>
#include <cmath>


#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <cassert>

namespace BevStream {

using namespace std;


// static void
// _example_clock_event(int fd, short kind, void *userp)
// {
//     /* Print current time. */
//     time_t t = time(NULL);
//     int now_s = t % 60;
//     int now_m = (t / 60) % 60;
//     int now_h = (t / 3600) % 24;
//     /* The trailing \r will make cursor return to the beginning
//      * of the current line. */
//     printf("%02d:%02d:%02d\n", now_h, now_m, now_s);
//     fflush(stdout);

//     /* Next clock update in one second. */
//     /* (A more prudent timer strategy would be to update clock
//      * on the next second _boundary_.) */


//     struct timeval timeout = { .tv_sec = 10, .tv_usec = 0 };
//     HiggsEvent* event = (HiggsEvent*)userp;
//     evtimer_add(event->_example_event, &timeout);
// }



/////////////////////////////

static void _handle_udp_callback(struct bufferevent *bev, void *_dsp)
{
    BevStream *dsp = reinterpret_cast<BevStream*>(_dsp);
    (void)dsp;

    // struct evbuffer *input = bufferevent_get_input(bev);
    // dsp->parseFeedbackBuffer(input, dsp);
}

static void _handle_udp_event(bufferevent* d, short kind, void* v) {
     cout << "_handle_udp_event" << endl;
}





/////////////////////////////



BevStream::BevStream(bool defer_callbacks, bool print):
    _print(print),
    _defer_callbacks(defer_callbacks),
    _thread_should_terminate(false) {

    if(_print) {
        cout << "BevStream() ctons" << endl;
    }
    
    evthread_use_pthreads();
    // construct here or in thread?

    // must be called before event base is created
    // event_enable_debug_mode();

    evbase = event_base_new();
}

void BevStream::init() {
    setupBuffers();


    // pass this
    _thread = std::thread(&BevStream::threadMain, this);    
}

void BevStream::threadMain() {
    // HiggsDriver *soapy = (HiggsDriver*)userdata;
    // size_t i = (size_t)userdata;
    // char c = *((char*)userdata);
    // cout << i << endl;

    // dsp_evbase = event_base_new();

    cout << "event dispatch thread running" << endl;

    auto retval = event_base_loop(evbase, EVLOOP_NO_EXIT_ON_EMPTY);


    cout << "!!!!!!!!!!!!!!!!! BevStream::threadMain() exiting " << endl;
    cout << "!!!!!!!!!!!!!!!!! BevStream::threadMain() exiting " << endl;
    cout << "!!!!!!!!!!!!!!!!! BevStream::threadMain() exiting " << endl;
    

    cout << "event_base_dispatch: " << retval << endl;

    std::cout << "BevStream::threadMain() closing" << std::endl;
}

void BevStream::stopThread() {
    cout << "fixme stopThread()" << endl;
    _thread_should_terminate = true;
    stopThreadDerivedClass();
}



///
/// Setup of callbacks and buffer waterlevels
///
void BevStream::setupBuffers() {
    // bufferevent_options(udp_payload_in_bev, BE_VOPT_THREADSAFE);


    // shared set of pointers that we overwrite and then copy out from
    // (copied to BevPair class)
    struct bufferevent * rsps_bev[2];


    ////////////////////
    //
    // raw udp buffer
    //
    // could be replaced by reading fid directly
    // fixme
    //
    // This buffer is cross threads while the others do not
    // this allows us to have an entire thread dedicated to rx
    int flags = BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE;

    if( _defer_callbacks ) {
        flags |= BEV_OPT_DEFER_CALLBACKS;
    }

    int ret = bufferevent_pair_new(evbase, 
        flags, 
        rsps_bev);
    assert(ret>=0);

    bev = new BevPair2();
    bev->set(rsps_bev);

    setBufferOptions(bev->in, bev->out); // this will take care of this:
    // bufferevent_setwatermark(bev->out, EV_READ, (1024+16)*4, 0);
    // bufferevent_set_max_single_read(bev->out, (1024+16)*4 );

    bufferevent_setcb(bev->out, _handle_udp_callback, NULL, _handle_udp_event, this);
    bufferevent_enable(bev->out, EV_READ | EV_WRITE | EV_PERSIST);

}


}