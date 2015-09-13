#include "Functions.h"

NAN_METHOD(Connect) {
    info.GetReturnValue().Set(Nan::New("This is a thing.").ToLocalChecked());
}