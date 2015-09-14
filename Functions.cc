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
  epocutils::EpocHeadset_t user;

  int eventsToHandleBeforeQuit = 5;
  int handledEvents = 0;

  epocutils::initializeEpocHeadsetStruct( userID, user);

  // connection to the Epoc headset
  int epoc_conn_retval = epocutils::connect(connected); // try to connect to the Epoc headset
  if ( epoc_conn_retval != 0 ) {
    std::cout << "Failed to connect to the Epoc headset." << std::endl;
    std::cout << "Program says: I'm of none use if no Epoc headset if connected ! -> Exiting." << std::endl;
    //exit 1; // for certain programs that are pretty useless without the Epoc headset ( for games, we can still use the mouse & keyboard or some Arduino-made joystick ;p )
  } else {
    std::cout << "Connected to the Epoc headset." << std::endl;
  }

  while (handledEvents < eventsToHandleBeforeQuit) {

    // handle the SDL events & other stuff like that
    // Nb: if we need to update the position or parameters of stuff, we'll do so just after getting stuff from the Epoc headset

    // handle fresh data from the Epoc headset if it is connected
    epocutils::handleEvents(connected, epoc_state, eEvent, eState, userID, user); // the latest data from the Epoc headset 'll update the 'epocheadset' struct content
    // check if new data is available

    //std::cout << "user time: " << user.time << " UserID: " << user.userID << std::endl;
    std::cout << "\n\n-----------------------------------------------------------------" << std::endl;
    std::cout << "-Epoc Headset infos-------------------" << std::endl;
    std::cout << "Time" << "\t  UserID" << "\t  Signal" << std::endl;
    std::cout << user.time << "\t  " << user.userID << "\t  " << user.wirelessSignalStatus << std::endl;
    std::cout << "-Expressiv suite----------------------" << std::endl;
    std::cout << "Blink" << "        WinkL" << "        WinkR" << "        LookL" << "        LookR" << "        Eyebrow" << "        Furrow" << std::endl;
    std::cout << user.isBlinking << "        " << user.isWinkingLeft << "        " << user.isWinkingRight << "        "
          << user.isLookingLeft << "        " << user.isLookingRight << "        " << user.eyebrow << "        " << user.furrow << std::endl;
    std::cout << "Smile" << "        Clench" << "        SmirkL" << "        SmirkR" << "        Laugh" << std::endl;
    std::cout << user.smile << "        " << user.clench << "        "
          << user.smirkLeft << "        " << user.smirkRight << "        " <<user.laugh << std::endl;
    std::cout << "-Affectiv suite-----------------------" << std::endl;
    std::cout << "Short-T ext" << "\t  Long-T ext" << "\t  Eng/Boredom" << std::endl;
    std::cout << user.shortTermExcitement << "\t  " << user.longTermExcitement << "\t  " << user.engagementBoredom << std::endl;
    std::cout << "-Cognitiv suite-----------------------" << std::endl;
    std::cout << "Cog action" << "\t  Cog action conf" << std::endl;
    std::cout << user.cognitivAction << "\t  " << user.cognitiviActionConfidence << std::endl;
    std::cout << "-----------------------------------------------------------------\n\n\n" << std::endl;

    user.newDataToRead = false; // set the 'newDataToRead' bool to false to prevent unnecessary polling ( ... )

    v8::Local<v8::Object> event = Nan::New<v8::Object>();
    Nan::Set(event, Nan::New("newData").ToLocalChecked(), Nan::New(user.newDataToRead));
    Nan::Set(event, Nan::New("wirelessSignalStatus").ToLocalChecked(), Nan::New(user.wirelessSignalStatus));
    Nan::Set(event, Nan::New("time").ToLocalChecked(), Nan::New(user.time));
    Nan::Set(event, Nan::New("blink").ToLocalChecked(), Nan::New(user.isBlinking));
    Nan::Set(event, Nan::New("winkingLeft").ToLocalChecked(), Nan::New(user.isWinkingLeft));
    Nan::Set(event, Nan::New("winkingRight").ToLocalChecked(), Nan::New(user.isWinkingRight));
    Nan::Set(event, Nan::New("lookingLeft").ToLocalChecked(), Nan::New(user.isLookingLeft));
    Nan::Set(event, Nan::New("lookingRight").ToLocalChecked(), Nan::New(user.isLookingRight));
    Nan::Set(event, Nan::New("smile").ToLocalChecked(), Nan::New(user.smile));
    Nan::Set(event, Nan::New("smirkRight").ToLocalChecked(), Nan::New(user.smirkRight));
    Nan::Set(event, Nan::New("smirkLeft").ToLocalChecked(), Nan::New(user.smirkLeft));
    Nan::Set(event, Nan::New("laugh").ToLocalChecked(), Nan::New(user.laugh));
    Nan::Set(event, Nan::New("cognitivAction").ToLocalChecked(), Nan::New(user.cognitivAction));

    v8::Local<v8::Value> parameters[1];
    parameters[0] = event;

    Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callbackHandle, 1, parameters);

    handledEvents += 1;

    // render ( updated ) SDL stuff & other things like that
  }

  // free all the resources used by the Epoc headset
  epocutils::disconnect(connected, eState, eEvent); // disconnect from the Epoc headset & free all the resources involved (bool to 'false', EmoEngine resources freed ( gone ^^! ) )
}