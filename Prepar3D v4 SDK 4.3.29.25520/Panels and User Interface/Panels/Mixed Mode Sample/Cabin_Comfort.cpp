//Copyright (c) Microsoft Corporation.  All rights reserved. 
//
//	Cabin_Comfort.cpp
//
#define _CRT_RAND_S
#include "gauges.h"
#include <math.h>

#pragma once

GAUGE_CALLBACK gauge_callback;

// Note: The items in the property table correspond to the indices that
// will be returned in the Get/Set Property functions
struct PROPERTY_TABLE
{
    PCSTRINGZ szPropertyName;
    PCSTRINGZ szUnitsName;
    ENUM units;
};

// 
// PanelCallback is an abstract base class that can be overridden.  Implementors
// should override the functions CreateAircraftCallback() as
// well as GetPropertyTable.
class PanelCallback : public IPanelCCallback
{
    DECLARE_PANEL_CALLBACK_REFCOUNT(PanelCallback);
    
public:
    PanelCallback();
    
    // ******* IPanelCCallback Methods *****************    
    IPanelCCallback* QueryInterface(PCSTRINGZ pszInterface);    
    UINT32 GetVersion();
    bool ConvertStringToProperty (PCSTRINGZ keyword, SINT32* pID);
    bool ConvertPropertyToString (SINT32 id, PPCSTRINGZ pKeyword);
    bool GetPropertyUnits (SINT32 id, ENUM* pEnum);

protected:
    // ******** PanelCallback Methods ******************
    virtual const PROPERTY_TABLE *GetPropertyTable(UINT &uLength) = 0;
};

// 
// AircraftCallback is an abstract base class that can be overridden.  Implementors
// should override the function CreateGaugeCCallback(UINT32 ContainerId)
class AircraftCallback : public IAircraftCCallback 
{
    DECLARE_PANEL_CALLBACK_REFCOUNT(AircraftCallback);
public:
    AircraftCallback(UINT32 containerId);

    // ******* IAircraftCCallback Methods ************* 
    IAircraftCCallback* QueryInterface(PCSTRINGZ pszInterface);
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
 
IPanelCCallback* PanelCallback::QueryInterface(PCSTRINGZ pszInterface)
{
    return NULL;
}

UINT32 PanelCallback::GetVersion()
{
    return 1;
}

bool PanelCallback::ConvertStringToProperty (PCSTRINGZ keyword, SINT32* pID)
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


DEFINE_PANEL_CALLBACK_REFCOUNT(AircraftCallback);

AircraftCallback::AircraftCallback(UINT32 containerId)
    : m_containerId(containerId),
      m_RefCount(1)
{
}

IAircraftCCallback* AircraftCallback::QueryInterface(PCSTRINGZ pszInterface)
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
static const char CABIN_CALLBACK_NAME[] = "CABIN";

#define ROOM_TEMPERATURE		25			// degrees celcius

static PROPERTY_TABLE CABIN_PROPERTY_TABLE[] = 
{
    { "CabinTemperature",			"Celcius",		UNITS_UNKNOWN},         
    { "BankAngle",					"Degrees", 		UNITS_UNKNOWN},         
    { "FlightAttendants",			"String", 		UNITS_WSTRING},         
    { "Restroom",					"String", 		UNITS_WSTRING},         
    { "PassengerComfort",			"Number", 		UNITS_UNKNOWN},
};
// Enum that contains the properties 
enum CABIN_VAR
{
    CABIN_VAR_CABIN_TEMPERATURE,
    CABIN_VAR_BANK_ANGLE,
    CABIN_VAR_FLIGHT_ATTENDANTS,
    CABIN_VAR_RESTROOM,
    CABIN_VAR_PASSENGER_COMFORT,
};

enum FLIGHT_ATTENDANTS
{
    PASSIVE = 0,
    ACTIVE,
    SERVING,
};

struct stringValuePair {
    TCHAR*	string;
    int		value;
};

const struct stringValuePair attendantStrings[3] = {
    { TEXT("Passive"),		-10},
    { TEXT("Active"),		10},
    { TEXT("Serving"),		15}
};

enum RESTROOM
{
    VACANT = 0,
    ENGAGED
};

const struct stringValuePair restroomStrings[2] = {
    { TEXT("Vacant"),		5},
    { TEXT("Engaged"),		-5}
};

//
// Class that implements IGaugeCCallback
//
class CABINGaugeCallback : public IGaugeCCallback
{
    DECLARE_PANEL_CALLBACK_REFCOUNT(CABINGaugeCallback);
    
    // Declare member variables representing cabin state
    float				cabin_temperature;
    FLIGHT_ATTENDANTS	flight_attendant_state;
    RESTROOM			restroom_state;

public:
    CABINGaugeCallback(UINT32 containerId);	
    
    // ************* IGaugeCCallback Methods ***************
    IGaugeCCallback* QueryInterface(PCSTRINGZ pszInterface);
    void Update();
    bool GetPropertyValue (SINT32 id, FLOAT64* pValue);
    bool GetPropertyValue (SINT32 id, LPCSTR* pszValue);
    bool GetPropertyValue (SINT32 id, LPCWSTR* pszValue);
    bool SetPropertyValue (SINT32 id, FLOAT64 value);
    bool SetPropertyValue (SINT32 id, LPCSTR szValue);
    bool SetPropertyValue (SINT32 id, LPCWSTR szValue);
    IGaugeCDrawable* CreateGaugeCDrawable(SINT32 id, const IGaugeCDrawableCreateParameters* pParameters);
    
    float getCabinTemperature()
    {
        return cabin_temperature;
    }
    FLIGHT_ATTENDANTS getFlightAttendantState()
    {
        return flight_attendant_state;
    }
    RESTROOM getRestroomState()
    {
        return restroom_state;
    }
    void setCabinTemperature(float temp)
    {
        cabin_temperature = temp;
    }
    void setFlightAttendantState(FLIGHT_ATTENDANTS state)
    {
        flight_attendant_state = state;
    }
    void setRestroomState(RESTROOM state)
    {
        restroom_state = state;
    }
private:
    UINT32 m_containerId;
};


DEFINE_PANEL_CALLBACK_REFCOUNT(CABINGaugeCallback)

CABINGaugeCallback::CABINGaugeCallback(UINT32 containerId)
    : m_RefCount(1),
      m_containerId(containerId)
{
    // Initialize aircraft variables
    cabin_temperature		= ROOM_TEMPERATURE;
    flight_attendant_state	= PASSIVE;
    restroom_state			= VACANT;
}
IGaugeCCallback* CABINGaugeCallback::QueryInterface(PCSTRINGZ pszInterface)
{
    return NULL;
}
//
// The Update functions is called on a 18Hz cycle
//
void CABINGaugeCallback::Update()
{
    unsigned int	random;

    // Inane logic to update the attendants and restroom settings

    if (rand_s(&random) == 0)
    {
        if (random < UINT_MAX/20)
        {
            if (rand_s(&random) == 0)
            {
                if (random < UINT_MAX/6)
                    flight_attendant_state = SERVING; else
                if (random < UINT_MAX/3)
                    flight_attendant_state = ACTIVE; else
                if (random < UINT_MAX/2)
                    flight_attendant_state = PASSIVE;
            }
            if (rand_s(&random) == 0)
            {
                if (random < UINT_MAX/4)
                    restroom_state = VACANT; else
                if (random < UINT_MAX/2)
                    restroom_state = ENGAGED;
            }
        }
    }
}
//
// Getting float/numeric values
//
bool CABINGaugeCallback::GetPropertyValue (SINT32 id, FLOAT64* pValue)
{
    if(!pValue)
    {
       return false;
    }

    *pValue = 1.0;      // Start with a reasonable default
   
    CABIN_VAR eCABINVar = (CABIN_VAR)id;

    switch(eCABINVar)
    {
    case CABIN_VAR_CABIN_TEMPERATURE:

        *pValue = getCabinTemperature();

        break;

    case CABIN_VAR_BANK_ANGLE:

        // Get the variable name from Simulation Variables, not Token Variables, for aircraft_varget

        *pValue = fabs( aircraft_varget( get_aircraft_var_enum("PLANE BANK DEGREES"), get_units_enum("degrees"), 0) );

        break;

    case CABIN_VAR_PASSENGER_COMFORT:

        // Lame equation to calculate passenger comfort, in range 0 to 100
        
        *pValue = - fabs( aircraft_varget( get_aircraft_var_enum("PLANE BANK DEGREES"), get_units_enum("degrees"), 0) );

        *pValue += 75 - fabs(ROOM_TEMPERATURE - getCabinTemperature()) + 
            attendantStrings[ (int) getFlightAttendantState() ].value +
            restroomStrings[ (int) getRestroomState() ].value;
        
        if (*pValue > 100)
            *pValue = 100;

        if (*pValue < 0)
            *pValue = 0;
        
        break;

     default:
        return false;
    }
    return true; 
}
//
// Getting string property values
//
bool CABINGaugeCallback::GetPropertyValue (SINT32 id, LPCSTR* pszValue)
{
    return false;
}
bool CABINGaugeCallback::GetPropertyValue (SINT32 id, LPCWSTR* pszValue)
{
    if(!pszValue)
    {
       return false;
    }

    *pszValue = TEXT("null");     // Return a reasonable default
    
    CABIN_VAR eCABINVar = (CABIN_VAR)id;

    switch(eCABINVar)
    {
    case CABIN_VAR_FLIGHT_ATTENDANTS:

        *pszValue = attendantStrings[ (int) getFlightAttendantState() ].string;

        break;

    case CABIN_VAR_RESTROOM:

        *pszValue = restroomStrings[ (int) getRestroomState() ].string;

        break;

    default:
        return false;
    }
    return true; 
}
//
// Setting float/numeric values
//
bool CABINGaugeCallback::SetPropertyValue (SINT32 id, FLOAT64 value)
{

    CABIN_VAR eCABINVar = (CABIN_VAR)id;

    switch(eCABINVar)
    {
    case CABIN_VAR_CABIN_TEMPERATURE:

        setCabinTemperature(value);

        break;

    default:
        return false;
    }
    return true; 
}
//
// Setting string values
//
bool CABINGaugeCallback::SetPropertyValue (SINT32 id, LPCSTR szValue)
{
    return false;   // String properties not permitted to be set
}
bool CABINGaugeCallback::SetPropertyValue (SINT32 id, LPCWSTR szValue)
{
    return false;   // String properties not permitted to be set
}

IGaugeCDrawable* CABINGaugeCallback::CreateGaugeCDrawable(SINT32 id, const IGaugeCDrawableCreateParameters* pParameters)
{
    return NULL;	// No implementation of this necessary
}

//
// AircraftCallback Override
//
class CABINAircraftCallback : public AircraftCallback
{
private:

public:
    CABINAircraftCallback(UINT32 ContainerID): AircraftCallback(ContainerID)
    {}
    IGaugeCCallback* CreateGaugeCCallback ()
    {
        return new CABINGaugeCallback(GetContainerId());
    }
};

//
// PanelCallback Override
//

class CABINPanelCallback : public PanelCallback
{
public:

    CABINPanelCallback::CABINPanelCallback()
    {
        // init property table
        for (int n = 0; n < 5; n++)
        {
            if (ImportTable.PANELSentry.fnptr != NULL &&
                CABIN_PROPERTY_TABLE[n].units == UNITS_UNKNOWN)
            {
                CABIN_PROPERTY_TABLE[n].units = get_units_enum ( CABIN_PROPERTY_TABLE[n].szUnitsName );
            }
        }
    }
    
    IAircraftCCallback* CreateAircraftCCallback (UINT32 ContainerID)
    {
        return new CABINAircraftCallback(ContainerID);
    }


protected:
    const PROPERTY_TABLE *GetPropertyTable(UINT &uLength)
    {
        uLength = LENGTHOF(CABIN_PROPERTY_TABLE);
        return CABIN_PROPERTY_TABLE;
    }
};

void CABINPanelCallbackInit()
{
    
    CABINPanelCallback *pPanelCallback = new CABINPanelCallback();

    if(pPanelCallback)
    {
        bool b = panel_register_c_callback(CABIN_CALLBACK_NAME, pPanelCallback);
        pPanelCallback->Release();
    }   
}

void CABINPanelCallbackDeInit()
{
    panel_register_c_callback(CABIN_CALLBACK_NAME, NULL);
}


// The Panels pointer will get filled in during the loading process
// if this DLL is listed in DLL.XML

PPANELS Panels = NULL;

GAUGESIMPORT    ImportTable =                           
{                                                       
    { 0x0000000F, (PPANELS)NULL },                     
    { 0x00000000, NULL }                                
};                                                      
   
void FSAPI  module_init(void)
{
    if (NULL != Panels)
    {
        ImportTable.PANELSentry.fnptr = (PPANELS)Panels;
        CABINPanelCallbackInit();
    }
}                         

void FSAPI  module_deinit(void){
    CABINPanelCallbackDeInit();
}                       

BOOL WINAPI DllMain (HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved) 
{                                                       
    return TRUE;                                        
}                  
 
// This is the module's export table.
GAUGESLINKAGE   Linkage =                               
{                                                       
    0x00000013,                                         
    module_init,                                       
    module_deinit,                                      
    0,                                                  
    0,                        

    FS9LINK_VERSION, { 0 }
};

// 
// end of cabin_comfort.cpp
//
