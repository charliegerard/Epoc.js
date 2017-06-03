#ifndef EXPRESSIVDETECTION_H
#define EXPRESSIVDETECTION_H

#ifdef __cplusplus
extern "C" {
#endif

#if (!EDK_STATIC_LIB)
#   ifdef EDK_EXPORTS
#       ifdef _WIN32
#           define EDK_API __declspec(dllexport)
#       else
#           if (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER || defined __clang__
#               define EDK_API __attribute__ ((visibility("default")))
#           else
#               define EDK_API
#           endif
#       endif
#   else
#       ifdef _WIN32
#           define EDK_API __declspec(dllimport)
#       else
#           define EDK_API
#       endif
#   endif
#else
#   define EDK_API extern
#endif

#include "EmoStateDLL.h"

    //! Handle to internal event structure allocated by EE_EmoEngineEventCreate()
    typedef void * EmoEngineEventHandle;


    //! Expressiv Suite training control enumerator
    typedef enum EE_ExpressivTrainingControl_enum {
        EXP_NONE = 0,
        EXP_START,
        EXP_ACCEPT,
        EXP_REJECT,
        EXP_ERASE,
        EXP_RESET
    } EE_ExpressivTrainingControl_t;


    //! Expressiv Suite signature type enumerator
    typedef enum EE_ExpressivSignature_enum {
        EXP_SIG_UNIVERSAL = 0,
        EXP_SIG_TRAINED
    } EE_ExpressivSignature_t;


    //! Expressiv Suite threshold type enumerator
    typedef enum EE_ExpressivThreshold_enum {
        EXP_SENSITIVITY
    } EE_ExpressivThreshold_t;


    //! Expressiv-specific event types
    typedef enum EE_ExpressivEvent_enum {
        EE_ExpressivNoEvent = 0,
        EE_ExpressivTrainingStarted,
        EE_ExpressivTrainingSucceeded,
        EE_ExpressivTrainingFailed,
        EE_ExpressivTrainingCompleted,
        EE_ExpressivTrainingDataErased,
        EE_ExpressivTrainingRejected,
        EE_ExpressivTrainingReset
    } EE_ExpressivEvent_t;


    //! Returns the Expressiv-specific event type for an EE_ExpressivEvent event already retrieved using EE_EngineGetNextEvent.
    /*!
        \param hEvent - a handle returned by EE_EmoEngineEventCreate()

        \return EE_ExpressivEvent_t
    */
    EDK_API EE_ExpressivEvent_t
        EE_ExpressivEventGetType(EmoEngineEventHandle hEvent);


    //! Set threshold for Expressiv algorithms
    /*!
        \param userId - user ID
        \param algoName - Expressiv algorithm type
        \param thresholdName - Expressiv threshold type
        \param value - threshold value (min: 0 max: 1000)

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h, EE_ExpressivAlgo_t, EE_ExpressivThreshold_t
    */
    EDK_API int
        EE_ExpressivSetThreshold(unsigned int userId, EE_ExpressivAlgo_t algoName,
                                 EE_ExpressivThreshold_t thresholdName, int value);


    //! Get threshold from Expressiv algorithms
    /*!
        \param userId - user ID
        \param algoName - Expressiv algorithm type
        \param thresholdName - Expressiv threshold type
        \param pValueOut - receives threshold value

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h, EE_ExpressivAlgo_t, EE_ExpressivThreshold_t
    */
    EDK_API int
        EE_ExpressivGetThreshold(unsigned int userId, EE_ExpressivAlgo_t algoName,
                                 EE_ExpressivThreshold_t thresholdName, int * pValueOut);


    //! Set the current facial expression for Expressiv training
    /*!
        Blocking call

        \param userId - user ID
        \param action - which facial expression would like to be trained

        \return EDK_ERROR_CODE - current status of EmoEngine. If the query is successful, EDK_ERROR_CODE = OK.

        \sa edkErrorCode.h, EE_ExpressivAlgo_t
    */
    EDK_API int
        EE_ExpressivSetTrainingAction(unsigned int userId, EE_ExpressivAlgo_t action);


    //! Set the control flag for Expressiv training
    /*!
        Blocking call

        \param userId - user ID
        \param control - pre-defined control command

        \return EDK_ERROR_CODE - current status of EmoEngine. If the query is successful, EDK_ERROR_CODE = OK.

        \sa edkErrorCode.h, EE_ExpressivTrainingControl_t
    */
    EDK_API int
        EE_ExpressivSetTrainingControl(unsigned int userId,
                                       EE_ExpressivTrainingControl_t control);


    //! Gets the facial expression currently selected for Expressiv training
    /*!
        Blocking call

        \param userId - user ID
        \param pActionOut - receives facial expression currently selected for training

        \return EDK_ERROR_CODE - current status of EmoEngine. If the query is successful, EDK_ERROR_CODE = OK.

        \sa EDK_ERROR_CODE, EE_ExpressivAlgo_t
    */
    EDK_API int
        EE_ExpressivGetTrainingAction(unsigned int userId,
                                      EE_ExpressivAlgo_t * pActionOut);


    //! Return the duration of a Expressiv training session
    /*!
        \param userId - user ID
        \param pTrainingTimeOut - receive the training time in ms

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h
    */
    EDK_API int
        EE_ExpressivGetTrainingTime(unsigned int userId,
                                    unsigned int * pTrainingTimeOut);


    //! Gets a list of the actions that have been trained by the user
    /*!
        Blocking call

        \param userId - user ID
        \param pTrainedActionsOut - receives a bit vector composed of EE_ExpressivAlgo_t contants

        \return EDK_ERROR_CODE - current status of EmoEngine. If the query is successful, EDK_ERROR_CODE = OK.

        \sa EDK_ERROR_CODE, EE_ExpressivAlgo_t
    */
    EDK_API int
        EE_ExpressivGetTrainedSignatureActions(unsigned int userId,
                                               unsigned long * pTrainedActionsOut);


    //! Gets a flag indicating if the user has trained sufficient actions to activate a trained signature
    /*!
        *pfAvailableOut will be set to 1 if the user has trained EXP_NEUTRAL and at least
        one other Expressiv action.  Otherwise, *pfAvailableOut == 0.

        Blocking call

        \param userId - user ID
        \param pfAvailableOut - receives an int that is non-zero if a trained signature can be activated

        \return EDK_ERROR_CODE - current status of EmoEngine. If the query is successful, EDK_ERROR_CODE = OK.

        \sa EDK_ERROR_CODE
    */
    EDK_API int
        EE_ExpressivGetTrainedSignatureAvailable(unsigned int userId,
                                                 int * pfAvailableOut);


    //! Configures the Expressiv suite to use either the built-in, universal signature or a personal, trained signature
    /*!
        Note: Expressiv defaults to use its universal signature.  This function will fail if EE_ExpressivGetTrainedSignatureAvailable returns false.

        Blocking call

        \param userId - user ID
        \param sigType - signature type to use

        \return EDK_ERROR_CODE - current status of EmoEngine. If the query is successful, EDK_ERROR_CODE = OK.

        \sa EDK_ERROR_CODE, EE_ExpressivSignature_t
    */
    EDK_API int
        EE_ExpressivSetSignatureType(unsigned int userId,
                                     EE_ExpressivSignature_t sigType);


    //! Indicates whether the Expressiv suite is currently using either the built-in, universal signature or a trained signature
    /*!
        Blocking call

        \param userId - user ID
        \param pSigTypeOut - receives the signature type currently in use

        \return EDK_ERROR_CODE - current status of EmoEngine. If the query is successful, EDK_ERROR_CODE = OK.

        \sa EDK_ERROR_CODE, EE_ExpressivSignature_t
    */
    EDK_API int
        EE_ExpressivGetSignatureType(unsigned int userId,
                                     EE_ExpressivSignature_t * pSigTypeOut);
#ifdef __cplusplus
}
#endif

#endif // EXPRESSIVDETECTION_H
