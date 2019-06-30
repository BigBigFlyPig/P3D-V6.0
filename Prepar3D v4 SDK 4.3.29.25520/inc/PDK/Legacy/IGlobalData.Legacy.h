//-----------------------------------------------------------------------------------
// IGlobalData.h
// Copyright (C) Lockheed Martin Corporation.  All rights reserved.
// Prepar3D SDK interface for data querying data that is global to the application
//----------------------------------------------------------------------------------

#pragma once

#include "Types.h"

namespace P3D
{
/**
* Service for requesting general data not associated with any specific object in the application.
* \remark Various "time" queries are made relative to either Zulu (UTC) time Local time using the TIMEREF enum.
**/
DECLARE_INTERFACE_(IGlobalDataV400, IUnknown)
{
    /**
    * Paused - Is application paused
    * \return   True if application is paused, false if not. 
    **/
    STDMETHOD_(BOOL, IsPaused)()    const PURE;    
    /**
    * Sim Rate.  Percentage scalar on real time.  Typically ranges 0.25 - 128.0. Normal = 1.0.
    * \return   The current simulation rate.
    **/
    STDMETHOD_(float, GetSimRate)()    const PURE;    
    /**
    * Provided here for convenience to convert unit string name to internal code
    * \param       pszPropertyUnits      string name for unit
    * \param[out]  iUnitCode      Will contain requested value
    **/
    STDMETHOD (GetUnitCode)(__in LPCWSTR pszPropertyUnits, __out int& iUnitCode) const PURE;
    /**
    * Time since 00:00:00 year 0000
    * \param[out]  dValue      Will contain requested value
    * \param       eUnits      Units of return value
    **/
    STDMETHOD(GetAbsoluteTime)(__out double& dValue, __in int eUnits) const PURE;
    /**
    * Time of day (since midnight) Zulu or Local time ref
    * \param[out]  dValue      Will contain requested value
    * \param       eUnits      Units of return value
    **/
    STDMETHOD(GetTime)(          __out double& dValue, __in int eUnits, __in TIMEREF eTimeRef) const PURE;
    /**
    * Get year
    * \param[out]  uValue      Will contain requested value
    * \param       eTimeRef    Indicates local or zulu time
    **/
    STDMETHOD(GetYear)(          __out UINT& uValue, __in TIMEREF eTimeRef) const PURE;
    /**
    * Get month of year.
    * \param[out]  uValue      Will contain requested value
    * \param       eTimeRef    Indicates local or zulu time
    **/
    STDMETHOD(GetMonthOfTheYear)(__out UINT& uValue, __in TIMEREF eTimeRef) const PURE;
    /**
    * Get day of the year
    * \param[out]  uValue      Will contain requested value
    * \param       eTimeRef    Indicates local or zulu time
    **/
    STDMETHOD(GetDayOfTheYear)(  __out UINT& uValue, __in TIMEREF eTimeRef) const PURE;
    /**
    * Get day of the month
    * \param[out]  uValue      Will contain requested value
    * \param       eTimeRef    Indicates local or zulu time
    **/
    STDMETHOD(GetDayOfTheMonth)( __out UINT& uValue, __in TIMEREF eTimeRef) const PURE;
    /**
    * Get day of week
    * \param[out]  uValue      Will contain requested value
    * \param       eTimeRef    Indicates local or zulu time
    **/
    STDMETHOD(GetDayOfTheWeek)(  __out UINT& uValue, __in TIMEREF eTimeRef) const PURE;

    /**
    * Time Offset of current time zone from UTC
    * \param[out]  dValue      Will contain requested value
    * \param       eUnits      Units of return value
    **/
    STDMETHOD(GetTimeZoneOffset)(     __out double& dValue, __in int eUnits) const PURE;

    /**
     * General time of day.
     * \return  time of day.
     **/
    STDMETHOD_(TIMEOFDAY, GetGeneralTimeOfDay)()    const PURE;    
};
/// Current version of GlobalData interface
DEFINE_GUID(IID_IGlobalDataV400,    0xe079da98, 0xbb7a, 0x4a4e, 0xb1, 0x29, 0xf6, 0x3c, 0x3b, 0x48, 0xf, 0x1e);
}