/**
 * Emotiv SDK
 * Copyright (c) 2016 Emotiv Inc.
 *
 * This file is part of the Emotiv SDK.
 *
 * Header file for User Profile related API.
 *
 */


#ifndef EMOTIVPROFILE_H
#define EMOTIVPROFILE_H

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


    //! Handle to internal event structure allocated by EE_EmoEngineEventCreate()
    typedef void * EmoEngineEventHandle;

    //! Returns a handle to memory that can hold a profile byte stream. This handle can be reused by the caller to retrieve subsequent profile bytes.
        /*!
        \return EmoEngineEventHandle
    */
    EDK_API EmoEngineEventHandle
        EE_ProfileEventCreate();

    //! Loads an EmoEngine profile for the specified user.
    /*!
        \param userId - user ID
        \param profileBuffer - pointer to buffer containing a serialized user profile previously returned from EmoEngine.
        \param length - buffer size (number of bytes)

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE if the function succeeds in loading this profile

        \sa edkErrorCode.h
    */
    EDK_API int
        EE_SetUserProfile(unsigned int userId, const unsigned char profileBuffer[],
                          unsigned int length);


    //! Returns user profile data in a synchronous manner.
    /*!
         Fills in the event referred to by hEvent with an EE_ProfileEvent event
         that contains the profile data for the specified user.

         \param userId - user ID
         \param hEvent - a handle returned by EE_EmoEngineEventCreate()

         \return EDK_ERROR_CODE
                 - EDK_ERROR_CODE = EDK_OK if successful

         \sa edkErrorCode.h
    */
    EDK_API int
        EE_GetUserProfile(unsigned int userId, EmoEngineEventHandle hEvent);


    //! Returns a serialized user profile for a default user in a synchronous manner.
    /*!
        Fills in the event referred to by hEvent with an EE_ProfileEvent event
        that contains the profile data for the default user

         \param hEvent - a handle returned by EE_EmoEngineEventCreate()

         \return EDK_ERROR_CODE
                 - EDK_ERROR_CODE = EDK_OK if successful

         \sa edkErrorCode.h
    */
    EDK_API int
        EE_GetBaseProfile(EmoEngineEventHandle hEvent);


    //! Returns the number of bytes required to store a serialized version of the requested user profile.
    /*!
        \param hEvt - an EmoEngineEventHandle of type EE_ProfileEvent
        \param pProfileSizeOut - receives number of bytes required by the profile

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h
    */
    EDK_API int
        EE_GetUserProfileSize(EmoEngineEventHandle hEvt,
                              unsigned int * pProfileSizeOut);


    //! Copies a serialized version of the requested user profile into the caller's buffer.
    /*!
        \param hEvt - an EmoEngineEventHandle returned in a EE_ProfileEvent event
        \param destBuffer - pointer to a destination buffer
        \param length - the size of the destination buffer in bytes

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h
    */
    EDK_API int
        EE_GetUserProfileBytes(EmoEngineEventHandle hEvt, unsigned char destBuffer[],
                               unsigned int length);


    //!  Loads a user profile from disk and assigns it to the specified user
    /*!
        \param userID - a valid user ID
        \param szInputFilename - platform-dependent filesystem path of saved user profile

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h
    */
    EDK_API int
        EE_LoadUserProfile(unsigned int userID, const char * szInputFilename);


    //!  Saves a user profile for specified user to disk
    /*!
        \param userID - a valid user ID
        \param szOutputFilename - platform-dependent filesystem path for output file

        \return EDK_ERROR_CODE
                - EDK_ERROR_CODE = EDK_OK if successful

        \sa edkErrorCode.h
    */
    EDK_API int
        EE_SaveUserProfile(unsigned int userID, const char * szOutputFilename);
};

#endif // EMOTIVPROFILE_H
