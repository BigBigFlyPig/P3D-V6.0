//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//------------------------------------------------------------------------------
//
//  WheeledTank ISimObject (SimObject API) Example
//  TankCannon.h
//  
//  Description:
//              The tank cannon class is an implementation of the IGunSystem.  
//              This shows the capabilities of developing a gun system if the 
//              internal GunSystem does not meet desired functionality.  If 
//              unique capabilities are not required, creating guns using the 
//              GunSystem.xml file should suffice.
//------------------------------------------------------------------------------

#pragma once

#include "WheeledTank.h"

#include <vector>

using namespace P3D;

struct Round
{
    P3D::DXYZ position;
    P3D::DXYZ orientation;
    P3D::DXYZ velocity;

    float lifetime;

    void* pEffect;
};


/***********************************************************************************************
    IGun TankCannon
***********************************************************************************************/
class TankCannon : public IGunV400
{
    DECLARE_IUNKNOWN()

public:
    TankCannon(WheeledTank* pWheeledTank, IGunSystemV400* pGunSystem) :
        m_RefCount(1),
        m_pWheeledTank(pWheeledTank),
        m_pGunSystem(pGunSystem),
        m_ammo(12500),
        m_capacity(12500),
        m_rate(1000),
        m_muzzleVel(5000.0),
        m_timeFiring(0.0),
        m_maxLifetime(3.0)
        {
            m_name = TEXT("M68A2 IGun Example");
            m_type = TEXT("Rifled Cannon");

            m_rateSec = m_rate / 60.0;

            memset(&m_lla, 0, sizeof(m_lla));
            memset(&m_pbh, 0, sizeof(m_pbh));

            // initialize offset
            m_offset.dX = 0.0; // right
            m_offset.dY = 15.0;   // up
            m_offset.dZ = 0.0;
        }

    // Start IGun interface function declarations
    STDMETHOD_(void,        Simulate(__in double deltaT));
    STDMETHOD_(HRESULT,     Fire(__in double deltaT));
    STDMETHOD_(void,        Purge());
    STDMETHOD_(void,        Stop());

    STDMETHOD_(void,        SetRoundsRemaining(__in UINT ammoCount))  {}
    STDMETHOD_(UINT,        GetRoundsRemaining())  const { return m_ammo; }
    STDMETHOD_(void,        ResetRounds())               { m_ammo = m_capacity; }

    STDMETHOD_(const WCHAR*, GetName())             const { return m_name; }
    STDMETHOD_(const WCHAR*, GetGunType())          const { return m_type; }

    // user input rotation values
    STDMETHOD_(void, Rotate(__in double xAxisOffset, __in double yAxisOffset, __in double deltaT));

    // automatic gun target information
    STDMETHOD_(void, ProcessTargeting(__in const P3D::DXYZ &targetLla, __in const P3D::DXYZ &targetBodyVelocity, __in const P3D::DXYZ &targetBodyAcceleration, __in const P3D::DXYZ &targetOrientation, __in double deltaT));
    // End IGun interface function declarations

    void CreateRound();

private:

    WheeledTank* m_pWheeledTank;
    IGunSystemV400* m_pGunSystem; 

    void UpdateWorldPosition();
    void UpdateRounds(double deltaT);

    unsigned int m_ammo;
    unsigned int m_capacity;

    WCHAR* m_name;
    WCHAR* m_type;

    P3D::DXYZ m_offset;     // offset from parent
    P3D::DXYZ m_pbh;        // world pbh
    P3D::DXYZ m_lla;        // world lla

    unsigned int m_rate;    // rounds per minute
    double m_rateSec;       // rounds per second

    double m_muzzleVel;     // the cannon's muzzle velocity

    double m_timeFiring;    // the duration the trigger is down

    double m_maxLifetime;

    std::vector<Round> m_roundList;
};
