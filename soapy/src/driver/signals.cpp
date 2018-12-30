#include "signals.h"
#include "NapiHelper.hpp"

#include <iostream>

int signals::foo(int a, int b) {
  return a + b;
}

Napi::Number signals::fooWrapped(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  bool call_ok = info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber();
  if (call_ok) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
  }

  auto first = info[0].As<Napi::Number>().Int32Value();
  auto second = info[1].As<Napi::Number>().Int32Value();

  int returnValue = signals::foo(first, second);
  
  return Napi::Number::New(env, returnValue);
}
// Napi::Number signals::fooWrapped(const Napi::CallbackInfo& info) {
//   auto env = info.Env();
//   bool call_ok = info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber();
//   if (call_ok) {
//     Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
//   }

//   auto first = info[0].As<Napi::Number>().Int32Value();
//   auto second = info[1].As<Napi::Number>().Int32Value();

//   int returnValue = signals::foo(first, second);
  
//   return Napi::Number::New(env, returnValue);
// }


Napi::Object signals::Init(Napi::Env env, Napi::Object exports) {


  // auto cap = [] () { std::cout << "cap" << std::endl; };
  // exports.Set("cap", Napi::Function::New(env, cap));

  exports.Set("foo", Napi::Function::New(env, signals::fooWrapped));
  

  return exports;
}

