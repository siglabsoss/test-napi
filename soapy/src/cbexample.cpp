// addon.cc
#include <node.h>
#include <v8.h>
#include <uv.h>

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

#include <iostream>


namespace demo {

using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::NewStringType;
using v8::Null;
using v8::Object;
using v8::String;
using v8::Value;
using namespace v8;

using namespace std;

struct Work {
uv_work_t request; Persistent<Function> callback;
  std::vector<uint32_t> locations;
  std::vector<uint32_t> results;
};


void CalculateResultsAsync(const v8::FunctionCallbackInfo<v8::Value>&args) {
    Isolate* isolate = args.GetIsolate();
     // we'll start a worker thread to do the job
     // and call the callback here...
    args.GetReturnValue().Set(Undefined(isolate));
}

void RunCallback(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();
  Local<Function> cb = Local<Function>::Cast(args[0]);

  auto foo = 2;
  cout << foo << endl;
  std::cout << __cplusplus << std::endl;

  const unsigned argc = 1;
  Local<Value> argv[argc] = {
      String::NewFromUtf8(isolate,
                          "hello world",
                          NewStringType::kNormal).ToLocalChecked() };
  cb->Call(context, Null(isolate), argc, argv).ToLocalChecked();
}

void Init(Local<Object> exports, Local<Object> module) {
  NODE_SET_METHOD(module, "exports", RunCallback);
  NODE_SET_METHOD(module, "calculate_results_async", CalculateResultsAsync);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace demo