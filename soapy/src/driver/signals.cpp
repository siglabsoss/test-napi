#include "signals.h"
#include "NapiHelper.hpp"

#include <iostream>

#include <string.h>



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




napi_value RunCallback(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  NAPI_ASSERT(env, argc == 1,
      "Wrong number of arguments. Expects a single argument.");

  napi_valuetype valuetype0;
  NAPI_CALL(env, napi_typeof(env, args[0], &valuetype0));
  NAPI_ASSERT(env, valuetype0 == napi_function,
      "Wrong type of arguments. Expects a function as first argument.");

  napi_valuetype valuetype1;
  NAPI_CALL(env, napi_typeof(env, args[1], &valuetype1));
  NAPI_ASSERT(env, valuetype1 == napi_undefined,
      "Additional arguments should be undefined.");

  napi_value argv[1];
  const char* str = "hello worldd";
  size_t str_len = strlen(str);
  NAPI_CALL(env, napi_create_string_utf8(env, str, str_len, argv));

  napi_value global;
  NAPI_CALL(env, napi_get_global(env, &global));

  napi_value cb = args[0];
  NAPI_CALL(env, napi_call_function(env, global, cb, 1, argv, NULL));

  std::cout << "end of RunCallback" << std::endl;

  return NULL;
}

napi_value RunCallbackWithRecv(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value args[2];
  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, NULL, NULL));

  napi_value cb = args[0];
  napi_value recv = args[1];
  NAPI_CALL(env, napi_call_function(env, recv, cb, 0, NULL, NULL));
  return NULL;
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

  // for some reason the example does it this way?
  napi_property_descriptor desc[2] = {
    DECLARE_NAPI_PROPERTY("RunCallback", RunCallback),
    DECLARE_NAPI_PROPERTY("RunCallbackWithRecv", RunCallbackWithRecv),
  };

  if ((napi_define_properties(env, exports, 2, desc)) != napi_ok) {
      GET_AND_THROW_LAST_ERROR((env));
      std::cout << "napi_define_properties unhappy" << std::endl;
  }

  return exports;
}

