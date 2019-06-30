//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//------------------------------------------------------------------------------
//
//  Target Info Gauge Supplement Sample
//  
//  Description:
//              Use in conjunction with the XML WeaponsGauge to display target 
//              information.  This uses SimConnect calls to retrieve target data, 
//              including location and health points.  This information is then 
//              displayed in the bottom window panel of the WeaponsGauge.  Should 
//              be added to a SimObject’s panel.cfg using the standard notation:
//
//                  gauge00=WeaponsGauge!WeaponsGauge,0,0
//
//
//
//                            PROFESSIONAL PLUS ONLY
//
//
//------------------------------------------------------------------------------

#pragma once

#define _CRT_RAND_S

#include <windows.h>  
#include <WinDef.h> 
#include <math.h>
#include <tchar.h>

#include "gauges.h"
#include "SimConnect.h"

#define     VERSION_MAJOR           1
#define     VERSION_MINOR           0
#define     VERSION_BUILD           0

// Preprocessor
#define     lita(arg) #arg
#define     xlita(arg) lita(arg)
#define     cat3(w,x,z) w##.##x##.##z##\000
#define     xcat3(w,x,z) cat3(w,x,z)
#define     VERSION_STRING xlita(xcat3(VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD))

#ifndef     VS_VERSION_INFO
#define     VS_VERSION_INFO     0x0001
#endif

#define EARTH_RADIUS 6371009 // Meters
#define PI 3.141592653589793238462643
#define TWO_PI (2*PI)

#define METERS_TO_FEET 3.28084
#define RADIANS_TO_DEGREES (360.0/TWO_PI)
#define DEGREES_TO_RADIANS (TWO_PI/360.0)

FLOAT64 GetDistance(FLOAT64 lat1, FLOAT64 lon1, FLOAT64 lat2, FLOAT64 lon2);
FLOAT64 DegreesToRadians(FLOAT64 degrees);

enum EVENT_ID
{ 
    EVENT_OBTAIN_TARGETS,
    EVENT_PREV_TARGET,
    EVENT_NEXT_TARGET,
    EVENT_ADD_TARGET,
    EVENT_REMOVE_TARGET,
    EVENT_SELECT_TARGET,
};

enum DATA_REQUEST_ID7
{ 
    REQUEST_TARGET_INFO = 100000,
    REQUEST_USER_INFO,
}; 

enum GROUP_ID
{ 
    GROUP_TARGET_INFO, 
    GROUP_USER_INFO,
}; 

enum DEFINITION_ID
{     
    DEFINITION_TARGET_INFO, 
    DEFINITION_USER_INFO,
};

// SimObject Target Information
struct TargetInfo
{
    wchar_t title[256];
    wchar_t atcType[128];
    wchar_t atcId[64];
    double  health;
    double  heading;
    double  altitude;
    double  lat;
    double  lon;
};

struct UserInfo
{
    double lat;                             // User Latitude
    double lon;                             // User Longitude
    double targetId;                        // Fire Control System (FCS) SimObject target ID
    SIMCONNECT_DATA_LATLONALT targetLla;    // FCS LLA target values
};

static const int LLA_TARGET = -2;
static const int NO_TARGET  = -1;

HANDLE      hSimConnect = NULL;

UserInfo    userInfo;
TargetInfo  targetInfo;
int         selectedTarget = NO_TARGET; // -2 LLA target, -1 No target, n > 0 SimObject ID

void ClearTargetInfo()
{
    targetInfo.title[0] = '\0';
    targetInfo.atcType[0] = '\0';
    targetInfo.atcId[0] = '\0';
    targetInfo.health = 1.0;
    targetInfo.heading = 0.0;
    targetInfo.altitude = 0.0;
    targetInfo.lat = 0.0;
    targetInfo.lon = 0.0;

    selectedTarget = NO_TARGET;
}

void ClearUserInfo()
{
    // User position
    userInfo.lat = 0.0;
    userInfo.lon = 0.0;

    // User Object target ID
    userInfo.targetId = NO_TARGET;

    // User LLA target details
    userInfo.targetLla.Latitude = 0.0;
    userInfo.targetLla.Longitude = 0.0;
    userInfo.targetLla.Altitude = 0.0;
}

void CALLBACK MyDispatchProcFCS(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{    
    HRESULT hr; 

    switch(pData->dwID) 
    { 
        case SIMCONNECT_RECV_ID_EVENT: 
        { 
            SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData; 

            switch(evt->uEventID) 
            { 
                // Fire control system events
                case EVENT_OBTAIN_TARGETS:
                case EVENT_PREV_TARGET:
                case EVENT_NEXT_TARGET:
                case EVENT_ADD_TARGET:
                case EVENT_REMOVE_TARGET:
                case EVENT_SELECT_TARGET:
                {
                    break;
                }
                default:
                {
                    break;
                }
            }
            break; 
        } 
        
        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
        {
            SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*) pData;

            switch(pObjData->dwRequestID)
            {
                case REQUEST_USER_INFO:
                {
                    // store the user info
                    UserInfo *pInfo = (UserInfo*)&pObjData->dwData;

                    userInfo.lat = pInfo->lat;
                    userInfo.lon = pInfo->lon;

                    double targetId = pInfo->targetId; // the requested target ID

                    userInfo.targetLla = pInfo->targetLla;

                    // Check if this ID is valid
                    if(targetId <= 0)
                    {
                        if(userInfo.targetLla.Latitude != 0 && userInfo.targetLla.Longitude != 0 && userInfo.targetLla.Altitude != 0)
                        {
                            selectedTarget = LLA_TARGET;
                        }
                        else
                        {
                            ClearTargetInfo();
                        }
                    }
                    else
                    {
                        // Request data on the selected target SimObject
                        hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST_TARGET_INFO, DEFINITION_TARGET_INFO, (SIMCONNECT_OBJECT_ID)targetId, SIMCONNECT_PERIOD_SIM_FRAME);
                        // Store new target ID
                        selectedTarget = (int)targetId;
                    }
                    break;
                }
                case REQUEST_TARGET_INFO:
                {
                    // store the target info
                    TargetInfo *pInfo = (TargetInfo*)&pObjData->dwData;

                    wcscpy_s(targetInfo.title, pInfo->title);
                    wcscpy_s(targetInfo.atcType, pInfo->atcType);
                    wcscpy_s(targetInfo.atcId, pInfo->atcId);

                    targetInfo.health = pInfo->health;
                    targetInfo.heading = pInfo->heading;
                    targetInfo.altitude = pInfo->altitude;

                    targetInfo.lat = pInfo->lat;
                    targetInfo.lon = pInfo->lon;
                    
                    break;
                }
                default:
                    break;
            }

            break;
        }

        case SIMCONNECT_RECV_ID_QUIT: 
        { 
            // Close the client
            if(hSimConnect != NULL)
            {
                HRESULT hr = SimConnect_Close(hSimConnect);
            }
            break; 
        } 

        default: 
            break; 
    } 
}

void StartSimConnect()
{
    // Initialize Client
    HRESULT hr; 
    hSimConnect = NULL;
    selectedTarget = NO_TARGET;

    ClearUserInfo();
    ClearTargetInfo();

    if(SUCCEEDED(SimConnect_Open(&hSimConnect, "TargetInfo - SimConnectClient", NULL, 0, NULL, 0)))
    {
        // Map client to events
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_OBTAIN_TARGETS, "fcs_populate_with_closest_objects");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_PREV_TARGET, "fcs_previous_target");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_NEXT_TARGET, "fcs_next_target");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_ADD_TARGET, "fcs_add_target");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_REMOVE_TARGET, "fcs_remove_target");
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_SELECT_TARGET, "fcs_select_target");

        // Sign up for notifications 
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_TARGET_INFO, EVENT_OBTAIN_TARGETS);
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_TARGET_INFO, EVENT_PREV_TARGET);
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_TARGET_INFO, EVENT_NEXT_TARGET);
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_TARGET_INFO, EVENT_ADD_TARGET);
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_TARGET_INFO, EVENT_REMOVE_TARGET);
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_TARGET_INFO, EVENT_SELECT_TARGET);

        // Add to data definition
        // Target information
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_TARGET_INFO, "Title", NULL, SIMCONNECT_DATATYPE_WSTRING256);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_TARGET_INFO, "Atc Type", NULL, SIMCONNECT_DATATYPE_WSTRING128);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_TARGET_INFO, "Atc Id", NULL, SIMCONNECT_DATATYPE_WSTRING64);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_TARGET_INFO, "Health Points", NULL, SIMCONNECT_DATATYPE_FLOAT64);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_TARGET_INFO, "Plane Heading Degrees True", NULL, SIMCONNECT_DATATYPE_FLOAT64);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_TARGET_INFO, "Indicated Altitude", NULL, SIMCONNECT_DATATYPE_FLOAT64);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_TARGET_INFO, "Plane Latitude", NULL, SIMCONNECT_DATATYPE_FLOAT64);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_TARGET_INFO, "Plane Longitude", NULL, SIMCONNECT_DATATYPE_FLOAT64);

        // User information
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_USER_INFO, "Plane Latitude", NULL, SIMCONNECT_DATATYPE_FLOAT64);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_USER_INFO, "Plane Longitude", NULL, SIMCONNECT_DATATYPE_FLOAT64);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_USER_INFO, "FCS Targeted Object Id", NULL);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_USER_INFO, "FCS Targeted LLA", NULL, SIMCONNECT_DATATYPE_LATLONALT);

        // Request the selected target ID from the user SimObject
        hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST_USER_INFO, DEFINITION_USER_INFO, 0, SIMCONNECT_PERIOD_SIM_FRAME);

        hr = SimConnect_CallDispatch(hSimConnect, MyDispatchProcFCS, NULL);
    } 
}

// Note: The items in the property table correspond to the indices that
// will be returned in the Get/Set Property functions
struct PROPERTY_TABLE
{
    LPCSTR szPropertyName;
    LPCSTR szUnitsName;
    ENUM units;
};

// 
// PanelCallback is an abstract base class that can be overrided.  Implementors
// should override the functions CreateAircraftCallback(UINT32 ContainerId) as
// well as GetPropertyTable.
class PanelCallback : public IPanelCCallback
{
    DECLARE_PANEL_CALLBACK_REFCOUNT(PanelCallback);
    
public:
    PanelCallback();
    
    // ******* IPanelCCallback Methods *****************    
    IPanelCCallback* QueryInterface(LPCSTR pszInterface);    
    UINT32 GetVersion();
    bool ConvertStringToProperty (LPCSTR keyword, SINT32* pID);
    bool ConvertPropertyToString (SINT32 id, PPCSTRINGZ pKeyword);
    bool GetPropertyUnits (SINT32 id, ENUM* pEnum);

protected:
    // ******** PanelCallback Methods ******************
    virtual const PROPERTY_TABLE *GetPropertyTable(UINT &uLength) = 0;
};

// 
// AircraftCallback is an abstract base class that can be overrided.  Implementors
// should override the function CreateGaugeCCallback(UINT32 ContainerId)
class AircraftCallback : public IAircraftCCallback 
{
    DECLARE_PANEL_CALLBACK_REFCOUNT(AircraftCallback);
public:
    AircraftCallback(UINT32 containerId);
    ~AircraftCallback();

    // ******* IAircraftCCallback Methods ************* 
    IAircraftCCallback* QueryInterface(LPCSTR pszInterface);
    void Update();

protected:    
    UINT32 GetContainerId() const;

private:
    UINT32 m_containerId;
};
// End of paneldefs.h

DEFINE_PANEL_CALLBACK_REFCOUNT(PanelCallback);

PanelCallback::PanelCallback() 
    : m_RefCount(1)
{
}
 
IPanelCCallback* PanelCallback::QueryInterface(LPCSTR pszInterface)
{
    return NULL;
}

UINT32 PanelCallback::GetVersion()
{
    return 1;
}

bool PanelCallback::ConvertStringToProperty (LPCSTR keyword, SINT32* pID)
{
    if(!keyword)
    {
        return false;
    }
    if(!pID)
    {
        return false;
    }

    UINT uNumProperties;
    const PROPERTY_TABLE *parPropertyTable = GetPropertyTable(uNumProperties);
    
    for(UINT i = 0; i < uNumProperties; i++)
    {
        if(_stricmp(parPropertyTable[i].szPropertyName, keyword) == 0)
        {
            *pID = i;
            return true;
        }
    }
    return false;         
}

bool PanelCallback::ConvertPropertyToString (SINT32 id, PPCSTRINGZ pKeyword)
{
    if(!pKeyword)
    {
        return false;
    }
    
    UINT uNumProperties;
    const PROPERTY_TABLE *parPropertyTable = GetPropertyTable(uNumProperties);

    if(id < 0 || id >= (SINT32)uNumProperties)
    {
        return false;
    }
    *pKeyword = parPropertyTable[id].szPropertyName;
    return true; 
}

bool PanelCallback::GetPropertyUnits (SINT32 id, ENUM* pEnum)
{
    if(!pEnum)
    {
        return false;
    }

    UINT uNumProperties;
    const PROPERTY_TABLE *parPropertyTable = GetPropertyTable(uNumProperties);
    
    if(id < 0 || id >= (SINT32)uNumProperties)
    {
        return false;
    }

    *pEnum = parPropertyTable[id].units;
    return true;
}

PPANELS Panels = NULL;

GAUGESIMPORT    ImportTable =
{
    { 0x0000000F, (PPANELS)NULL },
    { 0x00000000, NULL }
};

DEFINE_PANEL_CALLBACK_REFCOUNT(AircraftCallback);

AircraftCallback::AircraftCallback(UINT32 containerId)
    : m_containerId(containerId),
      m_RefCount(1)
{
    // Start the simconnect client
    if(Panels != NULL)
    {
        StartSimConnect();
    }
}

AircraftCallback::~AircraftCallback()
{
    // Stop the simconnect client
    if(hSimConnect != NULL)
    {
        HRESULT hr = SimConnect_Close(hSimConnect);
    }
}

IAircraftCCallback* AircraftCallback::QueryInterface(LPCSTR pszInterface)
{
    return NULL;
}

void AircraftCallback::Update()
{
}

UINT32 AircraftCallback::GetContainerId() const
{
    return m_containerId;
}

//----------------------------------------------------------------------------
// Static Variables
//----------------------------------------------------------------------------
#define PROPERTY_TABLE_LENGTH 11
static const char TARGET_CALLBACK_NAME[] = "TargetInfo";
static PROPERTY_TABLE TARGET_PROPERTY_TABLE[] = 
{
    { "Title",                              "String",       UNITS_WSTRING},
    { "Atc Type",                           "String",       UNITS_WSTRING},
    { "Atc Id",                             "String",       UNITS_WSTRING},
    { "Is Alive",                           "Bool",         UNITS_UNKNOWN},
    { "Health Points",                      "Number",       UNITS_UNKNOWN},
    { "Heading Degrees True",               "Radians",      UNITS_UNKNOWN},
    { "Altitude",                           "Number",       UNITS_UNKNOWN},
    { "Distance",                           "Number",       UNITS_UNKNOWN},
    { "TargetId",                           "Number",       UNITS_UNKNOWN},
    { "Latitude",                           "Number",       UNITS_UNKNOWN},
    { "Longitude",                          "Number",       UNITS_UNKNOWN},
};

// Enum that contains the properties 
enum TARGET_VAR
{
    TARGET_VAR_TITLE,
    TARGET_VAR_ATC_TYPE,
    TARGET_VAR_ATC_ID,
    TARGET_VAR_IS_ALIVE,
    TARGET_VAR_HEALTH,
    TARGET_VAR_HEADING,
    TARGET_VAR_ALTITUDE,
    TARGET_VAR_DISTANCE,
    TARGET_VAR_TARGET_ID,
    TARGET_VAR_LATITUDE,
    TARGET_VAR_LONGITUDE,
};

struct stringValuePair {
    char*   string;
    int     value;
};

//
// Class that implements IGaugeCCallback
//
class TARGETGaugeCallback : public IGaugeCCallback
{
    DECLARE_PANEL_CALLBACK_REFCOUNT(TARGETGaugeCallback);
    
public:
    TARGETGaugeCallback(UINT32 containerId);    
    
    // ************* IGaugeCCallback Methods ***************
    IGaugeCCallback* QueryInterface(LPCSTR pszInterface);
    void Update();
    bool GetPropertyValue(SINT32 id, FLOAT64* pValue);
    bool GetPropertyValue(SINT32 id, LPCSTR* pszValue);
    bool GetPropertyValue(SINT32 id, LPCWSTR* pszValue);
    bool SetPropertyValue(SINT32 id, FLOAT64 value);
    bool SetPropertyValue(SINT32 id, LPCSTR szValue);
    bool SetPropertyValue(SINT32 id, LPCWSTR szValue);
    IGaugeCDrawable* CreateGaugeCDrawable(SINT32 id, const IGaugeCDrawableCreateParameters* pParameters);

private:
    UINT32 m_containerId;
};

DEFINE_PANEL_CALLBACK_REFCOUNT(TARGETGaugeCallback)

TARGETGaugeCallback::TARGETGaugeCallback(UINT32 containerId)
    : m_RefCount(1),
      m_containerId(containerId)
{
}

IGaugeCCallback* TARGETGaugeCallback::QueryInterface(LPCSTR pszInterface)
{
    return NULL;
}

//
// The Update functions is called on a 18Hz cycle
//
void TARGETGaugeCallback::Update()
{
}

//
// Getting float/numeric values
//
bool TARGETGaugeCallback::GetPropertyValue (SINT32 id, FLOAT64* pValue)
{
    if(!pValue)
    {
       return false;
    }

    *pValue = 0.0;
   
    TARGET_VAR eTARGETVar = (TARGET_VAR)id;

    switch(eTARGETVar)
    {
    case TARGET_VAR_TARGET_ID:
        {
            *pValue = selectedTarget;
            break;
        }
    case TARGET_VAR_IS_ALIVE:
        {
            if(selectedTarget > 0)
            {
                *pValue = targetInfo.health > 0;
            }
            else
            {
                *pValue = FALSE;
            }
            break;
        }
    case TARGET_VAR_HEALTH:
        {
            if(selectedTarget > 0)
            {
                *pValue = targetInfo.health;
            }
            else
            {
                *pValue = 0.0;
            }
            break;
        }
    case TARGET_VAR_HEADING:
        {
            // Convert to degrees
            if(selectedTarget > 0)
            {
                *pValue = targetInfo.heading * RADIANS_TO_DEGREES;
            }
            else
            {
                *pValue = 0.0;
            }
            break;
        }
    case TARGET_VAR_ALTITUDE:
        {
            if(selectedTarget > 0)
            {
                *pValue = targetInfo.altitude * METERS_TO_FEET;         // convert to feet
            }
            else if(selectedTarget == LLA_TARGET)
            {
                *pValue = userInfo.targetLla.Altitude * METERS_TO_FEET; // convert to feet
            }
            else
            {
                *pValue = 0.0;
            }
            break;
        }
    case TARGET_VAR_DISTANCE:
        {
            // Check we have a valid target
            if(selectedTarget > 0)
            {
                *pValue = GetDistance(userInfo.lat, userInfo.lon, targetInfo.lat, targetInfo.lon);
            }
            else if(selectedTarget == LLA_TARGET)
            {
                *pValue = GetDistance(userInfo.lat, userInfo.lon, DegreesToRadians(userInfo.targetLla.Latitude), DegreesToRadians(userInfo.targetLla.Longitude));
            }
            else
            {
                *pValue = 0.0;
            }
            break;
        }
    case TARGET_VAR_LATITUDE:
        {
            if(selectedTarget > 0)
            {
                *pValue = targetInfo.lat;
            }
            else if(selectedTarget == LLA_TARGET)
            {
                *pValue = userInfo.targetLla.Latitude;
            }
            else
            {
                *pValue = 0.0;
            }
        }
        break;
    case TARGET_VAR_LONGITUDE:
        {
            if(selectedTarget > 0)
            {
                *pValue = targetInfo.lon;
            }
            else if(selectedTarget == LLA_TARGET)
            {
                *pValue = userInfo.targetLla.Longitude;
            }
            else
            {
                *pValue = 0.0;
            }
        }
        break;
    default:
        return false;
    }
    return true; 
}

//
// Getting string property values
//
//
// Getting string property values
//
bool TARGETGaugeCallback::GetPropertyValue (SINT32 id, LPCSTR* pszValue)
{
    return false;
}

bool TARGETGaugeCallback::GetPropertyValue (SINT32 id, LPCWSTR* pszValue)
{
    if(!pszValue)
    {
       return false;
    }

    *pszValue = L"null";     // Return a reasonable default
    
    TARGET_VAR eTARGETVar = (TARGET_VAR)id;

    switch(eTARGETVar)
    {
    case TARGET_VAR_TITLE:
        {
            *pszValue = targetInfo.title;
            break;
        }
    case TARGET_VAR_ATC_TYPE:
        {
            *pszValue = targetInfo.atcType;
            break;
        }
    case TARGET_VAR_ATC_ID:
        {
            *pszValue = targetInfo.atcId;
            break;
        }
    default:
        return false;
    }
    return true; 
}

//
// Setting float/numeric values
//
bool TARGETGaugeCallback::SetPropertyValue (SINT32 id, FLOAT64 value)
{
    return false;  // No float properties need to be set
}

//
// Setting string values
//
bool TARGETGaugeCallback::SetPropertyValue (SINT32 id, LPCSTR szValue)
{
    return false;   // String properties not permitted to be set
}

bool TARGETGaugeCallback::SetPropertyValue (SINT32 id, LPCWSTR szValue)
{
    return false;   // String properties not permitted to be set
}

IGaugeCDrawable* TARGETGaugeCallback::CreateGaugeCDrawable(SINT32 id, const IGaugeCDrawableCreateParameters* pParameters)
{
    return NULL;    // No implementation of this necessary
}

//
// AircraftCallback Override
//
class TARGETAircraftCallback : public AircraftCallback
{
public:
    TARGETAircraftCallback(UINT32 ContainerID) : AircraftCallback(ContainerID)
    {
    }

    IGaugeCCallback* CreateGaugeCCallback ()
    {
        return new TARGETGaugeCallback(GetContainerId());
    }
};

//
// PanelCallback Override
//
class TARGETPanelCallback : public PanelCallback
{
public:

    TARGETPanelCallback::TARGETPanelCallback()
    {
    }
    
    IAircraftCCallback* CreateAircraftCCallback (UINT32 ContainerID)
    {
        return new TARGETAircraftCallback(ContainerID);
    }

protected:
    const PROPERTY_TABLE *GetPropertyTable(UINT &uLength)
    {
        uLength = LENGTHOF(TARGET_PROPERTY_TABLE);
        return TARGET_PROPERTY_TABLE;
    }
};

void TARGETPanelCallbackInit()
{
    TARGETPanelCallback *pPanelCallback = new TARGETPanelCallback();

    if(pPanelCallback)
    {
        panel_register_c_callback(TARGET_CALLBACK_NAME, pPanelCallback);
        pPanelCallback->Release();
    }
}

void TARGETPanelCallbackDeInit()
{
    panel_register_c_callback(TARGET_CALLBACK_NAME, NULL);
}

// DLLStart function
int __stdcall DLLStart(void)
{
    if (Panels != NULL)
    {
        ImportTable.PANELSentry.fnptr = (PPANELS)Panels;
        TARGETPanelCallbackInit();
    }
    return 0;
}


// DLLStop function
void __stdcall DLLStop(void)
{
    TARGETPanelCallbackDeInit();
}

BOOL WINAPI DllMain (HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
    return TRUE;
}


FLOAT64 GetDistance(FLOAT64 lat1, FLOAT64 lon1, FLOAT64 lat2, FLOAT64 lon2)
{
    FLOAT64 dLat = lat2 - lat1;
    FLOAT64 dLon = lon2 - lon1;

    FLOAT64 sin2Lat = pow(sin(dLat * 0.5), 2.0);
    FLOAT64 sin2Lon = pow(sin(dLon * 0.5), 2.0);

    // haversine
    return 2 * EARTH_RADIUS * asin(sqrt(sin2Lat + cos(lat1) * cos(lat2) * sin2Lon));
}

FLOAT64 DegreesToRadians(FLOAT64 degrees)
{
    return degrees * DEGREES_TO_RADIANS;
}
