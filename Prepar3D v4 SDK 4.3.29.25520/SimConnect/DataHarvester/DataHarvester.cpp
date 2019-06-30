//Copyright (c) Lockheed Martin Corporation.  All rights reserved.  
//------------------------------------------------------------------------------
//
//  SimConnect Data Harvester Sample
//  
//  Description:
//              Requests data on the user object and outputs it to a CSV file.
//              If the sample application is ran from within the SimConnect
//              Samples solution, the CSV file will be created relative to the
//              DataHarvester project file. If the sample application is executed
//              directly, the CSV file will be created relative to the application.
//              Start and stop data harvesting from the Prepar3D Add-ons menu item.
//------------------------------------------------------------------------------

#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>

#include "SimConnect.h"

int     g_bQuit = 0;
HANDLE  g_hSimConnect = NULL;
FILE*   g_pFile = NULL;
bool    g_bIsHarvesting = false;

static const char* TITLE_STRING = "Data Harvester";
static const char* START_STRING = "Start Harvest (SimConnect)";
static const char* STOP_STRING = "Stop Harvest (SimConnect)";

static const char* OUTPUT_FILE_NAME = "DataHarvester.csv";

enum GROUP_ID
{
    GROUP_ID_MENU,
};

enum EVENT_ID
{
    EVENT_ID_START_HARVEST,
    EVENT_ID_STOP_HARVEST,
};

enum DEFINITION_ID
{
    DEFINITION_ID_USER_OBJECT,
};

enum REQUEST_ID
{
    REQUEST_ID_USER_OBJECT_DATA,
};

// This struct must align with the sizes and types defined in the table below.
struct ObjectData
{
    char    szTitle[256];
    double  dAbsoluteTime;
    double  dTime;
    int     uDayOfTheYear;
    int     uYear;
    int     uMonthOfTheYear;
    int     uDayOfTheMonth;
    int     uDayOfTheWeek;
    int     uTimeZoneOffset;
    double  dSimTime;
    double  dLatitude;
    double  dLongitude;
    double  dAltitude;
    double  dPitch;
    double  dBank;
    double  dHeading;
    double  dVelocityX;
    double  dVelocityY;
    double  dVelocityZ;
    double  dTemperature;
    double  dAirPressure;
    double  dAirDensity;
    double  dWindVelocity;
    double  dWindDirection;
    double  dWindX;
    double  dWindY;
    double  dWindZ;
};

struct PropertyDefinition
{
    const char*         pszName;
    const char*         pszUnits;
    SIMCONNECT_DATATYPE eDataType;
};

// This table must align with the sizes and types defined in the struct above.
const PropertyDefinition g_aVariables[] =
{
    { "TITLE",                      NULL,                   SIMCONNECT_DATATYPE_STRING256   },
    { "ABSOLUTE TIME",              "Seconds",              SIMCONNECT_DATATYPE_FLOAT64     },
    { "ZULU TIME",                  "Seconds",              SIMCONNECT_DATATYPE_FLOAT64     },
    { "ZULU DAY OF YEAR",           "Number",               SIMCONNECT_DATATYPE_INT32       },
    { "ZULU YEAR",                  "Number",               SIMCONNECT_DATATYPE_INT32       },
    { "ZULU MONTH OF YEAR",         "Number",               SIMCONNECT_DATATYPE_INT32       },
    { "ZULU DAY OF MONTH",          "Number",               SIMCONNECT_DATATYPE_INT32       },
    { "ZULU DAY OF WEEK",           "Number",               SIMCONNECT_DATATYPE_INT32       },
    { "TIME ZONE OFFSET",           "Hours",                SIMCONNECT_DATATYPE_INT32       },
    { "SIM TIME",                   "Seconds",              SIMCONNECT_DATATYPE_FLOAT64     },
    { "PLANE LATITUDE",             "Degrees",              SIMCONNECT_DATATYPE_FLOAT64     },
    { "PLANE LONGITUDE",            "Degrees",              SIMCONNECT_DATATYPE_FLOAT64     },
    { "PLANE ALTITUDE",             "Feet",                 SIMCONNECT_DATATYPE_FLOAT64     },
    { "PLANE PITCH DEGREES",        "Degrees",              SIMCONNECT_DATATYPE_FLOAT64     },
    { "PLANE BANK DEGREES",         "Degrees",              SIMCONNECT_DATATYPE_FLOAT64     },
    { "PLANE HEADING DEGREES TRUE", "Degrees",              SIMCONNECT_DATATYPE_FLOAT64     },
    { "VELOCITY WORLD X",           "Feet per second",      SIMCONNECT_DATATYPE_FLOAT64     },
    { "VELOCITY WORLD Y",           "Feet per second",      SIMCONNECT_DATATYPE_FLOAT64     },
    { "VELOCITY WORLD Z",           "Feet per second",      SIMCONNECT_DATATYPE_FLOAT64     },
    { "AMBIENT TEMPERATURE",        "Celsius",              SIMCONNECT_DATATYPE_FLOAT64     },
    { "AMBIENT PRESSURE",           "Millibars",            SIMCONNECT_DATATYPE_FLOAT64     },
    { "AMBIENT DENSITY",            "Slugs per cubic feet", SIMCONNECT_DATATYPE_FLOAT64     },
    { "AMBIENT WIND VELOCITY",      "Knots",                SIMCONNECT_DATATYPE_FLOAT64     },
    { "AMBIENT WIND DIRECTION",     "Degrees",              SIMCONNECT_DATATYPE_FLOAT64     },
    { "AMBIENT WIND X",             "Meters per second",    SIMCONNECT_DATATYPE_FLOAT64     },
    { "AMBIENT WIND Y",             "Meters per second",    SIMCONNECT_DATATYPE_FLOAT64     },
    { "AMBIENT WIND Z",             "Meters per second",    SIMCONNECT_DATATYPE_FLOAT64     },
};

void PrintHeader()
{
    char szBuffer[2048] = { 0 };

    for (unsigned int i = 0; i < ARRAYSIZE(g_aVariables); ++i)
    {
        const PropertyDefinition& prop = g_aVariables[i];

        strcat_s(szBuffer, sizeof(szBuffer), prop.pszName);

        if (prop.pszUnits)
        {
            char szUnits[64] = { 0 };
            StringCchPrintfA(szUnits, sizeof(szUnits), " (%s)", prop.pszUnits);
            strcat_s(szBuffer, sizeof(szBuffer), szUnits);
        }

        if (i != ARRAYSIZE(g_aVariables) - 1)
        {
            strcat_s(szBuffer, sizeof(szBuffer), ",");
        }
    }

    strcat_s(szBuffer, sizeof(szBuffer), "\n");

    fprintf_s(g_pFile, szBuffer);
}

void PrintString(char* pszBuffer, unsigned int cbBuffer, const char* pszValue, bool bIsLast = false)
{
    if (pszBuffer != NULL && cbBuffer > 0)
    {
        char szTemp[256] = { 0 };
        if (!bIsLast)
        {
            if (pszValue)
            {
                StringCchPrintfA(szTemp, sizeof(szTemp), "%s,", pszValue);
                strcat_s(pszBuffer, cbBuffer, szTemp);
            }
            else
            {
                StringCchPrintfA(szTemp, sizeof(szTemp), ",", pszValue);
                strcat_s(pszBuffer, cbBuffer, szTemp);
            }
        }
        else
        {
            if (pszValue)
            {
                StringCchPrintfA(szTemp, sizeof(szTemp), "%s\n", pszValue);
                strcat_s(pszBuffer, cbBuffer, szTemp);
            }
            else
            {
                StringCchPrintfA(szTemp, sizeof(szTemp), "\n", pszValue);
                strcat_s(pszBuffer, cbBuffer, szTemp);
            }
        }
    }
}

void PrintInt(char* pszBuffer, unsigned int cbBuffer, int iValue, bool bIsLast = false)
{
    if (pszBuffer != NULL && cbBuffer > 0)
    {
        char szTemp[128] = { 0 };
        if (!bIsLast)
        {
            StringCchPrintfA(szTemp, sizeof(szTemp), "%d,", iValue);
            strcat_s(pszBuffer, cbBuffer, szTemp);
        }
        else
        {
            StringCchPrintfA(szTemp, sizeof(szTemp), "%d\n", iValue);
            strcat_s(pszBuffer, cbBuffer, szTemp);
        }
    }
}

void PrintDouble(char* pszBuffer, unsigned int cbBuffer, double dValue, bool bIsLast = false)
{
    if (pszBuffer != NULL && cbBuffer > 0)
    {
        char szTemp[128] = { 0 };
        if (!bIsLast)
        {
            StringCchPrintfA(szTemp, sizeof(szTemp), "%.20f,", dValue);
            strcat_s(pszBuffer, cbBuffer, szTemp);
        }
        else
        {
            StringCchPrintfA(szTemp, sizeof(szTemp), "%.20f\n", dValue);
            strcat_s(pszBuffer, cbBuffer, szTemp);
        }
    }
}

void CALLBACK MyDispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
    HRESULT hr;
    
    switch(pData->dwID)
    {
        case SIMCONNECT_RECV_ID_EVENT:
        {
            SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;

            switch (evt->uEventID)
            {
                case EVENT_ID_START_HARVEST:
                {
                    // Verify the file has been opened. Notify the user otherwise.
                    printf("\nAttempting to open CSV file...");
                    if (g_pFile)
                    {
                        fclose(g_pFile);
                        g_pFile = NULL;
                    }
                    fopen_s(&g_pFile, OUTPUT_FILE_NAME, "w");
                    if (!g_pFile)
                    {
                        printf("Failed to open \"%s\". Please ensure the file has the correct permissions and is not being used by another application.", OUTPUT_FILE_NAME);
                        break;
                    }
                    printf("\nFile successfully opened!");

                    g_bIsHarvesting = true;

                    printf("\nStarting data harvest...");

                    // Print variable titles into the CSV.
                    PrintHeader();

                    // Perframe data request on the user object.
                    hr = SimConnect_RequestDataOnSimObject(g_hSimConnect, REQUEST_ID_USER_OBJECT_DATA, DEFINITION_ID_USER_OBJECT, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME);

                    // Update menu items.
                    hr = SimConnect_MenuDeleteItem(g_hSimConnect, EVENT_ID_START_HARVEST);
                    hr = SimConnect_MenuAddItem(g_hSimConnect, STOP_STRING, EVENT_ID_STOP_HARVEST, 0);

                    break;
                }
                case EVENT_ID_STOP_HARVEST:
                {
                    // Cancel the data request on the user object.
                    hr = SimConnect_RequestDataOnSimObject(g_hSimConnect, REQUEST_ID_USER_OBJECT_DATA, DEFINITION_ID_USER_OBJECT, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_NEVER);

                    // Update menu items.
                    hr = SimConnect_MenuDeleteItem(g_hSimConnect, EVENT_ID_STOP_HARVEST);
                    hr = SimConnect_MenuAddItem(g_hSimConnect, START_STRING, EVENT_ID_START_HARVEST, 0);

                    // Close the file.
                    if (g_pFile)
                    {
                        fclose(g_pFile);
                        g_pFile = NULL;
                    }

                    printf("\nStopping data harvest.");

                    g_bIsHarvesting = false;

                    break;
                }
                default:
                {
                    break;
                }
            }

            break; // SIMCONNECT_RECV_ID_EVENT
        }

        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
        {
            SIMCONNECT_RECV_SIMOBJECT_DATA* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
            
            switch(pObjData->dwRequestID)
            {
                case REQUEST_ID_USER_OBJECT_DATA:
                {
                    DWORD dwObjectID = pObjData->dwObjectID;

                    ObjectData* pUserData = (ObjectData*)&pObjData->dwData;

                    // Console print a portion of the data.
                    if (SUCCEEDED(StringCbLengthA(&pUserData->szTitle[0], sizeof(pUserData->szTitle), NULL))) // security check
                    {
                        printf("\nData: Time=%f  Lat=%f  Lon=%f  Alt=%f",
                               pUserData->dTime,
                               pUserData->dLatitude,
                               pUserData->dLongitude,
                               pUserData->dAltitude);
                    }

                    if (g_pFile != NULL)
                    {
                        char szBuffer[4096] = { 0 };

                        // Stream the user object's data into the CSV file.
                        PrintString(szBuffer, sizeof(szBuffer), pUserData->szTitle);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dAbsoluteTime);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dTime);
                        PrintInt(szBuffer, sizeof(szBuffer), pUserData->uDayOfTheYear);
                        PrintInt(szBuffer, sizeof(szBuffer), pUserData->uYear);
                        PrintInt(szBuffer, sizeof(szBuffer), pUserData->uMonthOfTheYear);
                        PrintInt(szBuffer, sizeof(szBuffer), pUserData->uDayOfTheMonth);
                        PrintInt(szBuffer, sizeof(szBuffer), pUserData->uDayOfTheWeek);
                        PrintInt(szBuffer, sizeof(szBuffer), pUserData->uTimeZoneOffset);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dSimTime);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dLatitude);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dLongitude);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dAltitude);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dPitch);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dBank);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dHeading);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dVelocityX);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dVelocityY);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dVelocityZ);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dTemperature);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dAirPressure);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dAirDensity);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dWindVelocity);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dWindDirection);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dWindX);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dWindY);
                        PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dWindZ, true);

                        fprintf_s(g_pFile, szBuffer);
                    }

                    break;
                }
                default:
                {
                    break;
                }
            }

            break; // SIMCONNECT_RECV_ID_SIMOBJECT_DATA
        }
        
        case SIMCONNECT_RECV_ID_QUIT:
        {
            g_bQuit = 1;
            break;
        }

        default:
        {
            printf("\nReceived: %d", pData->dwID);
            break;
        }
    }
}

void RunDataHarvester()
{
    HRESULT hr;
    
    bool bConnected = false;

    // Attempt to connect to Prepar3D.
    printf("\nAttempting to connect to Prepar3D...");
    for (unsigned int i = 0; i < 60; ++i)
    {
        if (SUCCEEDED(SimConnect_Open(&g_hSimConnect, TITLE_STRING, NULL, 0, 0, 0)))
        {
            bConnected = true;
            break;
        }

        printf("\nAttempt %d", i + 1);
        Sleep(1000);
    }

    if (bConnected)
    {
        printf("\nConnected to Prepar3D!");

        // Set up the data definition.
        for (unsigned int i = 0; i < ARRAYSIZE(g_aVariables); ++i)
        {
            const PropertyDefinition& prop = g_aVariables[i];
            hr = SimConnect_AddToDataDefinition(g_hSimConnect, DEFINITION_ID_USER_OBJECT, prop.pszName, prop.pszUnits, prop.eDataType);
        }

        // Add the menu item that will start the harvest.
        hr = SimConnect_MenuAddItem(g_hSimConnect, START_STRING, EVENT_ID_START_HARVEST, 0);

        // Application main loop.
        while (0 == g_bQuit)
        {
            SimConnect_CallDispatch(g_hSimConnect, MyDispatchProcRD, NULL);
            Sleep(1);
        }

        // Clean up.
        if (g_bIsHarvesting)
        {
            hr = SimConnect_RequestDataOnSimObject(g_hSimConnect, REQUEST_ID_USER_OBJECT_DATA, DEFINITION_ID_USER_OBJECT, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_NEVER);
            hr = SimConnect_MenuDeleteItem(g_hSimConnect, EVENT_ID_STOP_HARVEST);

            if (g_pFile)
            {
                fclose(g_pFile);
                g_pFile = NULL;
            }
        }
        else
        {
            hr = SimConnect_MenuDeleteItem(g_hSimConnect, EVENT_ID_START_HARVEST);
        }

        // Close.
        hr = SimConnect_Close(g_hSimConnect);
    }
    else
    {
        printf("\nConnection timeout!");
    }
}

int __cdecl _tmain(int argc, _TCHAR* argv[])
{
    RunDataHarvester();
	return 0;
}
