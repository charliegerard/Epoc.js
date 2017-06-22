/**
* Emotiv SDK
* Copyright (c) 2016 Emotiv Inc.
*
* This file is part of the Emotiv SDK.
*
* All the error codes that are returned by IEE_ functions are listed here.
*/

//! \file IedkErrorCode.h

#ifndef IEDK_ERROR_CODE_H
#define IEDK_ERROR_CODE_H

//! Default success value.
#define EDK_OK                              0x0000

//! An internal error occurred.
#define EDK_UNKNOWN_ERROR                   0x0001

//! Invalid Developer ID.
#define EDK_INVALID_DEV_ID_ERROR            0x0002

//! The buffer supplied to IEE_SetUserProfile() is not a valid, serialized EmoEngine profile.
#define EDK_INVALID_PROFILE_ARCHIVE         0x0101

//! Returned from IEE_EmoEngineEventGetUserId() if the event supplied contains a base profile,
//! and is not associated with specific user.
#define EDK_NO_USER_FOR_BASEPROFILE         0x0102

//! The EmoEngine is unable to acquire data for processing.
//! \sa IEE_EngineConnect
#define EDK_CANNOT_ACQUIRE_DATA             0x0200

//! The buffer supplied to the function is not large enough.
#define EDK_BUFFER_TOO_SMALL                0x0300

//! A parameter supplied to the function is out of range.
#define EDK_OUT_OF_RANGE                    0x0301

//! One of the parameters supplied to the function is invalid
#define EDK_INVALID_PARAMETER               0x0302

//! The parameter is currently locked by the detection and cannot be modified at this time.
#define EDK_PARAMETER_LOCKED                0x0303

//! The supplied MentalCommand training action flag is invalid.
#define EDK_MC_INVALID_TRAINING_ACTION      0x0304

//! The supplied MentalCommand training control flag is invalid.
#define EDK_MC_INVALID_TRAINING_CONTROL     0x0305

//! The MentalCommand action bits vector is invalid.
#define EDK_MC_INVALID_ACTIVE_ACTION        0x0306

//! The MentalCommand action bits vector contains more action types than it is allowed.
#define EDK_MC_EXCESS_MAX_ACTIONS           0x0307

//! A trained signature is not currently available for use,
//! addition actions (including neutral) may be required.
#define EDK_FE_NO_SIG_AVAILABLE             0x0308

//! A filesystem error occurred.
#define EDK_FILESYSTEM_ERROR                0x0309

//! The user ID supplied to the function is invalid.
#define EDK_INVALID_USER_ID                 0x0400

//! The EDK needs to be initialized via IEE_EngineConnect() or IEE_EngineRemoteConnect().
#define EDK_EMOENGINE_UNINITIALIZED         0x0500

//! The connection with a remote instance of the EmoEngine made via EE_EngineRemoteConnect() has been lost.
#define EDK_EMOENGINE_DISCONNECTED          0x0501

//! The API was unable to establish a connection with a remote instance of the EmoEngine.
#define EDK_EMOENGINE_PROXY_ERROR           0x0502

//! There are no new EmoEngine events at this time.
#define EDK_NO_EVENT                        0x0600

//! The gyro is not calibrated. Please ask the user to stay still for at least 0.5 seconds.
#define EDK_GYRO_NOT_CALIBRATED             0x0700

//! Operation failure due to optimization.
#define EDK_OPTIMIZATION_IS_ON              0x0800

//! Reserved return value.
#define EDK_RESERVED1                       0x0900

//! An internal error occurred.
#define EDK_COULDNT_RESOLVE_PROXY           0x1001

#define EDK_COULDNT_RESOLVE_HOST            0x1002

#define EDK_COULDNT_CONNECT                 0x1003

//! Profile created by EDK_SaveUserProfile() is existed in Emotiv Cloud.
#define EDK_PROFILE_CLOUD_EXISTED           0x1010

//! The file uploaded to cloud is failed
#define EDK_UPLOAD_FAILED                   0x1011

//! The cloud user ID supplied to the function is invalid.
#define EDK_INVALID_CLOUD_USER_ID           0x1020

//! The user ID supplied to the function is invalid
#define EDK_INVALID_ENGINE_USER_ID          0x1021

//! The user ID supplied to the function dont login, call EDK_Login() first
#define EDK_CLOUD_USER_ID_DONT_LOGIN        0x1022

//! The Emotiv Cloud needs to be initialized via EDK_Connect()
#define EDK_EMOTIVCLOUD_UNINITIALIZED       0x1023


#define EDK_FILE_EXISTS                     0x2000	

//! The headset is not available to work
#define EDK_HEADSET_NOT_AVAILABLE           0x2001

//! The headset is off
#define EDK_HEADSET_IS_OFF                  0x2002

//! Other session of saving is running
#define EDK_SAVING_IS_RUNNING               0x2003

#define EDK_DEVICE_CODE_ERROR               0x2004

//! The license error. 
#define EDK_LICENSE_ERROR                   0x2010  

//! The license expried
#define EDK_LICENSE_EXPIRED                 0x2011

//! The license was not found
#define EDK_LICENSE_NOT_FOUND               0x2012

//! The license is over quota
#define EDK_OVER_QUOTA                      0x2013

//! The license is over quota in day
#define EDK_OVER_QUOTA_IN_DAY               0x2014

//! The license is over quota in month
#define EDK_OVER_QUOTA_IN_MONTH             0x2015


#define EDK_APP_QUOTA_EXCEEDED              0x2016


#define EDK_APP_INVALID_DATE                0x2017

//! Application register device number is exceeded. 
#define EDK_LICENSE_DEVICE_LIMITED          0x2019  

//! The license registered with the device. 
#define EDK_LICENSE_REGISTERED              0x2020

//! No license is activated
#define EDK_NO_ACTIVE_LICENSE               0x2021

//! The license is not EEG data ouput
#define EDK_LICENSE_NO_EEG                  0x2022

//! The license is updated
#define EDK_UPDATE_LICENSE                  0x2023

//! The file was not found
#define EDK_FILE_NOT_FOUND                  0x2030

#define EDK_ACCESS_DENIED                   0x2031

#endif