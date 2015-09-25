#include "Functions.h"

using v8::FunctionTemplate;

NAN_MODULE_INIT(InitAll) {
  Nan::Set(target, Nan::New("connect").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(Connect)).ToLocalChecked());
}

NODE_MODULE(NativeExtension, InitAll)
