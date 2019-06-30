///----------------------------------------------------------------------------
///
///  Copyright 2017 Lockheed Martin Corporation
///  Lockheed Martin Proprietary Information
///
///  Representation of a latitude, longitude, and altitude coordinate.
///
///----------------------------------------------------------------------------

#include "LLA.h"

// ========================================================================
// Name: LLA
//
// Desc: Constructor.
// ========================================================================
LLA::LLA()
{
    longitude = 0.0;
    altitude = 0.0;
    latitude = 0.0;
}

LLA::LLA(double dLat, double dLon)
{
    longitude = dLon;
    altitude = 0.0;
    latitude = dLat;
}

// ========================================================================
// Name: LLA
//
// Desc: Constructor that populates the LLA from a DXYZ.
//       dX -> Longitude
//       dY -> Altitude
//       dZ -> Latitude
// ========================================================================
LLA::LLA(const P3D::DXYZ &lonAltLat)
{
    longitude = lonAltLat.dX;
    altitude = lonAltLat.dY;
    latitude = lonAltLat.dZ;
}

// ========================================================================
// Name: LLA
//
// Desc: Constructor that populates the LLA from a radar LLA.
//       dX -> Longitude
//       dY -> Altitude
//       dZ -> Latitude
// ========================================================================
LLA::LLA(Radar::LLA lla)
{
    longitude = lla.Lon;
    altitude = lla.Alt;
    latitude = lla.Lat;
}

// ========================================================================
// Name: ToDXYZ
//
// Desc: Populates a DXYZ from the LLA.
//       Longitude -> dX
//       Altitude  -> dY
//       Latitude  -> dZ
// ========================================================================
P3D::DXYZ LLA::ToDXYZ() const
{
    P3D::DXYZ lonAltLat;

    lonAltLat.dX = longitude;
    lonAltLat.dY = altitude;
    lonAltLat.dZ = latitude;

    return lonAltLat;
}

// ========================================================================
// Name: ToRadarLLA
//
// Desc: Populated a radar LLA from the current LLA.
// ========================================================================
Radar::LLA LLA::ToRadarLLA() const
{
    Radar::LLA lla;

    lla.Lon = longitude;
    lla.Alt = altitude;
    lla.Lat = latitude;

    return lla;
}

// ========================================================================
// Name: FromDXYZ
//
// Desc: Populates the LLA from a DXYZ.
//       dX -> Longitude
//       dY -> Altitude
//       dZ -> Latitude
// ========================================================================
void LLA::FromDXYZ(const P3D::DXYZ &lonAltLat)
{
    longitude = lonAltLat.dX;
    altitude = lonAltLat.dY;
    latitude = lonAltLat.dZ;
}
