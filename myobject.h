#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <node.h>
#include <node_object_wrap.h>
#include <uv.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include "edk.h"
#include "edkErrorCode.h"
#include "EmoStateDLL.h"
#include "node_object_wrap.h"


class NodeEPOCDriver : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Persistent<v8::Function> dcb;
  static v8::Persistent<v8::Function> cb;
  uv_loop_s* loop;
  std::string profileFile;
  int profileLoaded;
  unsigned int userID;
  uv_rwlock_t lock;
  int connected;
  int run;

 private:

  uv_loop_s* work;
  int option;
  explicit NodeEPOCDriver();
  ~NodeEPOCDriver();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Connect(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Disconnect(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Rezero(const v8::FunctionCallbackInfo<v8::Value>& args);
  // static void Reconnect(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Reconnect(NodeEPOCDriver* obj);
  static v8::Persistent<v8::Function> constructor;
  // static v8::Persistent<v8::Function> cb;
  static void Work_cb(uv_work_t* req);
  static void After_work(uv_work_t* req);
  static void LoadUser(NodeEPOCDriver* obj);
  static void process(uv_work_t* req);
  static void after_process(uv_work_t* req);
  static void init (v8::Local<v8::Object> target);
};

#endif
