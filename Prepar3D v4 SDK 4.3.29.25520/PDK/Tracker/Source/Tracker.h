/**************************************************************************************************************
Copyright (C) Lockheed Martin Corporation.  All rights reserved.

Description: A sample demonstrating how to set postion on an ISimObject
***************************************************************************************************************/

#pragma once

#include <atlcomcli.h>
#include <initguid.h>

#include <ISimObject.h>

using namespace P3D;

class ILocationDataProvider;

class Simulation : public ISimulationV310
{
public:

    Simulation(IBaseObjectV400* pBaseObject);
    ~Simulation();

    STDMETHOD(QueryInterface)(__in REFIID riid, void **ppv) override;

private:

    CComPtr<IBaseObjectV400> m_spBaseObject;

    volatile LONG m_RefCount;

    STDMETHOD_(ULONG, AddRef)()   override { return InterlockedIncrement(&m_RefCount); }
    STDMETHOD_(ULONG, Release)()  override
    {
        ULONG   RetVal = InterlockedDecrement(&m_RefCount);
        if (RetVal == 0)         { delete this; }
        else if (RetVal & 0x80000000) { __debugbreak(); }
        return RetVal;
    }

    STDMETHOD(SaveLoadState)(__in __notnull PSaveLoadCallback pfnCallback, __in const BOOL bSave) override { return S_OK; }

    STDMETHOD(Serialize)(__in NetOutPublic& netOut) { return E_ABORT; }
    STDMETHOD(Deserialize)(__in NetInPublic& netIn) { return S_OK; }

    STDMETHOD(Update)(double dDeltaT) override;

    /*********************************************************************************************
    *********************************************************************************************/
    BOOL IsNormalMode() const;

    enum class TrackerState
    {
        WaitingForFirstPoint,
        WaitingForSecondPoint,
        Tracking
    };
    
    TrackerState m_eTrackerState;

    ILocationDataProvider* m_pDataProvider;

    DXYZ m_vPosCurrent;
    DXYZ m_vPosTarget;

    DXYZ m_vVelCurrent;

    DXYZ m_vOrientCurrent;
    DXYZ m_vOrientTarget;

    DXYZ m_vTrajectory;

    SurfaceInfoV400 m_SurfaceInfo;
    WeatherInfoV400 m_WeatherInfo;
};

// Ensure this GUID matches the CategoryId in the sim.cfg
class __declspec(uuid("{F1E6CE44-2E84-47C2-B317-075CA175CCF5}")) Tracker : public ISimObjectV03
{
public:

    //IServiceProvider Implementation
    volatile LONG m_RefCount;
    STDMETHOD_(ULONG, AddRef)()   override { return InterlockedIncrement(&m_RefCount); }
    STDMETHOD_(ULONG, Release)()  override
    {
        ULONG   RetVal = InterlockedDecrement(&m_RefCount);
        if (RetVal == 0)         { delete this; }
        else if (RetVal & 0x80000000) { __debugbreak(); }
        return RetVal;
    }

    STDMETHOD(QueryInterface)(__in REFIID riid, void **ppv) override;
    STDMETHOD(QueryService)(__in REFGUID guidService, __in REFIID riid, void **ppv) override;

    //ISimObject implementations
    STDMETHOD(LoadConstantData)(__out void** ppConstantData)       override { return S_OK; }         //on-disk data.  return loaded data
    STDMETHOD(UnloadConstantData)(__inout void** ppConstantData)   override { return S_OK; }         //on-disk data.  should point to loaded data, and return NULL
    STDMETHOD(LoadDynamicData)()                                   override { return S_OK; }         //e.g. create runtime subsystems based on disk data
    STDMETHOD(Init)()                                              override { return S_OK; }         //e.g. init relationships between subsystems
    STDMETHOD(DeInit)()                                            override { return S_OK; }
    STDMETHOD_(BOOL, SupportsLabels)()                       const override { return m_bSupportsLabel; }                         //Does this simobject support label display?
    STDMETHOD(SetSupportsLabels)(BOOL bOn)                         override { m_bSupportsLabel = (bOn != FALSE); return S_OK; }  //Should this simobject support label display?
    STDMETHOD_(void, OnModeChange)(int bfMode)                     override {}                     //Called when a mode has changed, with the new mode bitfield
    STDMETHOD_(void, OnPositionInit)()                             override {}                     //Called upon change in non-realtime object positions   (e.g. UI, slew)
    STDMETHOD_(void, OnSpeedInit)(float fSpeed)                    override {}                     //(Feet/Second) Called upon init in speed   (e.g. UI)
    STDMETHOD(QueryBaseObject)(REFIID riid, void** ppv)     override;                       //Accessor to get the base object from th ISimObject
    
    //Factory function
    static HRESULT New(__in __notnull IBaseObjectV400* pBaseObject, __out __notnull ISimObject** ppThisSim);

    static WorldConstants ms_WorldConstants;

private:

    Tracker(IBaseObjectV400* pBaseObject);

    Simulation m_Simulation;
    BOOL m_bSupportsLabel;
};

//Unique class Guid
#define CLSID_Tracker __uuidof(Tracker)

REFGUID GetClassId() { return CLSID_Tracker; }

WorldConstants Tracker::ms_WorldConstants;
