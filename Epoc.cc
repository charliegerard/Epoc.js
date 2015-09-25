#include "Functions.h"

using v8::FunctionTemplate;

// BrainSensor.cc represents the top level of the module.
// C++ constructs that are exposed to javascript are exported here

NAN_MODULE_INIT(InitAll) {
  Nan::Set(target, Nan::New("connect").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(Connect)).ToLocalChecked());
}

NODE_MODULE(NativeExtension, InitAll)
