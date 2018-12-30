#include "napi.h"


namespace signals {

  
  int foo(int,int);
  Napi::Number fooWrapped(const Napi::CallbackInfo& info);

  Napi::Object Init(Napi::Env env, Napi::Object exports);
};
