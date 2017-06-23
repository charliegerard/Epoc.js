/**
 * Emotiv SDK
 * Copyright (c) 2016 Emotiv Inc.
 *
 * This file is part of the Emotiv SDK.
 *
 * The main interface that allows interactions between external programs and the Emotiv detection engine.
 *
 * None of these API functions are thread-safe.
 *
 * This header file is designed to be included under C and C++ environment.
 *
 */

#ifndef IEDK_H
#define IEDK_H

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
#   include "IEmoStatePerformanceMetric.h"
#   include "IedkOptimization.h"
#   include "IedkPrivate.h"
#   define EDK_API extern
#endif

#include "IedkErrorCode.h"
#include "IEmoStateDLL.h"
#include "FacialExpressionDetection.h"
#include "MentalCommandDetection.h"
#include "IEmotivProfile.h"
#include "EmotivLicense.h"


    //! Handle to EmoState structure allocated by IEE_EmoStateCreate.
    /*!
        \sa IEE_EmoStateCreate()
     */
    typedef void* EmoStateHandle;


    //! Handle to EmoEngine event structure allocated by IEE_EmoEngineEventCreate.
    /*!
        \sa IEE_EmoEngineEventCreate()
     */
    typedef void* EmoEngineEventHandle;

    
    //! EEG and system data channel description
    typedef enum IEE_DataChannels_enum {
        IED_COUNTER = 0,        //!< Sample counter
        IED_INTERPOLATED,       //!< Indicate if data is interpolated
        IED_RAW_CQ,             //!< Raw contact quality value
        IED_AF3,                //!< Channel AF3
        IED_F7,                 //!< Channel F7
        IED_F3,                 //!< Channel F3
        IED_FC5,                //!< Channel FC5
        IED_T7,                 //!< Channel T7
        IED_P7,                 //!< Channel P7
        IED_Pz,                 //!< Channel Pz
        IED_O1 = IED_Pz,        //!< Channel O1
        IED_O2,                 //!< Channel O2
        IED_P8,                 //!< Channel P8
        IED_T8,                 //!< Channel T8
        IED_FC6,                //!< Channel FC6
        IED_F4,                 //!< Channel F4
        IED_F8,                 //!< Channel F8
        IED_AF4,                //!< Channel AF4
        IED_GYROX,              //!< Gyroscope X-axis
        IED_GYROY,              //!< Gyroscope Y-axis
        IED_TIMESTAMP,          //!< System timestamp
        IED_MARKER_HARDWARE,    //!< Marker from extender
        IED_ES_TIMESTAMP,       //!< EmoState timestamp
        IED_FUNC_ID,            //!< Reserved function id
        IED_FUNC_VALUE,         //!< Reserved function value
        IED_MARKER,             //!< Marker value from hardware
        IED_SYNC_SIGNAL         //!< Synchronisation signal
    } IEE_DataChannel_t;

    
    //! Handle to data placeholder allocated by IEE_MotionDataCreate.
    /*!
        \sa IEE_MotionDataCreate()
     */
    typedef void* DataHandle;


    //! EmoEngine event types
    typedef enum IEE_Event_enum {
        IEE_UnknownEvent          = 0x0000,     //!< An unknown event.
        IEE_EmulatorError         = 0x0001,     //!< Error event from emulator. Connection to EmoComposer could be lost.
        IEE_ReservedEvent         = 0x0002,     //!< Reserved event.
        IEE_UserAdded             = 0x0010,     //!< A headset is connected.
        IEE_UserRemoved           = 0x0020,     //!< A headset has been disconnected.
        IEE_EmoStateUpdated       = 0x0040,     //!< Detection results have been updated from EmoEngine.
        IEE_ProfileEvent          = 0x0080,     //!< A profile has been returned from EmoEngine.
        IEE_MentalCommandEvent    = 0x0100,     //!< A IEE_MentalCommandEvent_t has been returned from EmoEngine.
        IEE_FacialExpressionEvent = 0x0200,     //!< A IEE_FacialExpressionEvent_t has been returned from EmoEngine.
        IEE_InternalStateChanged  = 0x0400,     //!< Reserved for internal use.
        IEE_AllEvent              = IEE_UserAdded | IEE_UserRemoved | IEE_EmoStateUpdated |
                                    IEE_ProfileEvent | IEE_MentalCommandEvent |
                                    IEE_FacialExpressionEvent | IEE_InternalStateChanged
                                    //!< Bit-mask for all events except error types
    } IEE_Event_t;


    //! Input sensor description
    typedef struct IInputSensorDescriptor_struct {
        IEE_InputChannels_t channelId;  //!< Logical channel id
        int                 fExists;    //!< Non-zero if this sensor exists on this headset model
        const char*         pszLabel;   //!< Text label identifying this sensor
        double              xLoc;       //!< X coordinate from center of head towards nose
        double              yLoc;       //!< Y coordinate from center of head towards ears
        double              zLoc;       //!< Z coordinate from center of head toward top of skull
    } IInputSensorDescriptor_t;


    //! Detection type enumerator
    typedef enum IEE_Detection_enum {
        DT_BlinkAndWink     = 0x0001,   //!< Blink and Wink detection
        DT_FacialExpression = 0x0002,   //!< Other facial expression detection
        DT_EyeMovement      = 0x0004,   //!< Eye movement detection
        DT_Excitement       = 0x0008,   //!< Excitement detection (deprecated)
        DT_Engagement       = 0x0010,   //!< Engagement detection (deprecated)
        DT_Relaxation       = 0x0020,   //!< Relaxation detection (deprecated)
        DT_Interest         = 0x0040,   //!< Interest detection (deprecated)
        DT_Stress           = 0x0080,   //!< Stress detection (deprecated)
        DT_Focus            = 0x0100,   //!< Focus detection (deprecated)
        DT_MentalCommand    = 0x0200,   //!< Mental command detection
        DT_AllDetections    = (DT_BlinkAndWink | DT_FacialExpression | DT_EyeMovement |
                               DT_Excitement | DT_Engagement | DT_Relaxation |
                               DT_Interest | DT_Stress | DT_Focus |
                               DT_MentalCommand)

    } IEE_Detection_t;


    //! Motion data channel description
    typedef enum IEE_MotionDataChannel_enum {
        IMD_COUNTER = 0,        //!< Sample counter
        IMD_GYROX,              //!< Gyroscope X-axis
        IMD_GYROY,              //!< Gyroscope Y-axis
        IMD_GYROZ,              //!< Gyroscope Z-axis
        IMD_ACCX,               //!< Accelerometer X-axis
        IMD_ACCY,               //!< Accelerometer Y-axis
        IMD_ACCZ,               //!< Accelerometer Z-axis
        IMD_MAGX,               //!< Magnetometer X-axis
        IMD_MAGY,               //!< Magnetometer Y-axis
        IMD_MAGZ,               //!< Magnetometer Z-axis
        IMD_TIMESTAMP           //!< Timestamp of the motion data stream
    } IEE_MotionDataChannel_t;
    
    
    //! Windowing types enum for Fast Fourier Transform
    typedef enum IEE_WindowingTypes_enum {
        IEE_HANNING   = 0,      //!< Hanning Window
        IEE_HAMMING   = 1,      //!< Hamming Window
        IEE_HANN      = 2,      //!< Hann Window
        IEE_BLACKMAN  = 3,      //!< Blackman-Harris Window
        IEE_RECTANGLE = 4       //!< Uniform/rectangular Window
    } IEE_WindowingTypes;
    
    
    //! Initialize EmoEngine instance which reads data from the headset.
    /*!
        This function should be called at the beginning of programs that make use of EmoEngine, most probably in initialization routine or constructor.
     
        \return EDK_ERROR_CODE
                - EDK_OK if a connection is established

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_EngineConnect(const char* strDevID = "Emotiv Systems-5");

    
    //! Initialize the connection to a remote instance of EmoEngine.
    /*!
        Blocking call

        \param szHost - A null-terminated string identifying the hostname or IP address of the remote EmoEngine server
        \param port - The port number of the remote EmoEngine server
                    - If connecting to the Emotiv Control Panel, use port 3008
                    - If connecting to the EmoComposer, use port 1726
    
        \return EDK_ERROR_CODE
                - EDK_OK if a connection is established

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_EngineRemoteConnect(const char* szHost,
                                unsigned short port);
    

    //! Terminate the connection to EmoEngine.
    /*!
        This function should be called at the end of programs which make use of EmoEngine, most probably in clean up routine or destructor.
     
        \return EDK_ERROR_CODE
                - EDK_OK if disconnection is achieved

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_EngineDisconnect();


    //! Enable diagnostics mode.
    /*!
        Controls the output of logging information from EmoEngine (disabled by default).
        This should only be enabled if instructed to do so by Emotiv support for the purposes of collecting diagnostic information.
     
        \param szFilename - The path of the logfile
        \param fEnable - Write diagnostic information to logfile if enabled
        \param nReserved - Reserved for future use.

        \return EDK_ERROR_CODE
                - EDK_OK if the command succeeded
    */
    EDK_API int
        IEE_EnableDiagnostics(const char* szFilename,
                              int fEnable,
                              int nReserved);

    
    //! Return a handle to memory that can hold an EmoEngine event.
    /*!
        This handle can be reused by the caller to retrieve subsequent events.
     
        \return EmoEngineEventHandle
    */
    EDK_API EmoEngineEventHandle
        IEE_EmoEngineEventCreate();

    
    //! Free memory referenced by an event handle.
    /*!
        \param hEvent - a handle returned by IEE_EmoEngineEventCreate() or IEE_ProfileEventCreate()
    */
    EDK_API void
        IEE_EmoEngineEventFree(EmoEngineEventHandle hEvent);

    
    //! Return a handle to memory that can store an EmoState.
    /*!
        This handle can be reused by the caller to retrieve subsequent EmoStates.
     
        \return EmoStateHandle
    */
    EDK_API EmoStateHandle
        IEE_EmoStateCreate();

    
    //! Free memory referenced by an EmoState handle.
    /*!
        \param hState - a handle returned by IEE_EmoStateCreate()
    */
    EDK_API void
        IEE_EmoStateFree(EmoStateHandle hState);


    //! Return the event type for an event already retrieved using IEE_EngineGetNextEvent().
    /*!
        \param hEvent - a handle returned by IEE_EmoEngineEventCreate()
    
        \return IEE_Event_t
    */
    EDK_API IEE_Event_t
        IEE_EmoEngineEventGetType(EmoEngineEventHandle hEvent);


    //! Retrieve the user ID for IEE_UserAdded and IEE_UserRemoved events.
    /*!
        \param hEvent - a handle returned by IEE_EmoEngineEventCreate()
        \param pUserIdOut - receives the user ID associated with the current event

        \return EDK_ERROR_CODE
            - EDK_OK if successful

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_EmoEngineEventGetUserId(EmoEngineEventHandle hEvent,
                                    unsigned int *pUserIdOut);


    //! Copy an EmoState returned with a IEE_EmoStateUpdate event to memory referenced by an EmoStateHandle.
    /*!
        \param hEvent - a handle returned by IEE_EmoEngineEventCreate() and populated with IEE_EmoEngineGetNextEvent()
        \param hEmoState - a handle returned by IEE_EmoStateCreate()

        \return EDK_ERROR_CODE
            - EDK_OK if successful

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_EmoEngineEventGetEmoState(EmoEngineEventHandle hEvent,
                                      EmoStateHandle hEmoState);


    //! Retrieve the next EmoEngine event
    /*!
        Non-blocking call

        \param hEvent - a handle returned by IEE_EmoEngineEventCreate()

        \return EDK_ERROR_CODE
        - EDK_OK if an new event has been retrieved
        - EDK_NO_EVENT if no new events have been generated by EmoEngine

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_EngineGetNextEvent(EmoEngineEventHandle hEvent);


    //! Clear a specific EmoEngine event type or all events currently inside the event queue.
    /*!
        Event flags can be combined together as one argument except for IEE_UnknownEvent and IEE_EmulatorError.

        \param eventTypes - EmoEngine event type (IEE_Event_t), multiple events can be combined such as (IEE_UserAdded | IEE_UserRemoved)

        \return EDK_ERROR_CODE
        - EDK_OK if the events have been cleared from the queue
        - EDK_INVALID_PARAMETER if input event types are invalid

        \sa IEE_Event_t, IedkErrorCode.h
    */
    EDK_API int
        IEE_EngineClearEventQueue(int eventTypes);


    //! Retrieve number of active users (headset) connected to the EmoEngine.
    /*!
        \param pNumUserOut - receives number of users

        \return EDK_ERROR_CODE
            - EDK_OK if successful.

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_EngineGetNumUser(unsigned int* pNumUserOut);


    //! Set the player number display.
    /*!
        Sets the player number displayed on the physical input device (currently the USB Dongle) that corresponds to the specified user.

        \param userId - EmoEngine user ID
        \param playerNum - application assigned player number displayed on input device hardware (must be in the range 1-4)
        \return EDK_ERROR_CODE
            - EDK_OK if successful

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_SetHardwarePlayerDisplay(unsigned int userId,
                                     unsigned int playerNum);


	//! Get headset settings from EPOC+ headset.
	/*!
	    \remark Available for EPOC+ headset only. Headset settings can only be retrieved via USB connection.
     
	    \param userId       - user ID
	    \param EPOCmode	    - If 0, EPOC mode is EPOC.
	                        - If 1, EPOC mode is EPOC+.
	    \param eegRate      - If 0, EEG sample rate is 128Hz.
	                        - If 1, EEG sample rate is 256Hz.
	                        - If 2, no signal.
	    \param eegRes       - If 0, EEG resolution is 14bit.
	                        - If 1, EEG resolution is 16bit.
	                        - If 2, no signal.
	    \param memsRate     - If 0, motion sample rate is OFF.
	                        - If 1, motion sample rate is 32Hz.
	                        - If 2, motion sample rate is 64Hz.
	                        - If 3, motion sample rate is 128Hz.
	   \param memsRes       - If 0, motion resolution is 12bit.
	                        - If 1, motion resolution is 14bit.
	                        - If 2, motion resolution is 16bit.
	                        - If 3, no signal.

	    \return EDK_ERROR_CODE
                - EDK_OK if successful
	*/
    EDK_API int 
        IEE_GetHeadsetSettings(unsigned int userId, 
                               unsigned int* EPOCmode, 
                               unsigned int* eegRate, 
                               unsigned int* eegRes, 
                               unsigned int* memsRate, 
                               unsigned int* memsRes);


	//! Set headset setting for EPOC+ headset
	/*!
	    \remark Available for EPOC+ headset only. Headset settings can only be set via USB connection.
     
	    \param userId       - user ID
	    \param EPOCmode     - If 0, then EPOC mode is EPOC.
	                        - If 1, then EPOC mode is EPOC+.
	    \param eegRate      - If 0, then EEG sample rate is 128Hz.
	                        - If 1, then EEG sample rate is 256Hz.
	    \param eegRes       - If 1, then EEG resolution is 16bit. on EPOC+ mode
	    \param memsRate     - If 0, then motion sample rate is OFF.
	                        - If 1, then motion sample rate is 32Hz.
	                        - If 2, then motion sample rate is 64Hz.
	                        - If 3, then motion sample rate is 128Hz.
	    \param memsRes      - If 2, then motion resolution is 16bit.

	    \return EDK_ERROR_CODE
	            - EDK_OK if successful
	*/
    EDK_API int 
        IEE_SetHeadsetSettings(unsigned int userId, 
                               unsigned int EPOCmode, 
                               unsigned int eegRate, 
                               unsigned int eegRes, 
                               unsigned int memsRate, 
                               unsigned int memsRes);


    //! Return a struct containing details about a specific channel
    /*!
        \param channelId - channel identifier (see IEmoStateDll.h)
        \param pDescriptorOut - provides detailed sensor location and other info

        \return EDK_ERROR_CODE
            - EDK_OK if successful

        \sa IEmoStateDll.h, IedkErrorCode.h
    */
    EDK_API int
        IEE_HeadsetGetSensorDetails(IEE_InputChannels_t channelId,
                                    IInputSensorDescriptor_t* pDescriptorOut);


    //! Return the current hardware version of the headset and dongle (if available).
    /*!
        - 0x50XX / 0x90XX - Insight Consumer
        - 0x08XX / 0x09XX - Insight Premium
        - 0x30XX / 0x70XX - EPOC+ Consumer
        - 0x06XX / 0x07XX - EPOC+ Premium
        - 0x1000 / 0x1E00 - EPOC Consumer
        - 0x0565          - EPOC Premium

        \param userId - user ID for query
        \param pHwVersionOut - hardware version for the headset/dongle pair.
        - Upper 2 bytes: headset version
        - Lower 2 bytes: dongle version.

        \return EDK_ERROR_CODE
            - EDK_OK if successful

        \sa IEmoStateDll.h, IedkErrorCode.h
    */
    EDK_API int
        IEE_HardwareGetVersion(unsigned int userId,
                               unsigned long* pHwVersionOut);

    //! Return the current version of the Emotiv SDK
    /*!
        \param pszVersionOut - SDK software version in X.X.X format.
        \param nVersionChars - Length of char buffer pointed to by pszVersion argument.
        \param pBuildNumOut  - Build number. Unique for each release.

        \return EDK_ERROR_CODE
            - EDK_OK if successful

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_SoftwareGetVersion(char* pszVersionOut,
                               unsigned int nVersionChars,
                               unsigned long* pBuildNumOut);


    //! Return the current serial number of the headset (if available).
    /*!
        \param userId - user ID for query
        \param pHwSerialOut - serial number for the headset pair.
       
        \return EDK_ERROR_CODE
            - EDK_OK if successful

        \sa IedkErrorCode.h
    */
	EDK_API int
		IEE_HeadsetGetSerialNumber(unsigned int userId, 
			                        char** pHwSerialOut);


    //! Return the delta of the movement of the gyro since the previous call for a particular user
    /*!
        \param userId - user ID for query
        \param pXOut  - horizontal displacement
        \param pYOut  - vertical displacment

        \return EDK_ERROR_CODE
            - EDK_OK if successful

        \sa IEmoStateDll.h, IedkErrorCode.h
    */
    EDK_API int
        IEE_HeadsetGetGyroDelta(unsigned int userId,
                                int* pXOut,
                                int* pYOut);


    //! Re-zero the gyro for a particular user
    /*!
        \param userId - user ID for query

        \return EDK_ERROR_CODE
            - EDK_OK if successful

        \sa IEmoStateDll.h, IedkErrorCode.h
    */
    EDK_API int
        IEE_HeadsetGyroRezero(unsigned int userId);


    //! Return a handle to memory that can hold motion data.
    //  This handle can be reused by the caller to retrieve subsequent data.
    /*!
        \return DataHandle
    */
    EDK_API DataHandle
        IEE_MotionDataCreate();


    //! Free memory referenced by a data handle.
    /*!
        \param hData - a handle returned by IEE_MotionDataCreate()
    */
    EDK_API void
        IEE_MotionDataFree(DataHandle hData);


    //! Update the content of the data handle to point to new data since the last call
    /*!
        \param userId - user ID
        \param hData - a handle returned by IEE_MotionDataCreate()

        \return EDK_ERROR_CODE
            - EDK_OK if successful
    */
    EDK_API int
        IEE_MotionDataUpdateHandle(unsigned int userId,
                                   DataHandle hData);


    //! Extract data of a particular channel from the data handle
    /*!
        \param hData - a handle returned by IEE_MotionDataCreate()
        \param channel - channel that you are interested in
        \param buffer - pre-allocated buffer
        \param bufferSizeInSample - size of the pre-allocated buffer

        \return EDK_ERROR_CODE
            - EDK_OK if successful
    */
    EDK_API int
        IEE_MotionDataGet(DataHandle hData,
                          IEE_MotionDataChannel_t channel,
                          double buffer[],
                          unsigned int bufferSizeInSample);


    //! Extract data of a list of channels from the data handle
    /*!
        \param hData - a handle returned by IEE_MotionDataCreate()
        \param channels - a list of channel that you are interested in
        \param nChannels - number of channels in the channel list
        \param buffer - pre-allocated 2 dimensional buffer, has to be nChannels x bufferSizeInSample
        \param bufferSizeInSample - size of the pre-allocated buffer for each channel

        \return EDK_ERROR_CODE
            - EDK_OK if successful
    */
    EDK_API int
        IEE_MotionDataGetMultiChannels(DataHandle hData,
                                       IEE_MotionDataChannel_t channels[],
                                       unsigned int nChannels,
                                       double* buffer[],
                                       unsigned int bufferSizeInSample);


    //! Return number of sample of motion data stored in the data handle
    /*!
        \param hData - a handle returned by IEE_MotionDataCreate()
        \param nSampleOut - receives the number of sample of data stored in the data handle

        \return EDK_ERROR_CODE
            - EDK_OK if successful
    */
    EDK_API int
        IEE_MotionDataGetNumberOfSample(DataHandle hData,
                                        unsigned int* nSampleOut);


    //! Set the size of the motion data buffer.
    /*!
        The size of the buffer affects how frequent IEE_MotionDataUpdateHandle() needs to be called to prevent data loss.

        \param bufferSizeInSec - buffer size in second

        \return EDK_ERROR_CODE
            - EDK_OK if successful
    */
    EDK_API int
        IEE_MotionDataSetBufferSizeInSec(float bufferSizeInSec);


    //! Return the size of the motion data buffer
    /*!
        \param pBufferSizeInSecOut - receives the size of the data buffer

        \return EDK_ERROR_CODE
            - EDK_OK if successful
    */
    EDK_API int
        IEE_MotionDataGetBufferSizeInSec(float* pBufferSizeInSecOut);
    

    //! Get sampling rate of the motion data stream
    /*!
        \param userId - user ID
        \param samplingRateOut - receives the sampling rate

        \return EDK_ERROR_CODE
            - EDK_OK if successful
    */
    EDK_API int
        IEE_MotionDataGetSamplingRate(unsigned int userId,
                                      unsigned int* samplingRateOut);


    //! Enable/disable particular detections
    /*!
        By default, all detections are enabled.
        This method should be called before calling IEE_EngineConnect().
        If it is already connected, IEE_EngineDisconnect() should be called first before calling IEE_EngineConnect() again.
     
        \param value - bitwise value of detections to be enabled
     
        \sa IEE_CheckDetectionsEnabled(), IEE_Detection_t
     */
    EDK_API void
        IEE_EnableDetections(unsigned long value);
    
    
    //! Check if particular detections are enabled
    /*!
        \param result - store enabled detection bits in result
     
        \sa IEE_EnableDetections(), IEE_Detection_t
     */
    EDK_API void
        IEE_CheckDetectionsEnabled(unsigned long* result);

    
    //! Get averge band power values for a channel
    /*!
        Return the average band power for a specific channel from the latest epoch with
        0.5 seconds step size and 2 seconds window size.
     
        \param userId    - user ID
        \param channel   - channel that is interested in
        \param theta     - theta band value (4-8 Hz)
        \param alpha     - alpha band value (8-12 Hz)
        \param low_beta  - low-beta value (12-16 Hz)
        \param high_beta - high-beta value (16-25 Hz)
        \param gamma     - gamma value (25-45 Hz)

        \return EDK_ERROR_CODE
                - EDK_OK if successful
     
        \sa IedkErrorCode.h, IEE_FFTSetWindowingType
    */
    EDK_API int
        IEE_GetAverageBandPowers(unsigned int userId, IEE_DataChannel_t channel,
                                 double* theta, double* alpha, double* low_beta, double* high_beta, double* gamma);

    //! Set the current windowing type for band power calculation
    /*!
        \param userId - user ID
        \param type   - windowing type enum from IEE_WindowingTypes

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h, IEE_FFTGetWindowingType, IEE_GetAverageBandPowers
    */
    EDK_API int
        IEE_FFTSetWindowingType(unsigned int userId, IEE_WindowingTypes type);
    

    //! Get the current windowing type for band power calculation
    /*!
        \param userId - user ID
        \param type   - windowing type enum from IEE_WindowingTypes (default: IEE_HANNING)

        \return EDK_ERROR_CODE
                - EDK_OK if successful

        \sa IedkErrorCode.h, IEE_FFTSetWindowingType, IEE_GetAverageBandPowers
    */
    EDK_API int
        IEE_FFTGetWindowingType(unsigned int userId, IEE_WindowingTypes *type);


    //! Get the Edition name of Edk library. 
    /*!
        \param editionName - Edition Name

        \return EDK_ERROR_CODE
                    - EDK_OK if successful        
    */
    EDK_API int
        IEE_GetEditionName(char** editionName);
    
    
    //!
    //! The following API calls are only applicable for certain platforms to establish BTLE connection with the headset.
    //!
    
#if defined(__APPLE__)
    
    //! Initialize access to devices over BTLE
    /*!
        \remark Available on Mac/iOS only.
     
        Should be called before IEE_EngineConnect.
     
        \return true if initialised successfully
     */
    EDK_API bool
        IEE_EmoInitDevice();
    
#endif
    
#if defined(__APPLE__) || defined(__ANDROID__)
    
    //! Disconnect current device
    /*!
        \remark Available on Mac/iOS/Android only.

        \return true if connected successfully
     */
    EDK_API int
        IEE_DisconnectDevice();

    //! Connect to an Insight device
    /*!
        \remark Available on Mac/iOS/Android only.
     
        \param indexDevice - the index of device in list (start from 0)
     
        \return true if connected successfully
     */
    EDK_API int
        IEE_ConnectInsightDevice(int indexDevice);
    
    //! Get number of Insight devices nearby
    /*!
        \remark Available on Mac/iOS/Android only.
     
        \return number of Insight headsets
     */
    EDK_API int
        IEE_GetInsightDeviceCount();
    
    
    //! Get the name of an Insight device
    /*!
        The device name will include part of the serial number.
     
        \remark Available on Mac/iOS/Android only.

        \param index - index in device list
        \return const char* - name of the headset
     */
    EDK_API const char*
        IEE_GetInsightDeviceName(int index);

    
    //! Get current state of an Insight device
    /*!
        \remark Available on Mac/iOS/Android only.

        \param state - 0: if device not connect
                       1: if device is connected with another app in system
        \param index - index in device list
     */
    EDK_API void
        IEE_GetInsightDeviceState(int* state, int index);

    
    //! Connect to an EPOC+ device
    /*!
        \remark Available on Mac/iOS/Android only.
     
        \param indexDevice  -  the order of device in list (start from 0)
        \param isSettingMode - if true the get data feature will be disabled
        
        \return true if connect successfully
     */
    EDK_API int
        IEE_ConnectEpocPlusDevice(int indexDevice, bool isSettingMode=false);
    
    //! Get number of EPOC+ devices nearby
    /*!
        \remark Available on Mac/iOS/Android only.
     
        \return number of EPOC+ headsets
     */
    EDK_API int
        IEE_GetEpocPlusDeviceCount();
    
    
    //! Get the name of an EPOC+ device
    /*!
        The device name will include part of the serial number.
     
        \remark Available on Mac/iOS/Android only.
     
        \param index - index in device list
     
        \return const char* - name of device
     */
    EDK_API const char*
        IEE_GetEpocPlusDeviceName(int index);

    
    //! Get current state of an EPOC+ device
    /*!
        \remark Available on Mac/iOS/Android only.
     
        \param state - 0: if device not connect
                       1: if device is connected with another app in system
        \param index - index in device list
     */
    EDK_API void
        IEE_GetEpocPlusDeviceState(int* state, int index);
    
#endif
    
#ifdef __cplusplus
}
#endif
#endif // IEDK_H
