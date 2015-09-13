#include "Functions.h"
#include "epocutils.hpp"

NAN_METHOD(Connect) {

  v8::Local<v8::Function> callbackHandle = info[0].As<v8::Function>();

  // init code for the Epoc headset
  EmoEngineEventHandle eEvent = epocutils::createEventHandle(); // create an 'EmoEngineEventHandle' using the 'epocutils' helper
  EmoStateHandle       eState = epocutils::createStateHandle(); // create an 'EmoStateHandle' using the 'epocutils' helper
  unsigned int         userID = 0; // create a user ID
  int                  epoc_state = 0; // create a int variable that'll hold the return codes from the EmoEngine ( EDK_OK, etc .. )
  bool                 connected = false; // create a boolean that'll be used to gather data from the Epoc headset if true ( must be set to 'true' to handle new events with 'epocutils::handleEvents()' )
  //epocutils::EpocHeadset         epocheadset;  // create a new 'EpocHeadset' struct that we'll update with the latest data using the 'epocutils::handleEvents()' function
  //epocutils::EpocHeadset_struct   epocheadset2;
  epocutils::EpocHeadset_t billy;

  int eventsToHandleBeforeQuit = 5;
  int handledEvents = 0;

  epocutils::initializeEpocHeadsetStruct( userID, billy);

  // connection to the Epoc headset
  int epoc_conn_retval = epocutils::connect(connected); // try to connect to the Epoc headset
  if ( epoc_conn_retval != 0 )
  {
    std::cout << "Failed to connect to the Epoc headset." << std::endl;
    std::cout << "Program says: I'm of none use if no Epoc headset if connected ! -> Exiting." << std::endl;
    //exit 1; // for certain programs that are pretty useless without the Epoc headset ( for games, we can still use the mouse & keyboard or some Arduino-made joystick ;p )
  } else
  {
    std::cout << "Connected to the Epoc headset." << std::endl;
  }

  while (handledEvents < eventsToHandleBeforeQuit) {

    // handle the SDL events & other stuff like that
    // Nb: if we need to update the position or parameters of stuff, we'll do so just after getting stuff from the Epoc headset

    // handle fresh data from the Epoc headset if it is connected
    epocutils::handleEvents(connected, epoc_state, eEvent, eState, userID, billy); // the latest data from the Epoc headset 'll update the 'epocheadset' struct content
    // check if new data is available

    //std::cout << "billy time: " << billy.time << " UserID: " << billy.userID << std::endl;
    std::cout << "\n\n-----------------------------------------------------------------" << std::endl;
    std::cout << "-Epoc Headset infos-------------------" << std::endl;
    std::cout << "Time" << "\t  UserID" << "\t  Signal" << std::endl;
    std::cout << billy.time << "\t  " << billy.userID << "\t  " << billy.wirelessSignalStatus << std::endl;
    std::cout << "-Expressiv suite----------------------" << std::endl;
    std::cout << "Blink" << "        WinkL" << "        WinkR" << "        LookL" << "        LookR" << "        Eyebrow" << "        Furrow" << std::endl;
    std::cout << billy.isBlinking << "        " << billy.isWinkingLeft << "        " << billy.isWinkingRight << "        "
          << billy.isLookingLeft << "        " << billy.isLookingRight << "        " << billy.eyebrow << "        " << billy.furrow << std::endl;
    std::cout << "Smile" << "        Clench" << "        SmirkL" << "        SmirkR" << "        Laugh" << std::endl;
    std::cout << billy.smile << "        " << billy.clench << "        "
          << billy.smirkLeft << "        " << billy.smirkRight << "        " <<billy.laugh << std::endl;
    std::cout << "-Affectiv suite-----------------------" << std::endl;
    std::cout << "Short-T ext" << "\t  Long-T ext" << "\t  Eng/Boredom" << std::endl;
    std::cout << billy.shortTermExcitement << "\t  " << billy.longTermExcitement << "\t  " << billy.engagementBoredom << std::endl;
    std::cout << "-Cognitiv suite-----------------------" << std::endl;
    std::cout << "Cog action" << "\t  Cog action conf" << std::endl;
    std::cout << billy.cogntivAction << "\t  " << billy.cogntiviActionConfidence << std::endl;
    std::cout << "-----------------------------------------------------------------\n\n\n" << std::endl;

    billy.newDataToRead = false; // set the 'newDataToRead' bool to false to prevent unnecessary polling ( ... )

    v8::Local<v8::Object> event = Nan::New<v8::Object>();
    Nan::Set(event, Nan::New("newData").ToLocalChecked(), Nan::New(billy.newDataToRead));
    Nan::Set(event, Nan::New("wirelessSignalStatus").ToLocalChecked(), Nan::New(billy.wirelessSignalStatus));
    Nan::Set(event, Nan::New("time").ToLocalChecked(), Nan::New(billy.time));
    Nan::Set(event, Nan::New("blink").ToLocalChecked(), Nan::New(billy.isBlinking));

    v8::Local<v8::Value> parameters[1];
    parameters[0] = event;

    Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callbackHandle, 1, parameters);

    handledEvents += 1;

    // render ( updated ) SDL stuff & other things like that
  }

  // free all the resources used by the Epoc headset
  epocutils::disconnect(connected, eState, eEvent); // disconnect from the Epoc headset & free all the resources involved (bool to 'false', EmoEngine resources freed ( gone ^^! ) )
}