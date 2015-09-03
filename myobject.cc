#include "myobject.h"
#include "edk.h"
#include <uv.h>

using namespace v8;

#define REQ_FUN_ARG(I, VAR)                                             \
if (args.Length() <= (I) || !args[I]->IsFunction())                   \
isolate -> ThrowException(Exception::TypeError(                         \
String::NewFromUtf8(isolate, "Argument " #I " must be a function")));  \
// Local<Function> VAR = Local<Function>::Cast(args[I]);


#define REQ_STRING_ARG(I, VAR)                                             \
if (args.Length() <= (I) || !args[I]->IsString())                   \
isolate -> ThrowException(Exception::TypeError(                         \
String::NewFromUtf8(isolate, "Argument " #I " must be a string")));  \
String::Utf8Value VAR(args[I]->ToString());


Persistent<Function> NodeEPOCDriver::constructor;
Persistent<Function> NodeEPOCDriver::cb;
Persistent<Function> NodeEPOCDriver::dcb;


NodeEPOCDriver::NodeEPOCDriver() : profileLoaded(0), userID(0), connected(0), run(0), option(0){
  work = uv_loop_new();
  loop = uv_default_loop();
  uv_rwlock_init(&lock);
}

NodeEPOCDriver::~NodeEPOCDriver() {
  uv_rwlock_destroy(&lock);
  EE_EngineDisconnect();
}

struct baton_t {
    NodeEPOCDriver *obj;
    EmoEngineEventHandle* eEvent;
    EmoStateHandle* eState;
    bool update;
    int gyroX;
    int gyroY;
    int cog;
    float cog_power;
    int blink;
    int winkLeft;
    int winkRight;
    int lookLeft;
    int lookRight;
    float eyebrow;
    float furrow;
    float smile;
    float clench;
    float smirkLeft;
    float smirkRight;
    float laugh;
    float shortTermExcitement;
    float longTermExcitement;
    float engagementOrBoredom;
    float timestamp;
};

void NodeEPOCDriver::Init(Handle<Object> exports) {
  Isolate* isolate = Isolate::GetCurrent();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "NodeEPOCDriver"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "connect", Connect);
  NODE_SET_PROTOTYPE_METHOD(tpl, "rezero", Rezero);
  // NODE_SET_PROTOTYPE_METHOD(tpl, "reconnect", Reconnect);
  // NODE_SET_PROTOTYPE_METHOD(tpl, "work_cb", Work_cb);
  // NODE_SET_PROTOTYPE_METHOD(tpl, "after_work", After_work);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "NodeEPOCDriver"),
               tpl->GetFunction());
}

void NodeEPOCDriver::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  NodeEPOCDriver* obj = new NodeEPOCDriver();
  obj->Wrap(args.This());
  args.GetReturnValue().Set(args.This());

}

void NodeEPOCDriver::Connect(const FunctionCallbackInfo<Value>& args){

    NodeEPOCDriver* obj = ObjectWrap::Unwrap<NodeEPOCDriver>(args.This());
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    REQ_STRING_ARG(0,param0);
    REQ_FUN_ARG(1, cb);

    uv_rwlock_wrlock(&obj->lock);
    obj->profileFile = std::string(*param0);
    obj->option = 0;
    // // get the option for engine or remote connect
    if (args.Length()==3 && args[2]->IsInt32()){
        Local<Integer> i = Local<Integer>::Cast(args[2]);
        int selected = (int)(i->Int32Value());
        if (selected != 0){
          //OPTION IS 1
          obj->option = 1;
        }
    }

    // Local<Function> cb = Local<Function>::Cast(args[0]);
    // obj->cb = Local<Function>::New(cb);

    //cb : Local<Function> VAR = Local<Function>::Cast(args[I]);

    // obj->cb = v8::Persistent<v8::Function>::New(isolate, cb);
    // obj->cb = Persistent<Function>::New(isolate, cb);
    // obj->cb = v8::Persistent<v8::Function>::New(isolate, cb);

    // Local<Function> cons = Local<Function>::New(isolate, cb);
    args.GetReturnValue().Set(Local<Function>::New(isolate, obj->cb));

    Reconnect(obj);

    uv_rwlock_wrunlock(&obj->lock);

    // ************************
    // undefined is returned if we don't set a return value explicitely
    // ************************
}

// void NodeEPOCDriver::Reconnect(const FunctionCallbackInfo<Value>& args){
void NodeEPOCDriver::Reconnect(NodeEPOCDriver* obj){
   // NodeEPOCDriver* obj = ObjectWrap::Unwrap<NodeEPOCDriver>(args.This());
        // read arg for option, engine or remote..
        // read option for profile

        while (obj->connected == 0){

            switch(obj->option){
                case 0: {
                  obj->connected = (EE_EngineConnect() == EDK_OK)? 1 : 0;
                  break;
                }
                case 1: {
                    // I ALWAYS GET HERE
                    // obj->connected = (EE_EngineRemoteConnect("127.0.0.1",1726)==EDK_OK)? 1: 0;
                  obj->connected = (EE_EngineRemoteConnect("127.0.0.1",3008)==EDK_OK)? 1: 0;
                  break;
                }
                default: {break;}
            }
        }
        // std::cout << "7" << std::endl;

        if (obj->connected == 1){
            std::cout << "connected to emotiv" << std::endl;
            uv_work_t *req = new uv_work_t();
            req->data = obj;
            obj->run=1;

            int status = uv_queue_work(obj->loop, req, Work_cb, (uv_after_work_cb)After_work);
            assert(status == 0);

            std::cout << "starting epoc event loop" << std::endl;
        }

  uv_rwlock_wrunlock(&obj->lock);

}

void NodeEPOCDriver::LoadUser(NodeEPOCDriver* obj){
  bool errorOnce = true;

  if (obj->profileLoaded == 0){
    std::cout << "i should get here" << std::endl;
      int error = EE_LoadUserProfile(obj->userID, obj->profileFile.c_str());
      if (error == 0){
          unsigned long activeActions = 0;
          EE_CognitivGetActiveActions(obj->userID, &activeActions);

          std::cout << "connected and opened profile '"<<obj->profileFile << "' with " << activeActions << " active actions" << std::endl;

          obj->profileLoaded = 1;
      } else if (errorOnce) {
          std::cout << "condition: "<< error << ". either dongle is disconnected or could not find profile '" <<obj->profileFile << "'" << std::endl;
      }
      errorOnce = false;
  }
}


void NodeEPOCDriver::process(uv_work_t* req)
{
}

void NodeEPOCDriver::After_work(uv_work_t* req) {
// //        baton_t *baton = static_cast<baton_t *>(req->data);
  NodeEPOCDriver *obj = static_cast<NodeEPOCDriver *>(req->data);
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  std::cout << "after work loop" << std::endl;

  uv_rwlock_wrlock(&obj->lock);

  obj->profileLoaded = 0;
  obj->userID = 0;

  Local<Value> argv[0];
  obj->cb.Reset();

  uv_rwlock_wrunlock(&obj->lock);
      std::cout << "do i ever get here"<< std::endl;

  TryCatch try_catch;

  // if (obj->dcb !=Null())
  // if (obj->dcb != Null(isolate))
      // obj->dcb->Call(Context::GetCurrent()->Global(), 0, argv);

  if (try_catch.HasCaught()) {
      node::FatalException(try_catch);
  }
}




void NodeEPOCDriver::after_process(uv_work_t* req) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  baton_t* baton = static_cast<baton_t *>(req->data);

  Local<Value> argv[1];

  v8::Local<v8::Object> obj = v8::Object::New(isolate);

        if (baton->update){

    //         // these will be the fields from the event
            obj->Set(v8::String::NewFromUtf8(isolate, "time"), v8::Number::New(isolate, baton->timestamp));
            obj->Set(v8::String::NewFromUtf8(isolate, "userId"), v8::Number::New(isolate, baton->obj->userID));
            obj->Set(v8::String::NewFromUtf8(isolate, "gyroX"), v8::Number::New(isolate, baton->gyroX));
            obj->Set(v8::String::NewFromUtf8(isolate, "gyroY"), v8::Number::New(isolate, baton->gyroY));
            obj->Set(v8::String::NewFromUtf8(isolate, "wirelessSignalStatus"),  v8::Number::New(isolate, 2));
            obj->Set(v8::String::NewFromUtf8(isolate, "blink"),  v8::Number::New(isolate, baton->blink));
            obj->Set(v8::String::NewFromUtf8(isolate, "winkLeft"),  v8::Number::New(isolate, baton->winkLeft));
            obj->Set(v8::String::NewFromUtf8(isolate, "winkRight"),  v8::Number::New(isolate, baton->winkRight));
            obj->Set(v8::String::NewFromUtf8(isolate, "lookLeft"),  v8::Number::New(isolate, baton->lookLeft));
            obj->Set(v8::String::NewFromUtf8(isolate, "lookRight"),  v8::Number::New(isolate, baton->lookRight));
            obj->Set(v8::String::NewFromUtf8(isolate, "eyebrow"), v8::Number::New(isolate, baton->eyebrow));
            obj->Set(v8::String::NewFromUtf8(isolate, "furrow"),  v8::Number::New(isolate, baton->furrow));
            obj->Set(v8::String::NewFromUtf8(isolate, "smile"),  v8::Number::New(isolate, baton->smile));
            obj->Set(v8::String::NewFromUtf8(isolate, "clench"),  v8::Number::New(isolate, baton->clench));
            obj->Set(v8::String::NewFromUtf8(isolate, "smirkLeft"),  v8::Number::New(isolate, baton->smirkLeft));
            obj->Set(v8::String::NewFromUtf8(isolate, "smirkRight"),  v8::Number::New(isolate, baton->smirkRight));
            obj->Set(v8::String::NewFromUtf8(isolate, "laugh"),  v8::Number::New(isolate, baton->laugh));
            obj->Set(v8::String::NewFromUtf8(isolate, "shortTermExcitement"),  v8::Number::New(isolate, baton->shortTermExcitement));
            obj->Set(v8::String::NewFromUtf8(isolate, "longTermExcitement"),  v8::Number::New(isolate, baton->longTermExcitement));
            obj->Set(v8::String::NewFromUtf8(isolate, "engagementOrBoredom"),  v8::Number::New(isolate, baton->engagementOrBoredom));
            obj->Set(v8::String::NewFromUtf8(isolate, "cognitivAction"),  v8::Number::New(isolate, baton->cog));
            obj->Set(v8::String::NewFromUtf8(isolate, "cognitivPower"),  v8::Number::New(isolate, baton->cog_power));

            argv[0] = obj;

            TryCatch try_catch;

            if (baton->obj->cb != Null(isolate))
                // baton->obj->cb->Call(Context::GetCurrent()->Global(), 1, argv);
              // cb->Call(Isolate::GetCurrentContext(baton->obj), 1, argv);
              // baton->obj->cb->Call((* baton), 1, argv);

            if (try_catch.HasCaught()) {
                node::FatalException(try_catch);
            }
        }
        delete baton;
}




void NodeEPOCDriver::Work_cb(uv_work_t* req){

  NodeEPOCDriver *obj = static_cast<NodeEPOCDriver *>(req->data);

  uv_rwlock_wrlock(&obj->lock);
  LoadUser(obj);
  EE_HeadsetGyroRezero(obj->userID);
  uv_rwlock_wrunlock(&obj->lock);

  while (true){

    uv_rwlock_rdlock(&obj->lock);
    bool connected = (obj->connected == 1);
    bool run = (obj->run == 1);
    uv_rwlock_rdunlock(&obj->lock);

    if (connected && run){
//                 // read in the profile

        EmoEngineEventHandle eEvent = EE_EmoEngineEventCreate();
        EmoStateHandle eState = EE_EmoStateCreate();
        int state = EE_EngineGetNextEvent(eEvent);
        if (state == EDK_OK) {

           std::cout << "and there?" << std::endl;

                    // unsigned int userid = 0;
                    // EE_EmoEngineEventGetUserId(eEvent, &userid);
                    // EE_GetUserProfile(obj->userID, eEvent);

                    // int state1 = EE_GetUserProfile(obj->userID, eEvent);
                    // if(state1 == EDK_OK){
                    //   std::cout << "Get user profile all good" << std::endl;
                    // }
                    EE_Event_t eventType  = EE_EmoEngineEventGetType(eEvent);
                    std::cout << eventType << std::endl;


            if (eventType == EE_UserRemoved){
                uv_rwlock_wrlock(&obj->lock);
                obj->profileLoaded = 0;
                uv_rwlock_wrunlock(&obj->lock);

                std::cout << "the epoc dongle is disconnected, please reconnect" << std::endl;
            } else if (eventType == EE_UserAdded) {

               std::cout << "User Added" << std::endl;


                uv_rwlock_wrlock(&obj->lock);
                LoadUser(obj);
                uv_rwlock_wrunlock(&obj->lock);
            } else  if (eventType == EE_EmoStateUpdated){

               std::cout << "Emo State Updated" << std::endl;

                EE_EmoEngineEventGetEmoState(eEvent, eState);
                baton_t* baton = new baton_t();
                baton->obj = obj;
                baton->timestamp = ES_GetTimeFromStart(eState);

                uv_rwlock_wrlock(&obj->lock);
                EE_HeadsetGetGyroDelta(obj->userID, &baton->gyroX, &baton->gyroY);
                uv_rwlock_wrunlock(&obj->lock);

                baton->cog = static_cast<int>(ES_CognitivGetCurrentAction(eState));
                baton->cog_power = ES_CognitivGetCurrentActionPower(eState);
                baton->blink = ES_ExpressivIsBlink(eState);
                baton->winkLeft = ES_ExpressivIsLeftWink(eState);
                baton->winkRight = ES_ExpressivIsRightWink(eState);
                baton->lookLeft = ES_ExpressivIsLookingLeft(eState);
                baton->lookRight = ES_ExpressivIsLookingRight(eState);
                std::map<EE_ExpressivAlgo_t, float> expressivStates;
                EE_ExpressivAlgo_t upperFaceAction = ES_ExpressivGetUpperFaceAction(eState);
                float upperFacePower  = ES_ExpressivGetUpperFaceActionPower(eState);
                EE_ExpressivAlgo_t lowerFaceAction = ES_ExpressivGetLowerFaceAction(eState);
                float lowerFacePower  = ES_ExpressivGetLowerFaceActionPower(eState);
                expressivStates[ upperFaceAction ] = upperFacePower;
                expressivStates[ lowerFaceAction ] = lowerFacePower;
                baton->eyebrow= expressivStates[ EXP_EYEBROW ];
                baton->furrow= expressivStates[ EXP_FURROW ]; // furrow
                baton->smile= expressivStates[ EXP_SMILE ]; // smile

                // std::cout << baton->smile << std::endl;

                baton->clench= expressivStates[ EXP_CLENCH ]; // clench
                baton->smirkLeft= expressivStates[ EXP_SMIRK_LEFT  ]; // smirk left
                baton->smirkRight= expressivStates[ EXP_SMIRK_RIGHT ]; // smirk right
                baton->laugh= expressivStates[ EXP_LAUGH       ]; // laugh

                        // Affectiv Suite results
                baton->shortTermExcitement= ES_AffectivGetExcitementShortTermScore(eState);
                baton->longTermExcitement= ES_AffectivGetExcitementLongTermScore(eState);

                baton->engagementOrBoredom= ES_AffectivGetEngagementBoredomScore(eState);

                baton->update = true;

                         // create an event to throw on queue for processing..
                uv_work_t *req2 = new uv_work_t();
                req2->data = baton;

                int status = uv_queue_work(obj->loop, req2, process, (uv_after_work_cb)after_process);
                assert(status == 0);
             } else if(eventType == EE_EmulatorError){
              std::cout << "emulator error" << std::endl;
             } else if(eventType == EE_UnknownEvent){
              std::cout << "unknown event" << std::endl;
             } else if(eventType == EE_CognitivEvent){
              std::cout << "cognitive event" << std::endl;
             }
        }
        EE_EmoStateFree(eState);
        EE_EmoEngineEventFree(eEvent);

    } else {
        break;
    }
  }
}


void NodeEPOCDriver::Disconnect(const v8::FunctionCallbackInfo<Value>& args){
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
        REQ_FUN_ARG(0, dcb);

        NodeEPOCDriver* obj = ObjectWrap::Unwrap<NodeEPOCDriver>(args.This());
        uv_rwlock_rdlock(&obj->lock);
        bool connected = (obj->connected == 1);
        bool run = (obj->run ==1);
        uv_rwlock_rdunlock(&obj->lock);

        if (connected && run){
            uv_rwlock_wrlock(&obj->lock);
            if (obj->dcb != Null(isolate))
                obj->dcb.Reset();
    //         obj->dcb = Persistent<Function>::New(dcb);
            obj->run = 0;
            uv_rwlock_wrunlock(&obj->lock);

            std::cout << "disconnecting from epoc event loop" << std::endl;

        }
    //     return Undefined();
}

void NodeEPOCDriver::Rezero(const v8::FunctionCallbackInfo<Value>& args){
    NodeEPOCDriver* obj = ObjectWrap::Unwrap<NodeEPOCDriver>(args.This());

    uv_rwlock_wrlock(&obj->lock);
    bool connected = (obj->connected == 1);
    bool run = (obj->run ==1);

    std::cout << "am i in rezero??" << std::endl;

    if (connected && run){
        std::cout << "rezero the gryo" << std::endl;

        EE_HeadsetGyroRezero(obj->userID);
    }
    uv_rwlock_wrunlock(&obj->lock);
    // return Undefined();
}

