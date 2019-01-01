#include "signals.h"
#include "NapiHelper.hpp"

#include <iostream>



#include <iostream>
#include <unistd.h> //usleep
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <set>

#include <thread>




_NAPI_BODY(signals::,foo,int,int,int);
int signals::foo(int a, int b) {
  return a + b;
}


_NAPI_BODY(signals::,doubleit,int,int);
int signals::doubleit(int a) {
  return a * 2;
}

namespace signals {


_NAPI_BODY(,halfit,int,int);
int halfit(int a) {
  return a >> 1;
}


_NAPI_BODY(,printint,void,int);
void printint(int a) {
  std::cout << a << std::endl;
}

std::thread a_thread;

void threadMain(void) {
  auto i = 0;
  while(1) {
    std::cout << i << std::endl;
    i++;
  }
}


_NAPI_BODY(,startThread,void,void);
void startThread(void) {
  std::cout << "start thread" << std::endl;
  // rtlsdr_reset_buffer(dev);
  a_thread = std::thread(&threadMain);
}


}



void signals::debug(const Napi::CallbackInfo& info) {
  std::cout << "debug called" << std::endl;
  
  // std::uint32_t a = STATIC_HASH("hi");
  

// #if ((uint32_t)4) == 4
// #if 0 == STATIC_STRCMP ("a","a")
// #if 'c' == _STR(c)[0]
//   std::cout << "YES" << std::endl;
// #endif

}

Napi::Object signals::Init(Napi::Env env, Napi::Object exports) {


  // auto cap = [] () { std::cout << "cap" << std::endl; };
  // exports.Set("cap", Napi::Function::New(env, cap));

  // exports.Set("doubleit", Napi::Function::New(env, signals::doubleit__wrapped));

  _NAPI_BIND_NAME("foo",signals::,foo);
  
  _NAPI_BIND_NAME("doubleit",signals::,doubleit);

  _NAPI_BIND_NAME("halfit",signals::,halfit);

  _NAPI_BIND_MANUAL("debug",signals::,debug);
  
  exports.Set("startThread", Napi::Function::New(env, signals::startThread__wrapped));
  
  _NAPI_BIND_NAME("startThread",signals::,startThread);

  return exports;
}

