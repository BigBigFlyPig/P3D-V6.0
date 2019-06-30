/************************************************************************************************************** 
    DLLMain.cpp

    Copyright (C) Lockheed Martin Corporation.  All rights reserved.
    
    Description: External instance-based property sample
***************************************************************************************************************/

#include <atlcomcli.h>
#include <InitGuid.h>
#include <string>

//From Prepar3D SDK
#include <Pdk.h>   
#include <ISimObject.h>

#include <strsafe.h>

using namespace P3D;

DEFINE_GUID(SID_RadarAlitimeter, 0x4d38509c, 0xaec1, 0x4b0f, 0xa8, 0xe1, 0xbd, 0xc2, 0x81, 0x62, 0x76, 0x2a);
DEFINE_GUID(CLSID_RadarAltimeter,0x9d508d94, 0x6214, 0x43a8, 0x9c, 0xf6, 0x5e, 0x54, 0x15, 0xb6, 0x34, 0x56);

/********************************************************************
Sample SubSystem
********************************************************************/
class RadarAltimeter : public ISimulation
{
public:
    RadarAltimeter(IBaseObjectV400* pBaseObject) 
        :   m_RefCount(1), 
            m_spBaseObject(pBaseObject), 
            m_fRadarAltitude(0) 
    {
        memset(&m_vAngle, 0, sizeof(m_vAngle));

        m_sState = TEXT("ALTITUDE LOW");
    }

    ~RadarAltimeter() {}

    /**** IUnknown ************/
    volatile LONG   m_RefCount;\
    ULONG STDMETHODCALLTYPE AddRef() override {return InterlockedIncrement(&m_RefCount) ;}
    ULONG STDMETHODCALLTYPE Release() override 
    {
        ULONG   RetVal = InterlockedDecrement(&m_RefCount);
        if      (RetVal == 0)         {delete this;}
        else if (RetVal & 0x80000000) {__debugbreak();}
        return RetVal;
    }

    /*** QueryInterface ***/
    HRESULT STDMETHODCALLTYPE QueryInterface(__in REFIID riid, void **ppv) override
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
        else if (IsEqualIID(riid, CLSID_RadarAltimeter))
        {
            *ppv = static_cast<RadarAltimeter*>(this);
        }
    
        if (*ppv)
        {
            hr = S_OK;
            AddRef();
        }

    Error:

        return hr;
    }

    /**** ISimulation *******************************
    Runtime to update radar altitude
    *************************************************/
    STDMETHOD (Update)(double dDeltaT) override
    {
        HRESULT hr = E_FAIL;

        //Get surface elevation from Prepar3D
        float fElevation;
        if (SUCCEEDED(m_spBaseObject->GetSurfaceElevation(fElevation, NULL)))
        {
            DXYZ vPos, vOrient, vPosVel, vOrientVel;
            
            double dRadarAltitude;

            //Get altitue from the object
            if (SUCCEEDED(m_spBaseObject->GetPosition(vPos, vOrient, vPosVel, vOrientVel)))
            {
                dRadarAltitude = vPos.dY - fElevation;
            }
            else
            {
                dRadarAltitude = 0;
            }
            
            //"Set" the property using the registered property.
            //  NOTE:  This is done for demonstration purposes, as we could obviously just set the m_fRadarAltitude here.
            hr = m_spBaseObject->TriggerProperty(TEXT("RadarAltitude"), TEXT("feet"), dRadarAltitude, 0);

            hr = m_spBaseObject->TriggerProperty(TEXT("RadarAngle"), TEXT("Radians"), vOrient, 0);

            if (dRadarAltitude > 100) //feet
            {
                hr = m_spBaseObject->TriggerProperty(TEXT("RadarState"), TEXT("ALTITUDE HIGH"), 0);
            }
            else
            {
                hr = m_spBaseObject->TriggerProperty(TEXT("RadarState"), TEXT("ALTITUDE LOW"), 0);
            }
        }

        return hr;
    }

    // SaveLoadState - This could be used to save/load states from scenario files
    STDMETHOD (SaveLoadState)(__in __notnull PSaveLoadCallback pfnCallback, __in const BOOL bSave) override {return S_OK;}
    
    //Create the system
    static HRESULT New(__in IBaseObjectV400* pBaseObject, __out RadarAltimeter** ppNewAltimeter) {*ppNewAltimeter = new RadarAltimeter(pBaseObject); return S_OK;}

    //Callback registered to "RadarAltitude" property get.
    static HRESULT GetRadarAltitude(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex)
    {
        HRESULT hr = E_FAIL;

        CComPtr<ISimObjectV03> spSimObjectV03;
        if (SUCCEEDED(const_cast<ISimObject&>(Sim).QueryInterface(IID_ISimObjectV03, (void**)&spSimObjectV03)))
        {
            CComPtr<IBaseObjectV400> spBaseObject;
            if (SUCCEEDED(spSimObjectV03->QueryBaseObject(IID_IBaseObjectV400, (void**)&spBaseObject)))
            {
                CComPtr<RadarAltimeter> spRadarAltimeter;

                if (SUCCEEDED(spBaseObject->QueryService(SID_RadarAlitimeter, CLSID_RadarAltimeter, (void**)&spRadarAltimeter)))
                {
                    dProperty = spRadarAltimeter->GetIndication();
        
                    hr = S_OK;
                }
            }
        }

        return hr;
    }

    //Callback registered to "RadarAltitude" property set
    static HRESULT SetRadarAltitude(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
    {
        HRESULT hr = E_FAIL;

        CComPtr<ISimObjectV03> spSimObjectV03;
        if (SUCCEEDED(const_cast<ISimObject&>(Sim).QueryInterface(IID_ISimObjectV03, (void**)&spSimObjectV03)))
        {
            CComPtr<IBaseObjectV400> spBaseObject;
            if (SUCCEEDED(spSimObjectV03->QueryBaseObject(IID_IBaseObjectV400, (void**)&spBaseObject)))
            {
                CComPtr<RadarAltimeter> spRadarAltimeter;

                if (SUCCEEDED(spBaseObject->QueryService(SID_RadarAlitimeter, CLSID_RadarAltimeter, (void**)&spRadarAltimeter)))
                {
                    spRadarAltimeter->SetAltitude((float)dProperty);
        
                    hr = S_OK;
                }
            }
        }

        return hr;
    }

    //Callback registered to "RadarAngle" property get.
    static HRESULT GetRadarAngle(__in const ISimObject& Sim, __out DXYZ& vProperty, __in int iIndex)
    {
        HRESULT hr = E_FAIL;

        CComPtr<ISimObjectV03> spSimObjectV03;
        if (SUCCEEDED(const_cast<ISimObject&>(Sim).QueryInterface(IID_ISimObjectV03, (void**)&spSimObjectV03)))
        {
            CComPtr<IBaseObjectV400> spBaseObject;
            if (SUCCEEDED(spSimObjectV03->QueryBaseObject(IID_IBaseObjectV400, (void**)&spBaseObject)))
            {
                CComPtr<RadarAltimeter> spRadarAltimeter;

                if (SUCCEEDED(spBaseObject->QueryService(SID_RadarAlitimeter, CLSID_RadarAltimeter, (void**)&spRadarAltimeter)))
                {
                    vProperty = spRadarAltimeter->GetAngle();
        
                    hr = S_OK;
                }
            }
        }

        return hr;
    }

    //Callback registered to "RadarAngle" property set.
    static HRESULT SetRadarAngle(__in ISimObject& Sim, __in const DXYZ& vProperty, __in int iIndex)
    {
        HRESULT hr = E_FAIL;

        CComPtr<ISimObjectV03> spSimObjectV03;
        if (SUCCEEDED(const_cast<ISimObject&>(Sim).QueryInterface(IID_ISimObjectV03, (void**)&spSimObjectV03)))
        {
            CComPtr<IBaseObjectV400> spBaseObject;
            if (SUCCEEDED(spSimObjectV03->QueryBaseObject(IID_IBaseObjectV400, (void**)&spBaseObject)))
            {
                CComPtr<RadarAltimeter> spRadarAltimeter;

                if (SUCCEEDED(spBaseObject->QueryService(SID_RadarAlitimeter, CLSID_RadarAltimeter, (void**)&spRadarAltimeter)))
                {
                    spRadarAltimeter->SetAngle(vProperty);
        
                    hr = S_OK;
                }
            }
        }

        return hr;
    }

    //Callback registered to "RadarState" property get.
    static HRESULT GetRadarState(__in const ISimObject& Sim, __out LPTSTR  pszProperty, __in UINT uStringLength, __in int iIndex)
    {
        HRESULT hr = E_FAIL;

        CComPtr<ISimObjectV03> spSimObjectV03;
        if (pszProperty && SUCCEEDED(const_cast<ISimObject&>(Sim).QueryInterface(IID_ISimObjectV03, (void**)&spSimObjectV03)))
        {
            CComPtr<IBaseObjectV400> spBaseObject;
            if (SUCCEEDED(spSimObjectV03->QueryBaseObject(IID_IBaseObjectV400, (void**)&spBaseObject)))
            {
                CComPtr<RadarAltimeter> spRadarAltimeter;

                if (SUCCEEDED(spBaseObject->QueryService(SID_RadarAlitimeter, CLSID_RadarAltimeter, (void**)&spRadarAltimeter)))
                {
                    hr = StringCchCopy(pszProperty, uStringLength, spRadarAltimeter->GetState());
                }
            }
        }

        return hr;
    }

    //Callback registered to "RadarState" property set.
    static HRESULT SetRadarState(__in ISimObject& Sim, __in  LPCTSTR pszProperty, __in int iIndex)
    {
        HRESULT hr = E_FAIL;

        CComPtr<ISimObjectV03> spSimObjectV03;
        if (pszProperty && SUCCEEDED(const_cast<ISimObject&>(Sim).QueryInterface(IID_ISimObjectV03, (void**)&spSimObjectV03)))
        {
            CComPtr<IBaseObjectV400> spBaseObject;
            if (SUCCEEDED(spSimObjectV03->QueryBaseObject(IID_IBaseObjectV400, (void**)&spBaseObject)))
            {
                CComPtr<RadarAltimeter> spRadarAltimeter;

                if (SUCCEEDED(spBaseObject->QueryService(SID_RadarAlitimeter, CLSID_RadarAltimeter, (void**)&spRadarAltimeter)))
                {
                    spRadarAltimeter->SetState(pszProperty);
        
                    hr = S_OK;
                }
            }
        }

        return hr;
    }

private:
    float GetIndication() const {return m_fRadarAltitude;}
    void SetAltitude(float fAltitude) {m_fRadarAltitude = fAltitude;}

    const DXYZ GetAngle()               const  {return m_vAngle;}
    void SetAngle(const DXYZ& vAngle)          {m_vAngle = vAngle;}

    const TCHAR* GetState()                    const {return m_sState.c_str();}
    void SetState(const TCHAR* szState)              {m_sState = szState;}

    CComPtr<IBaseObjectV400> m_spBaseObject;
    float m_fRadarAltitude;
    DXYZ  m_vAngle;
    std::basic_string<TCHAR> m_sState;
};

/*******************************************************************************
OnSimObjectCreateCallbackHandler - called from Prepar3D when objects are created

- Register target service
*******************************************************************************/
static HRESULT STDMETHODCALLTYPE OnSimObjectCreateCallbackHandler(__in IUnknown& Obj )
{
    //This system is only implemented on the user object
    HRESULT hr = S_OK;

    CComPtr<IBaseObjectV400>spObject;
    
    if (SUCCEEDED(Obj.QueryInterface(IID_IBaseObjectV400, (void**)&spObject)))
    {
        if (spObject->IsUser())
        {
            hr = E_FAIL;

            CComPtr<RadarAltimeter> spRadarAltimeter;
            if (SUCCEEDED(RadarAltimeter::New(spObject, &spRadarAltimeter)))
            {
                /* NOTE: THE BELOW PROPERTIES MUST MATCH THOSE LISTED IN PROPERTIES.XML */

                if ( SUCCEEDED(spObject->RegisterService(SID_RadarAlitimeter, spRadarAltimeter))                                        //Register service ID for QueryService queries
                  && SUCCEEDED(spObject->RegisterSimulation(spRadarAltimeter, 30.0))                                                    //Register runtime callback 
                  && SUCCEEDED(spObject->RegisterProperty(TEXT("RadarAltitude"), TEXT("Feet"),      RadarAltimeter::GetRadarAltitude))                   //Register "get" property
                  && SUCCEEDED(spObject->RegisterProperty(TEXT("RadarAltitude"), TEXT("Feet"),      RadarAltimeter::SetRadarAltitude, EVENTTYPE_NORMAL)) //Register "set" property
                  && SUCCEEDED(spObject->RegisterProperty(TEXT("RadarAngle"),    TEXT("Radians"),   RadarAltimeter::GetRadarAngle))                      //Register "set" property
                  && SUCCEEDED(spObject->RegisterProperty(TEXT("RadarAngle"),    TEXT("Radians"),   RadarAltimeter::SetRadarAngle))                      //Register "set" property
                  && SUCCEEDED(spObject->RegisterProperty(TEXT("RadarState"),                 RadarAltimeter::GetRadarState))                      //Register "set" property
                  && SUCCEEDED(spObject->RegisterProperty(TEXT("RadarState"),                 RadarAltimeter::SetRadarState)))                     //Register "set" property
                {
                    hr = S_OK;   //It's ok if laser target isn't created.
                }
            }
        }
    }

    return hr;
}

/*******************************************************************************
OnSimObjectRemoveCallbackHandler - called from P3d

- Unregister target service
*******************************************************************************/
static HRESULT STDMETHODCALLTYPE OnSimObjectRemoveCallbackHandler(__in IUnknown& Obj )
{
    HRESULT hr = S_OK;

    CComPtr<IBaseObjectV400>spObject;
    
    if (SUCCEEDED(Obj.QueryInterface(IID_IBaseObjectV400, (void**)&spObject)))
    {
        if (spObject->IsUser())
        {
            
            CComPtr<RadarAltimeter> spRadarAltimeter;

            if (SUCCEEDED(spObject->QueryService(SID_RadarAlitimeter, CLSID_RadarAltimeter, (void**)&spRadarAltimeter)))
            {
                hr = spObject->UnregisterService(SID_RadarAlitimeter);

                spRadarAltimeter.Release();
            }
        }
    }

    return hr;
}


/***********************************************************************************************
***********************************************************************************************/
STDMETHODIMP_(void) DLLStart(__in __notnull IPdk* pPdk)
{      
    CComPtr<ISimObjectManagerV400> spSimObjectMgr;  //Smart pointer reference to SimObject Manager

    HRESULT hr = E_FAIL;
    
    if (!pPdk)
    {
        goto Error;
    }

    //Get the SimObject manager to register a simobject class and its create factory callback
    if (FAILED(pPdk->QueryService(SID_SimObjectManager, IID_ISimObjectManagerV400, (void**)&spSimObjectMgr)))
    {
        goto Error;
    }

    //Register callback functions to be notified when simobjects are created and destroyed.
    spSimObjectMgr->RegisterOnObjectCreateCallback(OnSimObjectCreateCallbackHandler);
    spSimObjectMgr->RegisterOnObjectRemoveCallback(OnSimObjectRemoveCallbackHandler);

Error:

    return;
}

/***********************************************************************************************
***********************************************************************************************/

STDMETHODIMP_(void) DLLStop()
{
    //Clean up statics on module unload as necessary
}

/***********************************************************************************************
***********************************************************************************************/
