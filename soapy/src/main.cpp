// DESCRIPTION: Verilator: Verilog example module
//
// This file ONLY is placed into the Public Domain, for any use,
// without warranty, 2017 by Wilson Snyder.
//======================================================================

// Include common routines
#include <napi.h>
#include "signals.h"


// Current simulation time (64-bit unsigned)
// vluint64_t main_time = 0;
// // Called by $time in Verilog
// double sc_time_stamp() {
//     return main_time;  // Note does conversion to real, to match SystemC
// }


Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return signals::Init(env, exports);
}
  

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll)
