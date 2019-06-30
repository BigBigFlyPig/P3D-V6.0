/**************************************************************************************************************
Copyright (C) Lockheed Martin Corporation.  All rights reserved.

Description: A sample demonstrating how to set postion on an ISimObject
***************************************************************************************************************/

#pragma once

#include "ILocationDataProvider.h"

#include <Windows.h>

#include <string>

class LocationDataProvider : public ILocationDataProvider
{
public:

    LocationDataProvider();

    virtual ~LocationDataProvider() override;

    virtual void SetUpdateFrequency(double dSeconds) override;

    virtual bool HasLocationUpdate() const override;
    virtual void ResetLocationUpdate() override;

    virtual double GetLatitudeRadians() const override;
    virtual double GetLongitudeRadians() const override;
    virtual double GetAltitudeFeet() const override;

    virtual double GetVelocityFeet() const override;

    void ThreadProc();

private:

    bool Initialize();
    void Deinitialize();

    bool ParseLocationDataFromFile();

    HANDLE m_hThread;
    DWORD m_dwThreadId;
    bool m_bContinueThread;

    CRITICAL_SECTION m_csDataLock;

    time_t m_tLastUpdate;

    std::string m_strFilePath;
    std::string m_strDataBuffer;

    double m_dUpdateFrequency;  // Seconds

    bool m_bIsInitialized;
    bool m_bHasLocationUpdate;

    double m_dLatitude;     // Radians
    double m_dLongitude;    // Radians
    double m_dAltitude;     // Feet

    double m_dVelocity;     // Feet
};
