/**
* Emotiv SDK
* Copyright (c) 2016 Emotiv Inc.
*
* This file is part of the Emotiv SDK.
*
* Header file for license related API.
*
*/

#ifndef EMOTIVLICENSE_H
#define EMOTIVLICENSE_H

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

    typedef enum IEE_LicenseType_enum {
        IEE_EEG = 0x001,      // Enable EEG data
        IEE_PM  = 0x002,      // Enable Performance Metric detection   
        IEE_EEG_PM = IEE_EEG | IEE_PM   // Enable EEG data and Performance Metric detection   
    } IEE_LicenseType_t;

    typedef struct IEE_LicenseInfos_struct {
        unsigned int scopes;       // license type
        unsigned int date_from;    // epoch time 
        unsigned int date_to;      // epoch time
        int     seat_count;        // number of seat

        int     quotaDayLimit;     // session limit in day
        int     usedQuotaDay;      // session used in day
        int     quotaMonthLimit;   // session limit in month
        int     usedQuotaMonth;    // session used in month
        int     usedQuota;         // total session used
        int     quota;             // total session in the license
    } IEE_LicenseInfos_t;


    //! Check infos of the license
    /*!    

        \param licenseInfo - License Information    
        \return    EDK_ERROR_CODE
                   EDK_OVER_QUOTA_IN_DAY
                   EDK_OVER_QUOTA_IN_MONTH
                   EDK_LICENSE_EXPIRED
                   EDK_OVER_QUOTA
                   EDK_ACCESS_DENIED
                   EDK_LICENSE_ERROR
                   EDK_NO_ACTIVE_LICENSE
                   EDK_OK

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_LicenseInformation(IEE_LicenseInfos_t * licenseInfo);


    //! Activate a license with license ID
    /*!

        \param licenseID - License ID
        \return EDK_ERROR_CODE
                     - EDK_OK if the command succeeded

        \sa IedkErrorCode.h
    */
    EDK_API int 
        IEE_ActivateLicense(const char* licenseID);


#ifdef __cplusplus
}
#endif
#endif // EMOTIVLICENSE_H
