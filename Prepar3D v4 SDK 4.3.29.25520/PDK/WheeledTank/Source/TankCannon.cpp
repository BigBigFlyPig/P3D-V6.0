//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//------------------------------------------------------------------------------
//
//  WheeledTank ISimObject (SimObject API) Example
//  TankCannon.cpp
//  
//  Description:
//              The tank cannon class is an implementation of the IGunSystem.  
//              This shows the capabilities of developing a gun system if the 
//              internal GunSystem does not meet desired functionality.  If 
//              unique capabilities are not required, creating guns using the 
//              GunSystem.xml file should suffice.
//------------------------------------------------------------------------------

#include "TankCannon.h"

using namespace P3D;

/***********************************************************************************************
    TankCannon::QueryInterface
***********************************************************************************************/
STDMETHODIMP TankCannon::QueryInterface(__in REFIID riid,  //Interface ID 
                                        __out void **ppv)  //Interface Ptr
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
    else if (IsEqualIID(riid , IID_IGunV400))
    {
        *ppv = static_cast<IGunV400*>(this);
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
    Tank cannon updates and simulation calls
***********************************************************************************************/
void TankCannon::Simulate(__in double deltaT)   
{
    // called once per step on all guns
    UpdateRounds(deltaT);

    // update the cannon world position
    UpdateWorldPosition();
}

/***********************************************************************************************
    Tank cannon firing
***********************************************************************************************/
HRESULT TankCannon::Fire(__in double deltaT)
{
    // called while trigger is engaged
    HRESULT hr = S_OK;

    if(m_ammo < 1)
    {
        return hr;
    }

    if(m_timeFiring == 0.0) // first round
    {
        CreateRound();
    }
    else if(m_timeFiring > m_rateSec)
    {
        CreateRound();
    }

    m_timeFiring += deltaT;   

    return hr;
}

/***********************************************************************************************
    Reset the time firing
***********************************************************************************************/
void TankCannon::Purge()
{
    // called when trigger is released
    m_timeFiring = 0.0;
}

/***********************************************************************************************
    Called when user inputs rotation commands
***********************************************************************************************/
void TankCannon::Rotate(__in double xAxisOffset, __in double yAxisOffset, __in double deltaT)
{
}

/***********************************************************************************************
    Called when automatic gun targeting is enabled
***********************************************************************************************/
void TankCannon::ProcessTargeting(__in const P3D::DXYZ &targetLla, __in const P3D::DXYZ &targetBodyVelocity, __in const P3D::DXYZ &targetBodyAcceleration, __in const P3D::DXYZ &targetOrientation, __in double deltaT)
{
}

/***********************************************************************************************
    Called when there is no user input and no automatic gun targeting
***********************************************************************************************/
void TankCannon::Stop()
{
}

/***********************************************************************************************
    Creates the round/bullet that is to be fired
***********************************************************************************************/
void TankCannon::CreateRound()
{
    Round round;
    // set initial position
    round.position = m_lla;
    // set initial velocity in world coordinates
    // velocity world vector
    P3D::DXYZ velocityVec;
    velocityVec.dX = sin(m_pbh.dY); // pitch
    velocityVec.dY = -sin(m_pbh.dX); // heading
    velocityVec.dZ = cos(m_pbh.dY); // bank
    
    // normalize velocity world vector
    double velocityVecMag = sqrt(velocityVec.dX*velocityVec.dX + velocityVec.dY*velocityVec.dY + velocityVec.dZ*velocityVec.dZ);
    if(velocityVecMag > 0.0)
    {
        velocityVec.dX /= velocityVecMag;
        velocityVec.dY /= velocityVecMag;
        velocityVec.dZ /= velocityVecMag;
    }
    // apply muzzle velocity to velocity world vector
    round.velocity.dX = velocityVec.dX * m_muzzleVel;
    round.velocity.dY = velocityVec.dY * m_muzzleVel;
    round.velocity.dZ = velocityVec.dZ * m_muzzleVel;
    // set initial orientation
    round.orientation.dX = m_pbh.dX;
    round.orientation.dY = m_pbh.dY;
    round.orientation.dZ = m_pbh.dZ;
    // set initial lifetime
    round.lifetime = 0.0;
    // create tracer
    round.pEffect = NULL;
    m_pGunSystem->CreateTracerEffect(TEXT("fx_tracer_m"), &m_lla, &round.orientation, &round.pEffect);
    // insert round
    m_roundList.push_back(round);
    // update ammo
    m_ammo = max(0, m_ammo - 1);
}

/***********************************************************************************************
    Manages the firing of the rounds/bullets
***********************************************************************************************/
void TankCannon::UpdateRounds(double deltaT)
{
    std::vector<Round>::iterator iter = m_roundList.begin();
    while(iter != m_roundList.end())
    {
        Round* pRound = &(*iter);

        // update lifetime
        pRound->lifetime += (float)deltaT;

        // check lifetime limit
        if(pRound->lifetime > m_maxLifetime)
        {
            m_pGunSystem->DestroyTracerEffect(pRound->pEffect);
            iter = m_roundList.erase(iter);
            continue;
        }

        // update round position
        pRound->position.dX += pRound->velocity.dX * cos(pRound->position.dZ) * deltaT / m_pWheeledTank->GetWorldConstants().m_dEquatorialRadius;
        pRound->position.dY += pRound->velocity.dY * deltaT;
        pRound->position.dZ += pRound->velocity.dZ * deltaT / m_pWheeledTank->GetWorldConstants().m_dPolarRadius;

        // update round velocity
        pRound->velocity.dY = pRound->velocity.dY - m_pWheeledTank->GetWorldConstants().m_fGravitySeaLevel * deltaT;

        // update tracer
        m_pGunSystem->MoveTracerEffect(&pRound->position, &pRound->orientation, pRound->pEffect);

        P3D::DXYZ offset;
        memset(&offset, 0, sizeof(offset));

        IUnknown* pUnknown;
        P3D::COLLISIONTYPE eCollisionType = P3D::COLLISIONTYPE_NONE;
        m_pGunSystem->CheckBulletCollision(&pRound->position, &offset, eCollisionType, &pUnknown);

        if(eCollisionType != P3D::COLLISIONTYPE_NONE)
        {
            void* pEffect = NULL;
            m_pGunSystem->CreateTracerEffect(TEXT("fx_dirtspray_s"), &pRound->position, &offset, &pEffect);
            m_pGunSystem->DestroyTracerEffect(pRound->pEffect);
            iter = m_roundList.erase(iter);
            continue;
        }

        ++iter;
    }
}

/***********************************************************************************************
    Correlate the position of the gun to the position of the object
***********************************************************************************************/
void TankCannon::UpdateWorldPosition()
{
    // update cannon position
    m_lla.dX = m_pWheeledTank->GetPosition().dX + FEET_TO_LONGITUDE(m_offset.dX, m_pWheeledTank->GetPosition().dZ);
    m_lla.dY = m_pWheeledTank->GetPosition().dY + m_offset.dY;
    m_lla.dZ = m_pWheeledTank->GetPosition().dZ + FEET_TO_LATITUDE(m_offset.dZ);

    // update cannon orientation
    m_pbh = m_pWheeledTank->GetOrientation();
}