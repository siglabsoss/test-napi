#pragma once

#define _NAPI_HEAD(path, fname) \
Napi::Number path##fname(const Napi::CallbackInfo& info) { \
  auto env = info.Env();

#define NAPI_INT_INT_INT(path, fname) \
\
;
