/************************************************************************************************************** 
    SimpleCar.cpp

    Copyright (C) Lockheed Martin Corporation.  All rights reserved.
    
    Description: A sample demonstrating how to implement a simple car ISimObject.
***************************************************************************************************************/

#include <atlcomcli.h>
#include <math.h>
#include <Pdk.h>
#include <ISimObject.h>

using namespace P3D;

static double GetPolarRadius();
static double GetEquatorialRadius();

class Simulation : public ISimulationV01
{
public:
    Simulation(IBaseObjectV400* pBaseObject) : m_RefCount(1), m_spBaseObject(pBaseObject), m_dThrottle(0) 
    {
        memset(&m_SurfaceInfo, 0, sizeof(m_SurfaceInfo));
        memset(&m_WeatherInfo, 0, sizeof(m_WeatherInfo));
    }

private:
    CComPtr<IBaseObjectV400> m_spBaseObject;

    volatile LONG   m_RefCount;
    STDMETHOD_(ULONG, AddRef)()   override {return InterlockedIncrement(&m_RefCount) ;}
    STDMETHOD_(ULONG, Release)()  override 
    {
        ULONG   RetVal = InterlockedDecrement(&m_RefCount);
        if      (RetVal == 0)         {delete this;}
        else if (RetVal & 0x80000000) {__debugbreak();}
        return RetVal;
    }

    STDMETHOD (QueryInterface)(__in REFIID riid, void **ppv)  override 
    {
        HRESULT hr = E_NOINTERFACE;

        if (!ppv)
        {
            goto Error;
        }
    
        *ppv = NULL;

        if (IsEqualIID(riid , IID_IUnknown))
        {
            *ppv = static_cast<IUnknown*>(this);
        }        
        else if (IsEqualIID(riid , IID_ISimulation))
        {
            *ppv = static_cast<ISimulation*>(this);
        }        
        else if (IsEqualIID(riid , IID_ISimulationV01))
        {
            *ppv = static_cast<ISimulationV01*>(this);
        }        

        if (*ppv)
        {
            hr = S_OK;
            AddRef();
        }

    Error:

        return hr;
    }

    STDMETHOD (SaveLoadState)(__in __notnull PSaveLoadCallback pfnCallback, __in const BOOL bSave) override {return S_OK;}  

    STDMETHOD (Update)(double dDeltaT)   override 
    {
        if (IsNormalMode())
        {
            static const float MAX_SPEED = 50.0f;

            DXYZ vPos;
            DXYZ vOrient;
            DXYZ vdVel;
            DXYZ vdRotVel;

            //Update surface and weather info
            m_spBaseObject->GetSurfaceInformation(m_SurfaceInfo, NULL);
            m_spBaseObject->GetWeatherInformation(m_WeatherInfo);

            //Get current position
            m_spBaseObject->GetPosition(vPos, vOrient, vdVel, vdRotVel);
    
            //Update new velocity
            vdVel.dZ = m_dThrottle * MAX_SPEED * cos(vOrient.dY);
            vdVel.dX = m_dThrottle * MAX_SPEED * sin(vOrient.dY);

            //Integrate postion
            vPos.dX += vdVel.dX * cos(vPos.dZ) * dDeltaT / GetEquatorialRadius();
            vPos.dY += vdVel.dY * dDeltaT;
            vPos.dZ += vdVel.dZ * dDeltaT / GetPolarRadius();

            m_spBaseObject->SetPosition(vPos, vOrient, vdVel, vdRotVel, /*assume on ground*/TRUE, dDeltaT);
        }

        return S_OK;
    }

public:
    void SetThrottle(double dThrottle)  {m_dThrottle = max(min(dThrottle, 1.0), 0.0);}
    void IncThrottle()                  {SetThrottle(m_dThrottle += 0.5);}
    void DecThrottle()                  {SetThrottle(m_dThrottle -= 0.5);}

    double GetThrottle() const  {return m_dThrottle;}

private:

    BOOL IsNormalMode() const 
    {
        return (0 == (m_spBaseObject->GetMode() & ( SIMOBJECT_MODE_PAUSE | SIMOBJECT_MODE_DISABLED | SIMOBJECT_MODE_SLEW | SIMOBJECT_MODE_CRASH | SIMOBJECT_MODE_FREEZE_POSITION
                                                    | SIMOBJECT_MODE_FREEZE_ATTITUDE | SIMOBJECT_MODE_FREEZE_ALTITUDE | SIMOBJECT_MODE_FREEZE_LATLON)));
    }

    SurfaceInfoV400 m_SurfaceInfo;
    WeatherInfoV400 m_WeatherInfo;

    double m_dThrottle;

};

// Ensure this GUID matches the CategoryId in the sim.cfg
class __declspec(uuid("{3F3B2498-7E88-420B-A46D-C9EEF6BB59D0}")) SimpleCar : public ISimObjectV02
{
public:

    //IServiceProvider Implementation
    volatile LONG   m_RefCount;
    STDMETHOD_(ULONG, AddRef)()   override {return InterlockedIncrement(&m_RefCount) ;}
    STDMETHOD_(ULONG, Release)()  override 
    {
        ULONG   RetVal = InterlockedDecrement(&m_RefCount);
        if      (RetVal == 0)         {delete this;}
        else if (RetVal & 0x80000000) {__debugbreak();}
        return RetVal;
    }

    STDMETHOD (QueryInterface)(__in REFIID riid, void **ppv)  override 
    {
        HRESULT hr = E_NOINTERFACE;

        if (!ppv)
        {
            goto Error;
        }
    
        *ppv = NULL;

        if (IsEqualIID(riid, IID_ISimObject))
        {
            *ppv = static_cast<ISimObject*>(this);
        }
        else if (IsEqualIID(riid, IID_ISimObjectV01))
        {
            *ppv = static_cast<ISimObjectV01*>(this);
        }
        else if (IsEqualIID(riid, IID_ISimObjectV02))
        {
            *ppv = static_cast<ISimObjectV02*>(this);
        }
        else if (IsEqualIID(riid , IID_IServiceProvider))
        {
            *ppv = static_cast<IServiceProvider*>(this);
        }
        else if (IsEqualIID(riid , IID_IUnknown))
        {
            *ppv = static_cast<IUnknown*>(this);
        }        


        if (*ppv)
        {
            hr = S_OK;
            AddRef();
        }

    Error:

        return hr;
    }
    
    STDMETHOD (QueryService)(  __in REFGUID guidService, __in REFIID riid, void **ppv) override 
    {
        HRESULT hr = E_NOINTERFACE;

        if (!ppv)
        {
            goto Error;
        }
    
        *ppv = NULL;
    
        if (IsEqualIID(guidService, SID_SimObject))
        {
            hr = QueryInterface(riid, ppv);
        }

    Error:

        return hr;

    }

    //ISimObject implementations
    STDMETHOD (LoadConstantData)(__out void** ppConstantData)       override {return S_OK;}       //on-disk data.  return loaded data
    STDMETHOD (UnloadConstantData)(__inout void** ppConstantData)   override {return S_OK;}       //on-disk data.  should point to loaded data, and return NULL
    STDMETHOD (LoadDynamicData)()                                   override {return S_OK;}       //e.g. create runtime subsystems based on disk data
    STDMETHOD (Init)()                                              override {return S_OK;}       //e.g. init relationships between subsystems
    STDMETHOD (DeInit)()                                            override {return S_OK;}
    STDMETHOD_(BOOL, SupportsLabels)()                      const   override {return m_bSupportsLabel;}                      //Does this simobject support label display?
    STDMETHOD (SetSupportsLabels)(BOOL bOn)                         override {m_bSupportsLabel = (bOn != FALSE); return S_OK;}     //Should this simobject support label display?
    STDMETHOD_(void, OnModeChange)(int bfMode)                      override {}                  //Called when a mode has changed, with the new mode bitfield
    STDMETHOD_(void, OnPositionInit)()                              override {}                  //Called upon change in non-realtime object positions   (e.g. UI, slew)
    STDMETHOD_(void, OnSpeedInit)(float fSpeed)                     override {}                  //(Feet/Second) Called upon init in speed   (e.g. UI)

    //Factory function
    static HRESULT New(__in __notnull IBaseObjectV400* pBaseObject, __out __notnull ISimObject** ppThisSim) 
    {
        HRESULT hr = E_FAIL;
        CComPtr<IBaseObjectV400> spBaseObjectV01;

        if (!ppThisSim)
        {
            goto Error;
        }

        *ppThisSim = NULL;

        if (SUCCEEDED(pBaseObject->QueryInterface(IID_IBaseObjectV400, (void**)&spBaseObjectV01)))
        {
            *ppThisSim = new SimpleCar(spBaseObjectV01);
        }

        if (!*ppThisSim)
        {
            goto Error;
        }

        hr = spBaseObjectV01->RegisterSimulation(&static_cast<SimpleCar*>(*ppThisSim)->m_Simulation, 60.0f /*Hz*/);


    Error:
        return hr;
    }

    static STDMETHODIMP SetThrottle(__in ISimObject& Sim, __in double dProperty, __in int iIndex) 
    {
        //dProperty = (1.0 - dProperty) / 2.0;
        dProperty = 0.5 - 0.51 * dProperty;
        dProperty = max(dProperty, 0);
        dProperty = min(dProperty, 1.0);

        static_cast<SimpleCar&>(Sim).SetThrottle((float)(dProperty)); 
        return S_OK;

    }

    static STDMETHODIMP IncThrottle(__in ISimObject& Sim, __in double dProperty, __in int iIndex) 
    {
        static_cast<SimpleCar&>(Sim).IncThrottle(); 
        return S_OK;
    }

    static STDMETHODIMP DecThrottle(__in ISimObject& Sim, __in double dProperty, __in int iIndex) 
    {
        static_cast<SimpleCar&>(Sim).DecThrottle(); 
        return S_OK;
    }

    static STDMETHODIMP GetThrottle(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const SimpleCar&>(Sim).GetThrottle(); 
        return S_OK;
    }

    static WorldConstants  ms_WorldConstants;

private:

    SimpleCar(IBaseObjectV400* pBaseObject) : m_Simulation(pBaseObject), m_bSupportsLabel(FALSE), m_RefCount(1)  {}

    void SetThrottle(double dThrottle)  {m_Simulation.SetThrottle(dThrottle);}
    void IncThrottle()                  {m_Simulation.IncThrottle();}
    void DecThrottle()                  {m_Simulation.DecThrottle();}

    double GetThrottle() const  {return m_Simulation.GetThrottle();}


    Simulation m_Simulation;
    BOOL m_bSupportsLabel;
};

//Unique class GUID
#define CLSID_Car __uuidof(SimpleCar)

REFGUID GetClassId() {return CLSID_Car;}

WorldConstants SimpleCar::ms_WorldConstants;

//Factory function wrapper
HRESULT New(__in __notnull IBaseObjectV400* pBaseObject, __out __notnull ISimObject** ppThisSim)
{
    return SimpleCar::New(pBaseObject, ppThisSim);
}

//Property Registration
HRESULT RegisterProperties(__in __notnull ISimObjectManagerV400* pSimObjectMgr)
{
    HRESULT hr = E_FAIL;

    pSimObjectMgr->RegisterProperty(CLSID_Car, TEXT("Throttle"),    TEXT("percent over 100"), SimpleCar::SetThrottle, EVENTTYPE_AXIS);
    pSimObjectMgr->RegisterProperty(CLSID_Car, TEXT("IncThrottle"), TEXT("percent over 100"), SimpleCar::IncThrottle, EVENTTYPE_NORMAL);
    pSimObjectMgr->RegisterProperty(CLSID_Car, TEXT("DecThrottle"), TEXT("percent over 100"), SimpleCar::DecThrottle, EVENTTYPE_NORMAL);
    pSimObjectMgr->RegisterProperty(CLSID_Car, TEXT("Throttle"),    TEXT("percent over 100"), SimpleCar::GetThrottle);

    hr = pSimObjectMgr->GetWorldConstants(SimpleCar::ms_WorldConstants);

    return S_OK;
}

static double GetPolarRadius()       {return SimpleCar::ms_WorldConstants.m_dPolarRadius;}
static double GetEquatorialRadius()  {return SimpleCar::ms_WorldConstants.m_dEquatorialRadius;}
