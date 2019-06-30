//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//------------------------------------------------------------------------------
//
//  WheeledTank ISimObject (SimObject API) Example
//  ContactReaction.cpp
//  
//  Description:
//              Contact reactions control the springs and supension of the wheels.
//              This is what causes the tank to bounce and react to the ground.
//------------------------------------------------------------------------------

#include <math.h>

#include "WheeledTank.h"
#include "ContactReaction.h"

#include <strsafe.h>

/***********************************************************************************************
    ContactReaction::QueryInterface
***********************************************************************************************/
STDMETHODIMP ContactReaction::QueryInterface(__in REFIID riid,  //Interface ID
                                             __out void **ppv)  //Interface Ptr
{
    HRESULT hr = E_NOINTERFACE;

    if (!ppv)
    {
        goto Error;
    }
    
    *ppv = NULL;
    
    if (IsEqualIID(riid , IID_ISimulation))
    {
        *ppv = static_cast<ISimulation*>(this);
    }        
    if (IsEqualIID(riid , IID_ISimulationV01))
    {
        *ppv = static_cast<ISimulationV01*>(this);
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

/***********************************************************************************************
    ContactReaction Initialization
***********************************************************************************************/
HRESULT ContactReaction::Init(const WheeledTank* pWheeledTank) 
{
    static const FXYZ vfOffset[8] = 
    {
        { 4.24f, -0.007f,  6.10f}, // right front
        { 4.24f, -0.007f,  3.15f}, // right front (inside)
        { 4.24f, -0.007f, -3.11f}, // right back (inside)
        { 4.24f, -0.007f, -6.35f}, // right back
        {-4.24f, -0.007f,  6.10f}, // left front
        {-4.24f, -0.007f,  3.15f}, // left front (inside)
        {-4.24f, -0.007f, -3.11f}, // left back (inside)
        {-4.24f, -0.007f, -6.35f}, // left back
    }; // feet

    m_pWheeledTank = pWheeledTank; 

    for (UINT iPoint = 0; iPoint < GetNumContactPoints(); iPoint++)
    {
        m_aContactPoints[iPoint].Init(iPoint, vfOffset[iPoint]);
    }

    return (m_pWheeledTank) ? S_OK : E_FAIL;

}

/***********************************************************************************************
    Updates the contact forces and the reaction
***********************************************************************************************/
STDMETHODIMP ContactReaction::Update(double dDeltaT)
{
    HRESULT hr = E_FAIL;

    memset(&m_vdTotalForce,  0, sizeof(m_vdTotalForce));
    memset(&m_vdTotalMoment, 0, sizeof(m_vdTotalMoment));
    
    m_bOnGround = FALSE;

    for (int iPoint = 0; iPoint < (int)GetNumContactPoints(); iPoint++)
    {
        DXYZ vdForce  = {0};
        DXYZ vdMoment = {0};

        if (SUCCEEDED(m_aContactPoints[iPoint].CalculateForce(m_pWheeledTank, vdForce, vdMoment)))
        {
            m_vdTotalForce.dX += vdForce.dX;
            m_vdTotalForce.dY += vdForce.dY;
            m_vdTotalForce.dZ += vdForce.dZ;

            m_vdTotalMoment.dX += vdMoment.dX;
            m_vdTotalMoment.dY += vdMoment.dY;
            m_vdTotalMoment.dZ += vdMoment.dZ;
        }

        if (!m_bOnGround)
        {
            m_bOnGround = m_aContactPoints[iPoint].IsOnGround();
        }
    }

    if (m_bOnGround)
    {
        FXYZ vOffset = {0};
        m_pWheeledTank->GetBaseObject()->TriggerContactSound(TEXT("GROUND_ROLL"), &vOffset, 0); //Name matches sound.cfg
    }

    return hr;
}

/***********************************************************************************************
    Calculates the contact forces and the reaction
***********************************************************************************************/
HRESULT ContactPoint::CalculateForce(__in __notnull const WheeledTank* pWheeledTank, __out DXYZ& vdForce, __out DXYZ& vdMoment)
{

    vdForce. dX  = vdForce.dY = vdForce. dZ = 0;
    vdMoment.dX = vdMoment.dY = vdMoment.dZ = 0;

    SurfaceInfoV400 SurfaceInfo = {0};
    pWheeledTank->GetBaseObject()->GetSurfaceInformation(SurfaceInfo, &m_vOffset);

    double dPointElevation = pWheeledTank->GetPosition().dY + m_vOffset.fY
        - m_vOffset.fZ * sin(pWheeledTank->GetOrientation().dX)
        + m_vOffset.fX * sin(pWheeledTank->GetOrientation().dZ);

    m_dCompression = SurfaceInfo.m_fElevation - dPointElevation;

    if (m_dCompression > 0)
    {
        static const float SPRING_K = 20000.0f;
        static const float DAMP_K   = -1600.0f;

        vdForce.dY = m_dCompression * SPRING_K;

        double dGearVelY = pWheeledTank->GetVelocity().dY 
            + pWheeledTank->GetRotVelocity().dZ * m_vOffset.fX
            - pWheeledTank->GetRotVelocity().dX * m_vOffset.fZ;

        double dDampForce = dGearVelY * DAMP_K;

        // bounce when main gun fires
        IGunSystemV400* pGunSystem = NULL;
        pWheeledTank->GetBaseObject()->QueryService(SID_GunSystem, IID_IGunSystemV400, (void**)&pGunSystem);
        if(pGunSystem)
        {
            if(pGunSystem->GetIsFiring())
            {
                int iStations = pGunSystem->GetNumberOfStations();
                for(int i = 0; i < iStations; i++)
                {
                    if(pGunSystem->GetIsGunSelectedAtStation(i))
                    {
                        const IGunV400* selectedGun = pGunSystem->GetGun(i);
                        if(wcscmp(selectedGun->GetName(), TEXT("M68A2")) == 0)
                        {
                            vdForce.dY += -10000.0;
                        }
                    }
                }
            }
            pGunSystem->Release();
        }

        vdForce.dY += dDampForce;
    }
    else
    {
        m_dCompression = 0;
    }

    vdMoment.dX = -vdForce.dY * m_vOffset.fZ;
    vdMoment.dZ =  vdForce.dY * m_vOffset.fX;

    return S_OK;
}

/***********************************************************************************************
***********************************************************************************************/
/*static*/
STDMETHODIMP ContactReaction::GetStrutCompressionLength(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex)
{   
    dProperty = static_cast<const WheeledTank&>(Sim).GetContactReaction().GetStrutCompression(iIndex);
    dProperty *= 100;

    return S_OK;
}
