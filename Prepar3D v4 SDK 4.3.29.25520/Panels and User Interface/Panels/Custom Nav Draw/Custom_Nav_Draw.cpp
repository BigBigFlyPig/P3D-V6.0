///----------------------------------------------------------------------------
///
///  Copyright 2017 Lockheed Martin Corporation
///  Lockheed Martin Proprietary Information
///
///----------------------------------------------------------------------------
#define _CRT_RAND_S
#include "gauges.h"
#include "Helpers\initpdk.h"
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

// PanelCallback is an abstract base class that can be overridden.  Implementors
// should override the functions CreateAircraftCallback(UINT32 ContainerId) as
// well as GetPropertyTable.
class PanelCallback : public IPanelCCallback
{
    DECLARE_PANEL_CALLBACK_REFCOUNT(PanelCallback);

public:
    PanelCallback();

    // ******* IPanelCCallback Methods *****************    
    IPanelCCallback* QueryInterface(PCSTRINGZ pszInterface);
    UINT32 GetVersion();
    bool ConvertStringToProperty(PCSTRINGZ keyword, SINT32* pID);
    bool ConvertPropertyToString(SINT32 id, PPCSTRINGZ pKeyword);
    bool GetPropertyUnits(SINT32 id, ENUM* pEnum);

protected:
    // ******** PanelCallback Methods ******************
    virtual const PROPERTY_TABLE *GetPropertyTable(UINT &uLength) = 0;
};

// 
// AircraftCallback is an abstract base class that can be overridden.  Implementors
// should override the function CreateGaugeCCallback()
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

bool PanelCallback::ConvertStringToProperty(PCSTRINGZ keyword, SINT32* pID)
{
    if (!keyword)
    {
        return false;
    }
    if (!pID)
    {
        return false;
    }

    UINT uNumProperties;
    const PROPERTY_TABLE *parPropertyTable = GetPropertyTable(uNumProperties);

    for (UINT i = 0; i < uNumProperties; i++)
    {
        if (_stricmp(parPropertyTable[i].szPropertyName, keyword) == 0)
        {
            *pID = i;
            return true;
        }
    }
    return false;
}

bool PanelCallback::ConvertPropertyToString(SINT32 id, PPCSTRINGZ pKeyword)
{
    if (!pKeyword)
    {
        return false;
    }

    UINT uNumProperties;
    const PROPERTY_TABLE *parPropertyTable = GetPropertyTable(uNumProperties);

    if (id < 0 || id >= (SINT32)uNumProperties)
    {
        return false;
    }
    *pKeyword = parPropertyTable[id].szPropertyName;
    return true;
}

bool PanelCallback::GetPropertyUnits(SINT32 id, ENUM* pEnum)
{
    if (!pEnum)
    {
        return false;
    }

    UINT uNumProperties;
    const PROPERTY_TABLE *parPropertyTable = GetPropertyTable(uNumProperties);

    if (id < 0 || id >= (SINT32)uNumProperties)
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
static const char NAVIGATION_EXAMPLE_CALLBACK_NAME[] = "NAV_DRAW";

#define ROOM_TEMPERATURE		25			// degrees celcius

static PROPERTY_TABLE NAVIGATION_PROPERTY_TABLE[] =
{
    { "Nav",                        "Number",       UNITS_UNKNOWN },
};
// Enum that contains the properties 
enum VAR
{
    NAV_DRAW,
};

//
// Class that implements IGaugeCCallback
//
class NavigationGaugeCallback : public IGaugeCCallback
{
    DECLARE_PANEL_CALLBACK_REFCOUNT(NavigationGaugeCallback);

public:
    NavigationGaugeCallback(UINT32 containerId);

    // ************* IGaugeCCallback Methods ***************
    IGaugeCCallback* QueryInterface(PCSTRINGZ pszInterface);
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


DEFINE_PANEL_CALLBACK_REFCOUNT(NavigationGaugeCallback)

NavigationGaugeCallback::NavigationGaugeCallback(UINT32 containerId)
    : m_RefCount(1),
    m_containerId(containerId)
{

}
IGaugeCCallback* NavigationGaugeCallback::QueryInterface(PCSTRINGZ pszInterface)
{
    return NULL;
}
//
// The Update functions is called on a 18Hz cycle
//
void NavigationGaugeCallback::Update()
{

}
//
// Getting float/numeric values
//
bool NavigationGaugeCallback::GetPropertyValue(SINT32 id, FLOAT64* pValue)
{
    return false;
}
//
// Getting string property values
//
bool NavigationGaugeCallback::GetPropertyValue(SINT32 id, LPCSTR* pszValue)
{
    return false;
}
bool NavigationGaugeCallback::GetPropertyValue(SINT32 id, LPCWSTR* pszValue)
{
    return false;
}
//
// Setting float/numeric values
//
bool NavigationGaugeCallback::SetPropertyValue(SINT32 id, FLOAT64 value)
{
    return false;
}
//
// Setting string values
//
bool NavigationGaugeCallback::SetPropertyValue(SINT32 id, LPCSTR szValue)
{
    return false;   // String properties not permitted to be set
}
bool NavigationGaugeCallback::SetPropertyValue(SINT32 id, LPCWSTR szValue)
{
    return false;   // String properties not permitted to be set
}

#include "NavigationDrawable.h"

IGaugeCDrawable* NavigationGaugeCallback::CreateGaugeCDrawable(SINT32 id, const IGaugeCDrawableCreateParameters* pParameters)
{
    Nav::NavigationDrawable* pDrawable = NULL;

    if (id == Nav::GAUGE_NAV)
    {
        pDrawable = new Nav::NavigationDrawable(pParameters);
    }

    if (pDrawable)
    {
        pDrawable->Init();
    }

    return pDrawable;
}

//
// AircraftCallback Override
//
class NavigationAircraftCallback : public AircraftCallback
{
private:

public:
    NavigationAircraftCallback(UINT32 ContainerID) : AircraftCallback(ContainerID)
    {}
    IGaugeCCallback* CreateGaugeCCallback()
    {
        return new NavigationGaugeCallback(GetContainerId());
    }
};

//
// PanelCallback Override
//

class NavigationPanelCallback : public PanelCallback
{
public:

    NavigationPanelCallback::NavigationPanelCallback()
    {
        // init property table
        for (int n = 0; n < 5; n++)
        {
            if (ImportTable.PANELSentry.fnptr != NULL &&
                NAVIGATION_PROPERTY_TABLE[n].units == UNITS_UNKNOWN)
            {
                NAVIGATION_PROPERTY_TABLE[n].units = get_units_enum(NAVIGATION_PROPERTY_TABLE[n].szUnitsName);
            }
        }
    }

    IAircraftCCallback* CreateAircraftCCallback(UINT32 ContainerID)
    {
        return new NavigationAircraftCallback(ContainerID);
    }


protected:
    const PROPERTY_TABLE *GetPropertyTable(UINT &uLength)
    {
        uLength = LENGTHOF(NAVIGATION_PROPERTY_TABLE);
        return NAVIGATION_PROPERTY_TABLE;
    }
};

void NavigationPanelCallbackInit()
{

    NavigationPanelCallback *pNavigationPanelCallback = new NavigationPanelCallback();

    if (pNavigationPanelCallback)
    {
        bool b = P3D::PdkServices::GetPanelSystem()->PanelRegisterCCallback(NAVIGATION_EXAMPLE_CALLBACK_NAME, pNavigationPanelCallback);
        pNavigationPanelCallback->Release();
    }
}

void NavigationPanelCallbackDeInit()
{
    P3D::PdkServices::GetPanelSystem()->PanelRegisterCCallback(NAVIGATION_EXAMPLE_CALLBACK_NAME, NULL);
}


// The Panels pointer will get filled in during the loading process
// if this DLL is listed in DLL.XML

PPANELS Panels = NULL;

GAUGESIMPORT    ImportTable =
{
    { 0x0000000F, (PPANELS)NULL },
    { 0x00000000, NULL }
};

void FSAPI  module_init(__in __notnull P3D::IPdk* pPdk)
{
    P3D::PdkServices::Init(pPdk);
    
    if (P3D::PdkServices::GetPanelSystem())
    {
        ImportTable.PANELSentry.fnptr = (PPANELS)Panels;
        NavigationPanelCallbackInit();
    }
}

void FSAPI  module_deinit(void) {
    NavigationPanelCallbackDeInit();
    P3D::PdkServices::Shutdown();
}

BOOL WINAPI DllMain(HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
    return TRUE;
}

// This is the module's export table.
GAUGESLINKAGE   Linkage =
{
    0x00000013,
    0,
    0,
    0,
    0,

    FS9LINK_VERSION,{ 0 }
};
