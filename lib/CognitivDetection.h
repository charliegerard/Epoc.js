/**
 * Emotiv SDK
 * Copyright (c) 2016 Emotiv Inc.
 *
 * This file is part of the Emotiv SDK.
 *
 * Header file for Cognitiv related API.
 *
 */


#ifndef COGNITIVDETECTION_H
#define COGNITIVDETECTION_H

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


    //! Cognitiv Suite training control enumerator
    typedef enum EE_CognitivTrainingControl_enum {
        COG_NONE = 0,
        COG_START,
        COG_ACCEPT,
        COG_REJECT,
        COG_ERASE,
        COG_RESET
    } EE_CognitivTrainingControl_t;


    //! Cognitiv-specific event types
    typedef enum EE_CognitivEvent_enum {
        EE_CognitivNoEvent = 0,
        EE_CognitivTrainingStarted,
        EE_CognitivTrainingSucceeded,
        EE_CognitivTrainingFailed,
        EE_CognitivTrainingCompleted,
        EE_CognitivTrainingDataErased,
        EE_CognitivTrainingRejected,
        EE_CognitivTrainingReset,
        EE_CognitivAutoSamplingNeutralCompleted,
        EE_CognitivSignatureUpdated
    } EE_CognitivEvent_t;


    //! Returns the Cognitiv-specific event type for an EE_CognitivEvent event already retrieved using EE_EngineGetNextEvent.
    /*!
        \param hEvent - a handle returned by EE_EmoEngineEventCreate()

        \return EE_CognitivEvent_t
    */
    EDK_API EE_CognitivEvent_t
        EE_CognitivEventGetType(EmoEngineEventHandle hEvent);


    //! Set the current Cognitiv active action types
    /*!
        \param userId - user ID
        \param activeActions - a bit vector composed of EE_CognitivAction_t contants

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h, EE_CognitivAction_t
    */
    EDK_API int
        EE_CognitivSetActiveActions(unsigned int userId,
                                    unsigned long activeActions);


    //! Get the current Cognitiv active action types
    /*!
        \param userId - user ID
        \param pActiveActionsOut - receive a bit vector composed of EE_CognitivAction_t contants

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h, EE_CognitivAction_t
    */
    EDK_API int
        EE_CognitivGetActiveActions(unsigned int userId,
                                    unsigned long * pActiveActionsOut);


    //! Return the duration of a Cognitiv training session
    /*!
        \param userId - user ID
        \param pTrainingTimeOut - receive the training time in ms

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h
    */
    EDK_API int
        EE_CognitivGetTrainingTime(unsigned int userId,
                                   unsigned int * pTrainingTimeOut);


    //! Set the training control flag for Cognitiv training
    /*!
        \param userId - user ID
        \param control - pre-defined Cognitiv training control

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h, EE_CognitivTrainingControl_t
    */
    EDK_API int
        EE_CognitivSetTrainingControl(unsigned int userId,
                                      EE_CognitivTrainingControl_t control);


    //! Set the type of Cognitiv action to be trained
    /*!
        \param userId - user ID
        \param action - which action would like to be trained

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h, EE_CognitivAction_t
    */
    EDK_API int
        EE_CognitivSetTrainingAction(unsigned int userId,
                                     EE_CognitivAction_t action);


    //! Get the type of Cognitiv action currently selected for training
    /*!
        \param userId - user ID
        \param pActionOut - action that is currently selected for training

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h, EE_CognitivAction_t
    */
    EDK_API int
        EE_CognitivGetTrainingAction(unsigned int userId,
                                     EE_CognitivAction_t * pActionOut);


    //! Gets a list of the Cognitiv actions that have been trained by the user
    /*!
        Blocking call

        \param userId - user ID
        \param pTrainedActionsOut - receives a bit vector composed of EE_CognitivAction_t contants

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h, EE_CognitivAction_t
    */
    EDK_API int
        EE_CognitivGetTrainedSignatureActions(unsigned int userId,
                                              unsigned long * pTrainedActionsOut);


    //! Gets the current overall skill rating of the user in Cognitiv
    /*!
        Blocking call

        \param userId - user ID
        \param pOverallSkillRatingOut - receives the overall skill rating [from 0.0 to 1.0]

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h
    */
    EDK_API int
        EE_CognitivGetOverallSkillRating(unsigned int userId,
                                         float * pOverallSkillRatingOut);


    //! Gets the current skill rating for particular Cognitiv actions of the user
    /*!
        Blocking call

        \param userId - user ID
        \param action - a particular action of EE_CognitivAction_t contant
        \param pActionSkillRatingOut - receives the action skill rating [from 0.0 to 1.0]

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h, EE_CognitivAction_t
    */
    EDK_API int
        EE_CognitivGetActionSkillRating(unsigned int userId,
                                        EE_CognitivAction_t action,
                                        float * pActionSkillRatingOut);


    //! Set the overall sensitivity for all Cognitiv actions
    /*!
        \param userId - user ID
        \param level - sensitivity level of all actions (lowest: 1, highest: 7)

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h
    */
    EDK_API int
        EE_CognitivSetActivationLevel(unsigned int userId,
                                      int level);


    //! Set the sensitivity of Cognitiv actions
    /*!
        \param userId - user ID
        \param action1Sensitivity - sensitivity of action 1 (min: 1, max: 10)
        \param action2Sensitivity - sensitivity of action 2 (min: 1, max: 10)
        \param action3Sensitivity - sensitivity of action 3 (min: 1, max: 10)
        \param action4Sensitivity - sensitivity of action 4 (min: 1, max: 10)

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h
    */
    EDK_API int
        EE_CognitivSetActionSensitivity(unsigned int userId,
                                        int action1Sensitivity, int action2Sensitivity,
                                        int action3Sensitivity, int action4Sensitivity);


    //! Get the overall sensitivity for all Cognitiv actions
    /*!
        \param userId - user ID
        \param pLevelOut - sensitivity level of all actions (min: 1, max: 10)

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h
    */
    EDK_API int
        EE_CognitivGetActivationLevel(unsigned int userId,
                                      int * pLevelOut);


    //! Query the sensitivity of Cognitiv actions
    /*!
        \param userId - user ID
        \param pAction1SensitivityOut - sensitivity of action 1
        \param pAction2SensitivityOut - sensitivity of action 2
        \param pAction3SensitivityOut - sensitivity of action 3
        \param pAction4SensitivityOut - sensitivity of action 4

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h
    */
    EDK_API int
        EE_CognitivGetActionSensitivity(unsigned int userId,
                                        int * pAction1SensitivityOut, int * pAction2SensitivityOut,
                                        int * pAction3SensitivityOut, int * pAction4SensitivityOut);


    //! Start the sampling of Neutral state in Cognitiv
    /*!
        \param userId - user ID

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h
    */
    EDK_API int
        EE_CognitivStartSamplingNeutral(unsigned int userId);


    //! Stop the sampling of Neutral state in Cognitiv
    /*!
        \param userId - user ID

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h
    */
    EDK_API int
        EE_CognitivStopSamplingNeutral(unsigned int userId);


    //! Enable or disable signature caching in Cognitiv
    /*!
        \param userId  - user ID
        \param enabled - flag to set status of caching (1: enable, 0: disable)

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h
    */
    EDK_API int
        EE_CognitivSetSignatureCaching(unsigned int userId,
                                       unsigned int enabled);


    //! Query the status of signature caching in Cognitiv
    /*!
        \param userId  - user ID
        \param pEnabledOut - flag to get status of caching (1: enable, 0: disable)

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h
    */
    EDK_API int
        EE_CognitivGetSignatureCaching(unsigned int userId,
                                       unsigned int * pEnabledOut);


    //! Set the cache size for the signature caching in Cognitiv
    /*!
        \param userId - user ID
        \param size   - number of signatures to be kept in the cache (0: unlimited)

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h
    */
    EDK_API int
        EE_CognitivSetSignatureCacheSize(unsigned int userId,
                                         unsigned int size);


    //! Get the current cache size for the signature caching in Cognitiv
    /*!
        \param userId - user ID
        \param pSizeOut - number of signatures to be kept in the cache (0: unlimited)

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h
    */
    EDK_API int
        EE_CognitivGetSignatureCacheSize(unsigned int userId,
                                         unsigned int * pSizeOut);
};

#endif // COGNITIVDETECTION_H
