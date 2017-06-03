/**
 * Emotiv Development Kit (EDK) API
 * Copyright (c) 2016 Emotiv Inc.
 *
 * The main interface that allows interactions between external programs and the Emotiv detection engine.
 *
 * None of the API functions are thread-safe.
 *
 * This header file is designed to be includable under C and C++ environment.
 *
 */


#ifndef EDK_H
#define EDK_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EDK_STATIC_LIB
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
#   include "EmotivProfile.h"
#   include "EmoStateAffectiv.h"
#   define EDK_API extern
#endif


#include "edkErrorCode.h"
#include "EmoStateDLL.h"
#include "CognitivDetection.h"
#include "ExpressivDetection.h"

#ifdef __APPLE__
//! Mode of Headset
typedef enum EE_ModeHeadset_enum {
    MODE_EPOC = 0 ,
    MODE_EPOCPLUS_NOMOTION,
    MODE_EPOCPLUS_MOTION
} EE_ModeHeadset_t;
#endif


//! Handle to internal EmoState structure allocated by EE_EmoStateCreate()
typedef void     *    EmoStateHandle;


//! Handle to internal event structure allocated by EE_EmoEngineEventCreate()
typedef void     *    EmoEngineEventHandle;


//! Handle to internal event structure allocated by EE_OptimizationParamCreate()
typedef void     *    OptimizationParamHandle;


typedef void     *    DataHandle;

typedef enum EE_DataChannels_enum {
    ED_COUNTER = 0,        //!< Sample counter
    ED_INTERPOLATED,       //!< Indicate if data is interpolated
    ED_RAW_CQ,             //!< Raw contact quality value
    ED_AF3,                //!< Channel AF3
    ED_F7,                 //!< Channel F7
    ED_F3,                 //!< Channel F3
    ED_FC5,                //!< Channel FC5
    ED_T7,                 //!< Channel T7
    ED_P7,                 //!< Channel P7
    ED_O1,                 //!< Channel O1
    ED_O2,                 //!< Channel O2
    ED_P8,                 //!< Channel P8
    ED_T8,                 //!< Channel T8
    ED_FC6,                //!< Channel FC6
    ED_F4,                 //!< Channel F4
    ED_F8,                 //!< Channel F8
    ED_AF4,                //!< Channel AF4
    ED_GYROX,              //!< Gyroscope X-axis
    ED_GYROY,              //!< Gyroscope Y-axis
    ED_TIMESTAMP,          //!< System timestamp
    ED_MARKER_HARDWARE,    //!< Marker value from hardware
    ED_ES_TIMESTAMP,       //!< EmoState timestamp
    ED_FUNC_ID,            //!< Reserved function id
    ED_FUNC_VALUE,         //!< Reserved function value
    ED_MARKER,             //!< Marker value from software
    ED_SYNC_SIGNAL         //!< Synchronisation signal
} EE_DataChannel_t;

//! EmoEngine event types
typedef enum EE_Event_enum {
    EE_UnknownEvent     = 0x0000,
    EE_EmulatorError    = 0x0001,
    EE_ReservedEvent    = 0x0002,
    EE_UserAdded        = 0x0010,
    EE_UserRemoved      = 0x0020,
    EE_EmoStateUpdated  = 0x0040,
    EE_ProfileEvent     = 0x0080,
    EE_CognitivEvent    = 0x0100,
    EE_ExpressivEvent   = 0x0200,
    EE_InternalStateChanged = 0x0400,
    EE_AllEvent         = EE_UserAdded | EE_UserRemoved | EE_EmoStateUpdated | EE_ProfileEvent |
                          EE_CognitivEvent | EE_ExpressivEvent | EE_InternalStateChanged
} EE_Event_t;


typedef enum EE_MotionDataChannels_enum {
    MD_COUNTER = 0,     //!< Sample counter
    MD_GYROX,           //!< Gyroscope X-axis
    MD_GYROY,           //!< Gyroscope Y-axis
    MD_GYROZ,           //!< Gyroscope Z-axis
    MD_ACCX,            //!< Accelerometer X-axis
    MD_ACCY,            //!< Accelerometer Y-axis
    MD_ACCZ,            //!< Accelerometer Z-axis
    MD_MAGX,            //!< Magnetometer X-axis
    MD_MAGY,            //!< Magnetometer Y-axis
    MD_MAGZ,            //!< Magnetometer Z-axis
    MD_TIMESTAMP        //!< Timestamp of the motion data stream
} EE_MotionDataChannel_t;


//! Input sensor description
typedef struct InputSensorDescriptor_struct {
    EE_InputChannels_t channelId;  // logical channel id
    int                fExists;    // does this sensor exist on this headset model
    const char    *    pszLabel;   // text label identifying this sensor
    double             xLoc;       // x coordinate from center of head towards nose
    double             yLoc;       // y coordinate from center of head towards ears
    double
    zLoc;       // z coordinate from center of head toward top of skull
} InputSensorDescriptor_t;


//! Windowing types enum for Fast Fourier Transform
typedef enum EE_WindowingTypes_enum {
    EE_HANNING   = 0,       //!< Hanning Window
    EE_HAMMING   = 1,       //!< Hamming Window
    EE_HANN      = 2,       //!< Hann Window
    EE_BLACKMAN  = 3,       //!< Blackman-Harris Window
    EE_RECTANGLE = 4        //!< Uniform/rectangular Window
} EE_WindowingTypes;


//! Initializes EmoEngine instance which reads EEG data from EPOC. This function should be called at the beginning of programs that make use of EmoEngine, most probably in initialization routine or constructor.
/*!
    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if a connection is established

    \sa edkErrorCode.h
*/
EDK_API int
EE_EngineConnect(const char * strDevID = "Emotiv Systems-5");


//! Initializes the connection to a remote instance of EmoEngine.
/*!
    Blocking call

    \param szHost - A null-terminated string identifying the hostname or IP address of the remote EmoEngine server
    \param port - The port number of the remote EmoEngine server
                - If connecting to the Emotiv Control Panel, use port 3008
                - If connecting to the EmoComposer, use port 1726

    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if a connection is established

    \sa edkErrorCode.h
*/
EDK_API int
EE_EngineRemoteConnect(const char * szHost, unsigned short port);


//! Terminates the connection to EmoEngine. This function should be called at the end of programs which make use of EmoEngine, most probably in clean up routine or destructor.
/*!
    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if disconnection is achieved

    \sa edkErrorCode.h
*/
EDK_API int
EE_EngineDisconnect();


//! Controls the output of logging information from EmoEngine (which is off by default). This should only be enabled if instructed to do so by Emotiv developer support for the purposes of collecting diagnostic information.
/*!
    \param szFilename - The path of the logfile
    \param fEnable - If non-zero, then diagnostic information will be written to logfile.
                   - If zero, then all diagnostic information is suppressed (default).
    \param nReserved - Reserved for future use.

    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if the command succeeded
*/
EDK_API int
EE_EnableDiagnostics(const char * szFilename, int fEnable, int nReserved);


//! Returns a handle to memory that can hold an EmoEngine event. This handle can be reused by the caller to retrieve subsequent events.
/*!
    \return EmoEngineEventHandle
*/
EDK_API EmoEngineEventHandle
EE_EmoEngineEventCreate();


//! Frees memory referenced by an event handle.
/*!
    \param hEvent - a handle returned by EE_EmoEngineEventCreate() or EE_ProfileEventCreate()
*/
EDK_API void
EE_EmoEngineEventFree(EmoEngineEventHandle hEvent);


//! Returns a handle to memory that can store an EmoState. This handle can be reused by the caller to retrieve subsequent EmoStates.
/*!
    \return EmoStateHandle
*/
EDK_API EmoStateHandle
EE_EmoStateCreate();


//! Frees memory referenced by an EmoState handle.
/*!
    \param hState - a handle returned by EE_EmoStateCreate()
*/
EDK_API void
EE_EmoStateFree(EmoStateHandle hState);


//! Returns the event type for an event already retrieved using EE_EngineGetNextEvent.
/*!
    \param hEvent - a handle returned by EE_EmoEngineEventCreate()

    \return EE_Event_t
*/
EDK_API EE_Event_t
EE_EmoEngineEventGetType(EmoEngineEventHandle hEvent);


//! Retrieves the user ID for EE_UserAdded and EE_UserRemoved events.
/*!
    \param hEvent - a handle returned by EE_EmoEngineEventCreate()
    \param pUserIdOut - receives the user ID associated with the current event

    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful.

    \sa edkErrorCode.h
*/
EDK_API int
EE_EmoEngineEventGetUserId(EmoEngineEventHandle hEvent,
                           unsigned int * pUserIdOut);


//! Copies an EmoState returned with a EE_EmoStateUpdate event to memory referenced by an EmoStateHandle.
/*!
    \param hEvent - a handle returned by EE_EmoEngineEventCreate() and populated with EE_EmoEngineGetNextEvent()
    \param hEmoState - a handle returned by EE_EmoStateCreate

    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful.

    \sa edkErrorCode.h
*/
EDK_API int
EE_EmoEngineEventGetEmoState(EmoEngineEventHandle hEvent,
                             EmoStateHandle hEmoState);


//! Retrieves the next EmoEngine event
/*!
    Non-blocking call

    \param hEvent - a handle returned by EE_EmoEngineEventCreate()

    \return EDK_ERROR_CODE
            <ul>
            <li> EDK_ERROR_CODE = EDK_OK if a new event has been retrieved
            <li> EDK_ERROR_CODE = EDK_NO_EVENT if no new events have been generated by EmoEngine
            </ul>

    \sa edkErrorCode.h
*/
EDK_API int
EE_EngineGetNextEvent(EmoEngineEventHandle hEvent);


//! Clear a specific EmoEngine event type or all events currently inside the event queue. Event flags can be combined together as one argument except EE_UnknownEvent and EE_EmulatorError.
/*!
    \param eventTypes - EmoEngine event type (EE_Event_t), multiple events can be combined such as (EE_UserAdded | EE_UserRemoved)

    \return EDK_ERROR_CODE
            <ul>
            <li> EDK_ERROR_CODE = EDK_OK if the events have been cleared from the queue
            <li> EDK_ERROR_CODE = EDK_INVALID_PARAMETER if input event types are invalid
            </ul>

    \sa EE_Event_t, edkErrorCode.h
*/
EDK_API int
EE_EngineClearEventQueue(int eventTypes);


//! Retrieves number of active users connected to the EmoEngine.
/*!
    \param pNumUserOut - receives number of users

    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful.

    \sa edkErrorCode.h
*/
EDK_API int
EE_EngineGetNumUser(unsigned int * pNumUserOut);


//! Sets the player number displayed on the physical input device (currently the USB Dongle) that corresponds to the specified user
/*!
    \param userId - EmoEngine user ID
    \param playerNum - application assigned player number displayed on input device hardware (must be in the range 1-4)
    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful

    \sa edkErrorCode.h
*/
EDK_API int
EE_SetHardwarePlayerDisplay(unsigned int userId, unsigned int playerNum);


//! Get headset settings
/*!
    \param userId - user ID
    \param EPOCmode     - If 0, EPOC mode is EPOC.
                        - If 1, EPOC mode is EPOC+.
    \param eegRate      - If 0, EEG sample rate is 128Hz.
                        - If 1, EEG sample rate is 256Hz.
                        - If 2, no signals.
    \param eegRes       - If 0, EEG Resolution is 14bit.
                        - If 1, EEG Resolution is 16bit.
                        - If 2, no signals.
    \param memsRate     - If 0, MEMS sample rate is OFF.
                        - If 1, MEMS sample rate is 32Hz.
                        - If 2, MEMS sample rate is 64Hz.
                        - If 3, MEMS sample rate is 128Hz.
    \param memsRes      - If 0, MEMS Resolution is 12bit.
                        - If 1, MEMS Resolution is 14bit.
                        - If 2, MEMS Resolution is 16bit.
                        - If 3, no signals.

    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if the command successful
*/
EDK_API int EE_GetHeadsetSettings(unsigned int userId, unsigned int * EPOCmode,
                                  unsigned int * eegRate, unsigned int * eegRes, unsigned int * memsRate,
                                  unsigned int * memsRes);


//! Set headset setting
/*!
    \param userId - user ID
    \param EPOCmode     - If 0, then EPOC mode is EPOC.
                        - If 1, then EPOC mode is EPOC+.
    \param eegRate      - If 0, then EEG sample rate is 128Hz.
                        - If 1, then EEG sample rate is 256Hz.
    \param eegRes       - If 0, then EEG Resolution is 14bit.
                        - If 1, then EEG Resolution is 16bit.
    \param memsRate     - If 0, then MEMS sample rate is OFF.
                        - If 1, then MEMS sample rate is 32Hz.
                        - If 2, then MEMS sample rate is 64Hz.
                        - If 3, then MEMS sample rate is 128Hz.
    \param memsRes      - If 0, then MEMS Resolution is 12bit.
                        - If 1, then MEMS Resolution is 14bit.
                        - If 2, then MEMS Resolution is 16bit.

    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if the command successful
*/
EDK_API int EE_SetHeadsetSettings(unsigned int userId, unsigned int EPOCmode,
                                  unsigned int eegRate, unsigned int eegRes, unsigned int memsRate,
                                  unsigned int memsRes);


//! Returns a struct containing details about the specified EEG channel's headset
/*!
    \param channelId - channel identifier (see EmoStateDll.h)
    \param pDescriptorOut - provides detailed sensor location and other info

    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful

    \sa EmoStateDll.h, edkErrorCode.h
*/
EDK_API int
EE_HeadsetGetSensorDetails(EE_InputChannels_t channelId,
                           InputSensorDescriptor_t * pDescriptorOut);


//! Returns the current hardware version of the headset and dongle for a particular user
/*!
    \param userId - user ID for query
    \param pHwVersionOut - hardware version for the user headset/dongle pair. hiword is headset version, loword is dongle version.

    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful

    \sa EmoStateDll.h, edkErrorCode.h
*/
EDK_API int
EE_HardwareGetVersion(unsigned int userId, unsigned long * pHwVersionOut);


//! Returns the current version of the Emotiv SDK software
/*!
    \param pszVersionOut - SDK software version in X.X.X.X format. Note: current beta releases have a major version of 0.
    \param nVersionChars - Length of char buffer pointed to by pszVersion argument.
    \param pBuildNumOut  - Build number.  Unique for each release.

    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful

    \sa edkErrorCode.h
*/
EDK_API int
EE_SoftwareGetVersion(char * pszVersionOut, unsigned int nVersionChars,
                      unsigned long * pBuildNumOut);


//! Returns the delta of the movement of the gyro since the previous call for a particular user
/*!
    \param userId - user ID for query
    \param pXOut  - horizontal displacement
    \param pYOut  - vertical displacment

    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful

    \sa EmoStateDll.h
    \sa edkErrorCode.h
*/
EDK_API int
EE_HeadsetGetGyroDelta(unsigned int userId, int * pXOut, int * pYOut);


//! Re-zero the gyro for a particular user
/*!
    \param userId - user ID for query

    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful

    \sa EmoStateDll.h
    \sa edkErrorCode.h
*/
EDK_API int
EE_HeadsetGyroRezero(unsigned int userId);


//! Returns a handle to memory that can hold an optimization paramaeter which is used to configure the behaviour of optimization
/*!
    \return OptimizationParamHandle
*/
EDK_API OptimizationParamHandle
EE_OptimizationParamCreate();


//! Frees memory referenced by an optimization parameter handle
/*!
    \param hParam - a handle returned by EE_OptimizationParamCreate()
*/
EDK_API void
EE_OptimizationParamFree(OptimizationParamHandle hParam);


//! Enable optimization. EmoEngine will try to optimize its performance according to the information passed in with optimization parameter. EmoEngine guarantees the correctness of the results of vital algorithms. For algorithms that are not vital, results are undefined.
/*!
    \param hParam - a handle returned by EE_OptimizationParamCreate()
    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful
*/
EDK_API int
EE_OptimizationEnable(OptimizationParamHandle hParam);


//! Determine whether optimization is on
/*!
    \param pEnabledOut - receives information about whether optimization is on
    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful
*/
EDK_API int
EE_OptimizationIsEnabled(bool * pEnabledOut);


//! Disable optimization
/*!
    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful
*/
EDK_API int
EE_OptimizationDisable();


//! Get optimization parameter.  If optimization is not enabled (this can be checked with EE_OptimmizationIsEnabled) then the results attached to the hParam parameter are undefined.
/*!
    \param hParam - a handle returned by EE_OptimizationParamCreate()
    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful
*/
EDK_API int
EE_OptimizationGetParam(OptimizationParamHandle hParam);


//! Get a list of vital algorithms of specific suite from optimization parameter
/*!
    \param hParam - a handle returned by EE_OptimizationParamCreate()
    \param suite - suite that you are interested in
    \param pVitalAlgorithmBitVectorOut - receives a list of vital algorithm composed of EE_ExpressivAlgo_t, EE_AffectivAlgo_t or EE_CognitivAction_t depending on the suite parameter
    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful
*/
EDK_API int
EE_OptimizationGetVitalAlgorithm(OptimizationParamHandle hParam,
                                 EE_EmotivSuite_t suite, unsigned int * pVitalAlgorithmBitVectorOut);


//! Set a list of vital algorithms of specific suite to optimization parameter
/*!
    \param hParam - a handle returned by EE_OptimizationParamCreate()
    \param suite - suite that you are interested in
    \param vitalAlgorithmBitVector - a list of vital algorithm composed of EE_ExpressivAlgo_t, EE_AffectivAlgo_t or EE_CognitivAction_t depended on the suite parameter passed in
    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful
*/
EDK_API int
EE_OptimizationSetVitalAlgorithm(OptimizationParamHandle hParam,
                                 EE_EmotivSuite_t suite, unsigned int vitalAlgorithmBitVector);


//! Resets all settings and user-specific profile data for the specified detection suite
/*!
    \param userId - user ID
    \param suite - detection suite (Expressiv, Affectiv, or Cognitiv)
    \param detectionBitVector - identifies specific detections.  Set to zero for all detections.
    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful
*/
EDK_API int EE_ResetDetection(unsigned int userId, EE_EmotivSuite_t suite,
                              unsigned int detectionBitVector);


/*
    Check Customer ID code
*/
EDK_API double  EE_GetSecurityCode(void);       //get id code from EDK


EDK_API int     EE_CheckSecurityCode(double x); //check your Id code


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

    \sa IedkErrorCode.h, EE_FFTSetWindowingType
 */
EDK_API int
EE_GetAverageBandPowers(unsigned int userId, EE_DataChannel_t channel,
                        double * theta, double * alpha, double * low_beta, double * high_beta,
                        double * gamma);


//! Set the current windowing type for band power calculation
/*!
    \param userId - user ID
    \param type   - windowing type enum from EE_WindowingTypes

    \return EDK_ERROR_CODE
            - EDK_OK if successful

    \sa IedkErrorCode.h, EE_FFTGetWindowingType, EE_GetAverageBandPowers
*/
EDK_API int
EE_FFTSetWindowingType(unsigned int userId, EE_WindowingTypes type);


//! Get the current windowing type for band power calculation
/*!
    \param userId - user ID
    \param type   - windowing type enum from EE_WindowingTypes (default: EE_HANNING)

    \return EDK_ERROR_CODE
            - EDK_OK if successful

    \sa IedkErrorCode.h, EE_FFTSetWindowingType, EE_GetAverageBandPowers
*/
EDK_API int
EE_FFTGetWindowingType(unsigned int userId, EE_WindowingTypes * type);


//! Returns a handle to memory that can hold motion data.
//  This handle can be reused by the caller to retrieve subsequent data.
/*!
    \return DataHandle
*/
EDK_API DataHandle EE_MotionDataCreate();


//! Frees memory referenced by a data handle.
/*!
    \param hData - a handle returned by EE_DataCreate()
*/
EDK_API void EE_MotionDataFree(DataHandle hData);


//! Updates the content of the data handle to point to
//! new data since the last call
/*!
    \param userId - user ID
    \param hData - a handle returned by EE_MotionDataCreate()
    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful
*/
EDK_API int EE_MotionDataUpdateHandle(unsigned int userId, DataHandle hData);


//! Extracts data of a particulat channel from the data handle
/*!
    \param hData - a handle returned by EE_MotionDataCreate()
    \param channel - channel that you are interested in
    \param buffer - pre-allocated buffer
    \param bufferSizeInSample - size of the pre-allocated buffer
    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful
*/
EDK_API int EE_MotionDataGet(DataHandle hData, EE_MotionDataChannel_t channel,
                             double buffer[],
                             unsigned int bufferSizeInSample);


//! Extracts data of a list of channels from the data handle
/*!
    \param hData - a handle returned by EE_MotionDataCreate()
    \param channels - a list of channel that you are interested in
    \param nChannel - number of channels in the channel list
    \param buffer - pre-allocated 2 dimensional buffer, has to be nChannels x bufferSizeInSample
    \param bufferSizeInSample - size of the pre-allocated buffer for each channel
    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful
*/
EDK_API int EE_MotionDataGetMultiChannels(DataHandle hData,
        EE_MotionDataChannel_t channels[],
        unsigned int nChannels, double * buffer[],
        unsigned int bufferSizeInSample);


//! Returns number of sample of motion data stored in the data handle
/*!
    \param hData - a handle returned by EE_MotionDataCreate()
    \param nSampleOut - receives the number of sample of data stored in the data handle
    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful
*/
EDK_API int EE_MotionDataGetNumberOfSample(DataHandle hData,
        unsigned int * nSampleOut);


//! Sets the size of the motion data buffer.
//! The size of the buffer affects how frequent EE_DataUpdateHandle() needs
//! to be called to prevent data loss.
/*!
    \param bufferSizeInSec - buffer size in second
    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful
*/
EDK_API int EE_MotionDataSetBufferSizeInSec(float bufferSizeInSec);


//! Returns the size of the motion data buffer
/*!
    \param pBufferSizeInSecOut - receives the size of the data buffer
    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if successful
*/
EDK_API int EE_MotionDataGetBufferSizeInSec(float * pBufferSizeInSecOut);


//! Gets sampling rate
/*!
    \param userId - user ID
    \param samplingRateOut - receives the sampling rate
    \return EDK_ERROR_CODE
            - EDK_ERROR_CODE = EDK_OK if the command succeeded
*/
EDK_API int EE_MotionDataGetSamplingRate(unsigned int userId,
        unsigned int * samplingRateOut);

#ifdef __APPLE__
//! Init Bluetooth of device
/*!
 \return true if init successfully
 */
EDK_API bool EE_EmoInitDevice();
//        //! Connect Device with ID
//        /*!
//         \param indexDevice  -  the order of device in list (start with 0)
//         \param isSettingMode - if true disable feature get data until setting succes
//         \return true if connect successfully
//         */
//     EDK_API int EE_EmoConnectDevice(int indexDevice,bool isSettingMode= false);
//     //! Get Signal Strength of Device
//         /*!
//          \param int value
//          \param indexDevice - the index of device in list (start with 0)
//          */
//     EDK_API void EE_GetSignalStrengthBLEEPOCPLUS(int& value, int indexDevice);
//        //! Get Number of Device Epoc Plus Headset
//        /*!
//         \return int
//         */
//     EDK_API int EE_GetNumberDeviceEpocPlus();
//        //! Get Name of  Headset in list device
//        /*!
//         \param int index in list device
//         \return const char* name of device
//         */
//     EDK_API const char* EE_GetNameDeviceEpocAtIndex(int index);
//        //! Switch mode of Epoc Headset
//        /*!
//         \param value - Mode of Headset
//         \return true if setting succes
//         */
//      int EE_EmoSettingMode(EE_ModeHeadset_t value);
#endif


#ifdef TRIAL_VERSION
EDK_API int
EE_GetRemainTrialDays(int & days);
#endif

#ifdef __cplusplus
}
#endif

#endif
