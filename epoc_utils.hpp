#include <iostream>
#include <map>
#include <v8.h>

/* include the Emotiv files from the SDK */
#include "IEmoStateDLL.h"
#include "Iedk.h"
#include "IedkErrorCode.h"
#include "IEegData.h"
#include "IEmoStatePerformanceMetric.h"
#include "FacialExpressionDetection.h"
#include "lib/includes/IEmotivProfile.h"

using namespace v8;
using namespace std;

#ifndef epocutils_h
#define epocutils_h

namespace epocutils{
    int optionChosen;
    EmoStateHandle       eState = IEE_EmoStateCreate();
    EmoEngineEventHandle eEvent = IEE_EmoEngineEventCreate();
    int epocState = 0;
    int connectionState = -1;
    unsigned int userID = 0;
    int dataOption;
    int xmax, ymax, gyroX, gyroY = 0;
    bool profileLoaded = false;
    bool profileNotLoadedIndicated = false;
    string pathToProfileFile;

    int getConnectionState(int optionChosen);
    void getGyroData(Local<Object> event, unsigned int userID);
    void sendGyroDataToJs(Local<Object> event, int xPos, int yPos);
    void loadProfile(unsigned int userID);
    void showTrainedActions(unsigned int userID);

    struct EpocHeadset_t{
      // Expressiv suite
      int isBlinking;
      int isWinkingLeft;
      int isWinkingRight;
      int isLookingLeft;
      int isLookingRight;
      int isLookingUp;
      int isLookingDown;

      float eyebrow;
      float furrow;
      float smile;
      float clench;
      float smirkLeft;
      float smirkRight;
      float laugh;
      float frown;

      // Affectiv suite
      float shortTermExcitement;
      float longTermExcitement;
      float engagementBoredom;

      // Cognitiv suite
      int cognitivAction;
      float cognitivActionPower;
    };

    EpocHeadset_t user;

    void handleEpocEvents(int dataOption, int& connectionState, EmoEngineEventHandle eEvent, EmoStateHandle eState, int& epocState, unsigned int userID, EpocHeadset_t user, Local<Function> callbackHandle, Local<Object> event);
    void handleFacialExpressionsEvents(EmoStateHandle eState, Local<Object> event, EpocHeadset_t user);
    void sendFacialExpressionsEventsToJs(Local<Object> event, EpocHeadset_t user);
    void handleMentalCommandsEvent(Local<Object> event, EpocHeadset_t user, EmoStateHandle eState, EmoEngineEventHandle eEvent);
    void showCurrentActionPower(EmoStateHandle eState);

    void checkEventType(IEE_Event_t eventType, EmoEngineEventHandle eEvent, EmoStateHandle eState, Local<Object> event);

};

#endif // epocutils_h
