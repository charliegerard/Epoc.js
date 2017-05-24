/*  epocutils.h - header file
*
*   Emotiv Epoc Ubuntu SDK Dev Edition - namespaced utility header file - by StephaneAG 2013
*
*   supposed to be used:  1 -> as a utility class ( include header file in necessary .cpp file )
*                         2 -> as a utility class, embedded in a library that is available system wide ( .so file )
*
*/

/* includes the necessary standard stuff */
#include <iostream>

#include <map>

/* include the Emotiv-proprietary stuff */
#include "edk.h"
#include "edkErrorCode.h"
#include "EmoStateDLL.h"

/* we define what this header file makes available to file that include it */

#ifndef epocutils_h
#define epocutils_h

namespace epocutils
{

  /* the REAL functions & stuff of the 'epocuutils' helper */


  /* a 'EpocHeadset_struct' structure to hold all the stuff we're interested in */
  //typedef struct EpocHeadset_struct
  struct EpocHeadset_t
  {
    // bool to prevent unnecessary polling
    bool newDataToRead;

    // -- Epoc 'original' values -- // R: " #include <typeinfo> & then  std::cout << typeid( <the-var-name> ).name() << std::endl; ", but still not that easy to use ( ...)
    float time; // aka, returned a 'f' from the comment above in comments
    unsigned int userID;
    unsigned long pActiveActionsOut;
    int gyroY;
    int gyroX;
    // float xmax = 0;
    // double buffer;
    unsigned int bufferSizeInSample;
    int wirelessSignalStatus; // R: do a 'static_cast<int>(ES_GetWIrelessSignalStatus(eState))' as else the type will be 'EE_SignalStrength_t' ( enum )
    // Expressiv suite
    int isBlinking;
    int isWinkingLeft;
    int isWinkingRight;
    int isLookingLeft;
    int isLookingRight;
    int isLookingUp;
    int isLookingDown;
        // Digg the doc & ask Emotiv for the 'IsEyesOpen', 'IsLookingUp' & 'IsLookingDown' fcn ( ... )
    float eyebrow; // R: " expressivStates[ EXP_EYEBROW ] "
    float furrow;
    float smile;
    float clench;
    float smirkLeft;
    float smirkRight;
    float laugh;
    // Affectiv suite
    float shortTermExcitement;
    float longTermExcitement;
    float engagementBoredom;
          // Digg the doc & ask Emotiv for the 'Meditation' & 'Frustration'
    // Cognitiv suite
    int cognitivAction;
    float cognitiviActionConfidence;

    //Test mental command
    int isPushing;
    int isNeutral;

  //} EpocHeadset;
  };


  /* connect to the Epoc headset */
  int connect(bool& connected);

  /* disconnect from the Epoc headset ( WARNING: this function WILL NOT free the 'EmoStateHandle' neither the 'EmoEngineEventHandle' ! ) */
  void disconnect(bool& connected);

  /* disconnect from the Epoc headset AND clean up */
  void disconnect(bool& connected, EmoStateHandle& eState, EmoEngineEventHandle& eEvent);

  /* create an 'EmoEngineEventHandle' */
  EmoEngineEventHandle createEventHandle();

  /* create an 'EmoStateHandle' */
  EmoStateHandle createStateHandle();

  DataHandle createDataHandle();


  /* initialize the struct members ( I actually don't know if it's actually required, but it's always nice to init parameters before setting values to them anyway ( ... ) */
  //void initializeEpocHeadsetStruct(unsigned int& userID, EpocHeadset& epocheadset);
  void initializeEpocHeadsetStruct(unsigned int& userID, unsigned long& pActiveActionsOut, EpocHeadset_t& epocheadset, unsigned int bufferSizeInSample);

  /* To DO - 'createInitializedEpocHeadsetStruct'*/

  /* handle fresh data from the Epoc headset, if connected, & update the passed 'EpocHeadset_struct' structure with that data */
  //void handleEvents(bool& connected, int& epoc_state, EmoEngineEventHandle& eEvent, EmoStateHandle& eState, unsigned int& userID, EpocHeadset& epocheadset);
  void handleEvents(bool& connected, int& epoc_state, EmoEngineEventHandle& eEvent, EmoStateHandle& eState, unsigned int& userID, unsigned long& pActiveActionsOut, EpocHeadset_t& epocheadset, DataHandle hData, unsigned int& bufferSizeInSample);

  /* TO DO - even just to know how to do it - ~same fcn as above but accept a 'fcn&' as last argument, to wich it 'd pass a struct/class when done processing new events ( if any ) */

}

#endif // epocutils_h
