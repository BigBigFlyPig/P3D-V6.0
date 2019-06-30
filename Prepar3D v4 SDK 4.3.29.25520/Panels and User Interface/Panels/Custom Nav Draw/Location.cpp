///----------------------------------------------------------------------------
///
///  Copyright 2017 Lockheed Martin Corporation
///  Lockheed Martin Proprietary Information
///
///  Representation of a location in the simulation.
///
///----------------------------------------------------------------------------

#include <exception>
#include "math.h"
#include "Location.h"
#include "Conversions.h"

Location::Location()
{
}

Location::Location(LLA lla)
{
    m_LLA = lla;
}

Location::Location(PHB phb)
{
    m_PHB = phb;
}

Location::Location(LLA lla, PHB phb)
{
    m_LLA = lla;
    m_PHB = phb;
}

Location::~Location()
{
}

// =======
// Getters
// =======

// ========================================================================
// Name: GetLLA
//
// Desc: Returns the latitude, longitude, and altitude associated with this
//       location.
// ========================================================================
LLA Location::GetLLA() const
{
    return m_LLA;
}

// ========================================================================
// Name: GetPHB
//
// Desc: Returns the pitch, heading, and bank associated with this 
//       location.
// ========================================================================
PHB Location::GetPHB() const
{
    return m_PHB;
}

// =======
// Setters
// =======

// ========================================================================
// Name: SetLLA
//
// Desc: Sets the latitude, longitude, and altitude associated with this
//       location.
// ========================================================================
void Location::SetLLA(const LLA &lonAltLat)
{
    m_LLA = lonAltLat;
}

// ========================================================================
// Name: SetPHB
//
// Desc: Sets the pitch, heading, and bank associated with this location.
// ========================================================================
void Location::SetPHB(const PHB &pitchHeadingBank)
{
    m_PHB = pitchHeadingBank;
}

// ========================================================================
// Name: SetFromWorldCoordinates
//
// Desc: Sets the latitude, longitude, and altitude associated with this
//       location from XYZ world coordinates, given an origin LLA.
// ========================================================================
void Location::SetFromWorldCoordinates(const P3D::DXYZ &xyz, const LLA &originLLA)
{
    P3D::DXYZ origin = originLLA.ToDXYZ();
    m_LLA.FromDXYZ(ConvertXYZToLonAltLat(xyz, origin));
}

// =======
// Helpers
// =======

// ========================================================================
// Name: FindRelativeDistance
//
// Desc: Finds the distance (slant range) in feet between two locations.
// ========================================================================
double Location::FindRelativeDistance(const Location &origin) const
{
    P3D::DXYZ relativePosition = FindRelativePosition(origin);
    double distance = 0.0;

    distance = sqrt(SQUARE(relativePosition.dX) + SQUARE(relativePosition.dY) + SQUARE(relativePosition.dZ));

    return distance;
}

// ========================================================================
// Name: FindLateralDistance
//
// Desc: Finds the distance in feet between two locations. (Lateral Only)
// ========================================================================
double Location::FindLateralDistance(const Location &origin) const
{
    P3D::DXYZ relativePosition = FindRelativePosition(origin);
    double distance = sqrt(SQUARE(relativePosition.dX) + SQUARE(relativePosition.dZ));

    return distance;
}

// ========================================================================
// Name: FindTrueHeading
//
// Desc: Finds true heading between two locations, in radians.
// ========================================================================
double Location::FindTrueHeading(const Location &origin) const
{
    P3D::DXYZ relativePosition = FindRelativePosition(origin);
    double heading = 0.0;

    double x = relativePosition.dX;
    double y = relativePosition.dZ;
    if (fabs(y) < 1.0)
    {
        y = 1.0;
    }

    double normalized = atan2(x, y);
    if (normalized < 0.0)
    {
        normalized += 6.28318530718;
    }

    return normalized;
}

// ========================================================================
// Name: FindRelativePosition
//
// Desc: Finds the relative position between two locations in feet, without
//       changing coordinate systems.
// ========================================================================
P3D::DXYZ Location::FindRelativePosition(const Location &origin) const
{
    P3D::DXYZ relativePosition;
    P3D::DXYZ originXYZ = origin.FindWorldCoordinates(origin.GetLLA());
    P3D::DXYZ ourXYZ = ConvertLonAltLatToXYZ(m_LLA.ToDXYZ(), origin.GetLLA().ToDXYZ());

    relativePosition.dX = ourXYZ.dX - originXYZ.dX;
    relativePosition.dY = ourXYZ.dY - originXYZ.dY;
    relativePosition.dZ = ourXYZ.dZ - originXYZ.dZ;

    return relativePosition;
}

// ========================================================================
// Name: FindWorldCoordinates
//
// Desc: Returns the latitude, longitude, and altitude associated with this
//       location, converted into XYZ world coordinates (in feet).
//       Longitude -> dX
//       Altitude  -> dY
//       Latitude  -> dZ
// ========================================================================
P3D::DXYZ Location::FindWorldCoordinates(const LLA &originLLA) const
{
    P3D::DXYZ latLonAlt = m_LLA.ToDXYZ();
    P3D::DXYZ origin = originLLA.ToDXYZ();

    return ConvertLonAltLatToXYZ(latLonAlt, origin);
}

// ========================================================================
// Name: FindRelativeLocation
//
// Desc: Finds the relative location by adding the distance in feet based on
// true heading and also adds in the altitude.
// ========================================================================
LLA Location::FindRelativeLocation(const double dDistanceFeet, const double dTrueHeadingRad, const double dAltitutdeChange) const
{
    LLA relativeLocation;

    relativeLocation.altitude = m_LLA.altitude + dAltitutdeChange;
    relativeLocation.latitude = m_LLA.latitude + (dDistanceFeet / RADIUS_OF_EARTH_AT_EQUATOR * cos(dTrueHeadingRad));
    double dCosLat = cos(relativeLocation.latitude);
    if (fabs(dCosLat) < 0.0001)
    {
        dCosLat = 1.0;
    }
    relativeLocation.longitude = m_LLA.longitude + (dDistanceFeet / RADIUS_OF_EARTH_AT_EQUATOR * sin(dTrueHeadingRad) / dCosLat);

    return relativeLocation;
}
