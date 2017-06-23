/**
 * Emotiv SDK
 * Copyright (c) 2016 Emotiv Inc.
 *
 * This file is part of the Emotiv SDK.
 *
 * Header file for EEG data related API.
 *
 */


#ifndef IEEGDATA_H
#define IEEGDATA_H

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
#define EDK_API extern
#endif

#include "Iedk.h"

	//! Handle to data placeholder allocated by IEE_DataCreate.
	/*!
	    \sa IEE_DataCreate()
	*/
	typedef void * DataHandle;


	//! Return a handle to memory that can hold EEG data.
	/*!
	    This handle can be reused by the caller to retrieve subsequent data.

	    \remark Only available in SDK Premium Edition.

		\return DataHandle
    */
	EDK_API DataHandle
		IEE_DataCreate();


	//! Free memory referenced by a data handle.
	/*!
	    \remark Only available in SDK Premium Edition.

		\param hData - a handle returned by IEE_DataCreate()
    */
	EDK_API void
		IEE_DataFree(DataHandle hData);


	//! Update the content of the data handle to point to new data since the last call.
	/*!
	    \remark Only available in SDK Premium Edition.

		\param userId - user ID
		\param hData - a handle returned by IEE_DataCreate()
		\return EDK_ERROR_CODE
		        - EDK_OK if successful
    */
	EDK_API int
		IEE_DataUpdateHandle(unsigned int userId,
		                     DataHandle hData);


	//! Extract data of a particulat channel from the data handle.
	/*!
	    \remark Only available in SDK Premium Edition.

		\param hData - a handle returned by IEE_DataCreate()
		\param channel - channel that you are interested in
		\param buffer - pre-allocated buffer
		\param bufferSizeInSample - size of the pre-allocated buffer
		\return EDK_ERROR_CODE
		         - EDK_OK if successful
    */
	EDK_API int
		IEE_DataGet(DataHandle hData,
		            IEE_DataChannel_t channel,
					double buffer[],
					unsigned int bufferSizeInSample);


	//! Extract data of a list of channels from the data handle.
	/*!
	   \remark Only available in SDK Premium Edition.

	   \param hData - a handle returned by IEE_DataCreate()
	   \param channels - a list of channel that you are interested in
	   \param nChannels - number of channels in the channel list
	   \param buffer - pre-allocated 2 dimensional buffer, has to be nChannels x bufferSizeInSample
	   \param bufferSizeInSample - size of the pre-allocated buffer for each channel
	   \return EDK_ERROR_CODE
	           - EDK_OK if successful
    */
	EDK_API int
		IEE_DataGetMultiChannels(DataHandle hData,
		                         IEE_DataChannel_t channels[],
		                         unsigned int nChannels,
								 double * buffer[],
								 unsigned int bufferSizeInSample);


	//! Return number of sample of data stored in the data handle.
	/*!
	    \remark Only available in SDK Premium Edition.

		\param hData - a handle returned by IEE_DataCreate()
		\param nSampleOut - receives the number of sample of data stored in the data handle
		\return EDK_ERROR_CODE
		    - EDK_OK if successful
    */
	EDK_API int
		IEE_DataGetNumberOfSample(DataHandle hData,
		                          unsigned int * nSampleOut);


	//! Set the size of the data buffer.
	/*!
	    The size of the buffer affects how frequent IEE_DataUpdateHandle() needs to be called to prevent data loss.

		\remark Only available in SDK Premium Edition.

		\param bufferSizeInSec - buffer size in second
		\return EDK_ERROR_CODE
		    - EDK_OK if successful
    */
	EDK_API int
		IEE_DataSetBufferSizeInSec(float bufferSizeInSec);


	//! Return the size of the data buffer.
	/*!
        \remark Only available in SDK Premium Edition.

		\param pBufferSizeInSecOut - receives the size of the data buffer
		\return EDK_ERROR_CODE
		    - EDK_OK if successful
    */
	EDK_API int
		IEE_DataGetBufferSizeInSec(float * pBufferSizeInSecOut);


	//! Control data acquisition inside EmoEngine (disabled by default).
	/*!
        \remark Only available in SDK Premium Edition.

		\param userId - user ID
		\param enable - enable if true
		\return EDK_ERROR_CODE
		    - EDK_OK if the command succeeded
    */
	EDK_API int
		IEE_DataAcquisitionEnable(unsigned int userId,
		                          bool enable);


	//! Check if data acquisition is enabled.
	/*!
	    \remark Only available in SDK Premium Edition.

		\param userId - user ID
		\param pEnableOut - get whether data acquisition is enabled
		\return EDK_ERROR_CODE
		    - EDK_OK if the command succeeded
    */
	EDK_API int
		IEE_DataAcquisitionIsEnabled(unsigned int userId,
		                             bool * pEnableOut);


	//! Insert sychronization signal to the data stream.
	/*!
        \remark Only available in SDK Premium Edition.

		\param userId - user ID
		\param signal - value of the sychronization signal
		\return EDK_ERROR_CODE
		    - EDK_OK if the command succeeded
    */
	EDK_API int
		IEE_DataSetSychronizationSignal(unsigned int userId,
		                                int signal);


	//! Insert marker to the data stream.
	/*!
        \remark Only available in SDK Premium Edition.

		\param userId - user ID
		\param marker - value of the marker
		\return EDK_ERROR_CODE
		    - EDK_OK if the command succeeded
    */
	EDK_API int
		IEE_DataSetMarker(unsigned int userId,
		                  int marker);


	//! Get sampling rate of the EEG data stream.
	/*!
        \remark Only available in SDK Premium Edition.

		\param userId - user ID
		\param samplingRateOut - receives the sampling rate
		\return EDK_ERROR_CODE
		    - EDK_OK if the command succeeded
    */
	EDK_API int
		IEE_DataGetSamplingRate(unsigned int userId,
		                        unsigned int * samplingRateOut);

#ifdef __cplusplus
}
#endif
#endif // IEEGDATA_H
