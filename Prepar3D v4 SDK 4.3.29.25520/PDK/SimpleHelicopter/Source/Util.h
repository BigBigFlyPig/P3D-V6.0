/************************************************************************************************************** 
    Copyright (C) Lockheed Martin Corporation.  All rights reserved.
    
    Description: A sample demonstrating how to implement a simple helicopter ISimObject.
***************************************************************************************************************/

#pragma once

#include <float.h>

#define PI                  3.14159265358979323846
#define TWO_PI              2.0 * PI
#define ONE_OVER_TWO_PI     1.0 / TWO_PI
#define ONE_OVER_60         1.0 / 60.0

#define SQRD(x) (x) * (x)

#define FEET_TO_METERS(feet)    ((feet) * (0.304800))
#define METERS_TO_FEET(meters)  ((meters) * (1.0 / 0.3048))

#define DEGREES_TO_RADIANS(deg) ((deg) * (TWO_PI / 360.0))
#define RADIANS_TO_DEGREES(rad) ((rad) * (360.0 / TWO_PI))

#define LIMIT_VECTOR(V, LIM)  \
    if (_isnan(V.dX)) V.dX = 0; \
    if (_isnan(V.dY)) V.dY = 0; \
    if (_isnan(V.dZ)) V.dZ = 0; \
    V.dX = max(V.dX, -LIM); V.dY = max(V.dY, -LIM); V.dZ = max(V.dZ, -LIM); \
    V.dX = min(V.dX,  LIM); V.dY = min(V.dY,  LIM); V.dZ = min(V.dZ,  LIM);
