/**
 * Emotiv SDK
 * Copyright (c) 2016 Emotiv Inc.
 *
 * This file is part of the Emotiv SDK.
 *
 * Header file for Facial Expression related API.
 *
 */
//! \file

#ifndef FACIALEXPRESSIONDETECTION_H
#define FACIALEXPRESSIONDETECTION_H

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

#include "IEmoStateDLL.h"

    //! Handle to EmoEngine event structure allocated by IEE_EmoEngineEventCreate.
    /*!
        \sa IEE_EmoEngineEventCreate()
    */
    typedef void* EmoEngineEventHandle;


    //! FacialExpression Suite threshold type enumerator
    typedef enum IEE_FacialExpressionThreshold_enum {
        FE_SENSITIVITY  //!< Sensitivity of each facial expression
    } IEE_FacialExpressionThreshold_t;


    //! FacialExpression Suite training control enumerator
    typedef enum IEE_FacialExpressionTrainingControl_enum {
        FE_NONE = 0,    //!< No action
        FE_START,       //!< Start a new training
        FE_ACCEPT,      //!< Accept current training
        FE_REJECT,      //!< Reject current training
        FE_ERASE,       //!< Erase training data for a facial expression
        FE_RESET        //!< Reset current training
    } IEE_FacialExpressionTrainingControl_t;


    //! FacialExpression Suite signature type enumerator
    typedef enum IEE_FacialExpressionSignature_enum {
        FE_SIG_UNIVERSAL = 0,   //!< Use built-in universal signature
        FE_SIG_TRAINED          //!< Use custom trained signature
    } IEE_FacialExpressionSignature_t;


    //! FacialExpression event types
    typedef enum IEE_FacialExpressionEvent_enum {
        IEE_FacialExpressionNoEvent = 0,        //!< No new event
        IEE_FacialExpressionTrainingStarted,    //!< The training has begun after FE_START is sent.
        IEE_FacialExpressionTrainingSucceeded,  //!< The training is succeeded, waiting for FE_ACCEPT or FE_REJECT.
        IEE_FacialExpressionTrainingFailed,     //!< The training is failed due to signal issues. Please restart training.
        IEE_FacialExpressionTrainingCompleted,  //!< The training is completed after FE_ACCEPT is sent.
        IEE_FacialExpressionTrainingDataErased, //!< The training data for a particular facial expression has been erased by FE_ERASE.
        IEE_FacialExpressionTrainingRejected,   //!< The training is rejected after FE_REJECT is sent.
        IEE_FacialExpressionTrainingReset       //!< The training has been reset after FE_RESET is sent.
    } IEE_FacialExpressionEvent_t;


    //! Return the FacialExpression-specific event type.
    /*!
        Returns the FacialExpression-specific event type for an IEE_FacialExpressionEvent event already retrieved using IEE_EngineGetNextEvent().

        \param hEvent - a handle returned by IEE_EmoEngineEventCreate()

        \return IEE_FacialExpressionEvent_t
    */
    EDK_API IEE_FacialExpressionEvent_t
        IEE_FacialExpressionEventGetType(EmoEngineEventHandle hEvent);


    //! Set threshold for FacialExpression algorithms
    /*!
        \param userId - user ID
        \param algoName - FacialExpression algorithm type
        \param thresholdName - FacialExpression threshold type
        \param value - threshold value (min: 0, max: 1000)

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h, IEE_FacialExpressionAlgo_t, IEE_FacialExpressionThreshold_t
    */
    EDK_API int
        IEE_FacialExpressionSetThreshold(unsigned int userId,
                                         IEE_FacialExpressionAlgo_t algoName,
                                         IEE_FacialExpressionThreshold_t thresholdName,
                                         int value);


    //! Get threshold from FacialExpression algorithms
    /*!
        \param userId - user ID
        \param algoName - FacialExpression algorithm type
        \param thresholdName - FacialExpression threshold type
        \param pValueOut - receives threshold value

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h, IEE_FacialExpressionAlgo_t, IEE_FacialExpressionThreshold_t
    */
    EDK_API int
        IEE_FacialExpressionGetThreshold(unsigned int userId,
                                         IEE_FacialExpressionAlgo_t algoName,
                                         IEE_FacialExpressionThreshold_t thresholdName,
                                         int *pValueOut);


    //! Set the current facial expression for FacialExpression training
    /*!
        Blocking call

        \param userId - user ID
        \param action - which facial expression would like to be trained

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h, IEE_FacialExpressionAlgo_t
    */
    EDK_API int
        IEE_FacialExpressionSetTrainingAction(unsigned int userId,
                                              IEE_FacialExpressionAlgo_t action);


    //! Set the control flag for FacialExpression training
    /*!
    Blocking call

        \param userId - user ID
        \param control - pre-defined control command

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h, IEE_FacialExpressionTrainingControl_t
    */
    EDK_API int
        IEE_FacialExpressionSetTrainingControl(unsigned int userId,
                                               IEE_FacialExpressionTrainingControl_t control);


    //! Get the facial expression currently selected for FacialExpression training
    /*!
        Blocking call

        \param userId - user ID
        \param pActionOut - receives facial expression currently selected for training

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h, IEE_FacialExpressionAlgo_t
    */
    EDK_API int
        IEE_FacialExpressionGetTrainingAction(unsigned int userId,
                                              IEE_FacialExpressionAlgo_t* pActionOut);


    //! Return the duration of a FacialExpression training session
    /*!
        \param userId - user ID
        \param pTrainingTimeOut - receive the training time in ms

        \return EDK_ERROR_CODE
                - EDK_OK if successful

    \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_FacialExpressionGetTrainingTime(unsigned int userId,
                                            unsigned int* pTrainingTimeOut);


    //! Get a list of expressions that have been trained by the user
    /*!
        Blocking call

        \param userId - user ID
        \param pTrainedActionsOut - receives a bit vector composed of IEE_FacialExpressionAlgo_t contants

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h, IEE_FacialExpressionAlgo_t
    */
    EDK_API int
        IEE_FacialExpressionGetTrainedSignatureActions(unsigned int userId,
                                                       unsigned long* pTrainedActionsOut);


    //! Check if the user has trained sufficient actions to activate a trained signature
    /*!
        *pfAvailableOut will be set to 1 if the user has trained FE_NEUTRAL and at least
        one other FacialExpression action.  Otherwise, *pfAvailableOut == 0.

        Blocking call

        \param userId - user ID
        \param pfAvailableOut - receives an int that is non-zero if a trained signature can be activated

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_FacialExpressionGetTrainedSignatureAvailable(unsigned int userId,
                                                         int* pfAvailableOut);


    //! Configure the FacialExpression suite to use either the built-in, universal signature or a personal, trained signature
    /*!
        Note: FacialExpression defaults to use its universal signature.  This function will fail if IEE_FacialExpressionGetTrainedSignatureAvailable returns false.

        Blocking call

        \param userId - user ID
        \param sigType - signature type to use

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h, IEE_FacialExpressionSignature_t
    */
    EDK_API int
        IEE_FacialExpressionSetSignatureType(unsigned int userId,
                                             IEE_FacialExpressionSignature_t sigType);


    //! Check whether the FacialExpression suite is currently using either the built-in, universal signature or a trained signature
    /*!
        Blocking call

        \param userId - user ID
        \param pSigTypeOut - receives the signature type currently in use

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h, IEE_FacialExpressionSignature_t
    */
    EDK_API int
        IEE_FacialExpressionGetSignatureType(unsigned int userId,
                                             IEE_FacialExpressionSignature_t* pSigTypeOut);
#ifdef __cplusplus
}
#endif

#endif // FACIALEXPRESSIONDETECTION_H
