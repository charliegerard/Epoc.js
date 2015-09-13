#include "Functions.h"

NAN_METHOD(Connect) {
    v8::Local<v8::Function> callbackHandle = info[0].As<v8::Function>();

	v8::Local<v8::Object> event = Nan::New<v8::Object>();
    Nan::Set(event, Nan::New("key").ToLocalChecked(), Nan::New("value").ToLocalChecked());

	v8::Local<v8::Value> parameters[1];
	parameters[0] = event;

    Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callbackHandle, 1, parameters);
}