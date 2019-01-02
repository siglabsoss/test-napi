#pragma once



///////
//
// Add new return types here

#define __RETURNTYPE_int Napi::Number
#define __RETURNTYPE_void void


///////
//
// For every type the system will see, you must copy a line here
// void is the only type that does not have this line

#define _CAP_RETURN_double auto returnValue = 
#define _CAP_RETURN_int auto returnValue = 
#define _CAP_RETURN_bool auto returnValue = 
#define _CAP_RETURN_string auto returnValue = 
#define _CAP_RETURN_unsigned auto returnValue = 
#define _CAP_RETURN_void



///////
//
// Add new code to check valid inputs for combinations of inputs

#define _NAPI_VALIDATE_INPUTS_int_int() \
  bool call_notok = info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber(); \
  if (call_notok) { \
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException(); \
  }


#define _NAPI_VALIDATE_INPUTS_int() \
  bool call_notok = info.Length() < 1 || !info[0].IsNumber(); \
  if (call_notok) { \
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException(); \
  }

#define _NAPI_VALIDATE_INPUTS_void() \
  bool call_notok = info.Length() != 0; \
  if (call_notok) { \
    Napi::TypeError::New(env, "Not expecting arguments").ThrowAsJavaScriptException(); \
    return; \
  }


///////
//
// Add new code to pass args for combinations of signatures
// this could be combined with previous section

#define _NAPI_CALL_int(path,fn,rt) \
auto first = info[0].As<Napi::Number>().Int32Value(); \
_CAP_RETURN(rt) path fn(first);

#define _NAPI_CALL_int_int(path,fn,rt) \
auto first = info[0].As<Napi::Number>().Int32Value(); \
auto second = info[1].As<Napi::Number>().Int32Value(); \
_CAP_RETURN(rt) path fn(first, second);

#define _NAPI_CALL_void(path,fn,rt) \
_CAP_RETURN(rt) path fn();


///////
//
// Add new code for converting return types

#define _NAPI_TAIL_int() \
  return Napi::Number::New(env, returnValue ); \
}

#define _NAPI_TAIL_void() \
  return ; \
}





#define _NAPI_HEADER(rt,fn) __RETURNTYPE_##rt fn##__wrapped(const Napi::CallbackInfo& info);


#define _NAPI_HEAD(path,x,rt) __RETURNTYPE_##rt path x##__wrapped (const Napi::CallbackInfo& info) { \
  auto env = info.Env();



// https://stackoverflow.com/questions/2831934/how-to-use-if-inside-define-in-the-c-preprocessor
#define _CAP_RETURN2(x) _CAP_RETURN_##x
#define _CAP_RETURN(x) _CAP_RETURN2(x)





#define _VA_GET(_1, _2, _3, _4, N, ...) N

#define _VA_COUNT(...) _GET_NTH_ARG(__VA_ARGS__, 4, 3, 2, 1)




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
_NAPI_CALL_##t0(path, fn, rt) \
_NAPI_TAIL_##rt()

#define _NAPI_BODY5(path, fn, rt, t0, t1) \
_NAPI_HEAD(path,fn,rt) \
_NAPI_VALIDATE_INPUTS_##t0##_##t1() \
_NAPI_CALL_##t0##_##t1(path, fn, rt) \
_NAPI_TAIL_##rt()




#define _NAPI_BIND_NAME(name,path,fn) \
exports.Set(name, Napi::Function::New(env, path fn##__wrapped))


#define _NAPI_BIND_MANUAL(name,path,fn) \
exports.Set(name, Napi::Function::New(env, path fn))


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




// Empty value so that macros here are able to return NULL or void
#define NAPI_RETVAL_NOTHING  // Intentionally blank #define

#define GET_AND_THROW_LAST_ERROR(env)                                    \
  do {                                                                   \
    const napi_extended_error_info *error_info;                          \
    napi_get_last_error_info((env), &error_info);                        \
    bool is_pending;                                                     \
    napi_is_exception_pending((env), &is_pending);                       \
    /* If an exception is already pending, don't rethrow it */           \
    if (!is_pending) {                                                   \
      const char* error_message = error_info->error_message != NULL ?    \
        error_info->error_message :                                      \
        "empty error message";                                           \
      napi_throw_error((env), NULL, error_message);                      \
    }                                                                    \
  } while (0)

#define NAPI_ASSERT_BASE(env, assertion, message, ret_val)               \
  do {                                                                   \
    if (!(assertion)) {                                                  \
      napi_throw_error(                                                  \
          (env),                                                         \
        NULL,                                                            \
          "assertion (" #assertion ") failed: " message);                \
      return ret_val;                                                    \
    }                                                                    \
  } while (0)

// Returns NULL on failed assertion.
// This is meant to be used inside napi_callback methods.
#define NAPI_ASSERT(env, assertion, message)                             \
  NAPI_ASSERT_BASE(env, assertion, message, NULL)

// Returns empty on failed assertion.
// This is meant to be used inside functions with void return type.
#define NAPI_ASSERT_RETURN_VOID(env, assertion, message)                 \
  NAPI_ASSERT_BASE(env, assertion, message, NAPI_RETVAL_NOTHING)

#define NAPI_CALL_BASE(env, the_call, ret_val)                           \
  do {                                                                   \
    if ((the_call) != napi_ok) {                                         \
      GET_AND_THROW_LAST_ERROR((env));                                   \
      return ret_val;                                                    \
    }                                                                    \
  } while (0)

// Returns NULL if the_call doesn't return napi_ok.
#define NAPI_CALL(env, the_call)                                         \
  NAPI_CALL_BASE(env, the_call, NULL)

// Returns empty if the_call doesn't return napi_ok.
#define NAPI_CALL_RETURN_VOID(env, the_call)                             \
  NAPI_CALL_BASE(env, the_call, NAPI_RETVAL_NOTHING)

#define DECLARE_NAPI_PROPERTY(name, func)                                \
  { (name), 0, (func), 0, 0, 0, napi_default, 0 }

#define DECLARE_NAPI_GETTER(name, func)                                  \
  { (name), 0, 0, (func), 0, 0, napi_default, 0 }
  

// eof
