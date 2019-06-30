//-----------------------------------------------------------------------------------
// IGlobalData.h
// Copyright (C) Lockheed Martin Corporation.  All rights reserved.
// Prepar3D SDK interface for data querying data that is global to the application
//----------------------------------------------------------------------------------

#pragma once

#include "Legacy\IGlobalData.Legacy.h"

namespace P3D
{
    typedef enum {
        LICENSE_TYPE_ACADEMIC,
        LICENSE_TYPE_PROFESSIONAL,
        LICENSE_TYPE_PROFESSIONAL_PLUS,
        LICENSE_TYPE_UNKNOWN,
    } LICENSE_TYPE;

    /** \defgroup   globaldata  Global Data Service 
     * This service is provides certain global data that is not associated with 
     * any specific object in the application such as time and pause state.
     * A sample implementation using this interface can be found in the SimpleAirplane 
     * sample in the ISimObject Samples.
     * \{ 
     */
/**
* Service for requesting general data not associated with any specific object in the application.
* \remark Various "time" queries are made relative to either Zulu (UTC) time Local time using the TIMEREF enum.
**/
DECLARE_INTERFACE_(IGlobalDataV430, IGlobalDataV400)
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
    * License Type - Get application License Type
    * \return   License type.
    **/
    STDMETHOD_(LICENSE_TYPE, GetLicenseType)()    const PURE;
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
    STDMETHOD(GetTimeZoneOffset)(__out double& dValue, __in int eUnits) const PURE;
    /**
    * Time since 00:00:00 year 0000
    * \param       dValue      The requested time
    * \param       eUnits      Units of return value
    **/
    STDMETHOD(SetAbsoluteTime)(__in double dValue, __in int eUnits) PURE;
    /**
    * Time of day (since midnight) Zulu or Local time ref
    * \param       uHour       The requested hour
    * \param       uMinute     The requested minute
    * \param       uSeconds    The requested seconds
    * \param       eTimeRef    Indicates local or zulu time
    **/
    STDMETHOD(SetTime)(__in UINT uHour, __in UINT uMinute, __in UINT uSeconds, __in TIMEREF eTimeRef) PURE;
    /**
    * Date in Zulu or Local time ref
    * \param       uYear            The requested year
    * \param       uMonth           The requested month
    * \param       uDayOfTheMonth   The requested day of the month
    * \param       eTimeRef         Indicates local or zulu time
    **/
    STDMETHOD(SetDate)(__in UINT uYear, __in UINT uMonth, __in UINT uDayOfTheMonth, __in TIMEREF eTimeRef) PURE;
};
/// Current version of GlobalData interface
DEFINE_GUID(IID_IGlobalDataV430,    0x64718ae, 0xc2b7, 0x4c1a, 0x99, 0x4, 0x9b, 0xe7, 0x21, 0x8d, 0xcf, 0x9);
/// GlobalData service ID
DEFINE_GUID(SID_GlobalData,         0x7eb08a5d, 0x4997, 0x48be, 0x92, 0x78, 0x7e, 0x22, 0xad, 0x81, 0x4, 0x94);

/** \} */
}
