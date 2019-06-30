/************************************************************************************************************** 
    Copyright (C) Lockheed Martin Corporation.  All rights reserved.
    
    Description: A sample demonstrating how to set postion on an ISimObject
***************************************************************************************************************/

#include "Tracker.h"

#include <sstream>

#include "ILocationDataProvider.h"
#include "Util.h"

const P3D::DXYZ DXYZ_ZERO = { 0 };

// Radians/Feet
void LatLonAltToVector(const DXYZ& vLonAltLatStart, const DXYZ& vLonAltLatEnd, DXYZ& vOffsetFeet)
{
    const double dDeltaLonRadians = vLonAltLatEnd.dX - vLonAltLatStart.dX;
    const double dDeltaAltFeet = vLonAltLatEnd.dY - vLonAltLatStart.dY;
    const double dDeltaLatRadians = vLonAltLatEnd.dZ - vLonAltLatStart.dZ;

    vOffsetFeet.dX = dDeltaLonRadians * Tracker::ms_WorldConstants.m_dEquatorialRadius * cos(vLonAltLatStart.dZ);
    vOffsetFeet.dY = dDeltaAltFeet;
    vOffsetFeet.dZ = dDeltaLatRadians * Tracker::ms_WorldConstants.m_dPolarRadius * ONE_OVER_HALF_PI;
}

// Radians/Feet
void VectorToLatLonAlt(const DXYZ& vOffsetFeet, const DXYZ& vLonAltLatCurrent, DXYZ& vLonAltLatResult)
{
    vLonAltLatResult.dX = vLonAltLatCurrent.dX + vOffsetFeet.dX / Tracker::ms_WorldConstants.m_dEquatorialRadius / cos(vLonAltLatCurrent.dZ);
    vLonAltLatResult.dY = vLonAltLatCurrent.dY + vOffsetFeet.dY;
    vLonAltLatResult.dZ = vLonAltLatCurrent.dZ + vOffsetFeet.dZ / (TWO_PI * Tracker::ms_WorldConstants.m_dPolarRadius * ONE_OVER_FOUR);
}

// Radians/Feet
void VectorToPitchBankHeading(const DXYZ& vOffsetFeet, DXYZ& vPitchHeadingBank)
{
    double dH = sqrt(vOffsetFeet.dZ * vOffsetFeet.dZ + vOffsetFeet.dX * vOffsetFeet.dX);
    vPitchHeadingBank.dX = -atan2(vOffsetFeet.dY, dH);
    vPitchHeadingBank.dY = atan2(vOffsetFeet.dX, vOffsetFeet.dZ);
    vPitchHeadingBank.dZ = 0;
}

// Returns the magnitude of the given vector.
double VectorMagnitude(const DXYZ& vVector)
{
    return sqrt(vVector.dX * vVector.dX + vVector.dY * vVector.dY + vVector.dZ * vVector.dZ);
}

// Normalize the given vector.
void VectorNormalize(DXYZ& vVector)
{
    double dMagInv = 1.0 / VectorMagnitude(vVector);

    vVector.dX *= dMagInv;
    vVector.dY *= dMagInv;
    vVector.dZ *= dMagInv;
}

// Scales the given vector.
void VectorScale(DXYZ& vVector, double dScale)
{
    vVector.dX *= dScale;
    vVector.dY *= dScale;
    vVector.dZ *= dScale;
}

Simulation::Simulation(IBaseObjectV400* pBaseObject) :
    m_RefCount(1),
    m_spBaseObject(pBaseObject),
    m_pDataProvider(NULL),
    m_eTrackerState(TrackerState::WaitingForFirstPoint)
{
    m_pDataProvider = CreateDataProvider();

    m_vPosCurrent = DXYZ_ZERO;
    m_vPosTarget = DXYZ_ZERO;

    m_vVelCurrent = DXYZ_ZERO;
    
    m_vOrientCurrent = DXYZ_ZERO;
    m_vOrientTarget = DXYZ_ZERO;

    m_vTrajectory = DXYZ_ZERO;
}

Simulation::~Simulation()
{
    DestroyDataProvider(m_pDataProvider);
}

STDMETHODIMP Simulation::QueryInterface(__in REFIID riid, void **ppv)
{
    HRESULT hr = E_NOINTERFACE;

    if (!ppv)
    {
        goto Error;
    }

    *ppv = NULL;

    if (IsEqualIID(riid, IID_IUnknown))
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    else if (IsEqualIID(riid, IID_ISimulation))
    {
        *ppv = static_cast<ISimulation*>(this);
    }
    else if (IsEqualIID(riid, IID_ISimulationV01))
    {
        *ppv = static_cast<ISimulationV01*>(this);
    }
    else if (IsEqualIID(riid, IID_ISimulationV310))
    {
        *ppv = static_cast<ISimulationV310*>(this);
    }

    if (*ppv)
    {
        hr = S_OK;
        AddRef();
    }

Error:

    return hr;
}


STDMETHODIMP Simulation::Update(double dDeltaT)
{
    if (IsNormalMode())
    {
        // Update surface and weather info
        m_spBaseObject->GetSurfaceInformation(m_SurfaceInfo, NULL);
        m_spBaseObject->GetWeatherInformation(m_WeatherInfo);

        if (m_pDataProvider)
        {
            // Set our object's position once the data provider is ready
            if (m_pDataProvider->HasLocationUpdate())
            {
                if (m_eTrackerState == TrackerState::WaitingForFirstPoint)
                {
                    // The first point is starting position.
                    m_vPosCurrent.dZ = m_pDataProvider->GetLatitudeRadians();
                    m_vPosCurrent.dX = m_pDataProvider->GetLongitudeRadians();
                    m_vPosCurrent.dY = m_pDataProvider->GetAltitudeFeet();

                    m_eTrackerState = TrackerState::WaitingForSecondPoint;
                }
                else if (m_eTrackerState == TrackerState::WaitingForSecondPoint)
                {
                    // The second point is our target.
                    m_vPosTarget.dZ = m_pDataProvider->GetLatitudeRadians();
                    m_vPosTarget.dX = m_pDataProvider->GetLongitudeRadians();
                    m_vPosTarget.dY = m_pDataProvider->GetAltitudeFeet();

                    // Calculate trajectory based on starting and target positions.
                    LatLonAltToVector(m_vPosCurrent, m_vPosTarget, m_vTrajectory);

                    // Normalize trajectory.
                    VectorNormalize(m_vTrajectory);

                    // Calculate pitch, bank, and heading based on normalized trajectory.
                    VectorToPitchBankHeading(m_vTrajectory, m_vOrientCurrent);

                    // Initialize target to match the current heading.
                    m_vOrientTarget = m_vOrientCurrent;

                    // Initialize position and heading with the second point.
                    m_spBaseObject->SetPosition(m_vPosCurrent, m_vOrientCurrent, DXYZ_ZERO, DXYZ_ZERO, FALSE, dDeltaT);

                    m_eTrackerState = TrackerState::Tracking;
                }
                else if (m_eTrackerState == TrackerState::Tracking)
                {
                    // Track the new target position.
                    m_vPosTarget.dZ = m_pDataProvider->GetLatitudeRadians();
                    m_vPosTarget.dX = m_pDataProvider->GetLongitudeRadians();
                    m_vPosTarget.dY = m_pDataProvider->GetAltitudeFeet();

                    // Calculate trajectory based on starting and target positions.
                    LatLonAltToVector(m_vPosCurrent, m_vPosTarget, m_vTrajectory);

                    // Normalize trajectory.
                    VectorNormalize(m_vTrajectory);

                    // Calculate pitch, bank, and heading based on normalized trajectory.
                    VectorToPitchBankHeading(m_vTrajectory, m_vOrientTarget);

                    // Calculate velocity vector based on normalized trajectory.
                    m_vVelCurrent = m_vTrajectory;
                    VectorScale(m_vVelCurrent, m_pDataProvider->GetVelocityFeet());

                    // Decrease the frequency of updates.
                    m_pDataProvider->SetUpdateFrequency(5.0);
                }

                m_pDataProvider->ResetLocationUpdate();
            }

            // Extrapolate to target position nad heading.
            if (m_eTrackerState == TrackerState::Tracking)
            {
                DXYZ vOffset;
                vOffset.dZ = m_vVelCurrent.dZ * dDeltaT;
                vOffset.dX = m_vVelCurrent.dX * dDeltaT;
                vOffset.dY = m_vVelCurrent.dY * dDeltaT;

                DXYZ vLonAltLatNew;
                VectorToLatLonAlt(vOffset, m_vPosCurrent, vLonAltLatNew);
                m_vPosCurrent = vLonAltLatNew;

                static const double HEADING_RADIANS_PER_SECOND = DegreesToRadians(2.0);    // Radians per second
                static const double HEADING_TARGET_THRESHOLD = DegreesToRadians(0.1);      // Radians

                double dDeltaHeading = WrapPI(m_vOrientCurrent.dY - m_vOrientTarget.dY);
                if (abs(dDeltaHeading) > HEADING_TARGET_THRESHOLD)
                {
                    if (dDeltaHeading < 0)
                    {
                        m_vOrientCurrent.dY += dDeltaT * HEADING_RADIANS_PER_SECOND;
                    }
                    else
                    {
                        m_vOrientCurrent.dY -= dDeltaT * HEADING_RADIANS_PER_SECOND;
                    }
                }

                m_spBaseObject->SetPosition(m_vPosCurrent, m_vOrientCurrent, DXYZ_ZERO, DXYZ_ZERO, FALSE, dDeltaT);
            }
        }
    }

    return S_OK;
}

BOOL Simulation::IsNormalMode() const
{
    return (0 == (m_spBaseObject->GetMode() & ( SIMOBJECT_MODE_PAUSE | SIMOBJECT_MODE_DISABLED | SIMOBJECT_MODE_SLEW | SIMOBJECT_MODE_CRASH | SIMOBJECT_MODE_FREEZE_POSITION
                                                | SIMOBJECT_MODE_FREEZE_ATTITUDE | SIMOBJECT_MODE_FREEZE_ALTITUDE | SIMOBJECT_MODE_FREEZE_LATLON)));
}

STDMETHODIMP Tracker::QueryInterface(__in REFIID riid, void **ppv)
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
        *ppv = static_cast<Tracker*>(this);
    }

    if (*ppv)
    {
        hr = S_OK;
        AddRef();
    }

Error:

    return hr;
}
    
STDMETHODIMP Tracker::QueryService(__in REFGUID guidService, __in REFIID riid, void **ppv)
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

STDMETHODIMP Tracker::QueryBaseObject(REFIID riid, void** ppv)  //Accessor to get the base object from th ISimObject
{
    HRESULT hr = E_NOINTERFACE;

    if (!ppv)
    {
        goto Error;
    }

    *ppv = NULL;

    hr = m_Simulation.QueryInterface(riid, (void**)ppv);

Error:

    return hr;
}

//Factory function
HRESULT Tracker::New(__in __notnull IBaseObjectV400* pBaseObject, __out __notnull ISimObject** ppThisSim)
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

    *ppThisSim = new Tracker(spBaseObject);

    if (!*ppThisSim)
    {
        goto Error;
    }

    hr = spBaseObject->RegisterSimulation(&static_cast<Tracker*>(*ppThisSim)->m_Simulation, 60.0f /*Hz*/);

Error:
    return hr;
}

Tracker::Tracker(IBaseObjectV400* pBaseObject) :
    m_Simulation(pBaseObject),
    m_bSupportsLabel(FALSE),
    m_RefCount(1)
{
}

//Factory function wrapper
HRESULT New(__in __notnull IBaseObjectV400* pBaseObject, __out __notnull ISimObject** ppThisSim)
{
    return Tracker::New(pBaseObject, ppThisSim);
}

//Property Registration
HRESULT RegisterProperties(__in __notnull ISimObjectManagerV400* pSimObjectMgr)
{
    HRESULT hr = E_FAIL;

    hr = pSimObjectMgr->GetWorldConstants(Tracker::ms_WorldConstants);

    return S_OK;
}
