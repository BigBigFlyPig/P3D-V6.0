/**************************************************************************************************************
Copyright (C) Lockheed Martin Corporation.  All rights reserved.

Description: A sample demonstrating how to set postion on an ISimObject
***************************************************************************************************************/

#pragma once

class ILocationDataProvider
{
public:

    virtual ~ILocationDataProvider() = 0;

    virtual void SetUpdateFrequency(double dSeconds) = 0;

    virtual bool HasLocationUpdate() const = 0;
    virtual void ResetLocationUpdate() = 0;

    virtual double GetLatitudeRadians() const = 0;
    virtual double GetLongitudeRadians() const = 0;
    virtual double GetAltitudeFeet() const = 0;

    virtual double GetVelocityFeet() const = 0;

};

ILocationDataProvider* CreateDataProvider();
void DestroyDataProvider(ILocationDataProvider*& pDataProvider);
