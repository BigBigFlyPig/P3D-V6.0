///----------------------------------------------------------------------------
///
///  Copyright 2017 Lockheed Martin Corporation
///  Lockheed Martin Proprietary Information
///
///  Contains helpful conversion methods.
///
///----------------------------------------------------------------------------


#include "Conversions.h"
#include <math.h>

// ========================================================================
// Name: RotateXYZtoHdg
//
// Desc: Rotate X and Z feet to specific heading based on a 0,0 origin
//       Lateral position only.
// ========================================================================
P3D::DXYZ RotateXYZtoHdg(P3D::DXYZ xyz_ft, double hdg_rad)
{
    P3D::DXYZ rotated_xyz;
    rotated_xyz.dX = xyz_ft.dX * cos(hdg_rad) + xyz_ft.dZ * sin(hdg_rad);
    rotated_xyz.dY = xyz_ft.dY;
    rotated_xyz.dZ = xyz_ft.dZ * cos(hdg_rad) + xyz_ft.dX * -sin(hdg_rad);
    return rotated_xyz;
}

// ========================================================================
// Name: ConvertXYZtoHdgDist
//
// Desc: Find Distance and True Heading for X and Z position.
// ========================================================================
void ConvertXYZtoHdgDist(P3D::DXYZ xyz_ft, double &hdg_rad, double &dist_feet)
{
    dist_feet = sqrt(xyz_ft.dX * xyz_ft.dX + xyz_ft.dZ * xyz_ft.dZ);
    if (fabs(xyz_ft.dX)<1.0)
    {
        xyz_ft.dX = 1.0;
    }
    hdg_rad = atan2(xyz_ft.dZ, xyz_ft.dX);
}

// ========================================================================
// Name: ConvertLonAltLatToXYZ
//
// Desc: Converts an LLA world coordinate (in radians) into a local XYZ 
//       coordinate (in feet).
// ========================================================================
P3D::DXYZ ConvertLonAltLatToXYZ(P3D::DXYZ lonAltLat_radians, P3D::DXYZ referenceLonAltLat)
{
    double deltaLatitude_radians = lonAltLat_radians.dZ - referenceLonAltLat.dZ;
    double deltaLongitude_radians = lonAltLat_radians.dX - referenceLonAltLat.dX;
    static const double rads_to_meters = 20037500.0 / PI;
    P3D::DXYZ lonAltLat_ft;
    lonAltLat_ft.dX = deltaLongitude_radians * rads_to_meters;
    lonAltLat_ft.dY = lonAltLat_radians.dY;
    lonAltLat_ft.dZ = deltaLatitude_radians  * rads_to_meters;

    return lonAltLat_ft;
}

// ========================================================================
// Name: ConvertXYZToLonAltLat
//
// Desc: Converts an XYZ local coordinate (in feet) into a local LLA 
//       coordinate (in radians).
// ========================================================================
P3D::DXYZ ConvertXYZToLonAltLat(P3D::DXYZ lonAltLat_ft, P3D::DXYZ referenceLonAltLat)
{
    P3D::DXYZ lonAltLat_radians;
    double deltaLatitude = lonAltLat_ft.dZ / RADIUS_OF_EARTH_AT_EQUATOR;
    lonAltLat_radians.dZ = referenceLonAltLat.dZ + deltaLatitude;
    double deltaLongitude = lonAltLat_ft.dX / (RADIUS_OF_EARTH_AT_EQUATOR * cos(lonAltLat_radians.dZ));
    lonAltLat_radians.dX = referenceLonAltLat.dX + deltaLongitude;
    lonAltLat_radians.dY = lonAltLat_ft.dY;

    return lonAltLat_radians;
}

double NormalizeRadians(double dRadians)
{
    if (dRadians >= PI_2)
    {
        dRadians -= PI_2;
    }
    else if (dRadians<0.0)
    {
        dRadians += PI_2;
    }
    return dRadians;
}

double NormalizeCenterRadians(double dRadians)
{
    if (dRadians >= PI)
    {
        dRadians -= PI_2;
    }
    if (dRadians <= -PI)
    {
        dRadians += PI_2;
    }
    return dRadians;
}