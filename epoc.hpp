#include <iostream>
#include <map>

/* include the Emotiv files from the SDK */
#include "lib/IEmoStateDLL.h"
#include "lib/Iedk.h"
#include "lib/IedkErrorCode.h"
#include "lib/IEegData.h"
#include "lib/IEmoStatePerformanceMetric.h"
#include "lib/IEmotivProfile.h"

#ifndef epocutils_h
#define epocutils_h

namespace epocutils
{
  /* a 'EpocHeadset_struct' structure to hold all the stuff we're interested in */
  //typedef struct EpocHeadset_struct
  struct EpocHeadset_t
  {
    // bool to prevent unnecessary polling
    bool newDataToRead;
    float time;
    unsigned int userID;
    unsigned long pActiveActionsOut;

    unsigned int bufferSizeInSample;
    int wirelessSignalStatus;
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

  /* connect to the Epoc headset */
  int connect(bool& connected);

  /* disconnect from the Epoc headset ( WARNING: this function WILL NOT free the 'EmoStateHandle' neither the 'EmoEngineEventHandle' ! ) */
  void disconnect(bool& connected);

  /* disconnect from the Epoc headset AND clean up */
  void disconnect(bool& connected, EmoStateHandle& eState, EmoEngineEventHandle& eEvent);

  EmoEngineEventHandle createEventHandle();

  EmoStateHandle createStateHandle();

  DataHandle createDataHandle();

  void initializeEpocHeadsetStruct(unsigned int& userID, unsigned long& pActiveActionsOut, EpocHeadset_t& epocheadset, unsigned int bufferSizeInSample);

  void handleEvents(bool& connected, int& epoc_state, EmoEngineEventHandle& eEvent, EmoStateHandle& eState, unsigned int& userID, unsigned long& pActiveActionsOut, EpocHeadset_t& epocheadset, unsigned int& bufferSizeInSample);
}

#endif // epocutils_h
