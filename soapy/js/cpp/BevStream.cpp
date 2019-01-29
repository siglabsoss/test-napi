#include "BevStream.hpp"

#include <math.h>
#include <cmath>


#include <stdio.h>
#include <iostream>
#include <unistd.h>

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




BevStream::BevStream():_thread_should_terminate(false) {


    cout << "BevStream() ctons" << endl;
    
    evthread_use_pthreads();
    // construct here or in thread?

    // must be called before event base is created
    // event_enable_debug_mode();

    evbase = event_base_new();


    // // Example event
    // if( run_timer ) {
    //     _example_event = evtimer_new(evbase, _example_clock_event, this);
    //     _example_clock_event(-1, EV_TIMEOUT, this);
    // }

    // pass this
    _thread = std::thread(&BevStream::threadMain, this);
}

void BevStream::threadMain() {
    // HiggsDriver *soapy = (HiggsDriver*)userdata;
    // size_t i = (size_t)userdata;
    // char c = *((char*)userdata);
    // cout << i << endl;

    // dsp_evbase = event_base_new();

    printf("event dispatch thread running\n");

    auto retval = event_base_loop(evbase, EVLOOP_NO_EXIT_ON_EMPTY);


    cout << "!!!!!!!!!!!!!!!!! BevStream::threadMain() exiting " << endl;
    cout << "!!!!!!!!!!!!!!!!! BevStream::threadMain() exiting " << endl;
    cout << "!!!!!!!!!!!!!!!!! BevStream::threadMain() exiting " << endl;
    

    printf("event_base_dispatch: %i\n", retval);


    // return NULL;

    // std::cout << "printRbThread()" << std::endl;
    //higgsRunRbPrintThread
    // while(higgsRunRbPrintThread) {
    //         uint32_t data, rb_error;
    //         rb->get(data, rb_error);
    //         if( rb_error == 0 ) {
    //             rb_buffer.enqueue(data);
    //             std::cout << "Ringbus: 0x" << HEX32_STRING(data) << std::endl;
    //         }

    // }

    std::cout << "BevStream::threadMain() closing" << std::endl;
}

int BevStream::stopThread() {
    cout << "fixme stopThread()" << endl;
    _thread_should_terminate = true;
    stopThreadDerivedClass();
    return 0;
}



}