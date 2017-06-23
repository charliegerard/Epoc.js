/**
 * Emotiv SDK
 * Copyright (c) 2016 Emotiv Inc.
 *
 * This file is part of the Emotiv SDK.
 *
 * Header file for Mental Command related API.
 *
 */

//! \file MentalCommandDetection.h

#ifndef MENTALCOMMANDDETECTION_H
#define MENTALCOMMANDDETECTION_H

#ifdef __cplusplus
extern "C"
{
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

#include "IEmoStateDLL.h"

    //! Handle to EmoEngine event structure allocated by IEE_EmoEngineEventCreate.
    /*!
        \sa IEE_EmoEngineEventCreate()
    */
    typedef void* EmoEngineEventHandle;


    //! MentalCommand Suite training control enumerator
    typedef enum IEE_MentalCommandTrainingControl_enum {
        MC_NONE = 0,    //!< No action
        MC_START,       //!< Start a new training
        MC_ACCEPT,      //!< Accept current training
        MC_REJECT,      //!< Reject current training
        MC_ERASE,       //!< Erase training data for an action
        MC_RESET        //!< Reset current training
    } IEE_MentalCommandTrainingControl_t;


    //! MentalCommand event types
    typedef enum IEE_MentalCommandEvent_enum {
        IEE_MentalCommandNoEvent = 0,                   //!< No new event
        IEE_MentalCommandTrainingStarted,               //!< The training has begun after MC_START is sent.
        IEE_MentalCommandTrainingSucceeded,             //!< The training is succeeded, waiting for MC_ACCEPT or MC_REJECT.
        IEE_MentalCommandTrainingFailed,                //!< The training is failed due to signal issues. Please restart training.
        IEE_MentalCommandTrainingCompleted,             //!< The training is completed after MC_ACCEPT is sent.
        IEE_MentalCommandTrainingDataErased,            //!< The training data for a particular action has been erased by MC_ERASE.
        IEE_MentalCommandTrainingRejected,              //!< The training is rejected after MC_REJECT is sent.
        IEE_MentalCommandTrainingReset,                 //!< The training has been reset after MC_RESET is sent.
        IEE_MentalCommandAutoSamplingNeutralCompleted,  //!< The neutral training is completed.
        IEE_MentalCommandSignatureUpdated               //!< The mental command signature has been updated for new actions.
    } IEE_MentalCommandEvent_t;


    //! Return the MentalCommand-specific event type.
    /*!
        Returns the MentalCommand-specific event type for an IEE_MentalCommandEvent event already retrieved using IEE_EngineGetNextEvent().

        \param hEvent - a handle returned by IEE_EmoEngineEventCreate()

        \return IEE_MentalCommandEvent_t
    */
    EDK_API IEE_MentalCommandEvent_t
        IEE_MentalCommandEventGetType(EmoEngineEventHandle hEvent);


    //! Set the current MentalCommand active action types
    /*!
        \param userId - user ID
        \param activeActions - a bit vector composed of IEE_MentalCommandAction_t contants

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h, IEE_MentalCommandAction_t
    */
    EDK_API int
        IEE_MentalCommandSetActiveActions(unsigned int userId,
                                          unsigned long activeActions);


    //! Get the current MentalCommand active action types
    /*!
        \param userId - user ID
        \param pActiveActionsOut - receive a bit vector composed of IEE_MentalCommandAction_t contants

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h, IEE_MentalCommandAction_t
    */
    EDK_API int
        IEE_MentalCommandGetActiveActions(unsigned int userId,
                                          unsigned long* pActiveActionsOut);


    //! Return the duration of a MentalCommand training session
    /*!
        \param userId - user ID
        \param pTrainingTimeOut - receive the training time in ms

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_MentalCommandGetTrainingTime(unsigned int userId,
                                         unsigned int* pTrainingTimeOut);


    //! Set the training control flag for MentalCommand training
    /*!
        \param userId - user ID
        \param control - pre-defined MentalCommand training control

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h, IEE_MentalCommandTrainingControl_t
    */
    EDK_API int
        IEE_MentalCommandSetTrainingControl(unsigned int userId,
                                            IEE_MentalCommandTrainingControl_t control);


    //! Set the type of MentalCommand action to be trained
    /*!
        \param userId - user ID
        \param action - which action would like to be trained

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h, IEE_MentalCommandAction_t
    */
    EDK_API int
        IEE_MentalCommandSetTrainingAction(unsigned int userId,
                                           IEE_MentalCommandAction_t action);


    //! Get the type of MentalCommand action currently selected for training
    /*!
        \param userId - user ID
        \param pActionOut - action that is currently selected for training

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h, IEE_MentalCommandAction_t
    */
    EDK_API int
        IEE_MentalCommandGetTrainingAction(unsigned int userId,
                                           IEE_MentalCommandAction_t* pActionOut);


    //! Get a list of the actions that have been trained by the user
    /*!
        Blocking call

        \param userId - user ID
        \param pTrainedActionsOut - receives a bit vector composed of IEE_MentalCommandAction_t contants

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h, IEE_MentalCommandAction_t
    */
    EDK_API int
        IEE_MentalCommandGetTrainedSignatureActions(unsigned int userId,
                                                    unsigned long* pTrainedActionsOut);


    //! Get the current overall skill rating of the user in MentalCommand
    /*!
        Blocking call

        \param userId - user ID
        \param pOverallSkillRatingOut - receives the overall skill rating [from 0.0 to 1.0]

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_MentalCommandGetOverallSkillRating(unsigned int userId,
                                               float* pOverallSkillRatingOut);


    //! Get the current skill rating for particular MentalCommand actions of the user
    /*!
        Blocking call

        \param userId - user ID
        \param action - a particular action of IEE_MentalCommandAction_t contant
        \param pActionSkillRatingOut - receives the action skill rating [from 0.0 to 1.0]

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h, IEE_MentalCommandAction_t
    */
    EDK_API int
        IEE_MentalCommandGetActionSkillRating(unsigned int userId,
                                              IEE_MentalCommandAction_t action,
                                              float* pActionSkillRatingOut);


    //! Set the overall sensitivity for all MentalCommand actions
    /*!
        \param userId - user ID
        \param level - sensitivity level of all actions (lowest: 1, highest: 7)

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_MentalCommandSetActivationLevel(unsigned int userId,
                                            int level);


    //! Set the sensitivity of MentalCommand actions
    /*!
        \param userId - user ID
        \param action1Sensitivity - sensitivity of action 1 (min: 1, max: 10)
        \param action2Sensitivity - sensitivity of action 2 (min: 1, max: 10)
        \param action3Sensitivity - sensitivity of action 3 (min: 1, max: 10)
        \param action4Sensitivity - sensitivity of action 4 (min: 1, max: 10)

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_MentalCommandSetActionSensitivity(unsigned int userId,
                                              int action1Sensitivity,
                                              int action2Sensitivity,
                                              int action3Sensitivity,
                                              int action4Sensitivity);


    //! Get the overall sensitivity for all MentalCommand actions
    /*!
        \param userId - user ID
        \param pLevelOut - sensitivity level of all actions (min: 1, max: 10)

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_MentalCommandGetActivationLevel(unsigned int userId,
                                            int *pLevelOut);


    //! Query the sensitivity of MentalCommand actions
    /*!
        \param userId - user ID
        \param pAction1SensitivityOut - sensitivity of action 1
        \param pAction2SensitivityOut - sensitivity of action 2
        \param pAction3SensitivityOut - sensitivity of action 3
        \param pAction4SensitivityOut - sensitivity of action 4

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_MentalCommandGetActionSensitivity(unsigned int userId,
                                              int* pAction1SensitivityOut,
                                              int* pAction2SensitivityOut,
                                              int* pAction3SensitivityOut,
                                              int* pAction4SensitivityOut);


    //! Start the sampling of Neutral state in MentalCommand
    /*!
        \param userId - user ID

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_MentalCommandStartSamplingNeutral(unsigned int userId);


    //! Stop the sampling of Neutral state in MentalCommand
    /*!
        \param userId - user ID

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_MentalCommandStopSamplingNeutral(unsigned int userId);


    //! Enable or disable signature caching in MentalCommand
    /*!
        Enable signature caching will shorten the time to build the signature after each training,
        with the penalty of more memory usage.

        \param userId  - user ID
        \param enabled - flag to set status of caching (1: enable, 0: disable)

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_MentalCommandSetSignatureCaching(unsigned int userId,
                                             unsigned int enabled);


    //! Query the status of signature caching in MentalCommand
    /*!
        \param userId  - user ID
        \param pEnabledOut - flag to get status of caching (1: enable, 0: disable)

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_MentalCommandGetSignatureCaching(unsigned int userId,
                                             unsigned int* pEnabledOut);


    //! Set the cache size for the signature caching in MentalCommand
    /*!
        \param userId - user ID
        \param size   - number of signatures to be kept in the cache (0: unlimited)

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_MentalCommandSetSignatureCacheSize(unsigned int userId,
                                               unsigned int size);


    //! Get the current cache size for the signature caching in MentalCommand
    /*!
        \param userId - user ID
        \param pSizeOut - number of signatures to be kept in the cache (0: unlimited)

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_MentalCommandGetSignatureCacheSize(unsigned int userId,
                                               unsigned int* pSizeOut);
#ifdef __cplusplus
}
#endif

#endif // MENTALCOMMANDDETECTION_H
