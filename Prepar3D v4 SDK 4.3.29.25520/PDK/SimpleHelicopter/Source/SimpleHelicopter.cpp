/************************************************************************************************************** 
    Copyright (C) Lockheed Martin Corporation.  All rights reserved.
    
    Description: A sample demonstrating how to implement a simple helicopter ISimObject.
***************************************************************************************************************/

#include "SimpleHelicopter.h"

#include <atlcomcli.h>
#include <InitGuid.h>

#include "Constants.h"

using namespace P3D;

P3D::WorldConstants Constants::ms_WorldConstants = { 0 };

/**********************************************************************
**********************************************************************/
SimpleHelicopter::SimpleHelicopter(IBaseObjectV400& BaseObject) :
    m_RefCount(1),
    m_bSupportsLabel(TRUE),
    m_BaseObject(BaseObject), 
    m_Simulation(BaseObject)
{
}
    
/**********************************************************************
**********************************************************************/
SimpleHelicopter::~SimpleHelicopter()
{
}

/**********************************************************************
**********************************************************************/
STDMETHODIMP SimpleHelicopter::QueryInterface(__in REFIID riid, void **ppv)
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
    else if (IsEqualIID(riid, IID_ISimObjectV03))
    {
        *ppv = static_cast<ISimObjectV03*>(this);
    }
    else if (IsEqualIID(riid , IID_IAircraftService))
    {
        *ppv = static_cast<IAircraftService*>(this);
    }
    else if (IsEqualIID(riid , IID_IAircraftServiceV01))
    {
        *ppv = static_cast<IAircraftServiceV01*>(this);
    }
    else if (IsEqualIID(riid , IID_IAirplaneServiceV01))
    {
        *ppv = static_cast<IRotorcraftServiceV01*>(this);
    }
    else if (IsEqualIID(riid , IID_IAvatarAttachServiceV01))
    {
        *ppv = static_cast<IAvatarAttachServiceV01*>(this);
    }        
    else if (IsEqualIID(riid , IID_IServiceProvider))
    {
        *ppv = static_cast<IServiceProvider*>(this);
    }
    else if (IsEqualIID(riid , IID_IUnknown))
    {
        *ppv = static_cast<SimpleHelicopter*>(this);
    }        

    if (*ppv)
    {
        hr = S_OK;
        AddRef();
    }

Error:

    return hr;
}

/**********************************************************************
**********************************************************************/
STDMETHODIMP SimpleHelicopter::QueryBaseObject( __in REFIID riid, void** ppv)
{
    HRESULT hr = E_NOINTERFACE;

    if (!ppv)
    {
        goto Error;
    }

    *ppv = NULL;
    
    hr = m_BaseObject.QueryInterface(riid, (void**)ppv);

Error:

    return hr;
}

/**********************************************************************
**********************************************************************/
STDMETHODIMP SimpleHelicopter::QueryService(__in REFGUID guidService, __in REFIID riid, void **ppv)
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
    else if (IsEqualIID(guidService, SID_RotorcraftService))
    {
        hr = QueryInterface(riid, ppv);
    }
    else if (IsEqualIID(guidService, SID_AircraftService))
    {
        hr = QueryInterface(riid, ppv);
    }
    else if (IsEqualIID(guidService, SID_AvatarAttachService))
    {
        hr = QueryInterface(riid, ppv);
    }
    else if (IsEqualIID(guidService, SID_Simulation))
    {
        hr = m_Simulation.QueryInterface(riid, ppv);
    }

Error:

    return hr;
}

/**********************************************************************
Factory function
**********************************************************************/
/*static*/
HRESULT SimpleHelicopter::New(__notnull IBaseObjectV400* pBaseObject, __notnull ISimObject** ppThisSim) 
{
    HRESULT hr = E_FAIL;

    if (!ppThisSim)
    {
        goto Error;
    }

    *ppThisSim = new SimpleHelicopter(*pBaseObject);

    if (!*ppThisSim)
    {
        goto Error;
    }

    hr = pBaseObject->RegisterSimulation(&static_cast<SimpleHelicopter*>(*ppThisSim)->m_Simulation, 60.0f /*Hz*/);

Error:

    return hr;
}

/**********************************************************************
Factory function wrapper
**********************************************************************/
/*static*/
HRESULT New(__notnull IBaseObjectV400* pBaseObject, __notnull ISimObject** ppThisSim)
{
    CComPtr<IBaseObjectV400> spBaseObject;
    if (SUCCEEDED(pBaseObject->QueryInterface(IID_IBaseObjectV400, (void**)&spBaseObject)))
    {
        return SimpleHelicopter::New(spBaseObject, ppThisSim);
    }
    else
    {
        return E_FAIL;
    }
}

/**********************************************************************
Property Registration
**********************************************************************/
HRESULT RegisterProperties(__notnull ISimObjectManagerV400* pSimObjectMgr)
{
    HRESULT hr = E_FAIL;

    hr = Simulation::RegisterProperties(CLSID_SimpleHelicopter, pSimObjectMgr);

    P3D::WorldConstants World;
    hr = pSimObjectMgr->GetWorldConstants(World);
    Constants::Initialize(World);

    return S_OK;
}
