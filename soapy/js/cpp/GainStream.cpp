#include "GainStream.hpp"
#include <iostream>
#include <cassert>


namespace BevStream {

using namespace std;


static void _handle_udp_callback(struct bufferevent *bev, void *_dsp)
{
    GainStream *dsp = reinterpret_cast<GainStream*>(_dsp);

    // struct evbuffer *input = bufferevent_get_input(bev);
    // dsp->parseFeedbackBuffer(input, dsp);
}

static void _handle_udp_event(bufferevent* d, short kind, void* v) {
     cout << "_handle_udp_event" << endl;
}




GainStream::GainStream() {
  cout << "GainStream()" << endl;
}

///
/// Setup of callbacks and buffer waterlevels
///
void GainStream::setupBuffers() {
    // bufferevent_options(udp_payload_in_bev, BE_VOPT_THREADSAFE);


    // shared set of pointers that we overwrite and then copy out from
    // (copied to BevPair class)
    struct bufferevent * rsps_bev[2];


    auto init_drain_udp_payload = true;


    ////////////////////
    //
    // raw udp buffer
    //
    // could be replaced by reading fid directly
    // fixme
    //
    // This buffer is cross threads while the others do not
    // this allows us to have an entire thread dedicated to rx
    int ret = bufferevent_pair_new(evbase, 
        BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS | BEV_OPT_THREADSAFE, 
        rsps_bev);
    assert(ret>=0);

    // our little chained method for init and options all in one line
    udp_payload_bev = new BevPair2();
    udp_payload_bev->set(rsps_bev);//.enableLocking();
    // cout << "setupBuffers() ret " << ret << endl;

    if(init_drain_udp_payload) {
        bufferevent_setwatermark(udp_payload_bev->out, EV_READ, (1024+16)*4, 0);

        bufferevent_setcb(udp_payload_bev->out, _handle_udp_callback, NULL, _handle_udp_event, this);
        bufferevent_enable(udp_payload_bev->out, EV_READ | EV_WRITE | EV_PERSIST);

        bufferevent_set_max_single_read(udp_payload_bev->out, (1024+16)*4 );

        bufferevent_disable(udp_payload_bev->in, EV_READ);
        bufferevent_enable(udp_payload_bev->out, EV_READ);
        // bufferevent_setcb(udp_payload_in_bev, NULL, _write, NULL, NULL);
    }




/*


    ////////////////////
    //
    // cfo/sfo buf
    //
    ret = bufferevent_pair_new(evbase, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS , rsps_bev);
    assert(ret>=0);
    sfo_cfo_buf = new BevPair();
    sfo_cfo_buf->set(rsps_bev);//.enableLocking();

    // I'm guessing we would only adjust read of out
    // and we would only care about write of in, but we only need this for interrupts when data is 
    //   written?

    if(init_drain_sfo_cfo) {

        auto sfo_cfo_watermark = SFO_CFO_PACK_SIZE * 4 * 512;

        // set 0 for write because nobody writes to out
        bufferevent_setwatermark(sfo_cfo_buf->out, EV_READ, sfo_cfo_watermark, 0);

        // bufferevent_set_max_single_read(sfo_cfo_buf->out, (1024*3)*4 );


        // only get callbacks on read side
        bufferevent_setcb(sfo_cfo_buf->out, _handle_sfo_cfo_callback, NULL, _handle_sfo_cfo_event, this);
        bufferevent_enable(sfo_cfo_buf->out, EV_READ | EV_WRITE | EV_PERSIST);
    }





    ////////////////
    //
    // every subcarrier buf
    //
    ret = bufferevent_pair_new(evbase, BEV_OPT_CLOSE_ON_FREE, rsps_bev);
    assert(ret>=0);
    all_sc_buf = new BevPair();
    all_sc_buf->set(rsps_bev);//.enableLocking();

    if(init_drain_all_sc)   {
        // set 0 for write because nobody writes to out
        bufferevent_setwatermark(all_sc_buf->out, EV_READ, ALL_SC_BUFFER_SIZE*4, 0);
        bufferevent_set_max_single_read(all_sc_buf->out, ALL_SC_BUFFER_SIZE*4);
        // only get callbacks on read side
        bufferevent_setcb(all_sc_buf->out, _handle_all_sc_callback, NULL, _handle_all_sc_event, this);
        bufferevent_enable(all_sc_buf->out, EV_READ | EV_WRITE | EV_PERSIST);
    }


    ////////////////
    //
    // coarse 
    //
    ret = bufferevent_pair_new(evbase, BEV_OPT_CLOSE_ON_FREE, rsps_bev);
    assert(ret>=0);
    coarse_bev = new BevPair();
    coarse_bev->set(rsps_bev);//.enableLocking();

    if(init_drain_coarse) {
        bufferevent_setwatermark(coarse_bev->out, EV_READ, SUBCARRIER_CHUNK*4, 0);
        bufferevent_set_max_single_read(coarse_bev->out, SUBCARRIER_CHUNK*4);
        // only get callbacks on read side
        bufferevent_setcb(coarse_bev->out, _handle_coarse_callback, NULL, _handle_coarse_event, this);
        bufferevent_enable(coarse_bev->out, EV_READ | EV_WRITE | EV_PERSIST);
    }


    // event_ev = event_new(evbase, -1, 0, process_work_queue_cb, NULL);
    // event_active(event_ev, EV_TIMEOUT, 1);

    ////////////////
    //
    // data bound for gnuradio 
    //
    ret = bufferevent_pair_new(evbase, 
        BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS | BEV_OPT_THREADSAFE,
        rsps_bev);
    assert(ret>=0);
    gnuradio_bev = new BevPair();
    gnuradio_bev->set(rsps_bev);//.enableLocking();

    if(init_drain_for_gnuradio) {

        if( GET_GNURADIO_UDP_DO_CONNECT() ) {
            // new way where modem main is running stand-alone and sending udp to 
            bufferevent_setwatermark(gnuradio_bev->out, EV_READ, GNURADIO_UDP_SIZE, 0);
            bufferevent_set_max_single_read(gnuradio_bev->out, GNURADIO_UDP_SIZE);
            bufferevent_setcb(gnuradio_bev->out, _handle_gnuradio_callback_udp_mode, NULL, _handle_gnuradio_event, this);
            bufferevent_enable(gnuradio_bev->out, EV_READ | EV_WRITE | EV_PERSIST);
        }

        // these will never BOTH be set, as enforced by HiggsDriver() ctons
        if( GET_GNURADIO_ATTACHED_SEND_DATA() ) {
            // normal (old way) where python gnuradio compantion is running Higgs Soapy Driver
            bufferevent_setcb(gnuradio_bev->out, _handle_gnuradio_callback, NULL, _handle_gnuradio_event, this);

            // set 0 for write because nobody writes to out
            bufferevent_setwatermark(gnuradio_bev->out, EV_READ, DRAIN_GNURADIO_WATERMARK, 0);
            bufferevent_set_max_single_read(gnuradio_bev->out, DRAIN_GNURADIO_WATERMARK);
            bufferevent_enable(gnuradio_bev->out, EV_READ | EV_WRITE | EV_PERSIST);
        }

    }




    ////////////////
    //
    // data bound for events
    //
    ret = bufferevent_pair_new(evbase, 
        BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS | BEV_OPT_THREADSAFE, 
        rsps_bev);
    assert(ret>=0);
    events_bev = new BevPair();
    events_bev->set(rsps_bev);//.enableLocking();

    if(true) {

        // set 0 for write because nobody writes to out, in bytes
        bufferevent_setwatermark(events_bev->out, EV_READ, sizeof(custom_event_t), 0);
        bufferevent_set_max_single_read(events_bev->out, sizeof(custom_event_t) );
        // only get callbacks on read side
        bufferevent_setcb(events_bev->out, _handle_custom_event_callback, NULL, _handle_custom_event_event, this);
        bufferevent_enable(events_bev->out, EV_READ | EV_WRITE | EV_PERSIST);
    }



    ////////////////
    //
    // data bound for dsp
    //
    ret = bufferevent_pair_new(evbase, 
        BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS, 
        rsps_bev);
    assert(ret>=0);
    demod_bev = new BevPair();
    demod_bev->set(rsps_bev);//.enableLocking();

    if(init_drain_demod) {

        bufferevent_setwatermark(demod_bev->out, EV_READ, DEMOD_BEV_SIZE_WORDS*4, 0);
        bufferevent_set_max_single_read(demod_bev->out, DEMOD_BEV_SIZE_WORDS*4 );
        // only get callbacks on read side
        bufferevent_setcb(demod_bev->out, _handle_demod_callback, NULL, _handle_demod_event, this);
        bufferevent_enable(demod_bev->out, EV_READ | EV_WRITE | EV_PERSIST);
    }



    ////////////////
    //
    // data that came from zmq, feedback bus packets that are destined for pc
    //
    ret = bufferevent_pair_new(evbase, 
        BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS | BEV_OPT_THREADSAFE, 
        rsps_bev);
    assert(ret>=0);
    fb_pc_bev = new BevPair();
    fb_pc_bev->set(rsps_bev);//.enableLocking();

    if(init_drain_for_inbound_fb_bus) {

        // set 0 for write because nobody writes to out, in bytes
        // set to 8 bytes because we will send length as 4 bytes
        bufferevent_setwatermark(fb_pc_bev->out, EV_READ, 8, 0);
        // bufferevent_set_max_single_read(fb_pc_bev->out, DEMOD_BEV_SIZE_WORDS*4 );
        // only get callbacks on read side
        bufferevent_setcb(fb_pc_bev->out, _handle_fb_pc_callback, NULL, _handle_fb_pc_event, this);
        bufferevent_enable(fb_pc_bev->out, EV_READ | EV_WRITE | EV_PERSIST);
    }




    // not exactly a buffer

    // process_sfo_timer = evtimer_new(evbase, _handle_sfo_timer, this);

    // process_cfo_timer = evtimer_new(evbase, _handle_cfo_timer, this);



    ////////////////
    //
    // Fire up timer for sending to higgs
    //
    // sendto_higgs_timer = evtimer_new(evbase, _handle_sendto_higgs, this);

    // // setup once
    // struct timeval first_stht_timeout = { .tv_sec = 0, .tv_usec = 1000*200 };
    // evtimer_add(sendto_higgs_timer, &first_stht_timeout);


    ////////////////
    //
    // process ringbus
    // comes from another thread, should be threadsafe
    //
    ret = bufferevent_pair_new(evbase, 
        BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE, 
        rsps_bev);
    assert(ret>=0);
    rb_in_bev = new BevPair();
    rb_in_bev->set(rsps_bev);//.enableLocking();

    if(init_drain_for_inbound_fb_bus) {
        bufferevent_setwatermark(rb_in_bev->out, EV_READ, 4, 0);
        // only get callbacks on read side
        bufferevent_setcb(rb_in_bev->out, _handle_rb_callback, NULL, _handle_rb_event, this);
        bufferevent_enable(rb_in_bev->out, EV_READ | EV_WRITE | EV_PERSIST);
    }


    ////////////////
    //
    // process sliced data
    // comes from this same thread
    //
    ret = bufferevent_pair_new(evbase, 
        BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS,
        rsps_bev);
    assert(ret>=0);
    sliced_data_bev = new BevPair();
    sliced_data_bev->set(rsps_bev);//.enableLocking();

    if(init_drain_for_sliced_data) {
        bufferevent_setwatermark(sliced_data_bev->out, EV_READ, SLICED_DATA_BEV_WRITE_SIZE_WORDS*SLICED_DATA_CHUNK*4, 0);
        // only get callbacks on read side
        bufferevent_setcb(sliced_data_bev->out, _handle_sliced_data_callback, NULL, _handle_sliced_data_event, this);
        bufferevent_enable(sliced_data_bev->out, EV_READ | EV_WRITE | EV_PERSIST);
    }

*/

    (void)ret;

}


} // namespace