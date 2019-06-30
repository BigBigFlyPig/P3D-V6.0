///----------------------------------------------------------------------------
///
///  Copyright 2017 Lockheed Martin Corporation
///  Lockheed Martin Proprietary Information
///
///  Representation of a location in the simulation.
///
///----------------------------------------------------------------------------

#pragma once

#include <atlcomcli.h>
#include "InitGuid.h"

#include "LLA.h"
#include "PHB.h"

class Location
{
public:

    Location();
    Location(LLA);
    Location(PHB);
    Location(LLA, PHB);

    ~Location();

    // Getters
    LLA GetLLA() const;
    PHB GetPHB() const;

    // Setters
    void SetLLA(const LLA &lonAltLat);
    void SetPHB(const PHB &pitchHeadingBank);
    void SetFromWorldCoordinates(const P3D::DXYZ &xyz, const LLA &origin);

    // Helpers
    double    FindRelativeDistance(const Location&)       const;
    double    FindLateralDistance(const Location &origin) const;
    double    FindTrueHeading(const Location &origin)     const;
    P3D::DXYZ FindRelativePosition(const Location&)       const;
    P3D::DXYZ FindWorldCoordinates(const LLA &originLLA)  const;
    LLA Location::FindRelativeLocation(const double dDistanceFeet, const double dTrueHeadingRad, const double dAltitutdeChange) const;

private:

    LLA  m_LLA;
    PHB  m_PHB;
};