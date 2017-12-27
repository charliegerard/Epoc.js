#include <nan.h>
#include "epoc_utils.hpp"
#include <node.h>

void ConnectToLiveData(const Nan::FunctionCallbackInfo<Value>& info){
  if(info.Length() < 2){
    return Nan::ThrowSyntaxError("wrong number of arguments");
  }
  Local<Function> callbackHandle = info[1].As<Function>();

  v8::String::Utf8Value param1(info[0]->ToString());
  epocutils::pathToProfileFile = string(*param1);

  epocutils::dataOption = 1;
  epocutils::connectionState = epocutils::getConnectionState(epocutils::dataOption);

  if(epocutils::connectionState != -1){
    while(true){
      epocutils::handleEpocEvents(epocutils::dataOption, epocutils::connectionState, epocutils::eEvent, epocutils::eState, epocutils::epocState, epocutils::userID, epocutils::user, callbackHandle);
    }
  }
}

void ConnectToEmoComposer(const Nan::FunctionCallbackInfo<Value>& info){
  if(info.Length() > 1){
    return Nan::ThrowSyntaxError("wrong number of arguments");
  }

  Local<Function> callbackHandle = info[0].As<Function>();

  epocutils::dataOption = 2;
  epocutils::connectionState = epocutils::getConnectionState(epocutils::dataOption);

  if(epocutils::connectionState != -1){
    while(true){
      epocutils::handleEpocEvents(epocutils::dataOption, epocutils::connectionState, epocutils::eEvent, epocutils::eState, epocutils::epocState, epocutils::userID, epocutils::user, callbackHandle);
    }
  }
}

int epocutils::getConnectionState(int optionChosen){
  switch(optionChosen){
    case 1:
      if (IEE_EngineConnect("Emotiv Systems-5") != EDK_OK){
        cout << "Could not connect to EmoEngine" << endl;
        return -1;
      } else {
        cout << "Connected to EmoEngine" << endl;
        return 0;
      }
      break;

    case 2:
      if ( IEE_EngineRemoteConnect("127.0.0.1", 1726) != EDK_OK) {
        cout <<  "Cannot connect to EmoComposer on [127.0.0.1]:1726" << endl;
        return -1;
      } else {
        cout << "Connected to EmoComposer" << endl;
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

void epocutils::handleEpocEvents(int dataOption, int& connectionState, EmoEngineEventHandle eEvent, EmoStateHandle eState, int& epocState, unsigned int userID, epocutils::EpocHeadset_t user, Local<Function> callbackHandle){
  if(connectionState == 0){
    epocState = IEE_EngineGetNextEvent(eEvent);
    Local<Object> event = Nan::New<Object>();

    if(epocState == EDK_OK){
      IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
      IEE_EmoEngineEventGetUserId(eEvent, &userID);

      if(eventType == IEE_UserAdded){
        cout << "Connected to Epoc / Insight headset" << endl;
      } else if(eventType == IEE_UserRemoved){
        cout << "Disconnected from Epoc / Insight headset" << endl;
        IEE_DisconnectDevice();
        IEE_EngineDisconnect();
      }

      if(dataOption == 1){
        if(!profileLoaded){
          epocutils::loadProfile(userID);
          epocutils::showTrainedActions(userID);
        }
      }

      if(eventType == IEE_EmoStateUpdated){
        epocutils::getGyroData(event, 0);
        IEE_EmoEngineEventGetEmoState(eEvent, eState);
        epocutils::handleMentalCommandsEvent(event, user, eState, eEvent);
        epocutils::handleFacialExpressionsEvents(eState, event, user, callbackHandle);
      }

      Local<Value> parameters[1];
      parameters[0] = event;

      Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callbackHandle, 1, parameters);
    }
  }
}

void epocutils::loadProfile(unsigned int userID){
  if (IEE_LoadUserProfile(userID, epocutils::pathToProfileFile.c_str()) == EDK_OK){
    cout << "User profile loaded" << endl;
    profileLoaded = true;
  } else {
    if(!profileNotLoadedIndicated){
      cout << "Can't load profile." << endl;
    }
    profileLoaded = false;
    profileNotLoadedIndicated = true;
  }
}

void epocutils::showTrainedActions(unsigned int userID){
  unsigned long pTrainedActionsOut = 0;
  IEE_MentalCommandGetTrainedSignatureActions(userID, &pTrainedActionsOut);
  //
  // if(pTrainedActionsOut & 0x0002){
  //   std::cout << "* push" << std::endl;
  // }
}

void epocutils::getGyroData(Local<Object> event, unsigned int userID){
  IEE_HeadsetGetGyroDelta(userID, &gyroX, &gyroY);

  xmax += gyroX;
  ymax += gyroY;

  epocutils::sendGyroDataToJs(event, xmax, ymax);
}

void epocutils::sendGyroDataToJs(Local<Object> event, int xPos, int yPos){
  Nan::Set(event, Nan::New("gyroX").ToLocalChecked(), Nan::New(xPos));
  Nan::Set(event, Nan::New("gyroY").ToLocalChecked(), Nan::New(yPos));
}

void epocutils::handleMentalCommandsEvent(Local<Object> event, epocutils::EpocHeadset_t user, EmoStateHandle eState, EmoEngineEventHandle eEvent){
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

void epocutils::handleFacialExpressionsEvents(EmoStateHandle eState, Local<Object> event, epocutils::EpocHeadset_t user, Local<Function> callbackHandle){
  user.isBlinking = IS_FacialExpressionIsBlink(eState);
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

  epocutils::sendFacialExpressionsEventsToJs(event, user, callbackHandle);
}

void epocutils::sendFacialExpressionsEventsToJs(Local<Object> event, epocutils::EpocHeadset_t user, Local<Function> callbackHandle){
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
}

void epocutils::showCurrentActionPower(EmoStateHandle eState)
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

void Init(Local<Object> exports){
  exports->Set(Nan::New("connectToLiveData").ToLocalChecked(),
               Nan::New<FunctionTemplate>(ConnectToLiveData)->GetFunction());

  exports->Set(Nan::New("connectToEmoComposer").ToLocalChecked(),
               Nan::New<FunctionTemplate>(ConnectToEmoComposer)->GetFunction());
}

NODE_MODULE(index, Init);
