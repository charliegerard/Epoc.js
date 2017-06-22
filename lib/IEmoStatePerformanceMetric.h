/**
 * Emotiv SDK
 * Copyright (c) 2016 Emotiv Inc.
 *
 * This file is part of the Emotiv SDK.
 *
 * Header file for Performance Metrics related API.
 *
 */

#ifndef IEMOSTATEPERFORMANCEMETRIC_H
#define IEMOSTATEPERFORMANCEMETRIC_H

#ifdef __cplusplus
extern "C" {
#endif

#if (!EDK_STATIC_LIB)
#   ifdef EMOSTATE_DLL_EXPORTS
#       ifdef _WIN32
#           define EMOSTATE_DLL_API __declspec(dllexport)
#       else
#           if (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER || defined __clang__
#               define EMOSTATE_DLL_API __attribute__ ((visibility("default")))
#           else
#               define EMOSTATE_DLL_API
#           endif
#       endif
#   else
#       ifdef _WIN32
#           define EMOSTATE_DLL_API __declspec(dllimport)
#       else
#           define EMOSTATE_DLL_API
#       endif
#   endif
#else
#   define EMOSTATE_DLL_API extern
#endif

#include "IEmoStateDLL.h"

	//! PerformanceMetric emotional type enumerator
	typedef enum IEE_PerformanceMetricAlgo_enum {

		PM_EXCITEMENT = 0x0001,
		PM_RELAXATION = 0x0002,
		PM_STRESS     = 0x0004,
		PM_ENGAGEMENT = 0x0008,

		PM_INTEREST   = 0x0010,
		PM_FOCUS      = 0x0020

	} IEE_PerformanceMetricAlgo_t;

	//! Returns the long term excitement level of the user
	/*!
        \param state - EmoStateHandle
		
		\return excitement level (0.0 to 1.0)

		\sa IS_PerformanceMetricGetExcitementShortTermScore
    */
	EMOSTATE_DLL_API float
		IS_PerformanceMetricGetExcitementLongTermScore(EmoStateHandle state);


	//! Returns short term excitement level of the user
	/*!
        \param state - EmoStateHandle

		\return excitement level (0.0 to 1.0)

		\sa IS_PerformanceMetricGetExcitementLongTermScore
    */
	EMOSTATE_DLL_API float
		IS_PerformanceMetricGetInstantaneousExcitementScore(EmoStateHandle state);


	//! Query whether the signal is too noisy for PerformanceMetric detection to be active
	/*!
        \param state - EmoStateHandle
		\param type  - PerformanceMetric detection type

		\return detection state (0: Not Active, 1: Active)

		\sa IEE_PerformanceMetricAlgo_t
    */
	EMOSTATE_DLL_API int
		IS_PerformanceMetricIsActive(EmoStateHandle state,
		                             IEE_PerformanceMetricAlgo_t type);


	//! Returns relaxation level of the user
	/*!
	    \param state - EmoStateHandle

		\return relaxation level (0.0 to 1.0)
    */
	EMOSTATE_DLL_API float
		IS_PerformanceMetricGetRelaxationScore(EmoStateHandle state);


	//! Returns stress level of the user
	/*!
        \param state - EmoStateHandle

		\return stress level (0.0 to 1.0)
    */
	EMOSTATE_DLL_API float
		IS_PerformanceMetricGetStressScore(EmoStateHandle state);


	//! Returns engagement/boredom level of the user
	/*!
	    \param state - EmoStateHandle

		\return engagement/boredom level (0.0 to 1.0)
    */
	EMOSTATE_DLL_API float
		IS_PerformanceMetricGetEngagementBoredomScore(EmoStateHandle state);


	//! Returns interest level of the user
	/*!
        \param state - EmoStateHandle

        \return interest level (0.0 to 1.0)
    */
	EMOSTATE_DLL_API float
		IS_PerformanceMetricGetInterestScore(EmoStateHandle state);


	//! Returns focus level of the user
	/*!
        \param state - EmoStateHandle

		\return focus level (0.0 to 1.0)
    */
	EMOSTATE_DLL_API float
		IS_PerformanceMetricGetFocusScore(EmoStateHandle state);


	//! Returns short term excitement model parameters
	/*!
        \param state                - EmoStateHandle
		\param rawScore             - return raw score
		\param minScale, maxScale   - return scale range
    */
	EMOSTATE_DLL_API void
		IS_PerformanceMetricGetInstantaneousExcitementModelParams(EmoStateHandle state,
		                                                          double * rawScore,
																  double * minScale,
																  double * maxScale);


	//! Returns Relaxation model parameters
	/*!
        \param state                - EmoStateHandle
		\param rawScore             - return raw score
		\param minScale, maxScale   - return scale range
    */
	EMOSTATE_DLL_API void
		IS_PerformanceMetricGetRelaxationModelParams(EmoStateHandle state,
		                                             double * rawScore,
													 double * minScale,
													 double * maxScale);


	//! Returns EngagementBoredom model parameters
	/*!
        \param state                - EmoStateHandle
		\param rawScore             - return raw score
		\param minScale, maxScale   - return scale range
    */
	EMOSTATE_DLL_API void
		IS_PerformanceMetricGetEngagementBoredomModelParams(EmoStateHandle state,
		                                                    double * rawScore,
															double * minScale,
															double * maxScale);

	
	//! Returns Stress model parameters
	/*!
        \param state                - EmoStateHandle
		\param rawScore             - return raw score
		\param minScale, maxScale   - return scale range
    */
	EMOSTATE_DLL_API void
		IS_PerformanceMetricGetStressModelParams(EmoStateHandle state,
		                                         double * rawScore,
												 double * minScale,
												 double * maxScale);

	
	//! Returns Interest model parameters
	/*!
        \param state                - EmoStateHandle
		\param rawScore             - return raw score
		\param minScale, maxScale   - return scale range
    */
	EMOSTATE_DLL_API void
		IS_PerformanceMetricGetInterestModelParams(EmoStateHandle state,
		                                           double * rawScore,
												   double * minScale,
												   double * maxScale);


	//! Returns Focus model parameters
	/*!
        \param state                - EmoStateHandle
		\param rawScore             - return raw score
		\param minScale, maxScale   - return scale range
    */
	EMOSTATE_DLL_API void
		IS_PerformanceMetricGetFocusModelParams(EmoStateHandle state,
		                                        double * rawScore,
												double * minScale,
												double * maxScale);


	//! Check whether two states are with identical 'emotiv' state
    /*!
        \param a - EmoStateHandle
        \param b - EmoStateHandle

		\return 1: Equal, 0: Different

		\sa IS_PerformanceMetricEqual, IS_MentalCommandEqual, IS_EmoEngineEqual, IS_Equal
    */
	EMOSTATE_DLL_API int
		IS_PerformanceMetricEqual(EmoStateHandle a,
		                          EmoStateHandle b);

#ifdef __cplusplus
}
#endif

#endif // IEMOSTATEPERFORMANCEMETRIC_H
