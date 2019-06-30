//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//------------------------------------------------------------------------------
//
//  WheeledTank ISimObject (SimObject API) Example
//  ContactReaction.h
//  
//  Description:
//              Contact reactions control the springs and supension of the wheels.
//              This is what causes the tank to bounce and react to the ground.
//------------------------------------------------------------------------------

#pragma once

#include "IUnknownHelper.h"

using namespace P3D;

class WheeledTank;


/***********************************************************************************************
***********************************************************************************************/
class ContactPoint
{
public:
    ContactPoint() : m_uIndex(0), m_dCompression(0) {memset(&m_vOffset, 0, sizeof(m_vOffset));}

    HRESULT Init(__in UINT uIndex, __in const FXYZ vOffset) {m_uIndex = uIndex; m_vOffset = vOffset; return S_OK;}

    HRESULT CalculateForce(__in __notnull const WheeledTank* pWheeledTank, DXYZ& vdForce, DXYZ& vdMoment);

    double GetStrutCompression() const {return m_dCompression;}

    BOOL IsOnGround() const {return m_dCompression > 0;}

private:

    UINT m_uIndex;
    FXYZ m_vOffset;
    double m_dCompression;

};


/***********************************************************************************************
    ContactReaction Simulation
***********************************************************************************************/
class ContactReaction : public ISimulationV01
{
    static const UINT NUM_CONTACT_POINTS = 8;

    DECLARE_IUNKNOWN()

public:
    ContactReaction(__in __notnull IBaseObjectV400* pBaseObject)  
        :   m_RefCount(1),
            m_bOnGround(FALSE),
            m_spBaseObject(pBaseObject)
    {
        memset(&m_vdTotalForce,  0, sizeof(m_vdTotalForce));
        memset(&m_vdTotalMoment, 0, sizeof(m_vdTotalMoment));
    }
    ~ContactReaction() {}

    static STDMETHODIMP GetStrutCompressionLength(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex);

    double GetStrutCompression(__in int iIndex) const 
    {
        return (iIndex >= 0 && iIndex < (int)GetNumContactPoints()) ? m_aContactPoints[iIndex].GetStrutCompression() : 0.0;
    }

    HRESULT Init(const WheeledTank* pWheeledTank);

    HRESULT CalculateForceMoment(__out DXYZ& vdTotalForce, __out DXYZ& vdTotalMoment, __in double dDeltaT)
    {
        HRESULT hr = Update(dDeltaT);

        vdTotalForce  = m_vdTotalForce; 
        vdTotalMoment = m_vdTotalMoment; 

        return hr;
    }

    BOOL IsOnGround() const {return m_bOnGround;}


private:
    STDMETHOD (Update)(double dDeltaT) override;
    STDMETHOD (SaveLoadState)(__in __notnull PSaveLoadCallback pfnCallback, __in const BOOL bSave) override {return S_OK;}

    UINT GetNumContactPoints() const {return ARRAYSIZE(m_aContactPoints);}

    CComPtr<IBaseObjectV400> m_spBaseObject;
    const WheeledTank* m_pWheeledTank;

    ContactPoint m_aContactPoints[NUM_CONTACT_POINTS];

    BOOL m_bOnGround;
    DXYZ m_vdTotalForce;
    DXYZ m_vdTotalMoment;
};