#pragma once


#define _VA_GET(_1, _2, _3, _4, N, ...) N

#define _VA_COUNT(...) _GET_NTH_ARG(__VA_ARGS__, 4, 3, 2, 1)

///////
//
// Add new return types here

#define __RETURNTYPE_int Napi::Number
#define __RETURNTYPE_void void


///////
//
// Add new code to check valid inputs for combinations of inputs

#define _NAPI_VALIDATE_INPUTS_int_int() \
  bool call_ok = info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber(); \
  if (call_ok) { \
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException(); \
  }


#define _NAPI_VALIDATE_INPUTS_int() \
  bool call_ok = info.Length() < 1 || !info[0].IsNumber(); \
  if (call_ok) { \
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException(); \
  }


///////
//
// Add new code to pass args for combinations of signatures


#define _NAPI_CALL_void_int(path,fn) \
auto first = info[0].As<Napi::Number>().Int32Value(); \
path fn(first);

#define _NAPI_CALL_int_int(path,fn) \
auto first = info[0].As<Napi::Number>().Int32Value(); \
auto returnValue = path fn(first);

#define _NAPI_CALL_int_int_int(path,fn) \
auto first = info[0].As<Napi::Number>().Int32Value(); \
auto second = info[1].As<Napi::Number>().Int32Value(); \
auto returnValue = path fn(first, second);

#define _NAPI_TAIL_int() \
  return Napi::Number::New(env, returnValue ); \
}

#define _NAPI_TAIL_void() \
  return ; \
}





#define _NAPI_HEADER(rt,fn) __RETURNTYPE_##rt fn##__wrapped(const Napi::CallbackInfo& info);


#define _NAPI_HEAD(path,x,rt) __RETURNTYPE_##rt path x##__wrapped (const Napi::CallbackInfo& info) { \
  auto env = info.Env();



/////////////////
//
// Manual versions of variadic function
// Call this in the body
// The inputs are
// path, anything before the function name (ie namespace::namespace2:: ) 
// fn, the function name
// rt, the return type (void is valid as well)
// t0, t1, ... the n input types

#define _GET_NAPI_BODY_MACRO(_1,_2,_3,_4,_5,_6,NAME,...) NAME
#define _NAPI_BODY(...) \
_GET_NAPI_BODY_MACRO(__VA_ARGS__, _NAPI_BODY6, _NAPI_BODY5, _NAPI_BODY4, _NAPI_BODY3, _NAPI_BODY2)(__VA_ARGS__)


#define _NAPI_BODY3(path, fn, rt) \
_NAPI_HEAD(path,fn,rt) \
_NAPI_CALL_##rt(path, fn) \
_NAPI_TAIL_##rt()

#define _NAPI_BODY4(path, fn, rt, t0) \
_NAPI_HEAD(path,fn,rt) \
_NAPI_VALIDATE_INPUTS_##t0() \
_NAPI_CALL_##rt##_##t0(path, fn) \
_NAPI_TAIL_##rt()

#define _NAPI_BODY5(path, fn, rt, t0, t1) \
_NAPI_HEAD(path,fn,rt) \
_NAPI_VALIDATE_INPUTS_##t0##_##t1() \
_NAPI_CALL_##rt##_##t0##_##t1(path, fn) \
_NAPI_TAIL_##rt()




/////////
//
// graveyard
//
// these macros approximate this code:

//Napi::Number signals::fooWrapped(const Napi::CallbackInfo& info)
// Napi::Number signals::foo__wrapped(const Napi::CallbackInfo& info) {
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




// eof
