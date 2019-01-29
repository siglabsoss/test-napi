#include "BevStream.hpp"

namespace BevStream {



class GainStream : public BevStream
{
public:
    GainStream();
    void setupBuffers();
    // void setupRadioVector();
    // void setupAttachedRadio();
    struct event* event_boot = 0;

    struct event* event_hello = 0;

    struct event* event_pm = 0;



    // struct event* event_ev;

    // cross thread between rx_thread and this one
    BevPair2* udp_payload_bev = 0;

    // created on this thread
    BevPair2* sfo_cfo_buf = 0;
    BevPair2* all_sc_buf = 0;
    BevPair2* coarse_bev = 0;

    // created on this thread, drains into other thread
    BevPair2* gnuradio_bev = 0;
    BevPair2* demod_bev = 0;
    BevPair2* fb_pc_bev = 0;
    BevPair2* rb_in_bev = 0;
    BevPair2* sliced_data_bev = 0;



};












}
