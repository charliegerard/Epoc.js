#include "epocutils.hpp"
/* we define the functions declared in the corresponding header file in the current implementation file */

/* the REAL functions & stuff of the 'epocutils' helper */
/* ---------------------------------------------------- */

/* connect to the Epoc headset */
int epocutils::connect(bool& connected){
  if ( EE_EngineConnect() == EDK_OK ){
    connected = true; // update the Epoc headset connection status
    std::cout << "epocutils:: Now connected to the Epoc headset." << std::endl; // inform that we're successfully connected
    return 0; // return 0 as no errors occured
  } else {
    std::cout << "epocutils:: An error occured while trying to connect to the Epoc headset." << std::endl; // inform that an error occured
    return 1; // return 1 as an error occured
  }
}

/* disconnect from the Epoc headset ( WARNING: this function WILL NOT free the 'EmoStateHandle' neither the 'EmoEngineEventHandle' ! ) */
void epocutils::disconnect(bool& connected){
  EE_EngineDisconnect(); // we disconnect from the EmoEngine
  connected = false; // we update the headset connection status ( a 'connected = false' will prevent new readings from the EmoEngine using 'epocutils::handleEvents()' )
  std::cout << "epocutils:: Now disconnected from the Epoc headset." << std::endl; // inform that an error occured
}

/* disconnect from the Epoc headset AND clean up */
void epocutils::disconnect(bool& connected, EmoStateHandle& eState, EmoEngineEventHandle& eEvent){
  EE_EngineDisconnect(); // we disconnect from the EmoEngine
  connected = false; // we update the headset connection status ( a 'connected = false' will prevent new readings from the EmoEngine using 'epocutils::handleEvents()' )
  std::cout << "epocutils:: Now disconnected from the Epoc headset." << std::endl; // inform that an error occured
  EE_EmoStateFree(eState); // free the 'EmoStateHandle' instance
  std::cout << "epocutils:: EmoStateHandle resources freed." << std::endl; // inform that the EmoStateHandle instance has been freed
  EE_EmoEngineEventFree(eEvent); // free the 'EmoEngineEventHandle' instance
  std::cout << "epocutils:: EmoEngineEventHandle resources freed." << std::endl; // inform that the EmoEngineEventHandle instance has been freed
}

/* create an 'EmoEngineEventHandle' */
EmoEngineEventHandle epocutils::createEventHandle(){
  return EE_EmoEngineEventCreate(); // simply return the original fcn, wrapped
}

 /* create an 'EmoStateHandle' */
EmoStateHandle epocutils::createStateHandle(){
  return EE_EmoStateCreate(); // simply return the original fcn, wrapped
}

DataHandle epocutils::createDataHandle(){
  return EE_DataCreate(); // simply return the original fcn, wrapped
}

/*  initialize the struct members */
//void initializeEpocHeadsetStruct(unsigned int& userID, epocutils::EpocHeadset& epocheadset)
//void initializeEpocHeadsetStruct(unsigned int& userID, epocutils::EpocHeadset_struct& epocheadset)
void epocutils::initializeEpocHeadsetStruct(unsigned int& userID, epocutils::EpocHeadset_t& epocheadset, unsigned int bufferSizeInSample){
  // we init the bool that we'll use to know if we have unread data from the Epoc headset
  epocheadset.newDataToRead = false;
  // we initialize the EpocHeadset struct with all its member parameters to 0, except the userID wich equals the one passed as argument
  epocheadset.time = 0.0f;
  epocheadset.userID = userID;
  epocheadset.wirelessSignalStatus = 0;
  // Expressiv suite
  epocheadset.isBlinking = 0;
  epocheadset.isWinkingLeft = 0;
  epocheadset.isWinkingRight = 0;
  epocheadset.isLookingLeft = 0;
  epocheadset.isLookingRight = 0;
  epocheadset.isLookingUp = 0;
  epocheadset.isLookingDown = 0;
  epocheadset.eyebrow = 0.0f;
  epocheadset.furrow = 0.0f;
  epocheadset.smile = 0.0f;
  epocheadset.clench = 0.0f;
  epocheadset.smirkLeft = 0.0f;
  epocheadset.smirkRight = 0.0f;
  epocheadset.laugh = 0.0f;
  // Affectiv suite
  epocheadset.shortTermExcitement = 0.0f;
  epocheadset.longTermExcitement = 0.0f;
  epocheadset.engagementBoredom = 0.0f;
  // Cognitiv suite
  epocheadset.cognitivAction = 0;
  epocheadset.cognitiviActionConfidence = 0; // close, but not the same as 'power' ( YES, I DISAGREE with Emotiv's words on this ( ... )
  epocheadset.bufferSizeInSample = 1;
  epocheadset.gyroX;
  epocheadset.gyroY;

  std::cout << "epocutils:: Epoc headset struct initialized." << std::endl; // inform that the EpocHeadset struct has been initialized
}

/* handle fresh data from the Epoc headset, if connected, & update the passed 'EpocHeadset_struct' structure with that data */
//void epocutils::handleEvents(bool& connected, int& epoc_state, EmoEngineEventHandle& eEvent, EmoStateHandle& eState, unsigned int& userID, epocutils::EpocHeadset& epocheadset)
void epocutils::handleEvents(bool& connected, int& epoc_state, EmoEngineEventHandle& eEvent, EmoStateHandle& eState, unsigned int& userID, epocutils::EpocHeadset_t& epocheadset, DataHandle hData, unsigned int& bufferSizeInSample){
  hData = EE_DataCreate();
  EE_DataSetBufferSizeInSec(1);

  if ( connected ){
    epoc_state = EE_EngineGetNextEvent(eEvent); // get the latest EmoEngine events ( aka 'stuff from Epoc' )

    if (epoc_state == EDK_OK){
      EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
      EE_EmoEngineEventGetUserId(eEvent, &userID);
      EE_DataAcquisitionEnable(userID, true);

      // Log the EmoState if it has been updated
      if (eventType == EE_EmoStateUpdated){
      	// std::cout << "epocutils:: New data from the Epoc headset." << std::endl; // inform that new data is available
              EE_EmoEngineEventGetEmoState(eEvent, eState);
      	const float timestamp = ES_GetTimeFromStart(eState);
      	// std::cout<<"epocutils:: Timestamp: " << timestamp <<" New EmoState from user: " << userID << std::endl;

      	// write data from the EmoEngine to the EpocHeadset struct ... or just log it to stdout for the moment ?
      	epocheadset.time = ES_GetTimeFromStart(eState);
      	epocheadset.userID = userID;
      	epocheadset.wirelessSignalStatus = static_cast<int>(ES_GetWirelessSignalStatus(eState));

      /////////////////////////////////////////

        EE_DataUpdateHandle(0, hData);
        unsigned int bufferSizeInSample=1;
        EE_DataGetNumberOfSample(hData,&bufferSizeInSample);

        double* buffer = new double[bufferSizeInSample];
        EE_DataGet(hData, ED_GYROX, buffer, bufferSizeInSample);
        EE_DataGet(hData, ED_GYROY, buffer, bufferSizeInSample);

        int gyroX;
        int gyroY;
        //Test with that
        // EE_HeadsetGyroRezero(userID);

        if(EE_HeadsetGetGyroDelta(userID, &gyroX, &gyroY) == EDK_OK){
          epocheadset.gyroX = gyroX;
          epocheadset.gyroY = gyroY;
        }

      ////////////////////////////////////////////

      	// Expressiv suite
      	epocheadset.isBlinking = ES_ExpressivIsBlink(eState);
      	epocheadset.isWinkingLeft = ES_ExpressivIsLeftWink(eState);
      	epocheadset.isWinkingRight = ES_ExpressivIsRightWink(eState);
      	epocheadset.isLookingLeft = ES_ExpressivIsLookingLeft(eState);
      	epocheadset.isLookingRight = ES_ExpressivIsLookingRight(eState);
        epocheadset.isLookingUp = ES_ExpressivIsLookingUp(eState);
        epocheadset.isLookingDown = ES_ExpressivIsLookingDown(eState);

      	std::map<EE_ExpressivAlgo_t, float> expressivStates;
      	EE_ExpressivAlgo_t upperFaceAction = ES_ExpressivGetUpperFaceAction(eState);
      	float upperFacePower = ES_ExpressivGetUpperFaceActionPower(eState);
      	EE_ExpressivAlgo_t lowerFaceAction = ES_ExpressivGetLowerFaceAction(eState);
      	float lowerFacePower = ES_ExpressivGetLowerFaceActionPower(eState);

      	expressivStates[ upperFaceAction ] = upperFacePower;
      	expressivStates[ lowerFaceAction ] = lowerFacePower;

      	epocheadset.eyebrow = expressivStates[ EXP_EYEBROW ];
      	epocheadset.furrow = expressivStates[ EXP_FURROW ];
      	epocheadset.smile = expressivStates[ EXP_SMILE ];
      	epocheadset.clench = expressivStates[ EXP_CLENCH ];
      	epocheadset.smirkLeft = expressivStates[ EXP_SMIRK_LEFT ];
      	epocheadset.smirkRight = expressivStates[ EXP_SMIRK_RIGHT ];
      	epocheadset.laugh = expressivStates[ EXP_LAUGH ];

      	// Affective suite
      	epocheadset.shortTermExcitement = ES_AffectivGetExcitementShortTermScore(eState);
      	epocheadset.longTermExcitement = ES_AffectivGetExcitementLongTermScore(eState);
      	epocheadset.engagementBoredom = ES_AffectivGetEngagementBoredomScore(eState);

      	// Cognitiv suite
      	epocheadset.cognitivAction = static_cast<int>(ES_CognitivGetCurrentAction(eState));

      	epocheadset.cognitiviActionConfidence = ES_CognitivGetCurrentActionPower(eState);

      	epocheadset.newDataToRead = true; // we update our boolean ot indicate that data is yet to be read

      }

    } else if (epoc_state != EDK_NO_EVENT){
      std::cout << "epocutils:: No new data from the Epoc headset:" << std::endl; // inform that new data is available
      std::cout << "epocutils:: Internal error in Emotiv Engine!" << std::endl;
      // handle that :/
    }
  }
}
