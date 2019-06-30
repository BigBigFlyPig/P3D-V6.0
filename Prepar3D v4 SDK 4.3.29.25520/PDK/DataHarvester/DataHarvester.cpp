//----------------------------------------------------------------------------
//
//  Copyright 2017 Lockheed Martin Corporation 
//  Lockheed Martin Proprietary Information
//
//----------------------------------------------------------------------------

#include "DataHarvester.h"

#include "PdkServices.h"

#include <sstream>
#include <iomanip>

static const WCHAR* MODULE_FILE_NAME = L"DataHarvester.dll";
static const WCHAR* OUTPUT_FILE_NAME = L"DataHarvester.csv";

using namespace P3D;

HRESULT DataHarvester::Init()
{
    HRESULT hr = E_FAIL;

    // Lookup the path of the module. Store the output file in that directory.
    HMODULE hModule = GetModuleHandleW(MODULE_FILE_NAME);
    if (hModule != NULL)
    {
        WCHAR szModuleFileName[MAX_PATH];
        DWORD dwResult = GetModuleFileNameW(hModule, szModuleFileName, MAX_PATH);
        if (dwResult != 0)
        {
            m_strOutputFilePath = szModuleFileName;
            m_strOutputFilePath = m_strOutputFilePath.substr(0, m_strOutputFilePath.find_last_of(L"\\/") + 1);
            m_strOutputFilePath += OUTPUT_FILE_NAME;
            hr = S_OK;
        }
    }

    return hr;
}

HRESULT DataHarvester::Deinit()
{
    HRESULT hr = E_FAIL;

    // Clean up.
    hr = Stop();

    return hr;
}

HRESULT DataHarvester::Start()
{
    // Attempt to open the file, notifying the user upon failure.
    m_fs.open(m_strOutputFilePath, std::wfstream::out);
    if (m_fs.fail())
    {
        std::wstringstream strMessage;
        strMessage << L"Failed to open \"" << m_strOutputFilePath << L"\". Please ensure the file has the correct permissions and is not being used by another application.";
        PdkServices::GetReportingService()->ShowAlert(strMessage.str().c_str(), AlertWindowType::Alert_Ok);
        m_fs.clear();
        return E_FAIL;
    }
    m_fs << std::setprecision(20);

    // Register for the per-frame callback.
    if (FAILED(PdkServices::GetEventService()->RegisterCallback(EVENTID_Frame, this)))
    {
        return E_FAIL;
    }

    // Print variable title information into the CSV.
    PrintHeader();

    m_bIsHarvesting = true;

    return S_OK;
}

HRESULT DataHarvester::Stop()
{
    HRESULT hr = S_OK;

    if (m_bIsHarvesting)
    {
        // Unregister per-frame callback.
        if (FAILED(PdkServices::GetEventService()->UnregisterCallback(EVENTID_Frame, this)))
        {
            hr = E_FAIL;
        }

        // Close the file. Clear any errors.
        m_fs.close();
        m_fs.clear();
    }

    return hr;
}

/*static*/
HRESULT DataHarvester::StartCallback(void* pContext)
{
    HRESULT hr = E_FAIL;

    DataHarvester* pDataHarvester = static_cast<DataHarvester*>(pContext);
    if (pDataHarvester)
    {
        hr = pDataHarvester->Start();
    }

    return hr;
}

/*static*/
HRESULT DataHarvester::StopCallback(void* pContext)
{
    HRESULT hr = E_FAIL;

    DataHarvester* pDataHarvester = static_cast<DataHarvester*>(pContext);
    if (pDataHarvester)
    {
        hr = pDataHarvester->Stop();
    }

    return hr;
}

struct PropertyDefinition
{
    const WCHAR*  pszName;
    const WCHAR*  pszUnits;
};

// A list of global environment variables (only used for printing names).
const PropertyDefinition g_aGlobals[] =
{
    { L"ABSOLUTE TIME",                 L"Seconds"                  },
    { L"ZULU TIME",                     L"Seconds"                  },
    { L"ZULU DAY OF YEAR",              L"Number"                   },
    { L"ZULU YEAR",                     L"Number"                   },
    { L"ZULU MONTH OF YEAR",            L"Number"                   },
    { L"ZULU DAY OF MONTH",             L"Number"                   },
    { L"ZULU DAY OF WEEK",              L"Number"                   },
    { L"TIME ZONE OFFSET",              L"Hours"                    },
};

// A list of simulation variables / registered properties to be collected.
const PropertyDefinition g_aProperties[] =
{
    { L"SIM TIME",                      L"Seconds",                 },
    { L"PLANE LATITUDE",                L"Degrees"                  },
    { L"PLANE LONGITUDE",               L"Degrees"                  },
    { L"PLANE ALTITUDE",                L"Feet"                     },
    { L"PLANE PITCH DEGREES",           L"Degrees"                  },
    { L"PLANE BANK DEGREES",            L"Degrees"                  },
    { L"PLANE HEADING DEGREES TRUE",    L"Degrees"                  },
    { L"VELOCITY WORLD X",              L"Feet per second"          },
    { L"VELOCITY WORLD Y",              L"Feet per second"          },
    { L"VELOCITY WORLD Z",              L"Feet per second"          },
    { L"AMBIENT TEMPERATURE",           L"Celsius"                  },
    { L"AMBIENT PRESSURE",              L"Millibars"                },
    { L"AMBIENT DENSITY",               L"Slugs per cubic feet"     },
    { L"AMBIENT WIND VELOCITY",         L"Knots"                    },
    { L"AMBIENT WIND DIRECTION",        L"Degrees"                  },
    { L"AMBIENT WIND X",                L"Meters per second"        },
    { L"AMBIENT WIND Y",                L"Meters per second"        },
    { L"AMBIENT WIND Z",                L"Meters per second"        },
};

// Frame callback.
void DataHarvester::Invoke(IParameterListV400* pParams)
{
    CComPtr<IBaseObjectV400> spUserObject;
    if (SUCCEEDED(PdkServices::GetSimObjectManager()->GetUserObject(&spUserObject)))
    {
        // The user object's title.
        WCHAR szTitle[MAX_PATH];
        spUserObject->GetTitle(szTitle, ARRAYSIZE(szTitle));
        PrintValue(szTitle, wcslen(szTitle) > 0);

        // Global environment data such as time of day.
        IGlobalDataV400* pGlobalData = PdkServices::GetGlobalData();

        // Lookup the unit codes.
        int eSeconds = 0;
        pGlobalData->GetUnitCode(L"Seconds", eSeconds);

        int eHours = 0;
        pGlobalData->GetUnitCode(L"Hours", eHours);

        unsigned int uValue;
        double dValue;
        bool bIsValid;

        // Collect zulu time information.
        bIsValid = SUCCEEDED(pGlobalData->GetAbsoluteTime(dValue, eSeconds));
        PrintValue(dValue, bIsValid);

        bIsValid = SUCCEEDED(pGlobalData->GetTime(dValue, eSeconds, TIMEREF_ZULU));
        PrintValue(dValue, bIsValid);

        bIsValid = SUCCEEDED(pGlobalData->GetDayOfTheYear(uValue, TIMEREF_ZULU));
        PrintValue(uValue, bIsValid);

        bIsValid = SUCCEEDED(pGlobalData->GetYear(uValue, TIMEREF_ZULU));
        PrintValue(uValue, bIsValid);

        bIsValid = SUCCEEDED(pGlobalData->GetMonthOfTheYear(uValue, TIMEREF_ZULU));
        PrintValue(uValue, bIsValid);

        bIsValid = SUCCEEDED(pGlobalData->GetDayOfTheMonth(uValue, TIMEREF_ZULU));
        PrintValue(uValue, bIsValid);

        bIsValid = SUCCEEDED(pGlobalData->GetDayOfTheWeek(uValue, TIMEREF_ZULU));
        PrintValue(uValue, bIsValid);

        bIsValid = SUCCEEDED(pGlobalData->GetTimeZoneOffset(dValue, eHours));
        PrintValue(dValue, bIsValid);

        // Floating point properties (defined above).
        for (unsigned int i = 0; i < ARRAYSIZE(g_aProperties); ++i)
        {
            const PropertyDefinition& prop = g_aProperties[i];
            bool bIsValid = SUCCEEDED(spUserObject->GetProperty(prop.pszName, prop.pszUnits, dValue));
            bool bIsLast = (i == ARRAYSIZE(g_aProperties) - 1);
            PrintValue(dValue, bIsValid, bIsLast);
        }
    }

    m_fs.flush();
}

void DataHarvester::PrintHeader()
{
    // Print title.
    m_fs << L"TITLE,";

    // Print global environment names.
    for (unsigned int i = 0; i < ARRAYSIZE(g_aGlobals); ++i)
    {
        const PropertyDefinition& prop = g_aGlobals[i];
        m_fs << prop.pszName << " (" << prop.pszUnits << "),";
    }

    // Print floating point variable names.
    for (unsigned int i = 0; i < ARRAYSIZE(g_aProperties); ++i)
    {
        const PropertyDefinition& prop = g_aProperties[i];
        m_fs << prop.pszName << " (" << prop.pszUnits << ")";

        if (i != ARRAYSIZE(g_aProperties) - 1)
        {
            m_fs << ",";
        }
    }

    m_fs << std::endl;

    m_fs.flush();
}

template<class T>
void DataHarvester::PrintValue(T value, bool bIsValid /*=true*/, bool bIsLast /*=false*/)
{
    // NOTE: Invalid entries are left empty, but still require a comma.

    if (!bIsLast)
    {
        if (bIsValid)
        {
            m_fs << value << ",";
        }
        else
        {
            m_fs << ",";
        }
    }
    else
    {
        if (bIsValid)
        {
            m_fs << value << std::endl;
        }
        else
        {
            m_fs << std::endl;
        }
    }
}
