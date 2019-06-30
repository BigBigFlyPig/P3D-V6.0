/************************************************************************************************************** 
    Copyright (C) Lockheed Martin Corporation.  All rights reserved.
    
    Description: A sample demonstrating how to implement a simple helicopter ISimObject.
***************************************************************************************************************/

#pragma once

#include "ISimObject.h"
#include "Helpers\IUnknownHelper.h"
#include "Matrix.h"

#define DECLARE_PROPERTY_AXIS(PROPNAME) static STDMETHODIMP PROPNAME(__in P3D::ISimObject& Sim, __in double dProperty, __in int iIndex);
#define DECLARE_PROPERTY_SET(PROPNAME) static STDMETHODIMP PROPNAME(__in P3D::ISimObject& Sim, __in double dProperty, __in int iIndex);
#define DECLARE_PROPERTY_GET(PROPNAME) static STDMETHODIMP PROPNAME(__in const P3D::ISimObject& Sim, __out double& dProperty, __in int iIndex);

class __declspec(uuid("{EFB054B1-61FB-4F2A-818C-F78DF9676F43}")) Simulation : public P3D::ISimulationV01
{
public:

    Simulation(P3D::IBaseObjectV400& BaseObject);
    ~Simulation();

    HRESULT Init();

    void   InitPositionFromBase();

    void   SetCollective(float fCollective)     { m_fCollective = (float)max(min(fCollective, 1.0), 0.0); }
    void   IncCollective()                      { SetCollective(m_fCollective += 0.05f); }
    void   DecCollective()                      { SetCollective(m_fCollective -= 0.05f); }
    float  GetCollective() const                { return m_fCollective; }

    void   SetCyclicPitch(float fCyclicPitch)   { m_fCyclicPitch = (float)max(min(fCyclicPitch, 1.0), -1.0); }
    void   SetCyclicBank(float fCyclicBank)     { m_fCyclicBank = (float)max(min(fCyclicBank, 1.0), -1.0); }
    void   SetPedals(float fPedals)             { m_fPedals = (float)max(min(fPedals, 1.0), -1.0); }

    float  GetPctCyclicPitch()          const { return m_fCyclicPitch; }
    float  GetPctCyclicBank()           const { return m_fCyclicBank; }
    float  GetPctPedals()               const { return m_fPedals; }

    float  GetMainRotorRotationPercent()        const { return m_fMainRotorRotationPercent; }
    float  GetTailRotorRotationPercent()        const { return m_fTailRotorRotationPercent; }

    double GetTotalVelocity()           const { return m_dTotalVelocity; }

    BOOL   IsOnGround()                 const { return m_bOnGround; }
    BOOL   IsOverPlatform()             const { return m_SurfaceInfo.m_bOnPlatform; }
    float  GetSurfaceElevation()        const { return m_SurfaceInfo.m_fElevation; }

    void   InitAtAirspeed(float fAirspeed)        { m_vBodyVelocity.dZ = fAirspeed; }
    float  GetAirspeed()                    const { return (float)m_vBodyVelocity.dZ; }

    /*********** Static property handlers ***********/
    static HRESULT RegisterProperties(__in REFGUID guidCategory, __in __notnull P3D::ISimObjectManagerV400* pSimObjectMgr);
    DECLARE_PROPERTY_AXIS(SetCollective)
    DECLARE_PROPERTY_SET(IncCollective)
    DECLARE_PROPERTY_SET(DecCollective)

    DECLARE_PROPERTY_AXIS(SetCyclicPitch)
    DECLARE_PROPERTY_AXIS(SetCyclicBank)
    DECLARE_PROPERTY_AXIS(SetPedals)

    DECLARE_PROPERTY_GET(GetMainRotorRotationPercent)
    DECLARE_PROPERTY_GET(GetTailRotorRotationPercent)
    /*********** Static property handlers ***********/

private:

    DECLARE_IUNKNOWN_WITH_INLINE_REFCOUNT_IMPL();

    P3D::IBaseObjectV400& m_BaseObject;

    // ISimulationV01 - Begin
    STDMETHOD(SaveLoadState)(__in __notnull P3D::PSaveLoadCallback pfnCallback, __in const BOOL bSave) { return S_OK; }
    STDMETHOD(Update)(double dDeltaT);
    // ISimulationV01 - End

    void UpdateRotors(double dDeltaT);
    void UpdateAerodynamics(double dDeltaT);
    void UpdateGroundReaction(double dDeltaT);

private:

    BOOL IsNormalMode() const;

    P3D::SurfaceInfoV400 m_SurfaceInfo;
    P3D::WeatherInfoV400 m_WeatherInfo;

    // World-relative state
    P3D::DXYZ m_vdPos;
    P3D::DXYZ m_vdOrient;
    P3D::DXYZ m_vdVel;
    P3D::DXYZ m_vdRotVel;

    P3D::DXYZ m_vBodyRotationVelocity;
    P3D::DXYZ m_vBodyVelocity;

    BOOL m_bOnGround;

    Matrix m_Matrix;

    // Mass Properties
    float m_fMass;
    P3D::FXYZ m_vfMOI;

    // Flight Controls
    float m_fCyclicPitch;
    float m_fCyclicBank;
    float m_fPedals;
    float m_fCollective;

    // Rotors
    float m_fMainRotorRotationPercent;     // [0,1]
    float m_fTailRotorRotationPercent;     // [0,1]
    P3D::DXYZ m_vRotorForce;
    P3D::DXYZ m_vRotorMoment;

    // Aerodynamics
    P3D::DXYZ m_vAeroForce;
    P3D::DXYZ m_vAeroMoment;

    // Aerodynamics
    double m_dTotalVelocity;

    // Contact Reaction
    P3D::DXYZ m_vGroundReactionForce;
    P3D::DXYZ m_vGroundReactionMoment;
};
