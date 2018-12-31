#include "signals.h"
#include "NapiHelper.hpp"

#include <iostream>

_NAPI_BODY(signals::,foo,int,int,int);
int signals::foo(int a, int b) {
  return a + b;
}


_NAPI_BODY(signals::,doubleit,int,int);
int signals::doubleit(int a) {
  return a * 2 ;
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

  exports.Set("foo", Napi::Function::New(env, signals::foo__wrapped));
  

  exports.Set("doubleit", Napi::Function::New(env, signals::doubleit__wrapped));
  
  exports.Set("halfit", Napi::Function::New(env, signals::halfit__wrapped));
  

  exports.Set("debug", Napi::Function::New(env, signals::debug));
  

  return exports;
}

