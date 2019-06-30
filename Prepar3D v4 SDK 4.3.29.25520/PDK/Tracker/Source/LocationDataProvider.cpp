/**************************************************************************************************************
Copyright (C) Lockheed Martin Corporation.  All rights reserved.

Description: A sample demonstrating how to set postion on an ISimObject
***************************************************************************************************************/

#include "LocationDataProvider.h"

#include <fstream>
#include <vector>
#include <time.h>

#include <Windows.h>

#include "Util.h"

const char* TRACKER_DLL_FILENAME = "Tracker.dll";
const char* LOCATION_DATA_FILENAME = "LocationData.txt";
const char* LOCATION_DATA_URL = "https://api.wheretheiss.at/v1/satellites/25544";

void RemoveQuotes(std::string& Data)
{
    size_t nFront = Data.find_first_of("\\\"") + 1;
    size_t nEnd = Data.find_last_of("\\\"");

    if (nFront != std::string::npos && nEnd != std::string::npos)
    {
        Data = Data.substr(nFront, nEnd - nFront);
    }
}

ILocationDataProvider::~ILocationDataProvider()
{
}

ILocationDataProvider* CreateDataProvider()
{
    return new LocationDataProvider();
}

void DestroyDataProvider(ILocationDataProvider*& pDataProvider)
{
    if (pDataProvider)
    {
        delete pDataProvider;
        pDataProvider = nullptr;
    }
}

DWORD WINAPI ThreadProcCallback(LPVOID lpParam)
{
    LocationDataProvider* pDataProvider = static_cast<LocationDataProvider*>(lpParam);
    if (pDataProvider)
    {
        pDataProvider->ThreadProc();
    }

    return 0;
}

LocationDataProvider::LocationDataProvider() :
    m_hThread(NULL),
    m_dwThreadId(0),
    m_bContinueThread(false),
    m_tLastUpdate(0),
    m_bIsInitialized(false),
    m_dUpdateFrequency(1.0),
    m_bHasLocationUpdate(false),
    m_dLatitude(0.0),
    m_dLongitude(0.0),
    m_dAltitude(0.0),
    m_dVelocity(0.0)
{
    Initialize();
}

LocationDataProvider::~LocationDataProvider()
{
    Deinitialize();
}

void LocationDataProvider::SetUpdateFrequency(double dSeconds)
{
    EnterCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_csDataLock));
    m_dUpdateFrequency = dSeconds;
    LeaveCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_csDataLock));
}

bool LocationDataProvider::HasLocationUpdate() const
{
    bool bResult = false;

    EnterCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_csDataLock));
    bResult = m_bHasLocationUpdate;
    LeaveCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_csDataLock));

    return bResult;
}

void LocationDataProvider::ResetLocationUpdate()
{
    EnterCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_csDataLock));
    m_bHasLocationUpdate = false;
    LeaveCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_csDataLock));
}

double LocationDataProvider::GetLatitudeRadians() const
{
    double dResult = 0.0;

    EnterCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_csDataLock));
    dResult = m_dLatitude;
    LeaveCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_csDataLock));

    return dResult;
}

double LocationDataProvider::GetLongitudeRadians() const
{
    double dResult = 0.0;

    EnterCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_csDataLock));
    dResult = m_dLongitude;
    LeaveCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_csDataLock));

    return dResult;
}

double LocationDataProvider::GetAltitudeFeet() const
{
    double dResult = 0.0;

    EnterCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_csDataLock));
    dResult = m_dAltitude;
    LeaveCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_csDataLock));

    return dResult;
}

double LocationDataProvider::GetVelocityFeet() const
{
    double dResult = 0.0;

    EnterCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_csDataLock));
    dResult = m_dVelocity;
    LeaveCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_csDataLock));

    return dResult;
}

bool LocationDataProvider::Initialize()
{
    InitializeCriticalSection(&m_csDataLock);

    HMODULE hModule = GetModuleHandleA(TRACKER_DLL_FILENAME);
    if (hModule == NULL)
    {
        return false;
    }

    char strFilename[MAX_PATH];
    DWORD dwResult = GetModuleFileNameA(hModule, strFilename, MAX_PATH);
    if (dwResult == 0)
    {
        return false;
    }

    m_strFilePath = strFilename;

    m_strFilePath = m_strFilePath.substr(0, m_strFilePath.find_last_of("\\/") + 1);
    m_strFilePath += LOCATION_DATA_FILENAME;

    m_bContinueThread = true;

    m_hThread = CreateThread(NULL, 0, ThreadProcCallback, this, 0, &m_dwThreadId);
    if (m_hThread == NULL)
    {
        return false;
    }

    return true;
}

void LocationDataProvider::Deinitialize()
{
    if (m_hThread != NULL)
    {
        EnterCriticalSection(&m_csDataLock);
        m_bContinueThread = false;
        LeaveCriticalSection(&m_csDataLock);

        DWORD dwWaitResult = WaitForSingleObject(m_hThread, INFINITE);
        if (dwWaitResult == WAIT_OBJECT_0)
        {
            CloseHandle(m_hThread);
            m_hThread = NULL;
            m_dwThreadId = 0;
        }
    }

    DeleteCriticalSection(&m_csDataLock);
}

void LocationDataProvider::ThreadProc()
{
    bool bRunThread = true;

    while (bRunThread)
    {
        time_t tCurrent;
        time(&tCurrent);

        // Query the "http://wheretheiss.at/" web service for the current location data.
        if (difftime(tCurrent, m_tLastUpdate) > m_dUpdateFrequency || !m_bIsInitialized)
        {
            // Download and save the location to a file.
            HRESULT hr = URLDownloadToFileA(NULL, LOCATION_DATA_URL, m_strFilePath.c_str(), 0, NULL);
            if (SUCCEEDED(hr))
            {
                // Parse location and velocity information.
                if (ParseLocationDataFromFile())
                {
                    m_bIsInitialized = true;
                    time(&m_tLastUpdate);
                }
            }
        }

        EnterCriticalSection(&m_csDataLock);
        if (!m_bContinueThread)
        {
            bRunThread = false;
        }
        LeaveCriticalSection(&m_csDataLock);
    }
}

bool LocationDataProvider::ParseLocationDataFromFile()
{
    m_strDataBuffer.clear();

    std::ifstream File;
    File.open(m_strFilePath.c_str(), std::ifstream::in);
    if (!File.good())
    {
        return false;
    }

    std::getline(File, m_strDataBuffer);

    // Opening and closing curly braces.
    if (m_strDataBuffer.length() < 2)
    {
        return false;
    }
    if (m_strDataBuffer.front() != '{' || m_strDataBuffer.back() != '}')
    {
        return false;
    }

    // Temporary storage.
    double dLatitude = 0.0;
    double dLongitude = 0.0;
    double dAltitude = 0.0;
    double dVelocity = 0.0;

    // Read each key/value pair.
    std::string Key, Value;

    size_t nFront = 1;
    size_t nEnd = std::string::npos;

    bool bContinue = true;
    while (bContinue)
    {
        // Read key.
        nEnd = m_strDataBuffer.find(":", nFront);
        Key = m_strDataBuffer.substr(nFront, nEnd - nFront);
        nFront = nEnd + 1;

        // Read value.
        nEnd = m_strDataBuffer.find(",", nFront);
        if (nEnd == std::string::npos)
        {
            // Read final value.
            nEnd = m_strDataBuffer.find("}", nFront);
            bContinue = false;
        }
        Value = m_strDataBuffer.substr(nFront, nEnd - nFront);
        nFront = nEnd + 1;

        // Remove quotes.
        RemoveQuotes(Key);
        RemoveQuotes(Value);

        // Save values.
        if (Key.compare("latitude") == 0)
        {
            dLatitude = DegreesToRadians(atof(Value.c_str()));
        }
        else if (Key.compare("longitude") == 0)
        {
            dLongitude = DegreesToRadians(atof(Value.c_str()));
        }
        else if (Key.compare("altitude") == 0)
        {
            // Kilometers to meters.
            dAltitude = MetersToFeet(atof(Value.c_str()) * 1000.0);
        }
        else if (Key.compare("velocity") == 0)
        {
            // Kilometers per hour to Feet per second.
            dVelocity = MetersToFeet(atof(Value.c_str()) * 1000) * ONE_OVER_SIXTY * ONE_OVER_SIXTY;
        }
    }

    EnterCriticalSection(&m_csDataLock);
    m_bHasLocationUpdate = true;
    m_dLatitude = dLatitude;
    m_dLongitude = dLongitude;
    m_dAltitude = dAltitude;
    m_dVelocity = dVelocity;
    LeaveCriticalSection(&m_csDataLock);

    return true;
}
