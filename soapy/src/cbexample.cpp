// addon.cc

#pragma GCC diagnostic push 
#pragma GCC diagnostic ignored "-Wcast-function-type"
#include <node.h>
#include <v8.h>
#include <nan.h>
#pragma GCC diagnostic pop





#include <uv.h>


#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <condition_variable>
#include <mutex>

#include "GainStream.hpp"
#include "ToJs.hpp"

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

/////////////////////////////////
//
// Rain example

struct Work {
  uv_work_t request;
  Persistent<Function> callback;

  std::vector<uint32_t> locations;
  std::vector<uint32_t> results;
};

struct Work2 {
  uv_work_t request;
  Persistent<Function> callback;

  // std::vector<uint32_t> locations;
  // std::vector<uint32_t> results;
};



uint32_t calc_rain_stats(uint32_t &loc) {
  return loc * 2;
}

void pack_rain_result(v8::Isolate* isolate, v8::Local<v8::Object> & target, uint32_t & result){
  target->Set(String::NewFromUtf8(isolate, "mean"), Number::New(isolate, result));
  // target->Set(String::NewFromUtf8(isolate, "median"), Number::New(isolate, result.median));
  // target->Set(String::NewFromUtf8(isolate, "standard_deviation"), Number::New(isolate, result.standard_deviation));
  // target->Set(String::NewFromUtf8(isolate, "n"), Integer::New(isolate, result.n));
}








// called by libuv worker in separate thread
static void WorkAsync(uv_work_t *req)
{
    Work *work = static_cast<Work *>(req->data);

    // this is the worker thread, lets build up the results
    // allocated results from the heap because we'll need
    // to access in the event loop later to send back
    work->results.resize(work->locations.size());
    std::transform(work->locations.begin(), work->locations.end(), work->results.begin(), calc_rain_stats);


    // that wasn't really that long of an operation, so lets pretend it took longer...
    // std::this_thread::sleep_for(chrono::seconds(3));
}

// called by libuv in event loop when async function completes
static void WorkAsyncComplete(uv_work_t *req, int status)
{
    Isolate * isolate = Isolate::GetCurrent();

    // Fix for Node 4.x - thanks to https://github.com/nwjs/blink/commit/ecda32d117aca108c44f38c8eb2cb2d0810dfdeb
    v8::HandleScope handleScope(isolate);

    Local<Array> result_list = Array::New(isolate);
    Work *work = static_cast<Work *>(req->data);

    // the work has been done, and now we pack the results
    // vector into a Local array on the event-thread's stack.

    for (unsigned int i = 0; i < work->results.size(); i++ ) {
      Local<Object> result = Object::New(isolate);
      pack_rain_result(isolate, result, work->results[i]);
      result_list->Set(i, result);
    }

    // set up return arguments
    Handle<Value> argv[] = { Null(isolate) , result_list };

    // execute the callback
    // https://stackoverflow.com/questions/13826803/calling-javascript-function-from-a-c-callback-in-v8/28554065#28554065
    Local<Function>::New(isolate, work->callback)->Call(isolate->GetCurrentContext()->Global(), 2, argv);

    // Free up the persistent function callback
    work->callback.Reset();
    delete work;

}

void CalculateResultsAsync(const v8::FunctionCallbackInfo<v8::Value>&args) {
    Isolate* isolate = args.GetIsolate();

    Work * work = new Work();
    work->request.data = work;

    // extract each location (its a list) and store it in the work package
    // locations is on the heap, accessible in the libuv threads
    Local<Array> input = Local<Array>::Cast(args[0]);
    unsigned int num_locations = input->Length();
    // for (unsigned int i = 0; i < num_locations; i++) {
    //   work->locations.push_back(unpack_location(isolate, Local<Object>::Cast(input->Get(i))));
    // }

    for (unsigned int i = 0; i < num_locations; i++) {
      work->locations.push_back(i);
    }



    // store the callback from JS in the work package so we can
    // invoke it later
    Local<Function> callback = Local<Function>::Cast(args[1]);
    work->callback.Reset(isolate, callback);

    // kick of the worker thread
    uv_queue_work(uv_default_loop(),&work->request,WorkAsync,WorkAsyncComplete);


    args.GetReturnValue().Set(Undefined(isolate));
}




////////////////////
//
// My Stream types

BevStream::GainStream* gain1;
BevStream::GainStream* gain2;
BevStream::GainStream* gain25;
BevStream::GainStream* gain3;

BevStream::ToJs* toJs;

std::condition_variable streamOutputReadyCondition;
std::mutex mutfruit;
bool outputReady;
std::vector<BevStream::tojs_t> toJsPointers;




// called by libuv worker in separate thread
// BLOCK AT WILL
static void workAsync2(uv_work_t *req)
{
    Work2 *work = static_cast<Work2 *>(req->data); // grab the pointer to the object


    // this is the worker thread, lets build up the results
    // allocated results from the heap because we'll need
    // to access in the event loop later to send back
    // work->results.resize(work->locations.size());
    // std::transform(work->locations.begin(), work->locations.end(), work->results.begin(), calc_rain_stats);

    // as soon as we return, the other callback will fire

    // static int times = 0;

    // times++;


    // that wasn't really that long of an operation, so lets pretend it took longer...
    // std::this_thread::sleep_for(chrono::seconds(3));
}

// called by libuv in event loop when async function completes
// DO NOT BLOCK
static void workAsyncComplete2(uv_work_t *req, int status)
{
    Isolate * isolate = Isolate::GetCurrent();

    // Fix for Node 4.x - thanks to https://github.com/nwjs/blink/commit/ecda32d117aca108c44f38c8eb2cb2d0810dfdeb
    v8::HandleScope handleScope(isolate);

    Work2 *work = static_cast<Work2 *>(req->data);

    // the work has been done, and now we pack the results
    // vector into a Local array on the event-thread's stack.
    // Local<Array> result_list = Array::New(isolate);
    // for (unsigned int i = 0; i < work->results.size(); i++ ) {
    //   Local<Object> result = Object::New(isolate);
    //   pack_rain_result(isolate, result, work->results[i]);
    //   result_list->Set(i, result);
    // }

    char *dataIn = (char*)malloc(1024*4);
    size_t length = 1024*4;
    uint32_t *asint = (uint32_t*) dataIn;

    for(auto i = 0; i < 1024; i++) {
      asint[i] = 0;
    }

    asint[0] = 0xdeadbeef;
    

    MaybeLocal<Object> buffer = Nan::NewBuffer(dataIn, length);
    // args.GetReturnValue().Set(buffer.ToLocalChecked());


    // set up return arguments
    // Handle<Value> argv[] = { result_list };
    Handle<Value> argv[] = { buffer.ToLocalChecked() };

    // execute the callback
    // https://stackoverflow.com/questions/13826803/calling-javascript-function-from-a-c-callback-in-v8/28554065#28554065
    Local<Function>::New(isolate, work->callback)->Call(isolate->GetCurrentContext()->Global(), 1, argv);


    static int times = 0;

    times++;

    if( times < 3 ) {
      uv_queue_work(uv_default_loop(),&work->request,workAsync2,workAsyncComplete2);
    }

    // Free up the persistent function callback
    // work->callback.Reset();
    // delete work;

}




void setupStreams() {

  outputReady = false;

  gain1 = new BevStream::GainStream(true, false);
  gain2 = new BevStream::GainStream(true, false);
  gain25= new BevStream::GainStream(true, false);
  gain3 = new BevStream::GainStream(true, false);

  toJs = new BevStream::ToJs(true, false, &mutfruit, &streamOutputReadyCondition, &outputReady, &toJsPointers);

  gain1->name = "gain1";
  gain2->name = "gain2";
  gain25->name = "gain25";
  gain3->name = "gain3";

  gain1->pipe(gain2)->pipe(gain25)->pipe(gain3)->pipe(toJs);

  usleep(1000);

  (void)gain1;
}


void setStreamCallback(const v8::FunctionCallbackInfo<v8::Value>&args) {
    Isolate* isolate = args.GetIsolate();

    Work2 * work = new Work2();
    work->request.data = work; // the request member will be available to us in subsequent callbacks, so we set it to ourself

    // extract each location (its a list) and store it in the work package
    // locations is on the heap, accessible in the libuv threads
    // Local<Array> input = Local<Array>::Cast(args[0]);
    // unsigned int num_locations = input->Length();
    // for (unsigned int i = 0; i < num_locations; i++) {
    //   work->locations.push_back(unpack_location(isolate, Local<Object>::Cast(input->Get(i))));
    // }


    // store the callback from JS in the work package so we can
    // invoke it later
    Local<Function> callback = Local<Function>::Cast(args[0]);
    work->callback.Reset(isolate, callback);

    // kick off the worker thread
    uv_queue_work(uv_default_loop(),&work->request,workAsync2,workAsyncComplete2);


    args.GetReturnValue().Set(Undefined(isolate));
}


NAN_METHOD(writeStreamData) {
  // I am very unclear on the difference between this code for info[0]
  // and code beloe for info[1]
  // possibly because we want to end up with a Local<Object> in the first case
  // and a uint32_t in the second
  MaybeLocal<Object> maybeBufferObj = Nan::To<Object>(info[0]);
  Local<Object> bufferObj;
  if (!maybeBufferObj.ToLocal(&bufferObj)) {
    cout << "Argument 0 Invalid in call to TransformBuffer" << endl;
    return;
  }

  uint32_t length = info[1]->Uint32Value(Nan::GetCurrentContext()).ToChecked();

  const char* dataIn = node::Buffer::Data(bufferObj);

  bufferevent_write(gain1->pair->in, dataIn, length);

}


////////////////////
//
// NAN buffer

void GetBuffer(const FunctionCallbackInfo<Value>& args) {
  char *dataIn = (char*)malloc(1024*4);
  size_t length = 1024*4;
  uint32_t *asint = (uint32_t*) dataIn;

  for(auto i = 0; i < 1024; i++) {
    asint[i] = 0;
  }

  asint[0] = 0xdeadbeef;
  

  MaybeLocal<Object> buffer = Nan::NewBuffer(dataIn, length);
  args.GetReturnValue().Set(buffer.ToLocalChecked());
}

// https://github.com/nodejs/nan/issues/749
// https://github.com/bcoin-org/bcrypto/issues/7
// https://github.com/nodejs/nan/blob/master/doc/new.md
// https://github.com/nodejs/nan/blob/master/doc/maybe_types.md
NAN_METHOD(TransformBuffer)
{
  // I am very unclear on the difference between this code for info[0]
  // and code beloe for info[1]
  // possibly because we want to end up with a Local<Object> in the first case
  // and a uint32_t in the second
  MaybeLocal<Object> maybeBufferObj =  Nan::To<Object>(info[0]);
  Local<Object> bufferObj;
  if (!maybeBufferObj.ToLocal(&bufferObj)) {
    cout << "Argument 0 Invalid in call to TransformBuffer" << endl;
    return;
  }

  uint32_t length = info[1]->Uint32Value(Nan::GetCurrentContext()).ToChecked();

  const char* dataIn = node::Buffer::Data(bufferObj);
  // const uint32_t* asintIn = (uint32_t*) dataIn;

  char *dataOut = (char*)malloc(length);
  uint32_t *asint = (uint32_t*) dataIn;

  // cout << asint[0] << " " << asint[1] << endl;

  for(unsigned i = 0; i < length/4; i++) {
    dataOut[i] = dataIn[i] & 0xf0f0f0f0;
  }

  MaybeLocal<Object> buffer = Nan::NewBuffer(dataOut, length);
  info.GetReturnValue().Set(buffer.ToLocalChecked());







    // char* msg = Nan::Buffer::Data(bufferObj);

    // cout << msg[0] << msg[1] << endl;
 
    // info.GetReturnValue().Set(Nan::New<String>(msg, len).ToLocalChecked());
    // You can not use strlen(msg). Because it is a binary not a string. The end of  string is a null byte and  the end of binary data is not null.
    
    // auto ptr = (char*) bufferObj->GetIndexedPropertiesExternalArrayData();
    // cout << ptr << endl;

    // auto ptr = bufferObj->GetContents().Data();


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

// void Init(Local<Object> exports, Local<Object> module) {
NAN_MODULE_INIT(Init) {
  NODE_SET_METHOD(target, "runCallback", RunCallback);
  NODE_SET_METHOD(target, "calculate_results_async", CalculateResultsAsync);
  NODE_SET_METHOD(target, "GetBuffer", GetBuffer);
  NODE_SET_METHOD(target, "setStreamCallback", setStreamCallback);
  // NODE_SET_METHOD(target, "TransformBuffer", TransformBuffer);
  NAN_EXPORT(target, TransformBuffer);
  NAN_EXPORT(target, writeStreamData);


  setupStreams();
}

#pragma GCC diagnostic push 
#pragma GCC diagnostic ignored "-Wcast-function-type"
NODE_MODULE(NODE_GYP_MODULE_NAME, Init)
#pragma GCC diagnostic pop

}  // namespace demo