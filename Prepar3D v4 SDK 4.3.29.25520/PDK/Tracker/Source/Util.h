/**************************************************************************************************************
Copyright (C) Lockheed Martin Corporation.  All rights reserved.

Description: A sample demonstrating how to set postion on an ISimObject
***************************************************************************************************************/

#pragma once

const double PI = 3.14159265358979323846;
const double TWO_PI = 2.0 * PI;
const double HALF_PI = PI * 0.5;

const double ONE_OVER_TWO_PI = 1.0 / TWO_PI;
const double ONE_OVER_HALF_PI = 1.0 / HALF_PI;

const double DEGREES_TO_RADIANS = TWO_PI * (1.0 / 360.0);
const double METERS_TO_FEET = 1.0 / 0.3048;

const double ONE_OVER_FOUR = 1.0 / 4.0;
const double ONE_OVER_SIXTY = 1.0 / 60.0;

inline double DegreesToRadians(double dDegrees)
{
    return dDegrees * DEGREES_TO_RADIANS;
}

inline double MetersToFeet(double dMeters)
{
    return dMeters * METERS_TO_FEET;
}

inline double WrapPI(double dRadians)
{
    if (dRadians < -PI)
    {
        dRadians += TWO_PI;
    }
    if (dRadians > PI)
    {
        dRadians -= TWO_PI;
    }
    return dRadians;
}
