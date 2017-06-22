#include <nan.h>
#include "epoc.hpp"

void handleFacialExpressionEvent(std::ostream& , EmoEngineEventHandle );
void loadProfile(unsigned int userID);
void showTrainedActions(unsigned int userID);
void showCurrentActionPower(EmoStateHandle);
void showActionsFromComposer(EmoStateHandle);
int connect( bool connected);
void handleEpocEvents(int& connectionState, EmoEngineEventHandle eEvent, EmoStateHandle eState, int& epocState, unsigned int userID, epocutils::EpocHeadset_t user, v8::Local<v8::Function> callbackHandle);
void handleFacialExpressionsEvents(EmoStateHandle, v8::Local<v8::Object>, epocutils::EpocHeadset_t, v8::Local<v8::Function> callbackHandle);
void sendFacialExpressionsEventsToJs(v8::Local<v8::Object>, epocutils::EpocHeadset_t, v8::Local<v8::Function> callbackHandle);
void handleMentalCommandsEvent(v8::Local<v8::Object>, epocutils::EpocHeadset_t, EmoStateHandle, EmoEngineEventHandle);

int optionChosen;
int state = 0;
int epocState = 0;
int connectionState = -1;

IEE_MentalCommandAction_t previousAction;

std::string profileNameForLoading = ""; //"/Users/<username>/Library/Application Support/Emotiv/Profiles/<filename>.emu"

NAN_METHOD(Connect) {

  v8::Local<v8::Function> callbackHandle = info[0].As<v8::Function>();

  // init code for the Epoc headset
  EmoStateHandle       eState = IEE_EmoStateCreate();
  EmoEngineEventHandle eEvent = IEE_EmoEngineEventCreate();
  // DataHandle           hData = epocutils::createDataHandle();
  unsigned int         userID = 0;
  bool onStateChanged = false;
  bool                 connected = false;
  //epocutils::EpocHeadset         epocheadset;  // create a new 'EpocHeadset' struct that we'll update with the latest data using the 'epocutils::handleEvents()' function
  epocutils::EpocHeadset_t user;

  int batteryLevel = 0;
  int maxBatteryLevel = 0;

  IEE_SignalStrength_t wirelessStrength;

  int connectionState = connect(connected);

  if(connectionState != -1){
    while(true){
      handleEpocEvents(connectionState, eEvent, eState, epocState, userID, user, callbackHandle);

      // IEE_EngineDisconnect();
      // IEE_EmoStateFree(eState);
      // IEE_EmoEngineEventFree(eEvent);
    }
  }
}

int connect(bool connected){

  std::string input;
  std::cout << "===================================================================" << "\n";
  std::cout << "   Example to use MentalCommand for training with profile functions" << "\n";
  std::cout << "===================================================================" << "\n";
  std::cout << "1. Load profile and show MentalCommand actions		             " << "\n";
  std::cout << "2. Use EmoComposer                                     " << "\n";
  std::cout << ">> ";

  std::getline(std::cin, input, '\n');
  optionChosen = atoi(input.c_str());

  switch(optionChosen){
    case 1:
      if (IEE_EngineConnect() == EDK_OK){
        connected = true;
        std::cout << "Now connected to Epoc headset" << std::endl;
        return 0;
      } else {
        std::cout << "Could not connect to Epoc headset" << std::endl;
        return -1;
      }
      break;

    case 2:
      if ( IEE_EngineRemoteConnect("127.0.0.1", 1726) != EDK_OK) {
        std::string errMsg = "epocutils:: Cannot connect to EmoComposer on [127.0.0.1]:1726";
      } else {
        std::cout << "Connected to EmoComposer" << std::endl;
        connected = true;
        return 0;
      }
      break;
    default:
      break;
  }
  std::cout << "Option not available" << std::endl;
  std::cout << "Exit program" << std::endl;
  return -1;
}


void handleEpocEvents(int& connectionState, EmoEngineEventHandle eEvent, EmoStateHandle eState, int& epocState, unsigned int userID, epocutils::EpocHeadset_t user, v8::Local<v8::Function> callbackHandle){
  if(connectionState == 0){
    // epocState = IEE_EngineGetNextEvent(eEvent);
    v8::Local<v8::Object> event = Nan::New<v8::Object>();
    // std::cout << epocState << std::endl;
    // if(epocState == EDK_OK){

      if(optionChosen == 1){
        loadProfile(userID);
        showTrainedActions(userID);

        // IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
        // IEE_EmoEngineEventGetUserId(eEvent, &userID);
        //
        // if(eventType == IEE_EmoStateUpdated){
        //   IEE_EmoEngineEventGetEmoState(eEvent, eState);
        //   handleMentalCommandsEvent(event, user, eState, eEvent);
        //   handleFacialExpressionsEvents(eState, event, user, callbackHandle);
        // }
      } else if(optionChosen == 2){
        // IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
        // IEE_EmoEngineEventGetUserId(eEvent, &userID);
        //
        // if(eventType == IEE_EmoStateUpdated){
        //   IEE_EmoEngineEventGetEmoState(eEvent, eState);
        //   handleMentalCommandsEvent(event, user, eState, eEvent);
        //   handleFacialExpressionsEvents(eState, event, user, callbackHandle);
        // }

        int newEventRetrieved  = IEE_EngineGetNextEvent(eEvent);

        if(newEventRetrieved == EDK_OK){
          std::cout << "here" << std::endl;
          IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
          IEE_EmoEngineEventGetUserId(eEvent, &userID);

          if(eventType != IEE_UnknownEvent){

            std::cout << eventType << std::endl;

            if(eventType == IEE_EmoStateUpdated){
              IEE_EmoEngineEventGetEmoState(eEvent, eState);
              handleMentalCommandsEvent(event, user, eState, eEvent);
              handleFacialExpressionsEvents(eState, event, user, callbackHandle);
            }
          }
        }
      }

      v8::Local<v8::Value> parameters[1];
      parameters[0] = event;

      Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callbackHandle, 1, parameters);
    // }
  }
}

void handleFacialExpressionsEvents(EmoStateHandle eState, v8::Local<v8::Object> event, epocutils::EpocHeadset_t user, v8::Local<v8::Function> callbackHandle){
  user.isBlinking = IS_FacialExpressionIsBlink(eState);
  std::cout << user.isBlinking << std::endl;
  user.isWinkingLeft = IS_FacialExpressionIsLeftWink(eState);
  user.isWinkingRight = IS_FacialExpressionIsRightWink(eState);
  user.isLookingUp = IS_FacialExpressionIsLookingUp(eState);
  user.isLookingDown = IS_FacialExpressionIsLookingDown(eState);
  user.isLookingLeft = IS_FacialExpressionIsLookingLeft(eState);
  user.isLookingRight = IS_FacialExpressionIsLookingRight(eState);

  std::map<IEE_FacialExpressionAlgo_t, float> expressivStates;
  IEE_FacialExpressionAlgo_t upperFaceAction = IS_FacialExpressionGetUpperFaceAction(eState);
  float upperFacePower = IS_FacialExpressionGetUpperFaceActionPower(eState);
  IEE_FacialExpressionAlgo_t lowerFaceAction = IS_FacialExpressionGetLowerFaceAction(eState);
  float lowerFacePower = IS_FacialExpressionGetLowerFaceActionPower(eState);

  expressivStates[ upperFaceAction ] = upperFacePower;
  expressivStates[ lowerFaceAction ] = lowerFacePower;

  user.eyebrow = expressivStates[ FE_SURPRISE ];
  user.furrow = expressivStates[ FE_HORIEYE ];
  user.smile = expressivStates[ FE_SMILE ];
  user.clench = expressivStates[ FE_CLENCH ];
  user.smirkLeft = expressivStates[ FE_SMIRK_LEFT ];
  user.smirkRight = expressivStates[ FE_SMIRK_RIGHT ];
  user.laugh = expressivStates[ FE_LAUGH ];
  user.frown = expressivStates[ FE_FROWN ];

  sendFacialExpressionsEventsToJs(event, user, callbackHandle);
}

void sendFacialExpressionsEventsToJs(v8::Local<v8::Object> event, epocutils::EpocHeadset_t user, v8::Local<v8::Function> callbackHandle){
  Nan::Set(event, Nan::New("blink").ToLocalChecked(), Nan::New(user.isBlinking));
  Nan::Set(event, Nan::New("winkingLeft").ToLocalChecked(), Nan::New(user.isWinkingLeft));
  Nan::Set(event, Nan::New("winkingRight").ToLocalChecked(), Nan::New(user.isWinkingRight));
  Nan::Set(event, Nan::New("lookingUp").ToLocalChecked(), Nan::New(user.isLookingUp));
  Nan::Set(event, Nan::New("lookingDown").ToLocalChecked(), Nan::New(user.isLookingDown));
  Nan::Set(event, Nan::New("lookingLeft").ToLocalChecked(), Nan::New(user.isLookingLeft));
  Nan::Set(event, Nan::New("lookingRight").ToLocalChecked(), Nan::New(user.isLookingRight));

  Nan::Set(event, Nan::New("smile").ToLocalChecked(), Nan::New(user.smile));
  Nan::Set(event, Nan::New("smirkRight").ToLocalChecked(), Nan::New(user.smirkRight));
  Nan::Set(event, Nan::New("smirkLeft").ToLocalChecked(), Nan::New(user.smirkLeft));
  Nan::Set(event, Nan::New("laugh").ToLocalChecked(), Nan::New(user.laugh));
  Nan::Set(event, Nan::New("frown").ToLocalChecked(), Nan::New(user.frown));

  // v8::Local<v8::Value> parameters[1];
  // parameters[0] = event;
  //
  // Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callbackHandle, 1, parameters);
}

void handleMentalCommandsEvent(v8::Local<v8::Object> event, epocutils::EpocHeadset_t user, EmoStateHandle eState, EmoEngineEventHandle eEvent){

  IEE_EmoEngineEventGetEmoState(eEvent, eState);
  IEE_MentalCommandAction_t actionType = IS_MentalCommandGetCurrentAction(eState);
  float actionPower = IS_MentalCommandGetCurrentActionPower(eState);

  if(actionType == MC_NEUTRAL){
    // std::cout << "new mental command: neutral"  << std::endl;
    // std::cout << "power: " << static_cast<int>(actionPower*100.0f)  << std::endl;
  }

  if(actionType ==  MC_PUSH){
    // std::cout << "new mental command: push" << std::endl;
    // std::cout << "power: " << static_cast<int>(actionPower*100.0f)  << std::endl;
  }

  if(actionType & 0x0001){
    // std::cout << "new mental command: neutral"  << std::endl;
  }
  //

  // std::cout << actionType << std::endl;
  user.cognitivAction = actionType;
  user.cognitivActionPower = actionPower;
  Nan::Set(event, Nan::New("cognitivAction").ToLocalChecked(), Nan::New(user.cognitivAction));
  Nan::Set(event, Nan::New("cognitivActionPower").ToLocalChecked(), Nan::New(user.cognitivActionPower));
}

void loadProfile(unsigned int userID)
{
    if (IEE_LoadUserProfile(userID, profileNameForLoading.c_str()) == EDK_OK)
      std::cout << "Load Profile : done" << std::endl;
	  else
		 std::cout << "Can't load profile." << std::endl;
}

void showTrainedActions(unsigned int userID)
{
    unsigned long pTrainedActionsOut = 0;
    IEE_MentalCommandGetTrainedSignatureActions(userID, &pTrainedActionsOut);
    // std::cout << "Trained Actions: " << "\n";

    // pTrainedActionsOut is a bit vector.
    // It either returns an int like 39 for example and 'byte_to_binary(pTrainedActionsOut)' returns binary like 00000000100111.
    // both of these don't help check which actions have been trained
    // so I'm using bitwise operations.

    // Tested with a few for now, need to support all.

    // if(pTrainedActionsOut & 0x0002){
    //   std::cout << "* push" << std::endl;
    // }
    //
    // if(pTrainedActionsOut & 0x0004){
    //   std::cout << "* pull" << std::endl;
    // }
    //
    // if(pTrainedActionsOut & 0x0001){
    //   std::cout << "* neutral" << std::endl;
    // }
    //
    // if(pTrainedActionsOut & 0x0020){
    //   std::cout << "* left" << std::endl;
    // }
    //
    // if(pTrainedActionsOut & 0x2000){
    //   std::cout << "* Disappear" << std::endl;
    // }
}

void showCurrentActionPower(EmoStateHandle eState)
{
	IEE_MentalCommandAction_t eeAction = IS_MentalCommandGetCurrentAction(eState);
	float actionPower = IS_MentalCommandGetCurrentActionPower(eState);

	switch (eeAction)
	{
    case MC_NEUTRAL: { std::cout << "Neutral" << " : " << actionPower << "; \n"; break; }
    case MC_PUSH:    { std::cout << "Push" << " : " << actionPower << "; \n"; break; }
    case MC_PULL:    { std::cout << "Pull" << " : " << actionPower << "; \n"; break; }
    case MC_LIFT:    { std::cout << "Lift" << " : " << actionPower << "; \n"; break; }
    case MC_DROP:    { std::cout << "Drop" << " : " << actionPower << "; \n"; break; }
    case MC_LEFT:    { std::cout << "Left" << " : " << actionPower << "; \n"; break; }
    case MC_RIGHT:    { std::cout << "Right" << " : " << actionPower << "; \n"; break; }
    case MC_ROTATE_LEFT:    { std::cout << "Rotate left" << " : " << actionPower << "; \n"; break; }
    case MC_ROTATE_RIGHT:    { std::cout << "Rotate right" << " : " << actionPower << "; \n"; break; }
    case MC_ROTATE_CLOCKWISE:    { std::cout << "Rotate clockwise" << " : " << actionPower << "; \n"; break; }
    case MC_ROTATE_COUNTER_CLOCKWISE:    { std::cout << "Rotate counter clockwise" << " : " << actionPower << "; \n"; break; }
    case MC_ROTATE_FORWARDS:    { std::cout << "Rotate forwards" << " : " << actionPower << "; \n"; break; }
    case MC_ROTATE_REVERSE:    { std::cout << "Rotate reverse" << " : " << actionPower << "; \n"; break; }
    case MC_DISAPPEAR:    { std::cout << "Disappear" << " : " << actionPower << "; \n"; break; }
	}
}


NAN_METHOD(Connect);

using v8::FunctionTemplate;

NAN_MODULE_INIT(InitAll) {
  Nan::Set(target, Nan::New("connect").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(Connect)).ToLocalChecked());
}

NODE_MODULE(NativeExtension, InitAll)
