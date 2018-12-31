#pragma once

#define _XSTR(s) _STR(s)
#define _STR(s) #s


#define _VA_GET(_1, _2, _3, _4, N, ...) N

#define _VA_COUNT(...) _GET_NTH_ARG(__VA_ARGS__, 4, 3, 2, 1)


#define _NAPI_HEAD(path,x) Napi::Number path x##__wrapped (const Napi::CallbackInfo& info) { \
  auto env = info.Env();


#define _NAPI_VALIDATE_CALL() \
  bool call_ok = info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber(); \
  if (call_ok) { \
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException(); \
  }

#define _NAPI_TAIL() \
  auto first = info[0].As<Napi::Number>().Int32Value(); \
  auto second = info[1].As<Napi::Number>().Int32Value(); \
  return Napi::Number::New(env, signals::foo(first, second) ); \
}

#define NAPI_INT_INT_INT(path, fname) \
\
;

#define __TYPE_int 1
#define _NAPI_HEADERfoo(x) __TYPE_##x

#define __RETURNTYPE_int Napi::Number

#define _NAPI_HEADER(x,fn) __RETURNTYPE_##x fn##__wrapped(const Napi::CallbackInfo& info);
