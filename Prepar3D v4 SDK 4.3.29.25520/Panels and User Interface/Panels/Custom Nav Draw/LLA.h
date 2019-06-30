///----------------------------------------------------------------------------
///
///  Copyright 2017 Lockheed Martin Corporation
///  Lockheed Martin Proprietary Information
///
///  Representation of a latitude, longitude, and altitude coordinate.
///
///----------------------------------------------------------------------------

#pragma once

#include <atlcomcli.h>
#include "InitGuid.h"
#include "Types.h"
#include "ISimulatedRadar.h"
typedef double RADIANS;
typedef double FEET;

struct LLA
{
    LLA();
    LLA(double dLat, double dLon);
    LLA(const P3D::DXYZ &lonAltLat);
    LLA::LLA(Radar::LLA lla);

    P3D::DXYZ ToDXYZ() const;
    Radar::LLA ToRadarLLA() const;
    void FromDXYZ(const P3D::DXYZ&);

    RADIANS longitude;
    FEET    altitude;
    RADIANS latitude;
};