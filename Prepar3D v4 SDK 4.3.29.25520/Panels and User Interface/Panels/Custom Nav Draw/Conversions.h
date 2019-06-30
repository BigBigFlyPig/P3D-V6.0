///----------------------------------------------------------------------------
///
///  Copyright 2010 Lockheed Martin Corporation 
///  Lockheed Martin Proprietary Information
///  
///  @contract BWC PC21
///  
///  Contains helpful conversion methods.
///
///----------------------------------------------------------------------------

#ifndef _CONVERSIONS_H
#define _CONVERSIONS_H

#include <atlcomcli.h>
#include <InitGuid.h>
#include "Types.h"

//#include "Common\Matrix\Matrix_3x3.h"
//#include "Common\Vector\Vector_3x1.h"

// Constants
const double RADIUS_OF_EARTH_AT_EQUATOR = 20925646.9; // in feet
const double GRAVITY_ACCELERATION_FPS = -32.174;
const double PI   = 3.141592653589793238462643383279;
const double PI_2 = 6.283185307179586476925286766559;
// Conversion Macros
inline double RADIANS_TO_DEGREES(double x) { return x * 57.2957795131; }
inline double DEGREES_TO_RADIANS(double x) { return x * 0.01745329252; }
inline double FEET_TO_METERS(double x)     { return x * 0.3048; }
inline double FEET_TO_NM(double x)         { return x * 0.000164582; }
inline double METERS_TO_FEET(double x)     { return x * 3.2808399; }
inline double KM_TO_FEET(double x)         { return x * 3280.8399; }
inline double NM_TO_FEET(double x)         { return x * 6076.12; }
inline double SQUARE(double x)             { return x * x; }
inline double FPS_TO_KNOTS(double x)       { return FEET_TO_NM(x) * 3600.0; }
inline double HR_TO_SEC(double x)          { return x * 3600.0; }
inline double SEC_TO_HR(double x)          { return x * 0.000277777777; }

//// LLA to XYZ Conversions
P3D::DXYZ ConvertLonAltLatToXYZ(P3D::DXYZ lonAltLat_radians, P3D::DXYZ referenceLonAltLat);
P3D::DXYZ ConvertXYZToLonAltLat(P3D::DXYZ lonAltLat_ft, P3D::DXYZ referenceLonAltLat);
P3D::DXYZ RotateXYZtoHdg(P3D::DXYZ xyz_ft, double hdg_rad);
void ConvertXYZtoHdgDist(P3D::DXYZ xyz_ft, double &hdg_rad, double &dist_feet);
//
//// World-to-Body Rotations
//Matrix_3x3 GetWorldToBodyMatrix(const Vector_3x1 phb);
//Matrix_3x3 GetBodyToWorldMatrix(const Vector_3x1 &phb);
//Vector_3x1 WorldToBody(const Vector_3x1 &phb, const Vector_3x1 &worldVector);
//Vector_3x1 BodyToWorld(const Vector_3x1 &phb, const Vector_3x1 &i_bodyVector);
//P3D::DXYZ  WorldToBodyRotate(const P3D::DXYZ phbRelativePosition, const P3D::DXYZ phbOriginObject);
double NormalizeRadians(double dRadians);
double NormalizeCenterRadians(double dRadians);

#endif