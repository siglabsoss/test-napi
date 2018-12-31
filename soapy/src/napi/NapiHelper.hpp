#pragma once

#define _XSTR(s) _STR(s)
#define _STR(s) #s


#define _VA_GET(_1, _2, _3, _4, N, ...) N

#define _VA_COUNT(...) _GET_NTH_ARG(__VA_ARGS__, 4, 3, 2, 1)


#define _NAPI_HEAD(path,x) Napi::Number path x##__wrapped (const Napi::CallbackInfo& info) { \
  auto env = info.Env();


#define _NAPI_VALIDATE_INPUTS_int_int() \
  bool call_ok = info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber(); \
  if (call_ok) { \
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException(); \
  }

#define _NAPI_CALL_int_int_int(path,fn) \
auto first = info[0].As<Napi::Number>().Int32Value(); \
auto second = info[1].As<Napi::Number>().Int32Value(); \
auto returnValue = path fn(first, second);

#define _NAPI_TAIL() \
  return Napi::Number::New(env, returnValue ); \
}

#define NAPI_INT_INT_INT(path, fname) \
\
;

#define __TYPE_int 1
#define _NAPI_HEADERfoo(x) __TYPE_##x

#define __RETURNTYPE_int Napi::Number

#define _NAPI_HEADER(x,fn) __RETURNTYPE_##x fn##__wrapped(const Napi::CallbackInfo& info);


#define _NAPI_BODY(path, fn, rt, t0, t1) \
_NAPI_HEAD(path,fn) \
_NAPI_VALIDATE_INPUTS_##t0##_##t1() \
_NAPI_CALL_##rt##_##t0##_##t1(path, fn) \
_NAPI_TAIL()



// eof
