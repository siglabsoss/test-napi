#include "napi.h"
#include "NapiHelper.hpp"

namespace signals {

  void debug(const Napi::CallbackInfo& info);


  int foo(int,int);
  _NAPI_HEADER(int, foo);

  int doubleit(int);
  _NAPI_HEADER(int, doubleit);

  int halfit(int);
  _NAPI_HEADER(int, halfit);

  void printint(int);
  _NAPI_HEADER(void, printint);

  Napi::Object Init(Napi::Env env, Napi::Object exports);
};
