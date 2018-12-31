#include "napi.h"
#include "NapiHelper.hpp"

namespace signals {

  void debug(const Napi::CallbackInfo& info);


  int foo(int,int);
  _NAPI_HEADER(int, foo);

  
  // Napi::Number foo__wrapped(const Napi::CallbackInfo& info);

  Napi::Object Init(Napi::Env env, Napi::Object exports);
};
