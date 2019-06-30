/************************************************************************************************************** 
    Countermeasure.cpp

    Copyright (C) Lockheed Martin Corporation.  All rights reserved.
    
    Description: A sample demonstrating how to implement a Countermeasure ISimObject.
***************************************************************************************************************/

#include <atlcomcli.h>
#include <InitGuid.h>
#include <math.h>
#include <Pdk.h>
#include <ISimObject.h>
#include <ISimObjectAttachments.h>

using namespace P3D;

extern const ISimObjectManagerV400* GetSimObjectManager();

static double GetPolarRadius();
static double GetEquatorialRadius();
static double GetGravity();

class Simulation : public ISimulationV01
{
public:
    Simulation(IBaseObjectV400* pBaseObject) 
        :   m_RefCount(1), 
            m_spBaseObject(pBaseObject),
            m_bIsAttached(FALSE),
            m_pEffect(NULL),
            m_fLaunchImpulseTime(2.0f),
            m_fOnGroundTime(0.0f),
            m_uOwnerId(0)

    {
        memset(&m_vPos,    0, sizeof(m_vPos));
        memset(&m_vOrient, 0, sizeof(m_vOrient));
        memset(&m_vVel,    0, sizeof(m_vVel));
        memset(&m_vRotVel, 0, sizeof(m_vRotVel));

        memset(&m_SurfaceInfo, 0, sizeof(m_SurfaceInfo));
        memset(&m_WeatherInfo, 0, sizeof(m_WeatherInfo));
    }

    ~Simulation()
    {
        TriggerEffect(FALSE);
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
        //Very simple simulation
        if (IsNormalMode() && !m_bIsAttached)
        {
            static const float MAX_SPEED = 50.0f;

            //Update surface and weather info
            m_spBaseObject->GetSurfaceInformation(m_SurfaceInfo, NULL);
            m_spBaseObject->GetWeatherInformation(m_WeatherInfo);

            //Get current position
            m_spBaseObject->GetPosition(m_vPos, m_vOrient, m_vVel, m_vRotVel);  

            //Init acceleration vector
            DXYZ vAcceleration = {0, -GetGravity(), 0};

            //If not yet on ground
            if (m_vPos.dY > m_SurfaceInfo.m_fElevation)
            {
                //Impulse upon launch
                if (m_fLaunchImpulseTime > 0)
                {
                    m_fLaunchImpulseTime -= (float)dDeltaT;
                    m_fLaunchImpulseTime = max(m_fLaunchImpulseTime, 0.0f);

                    float fRandomFactor = (rand() % 10) / 10.0f;

                    vAcceleration.dZ += 8.0f * cos(m_vOrient.dY);
                    vAcceleration.dX += 8.0f * sin(m_vOrient.dY) * fRandomFactor;
                    vAcceleration.dY += 38.0f;
                }
                else
                {
                    //Decelerate horizontal movement
                    vAcceleration.dX = m_vVel.dX * -0.05;
                    vAcceleration.dZ = m_vVel.dZ * -0.05;
                }
            }
            else
            {
                //Decelerate on ground for a few seconds, then destroy.
                m_fOnGroundTime += (float)dDeltaT;

                vAcceleration.dX = m_vVel.dX * -0.2;
                vAcceleration.dZ = m_vVel.dZ * -0.2;

                vAcceleration.dY = 0;
                m_vVel.dY = 0.0f;

                if (m_fOnGroundTime > 3.0f)
                {
                    m_spBaseObject->Destroy();
                }
            }

            //Integrate
            m_vVel.dX += vAcceleration.dX * dDeltaT;
            m_vVel.dY += vAcceleration.dY * dDeltaT;
            m_vVel.dZ += vAcceleration.dZ * dDeltaT;

            //Integrate postion
            m_vPos.dX += m_vVel.dX * cos(m_vPos.dZ) * dDeltaT / GetEquatorialRadius();
            m_vPos.dY += m_vVel.dY * dDeltaT;
            m_vPos.dZ += m_vVel.dZ * dDeltaT / GetPolarRadius();

            m_spBaseObject->SetPosition(m_vPos, m_vOrient, m_vVel, m_vRotVel, /*assume on ground*/FALSE, dDeltaT);

            TriggerEffect(TRUE);
            RedirectGuidedMissiles();
        }

        return S_OK;
    }

public:
    //Get owner's object id
    UINT GetOwnerId()           const {return m_uOwnerId;}
    BOOL IsAttachedToOwner()    const {return m_bIsAttached;}

    HRESULT SetIsAttachedToOwner(BOOL bAttached, UINT uOwnerId)
    {
        HRESULT hr = E_FAIL;

        if (bAttached != m_bIsAttached)
        {
            m_bIsAttached = bAttached;

            hr = S_OK;
        }

        if(m_bIsAttached)
        {
            m_uOwnerId = uOwnerId;
            
            hr = S_OK;
        }

        return hr;
    }


private:
    /*********************************************************************************************
    Redirect Guided Missiles
    *********************************************************************************************/

    void RedirectGuidedMissiles()
    {
        static const UINT REQUESTED_OBJECT_COUNT = 30;
        UINT rgObjectIDs[REQUESTED_OBJECT_COUNT] = {0};
        UINT nObjects = REQUESTED_OBJECT_COUNT;

        if (SUCCEEDED(GetSimObjectManager()->GetObjectsInRadius(m_vPos, 1000.0f /* fRadiusFeet*/, nObjects, rgObjectIDs)))
        {
            for (UINT i = 0; i < nObjects; i++)
            {
                //Don't bother with parent aircraft
                if (m_uOwnerId == rgObjectIDs[i])
                {
                    continue;
                }

                //Don't bother with self
                if (m_spBaseObject->GetId() == rgObjectIDs[i])
                {
                    continue;
                }

                //If object's guidance system is targeting this owner, retarget it to this
                CComPtr<IBaseObjectV400> spObject;
                CComPtr<IGuidanceSystem> spGuidanceSystem;

                if (   SUCCEEDED(GetSimObjectManager()->GetObject(rgObjectIDs[i], &spObject))
                    && SUCCEEDED(spObject->QueryService(SID_GuidanceSystem,  IID_IGuidanceSystem,  (void**)&spGuidanceSystem))
                    && spGuidanceSystem->GetTargetObjectID() == m_uOwnerId)
                    {                        
                        spGuidanceSystem->SetTargetObjectID(m_spBaseObject->GetId());
                    }

            }
        }
    }

    /*********************************************************************************************
    *********************************************************************************************/
    BOOL IsNormalMode() const 
    {
        return (0 == (m_spBaseObject->GetMode() & ( SIMOBJECT_MODE_PAUSE | SIMOBJECT_MODE_DISABLED | SIMOBJECT_MODE_SLEW | SIMOBJECT_MODE_CRASH | SIMOBJECT_MODE_FREEZE_POSITION
                                                    | SIMOBJECT_MODE_FREEZE_ATTITUDE | SIMOBJECT_MODE_FREEZE_ALTITUDE | SIMOBJECT_MODE_FREEZE_LATLON)));
    }

    HRESULT TriggerEffect(BOOL bOn)
    {
        HRESULT hr = S_OK;

        if (bOn && !m_pEffect)
        {
            hr = m_spBaseObject->VisualEffectOn(TEXT("fx_flareCountermeasure"), NULL, &m_pEffect);
        }
        else if (!bOn && m_pEffect)
        {
            hr = m_spBaseObject->VisualEffectOff(m_pEffect);
        }

        return hr;
    }

    DXYZ m_vPos;
    DXYZ m_vOrient;
    DXYZ m_vVel;
    DXYZ m_vRotVel;

    BOOL m_bIsAttached;
    UINT m_uOwnerId;
    void* m_pEffect;
    float m_fLaunchImpulseTime;
    float m_fOnGroundTime;

    SurfaceInfoV400 m_SurfaceInfo;
    WeatherInfoV400 m_WeatherInfo;
        
};

// Ensure this GUID matches the CategoryId in the sim.cfg
class __declspec(uuid("{51967229-F0B1-44EF-A14A-40093E828954}")) Countermeasure : public ISimObjectV02, public P3D::ICountermeasureServiceV01
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

        if (IsEqualIID(riid, IID_ISimObjectV01))
        {
            *ppv = static_cast<ISimObjectV01*>(this);
        }
        else if (IsEqualIID(riid, IID_ISimObjectV02))
        {
            *ppv = static_cast<ISimObjectV02*>(this);
        }
        else if (IsEqualIID(riid, IID_ISimObject))
        {
            *ppv = static_cast<ISimObject*>(this);
        }
        else if (IsEqualIID(riid , IID_IServiceProvider))
        {
            *ppv = static_cast<IServiceProvider*>(this);
        }
        else if (IsEqualIID(riid , IID_IUnknown))
        {
            *ppv = static_cast<Countermeasure*>(this);
        }        
        else if (IsEqualIID(riid , IID_ICountermeasureService))
        {
            *ppv = static_cast<ICountermeasureService*>(this);
        }        
        else if (IsEqualIID(riid , IID_ICountermeasureServiceV01))
        {
            *ppv = static_cast<ICountermeasureServiceV01*>(this);
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
        else if (IsEqualIID(guidService, SID_CountermeasureService))
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
    STDMETHOD_(void, OnModeChange)(int bfMode)                      override {}                   //Called when a mode has changed, with the new mode bitfield
    STDMETHOD_(void, OnPositionInit)()                              override {}                   //Called upon change in non-realtime object positions   (e.g. UI, slew)
    STDMETHOD_(void, OnSpeedInit)(float fSpeed)                     override {}                  //(Feet/Second) Called upon init in speed   (e.g. UI)

    //ICountermeasure implementations
    STDMETHOD  (SetIsAttachedToOwner)(BOOL bAttached, UINT uOwnerId)          override {return m_Simulation.SetIsAttachedToOwner(bAttached, uOwnerId);}     //Called from weapon system when fired (0 = invalid id)
    STDMETHOD_ (BOOL, IsAttachedToOwner)()                              const override {return m_Simulation.IsAttachedToOwner();}
    STDMETHOD_ (UINT, GetOwnerId)()                                     const override {return m_Simulation.GetOwnerId();}                           //ID of object in which countermeasure is attached
    STDMETHOD (GetAttachOffsetFeet)(__out P3D::DXYZ& vOffset)           const override {vOffset.dX = 0; vOffset.dY = 0; vOffset.dZ = 0; return S_OK;}

    //Factory function
    static HRESULT New(__in __notnull IBaseObjectV400* pBaseObject, __out __notnull ISimObject** ppThisSim) 
    {
        HRESULT hr = E_FAIL;
        CComPtr<IBaseObjectV400> spBaseObject;

        if (!ppThisSim)
        {
            goto Error;
        }

        if (FAILED(pBaseObject->QueryInterface(IID_IBaseObjectV400, (void**)&spBaseObject)))
        {
            goto Error;
        }

        *ppThisSim = new Countermeasure(spBaseObject);

        if (!*ppThisSim)
        {
            goto Error;
        }

        hr = spBaseObject->RegisterSimulation(&static_cast<Countermeasure*>(*ppThisSim)->m_Simulation, 60.0f /*Hz*/);


    Error:
        return hr;
    }

    static WorldConstants  ms_WorldConstants;

private:

    Countermeasure(IBaseObjectV400* pBaseObject) : m_Simulation(pBaseObject), m_bSupportsLabel(FALSE), m_RefCount(1)  {}

    Simulation m_Simulation;
    BOOL m_bSupportsLabel;
};

//Unique class Guid
#define CLSID_Countermeasure __uuidof(Countermeasure)

REFGUID GetClassId() {return CLSID_Countermeasure;}

WorldConstants Countermeasure::ms_WorldConstants;

//Factory function wrapper
HRESULT New(__in __notnull IBaseObjectV400* pBaseObject, __out __notnull ISimObject** ppThisSim)
{
    return Countermeasure::New(pBaseObject, ppThisSim);
}

//Property Registration
HRESULT RegisterProperties(__in __notnull ISimObjectManagerV400* pSimObjectMgr)
{
    HRESULT hr = E_FAIL;

    hr = pSimObjectMgr->GetWorldConstants(Countermeasure::ms_WorldConstants);

    return S_OK;
}

static double GetPolarRadius()       {return Countermeasure::ms_WorldConstants.m_dPolarRadius;}
static double GetEquatorialRadius()  {return Countermeasure::ms_WorldConstants.m_dEquatorialRadius;}
static double GetGravity()           {return Countermeasure::ms_WorldConstants.m_fGravitySeaLevel;}
