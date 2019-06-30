//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//------------------------------------------------------------------------------
//
//  SimConnect AI ATC Aircraft sample
//  
//	Description:
//				Adds AI aircraft to make the flight from Yakima to Spokane busy.
//				First start the user aircraft at Yakima (or load the Yakima to Spokane
//				flight plan used by the AI aircraft - then drive off the runway to view
//				the goings on).
//				Press the Z key to add six AI aircraft
//				Press the X key to give the parked aircraft the Yakima to Spokane
//				flight plan
//				Both keys can only work once.
//------------------------------------------------------------------------------

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "SimConnect.h"
#include <strsafe.h>

int     quit = 0;
HANDLE  hSimConnect = NULL;
DWORD   ParkedMauleID = SIMCONNECT_OBJECT_ID_USER;
DWORD   ParkedMooneyID = SIMCONNECT_OBJECT_ID_USER;

enum EVENT_ID {
    EVENT_SIM_START,
    EVENT_Z,
    EVENT_X,
    EVENT_ADDED_AIRCRAFT,
    EVENT_REMOVED_AIRCRAFT,
};

enum DATA_REQUEST_ID7 {
    REQUEST_MOONEY1,
    REQUEST_MAULE1,
    REQUEST_MOONEY2,
    REQUEST_MAULE2,
    REQUEST_MAULE_PARKED,
    REQUEST_MOONEY_PARKED,
    REQUEST_MAULE_PARKED_FLIGHTPLAN,
    REQUEST_MOONEY_PARKED_FLIGHTPLAN,
};

enum GROUP_ID {
    GROUP_ZX,
};

enum INPUT_ID {
    INPUT_ZX,
};

// Set up flags so these operations only happen once
static bool plansSent       = false;
static bool aircraftCreated = false;

void sendFlightPlans()
{
    HRESULT hr;
    
    if (ParkedMauleID != SIMCONNECT_OBJECT_ID_USER)
    {
        hr = SimConnect_AISetAircraftFlightPlan(hSimConnect, ParkedMauleID,
            "IFR Yakima Air Term Mcallister to Spokane Intl", REQUEST_MAULE_PARKED_FLIGHTPLAN);
    }
    if (ParkedMooneyID != SIMCONNECT_OBJECT_ID_USER)
    {
        hr = SimConnect_AISetAircraftFlightPlan(hSimConnect, ParkedMooneyID,
            "IFR Yakima Air Term Mcallister to Spokane Intl", REQUEST_MOONEY_PARKED_FLIGHTPLAN);
    }
}


void setUpAIAircraft()
{
    HRESULT hr;

    // Add some AI controlled aircraft
    hr = SimConnect_AICreateEnrouteATCAircraft(hSimConnect, "Mooney Bravo Retro", "N100", 100,
        "IFR Yakima Air Term Mcallister to Spokane Intl", 0.0f, false, REQUEST_MOONEY1);

    hr = SimConnect_AICreateEnrouteATCAircraft(hSimConnect, "Maule M7 260C paint4", "N101", 101,
        "IFR Yakima Air Term Mcallister to Spokane Intl", 0.0f, false, REQUEST_MAULE1);

    hr = SimConnect_AICreateEnrouteATCAircraft(hSimConnect, "Mooney Bravo", "N200", 200,
        "IFR Yakima Air Term Mcallister to Spokane Intl", 0.0f, false, REQUEST_MOONEY2);
    
    hr = SimConnect_AICreateEnrouteATCAircraft(hSimConnect, "Maule M7 260C paint4", "N201", 201,
        "IFR Yakima Air Term Mcallister to Spokane Intl", 0.0f, false, REQUEST_MAULE2);

    // Park a few aircraft
    hr = SimConnect_AICreateParkedATCAircraft(hSimConnect, "Maule M7 260C paint3", "N102",
        "KYKM", REQUEST_MAULE_PARKED);

    hr = SimConnect_AICreateParkedATCAircraft(hSimConnect, "Mooney Bravo", "N202",
        "KYKM", REQUEST_MOONEY_PARKED);
}

void CALLBACK MyDispatchProcAI(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{   
    HRESULT hr;

    switch(pData->dwID)
    {
        case SIMCONNECT_RECV_ID_EVENT:
        {
            SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;

            switch(evt->uEventID)
            {
                case EVENT_SIM_START:

                    // Sim has started so turn input events on
                    hr = SimConnect_SetInputGroupState(hSimConnect, INPUT_ZX, SIMCONNECT_STATE_ON);
                    break;

                case EVENT_Z:
                    if (!aircraftCreated)
                    {
                        setUpAIAircraft();
                        aircraftCreated = true;
                    }
                    break;

                case EVENT_X:
                    if (!plansSent && aircraftCreated)
                    {
                        sendFlightPlans();
                        plansSent = true;
                    }
                    break;

                default:
                    printf("\nUnknown event: %d", evt->uEventID);
                    break;
            }
            break;
        }
        
        case SIMCONNECT_RECV_ID_EVENT_OBJECT_ADDREMOVE:
        {
            SIMCONNECT_RECV_EVENT_OBJECT_ADDREMOVE *evt = (SIMCONNECT_RECV_EVENT_OBJECT_ADDREMOVE*)pData;
            
            switch(evt->uEventID)
            {
                case EVENT_ADDED_AIRCRAFT:
                    printf("\nAI object added: Type=%d, ObjectID=%d", evt->eObjType, evt->dwData);
                    break;

                case EVENT_REMOVED_AIRCRAFT:
                    printf("\nAI object removed: Type=%d, ObjectID=%d", evt->eObjType, evt->dwData);
                    break;
            }
            break;
        }

        case SIMCONNECT_RECV_ID_ASSIGNED_OBJECT_ID:
        {
            SIMCONNECT_RECV_ASSIGNED_OBJECT_ID *pObjData = (SIMCONNECT_RECV_ASSIGNED_OBJECT_ID*)pData;
    
            switch( pObjData ->dwRequestID)
            {
                // Do nothing specific in these cases, as the aircraft already have their flight plans
            
            case REQUEST_MOONEY1:
                printf("\nCreated Mooney Bravo id = %d", pObjData->dwObjectID);
                break;

            case REQUEST_MAULE1:
                printf("\nCreated Maule M7 id = %d", pObjData->dwObjectID);
                break;

            case REQUEST_MOONEY2:
                printf("\nCreated Mooney Bravo id = %d", pObjData->dwObjectID);
                break;

            case REQUEST_MAULE2:
                printf("\nCreated Maule M7 id = %d", pObjData->dwObjectID);
                break;

            case REQUEST_MAULE_PARKED:

                // Record the object ID, so the flightplan can be sent out later
                ParkedMauleID = pObjData ->dwObjectID;

                printf("\nCreated parked Maule M7 %d", pObjData->dwObjectID);

                break;

            case REQUEST_MOONEY_PARKED:

                // Record the object ID, so the flightplan can be sent out later
                ParkedMooneyID = pObjData ->dwObjectID;

                printf("\nCreated parked Mooney %d", pObjData->dwObjectID);
                
                break;

            default:
                printf("\nUnknown creation %d", pObjData->dwRequestID);
                break;

            }
            break;
        }

        case SIMCONNECT_RECV_ID_QUIT:
        {
            quit = 1;
            break;
        }

        default:
            printf("\nReceived:%d",pData->dwID);
            break;
    }
}

void testAIAircraft()
{
    HRESULT hr;

    if (SUCCEEDED(SimConnect_Open(&hSimConnect, "AI Traffic", NULL, 0, 0, 0)))
    {
        printf("\nConnected to Prepar3D!");   
          
        // Create some private events
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_Z);
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_X);

        // Link the private events to keyboard keys, and ensure input events are off
        hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT_ZX, "Z", EVENT_Z);
        hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT_ZX, "X", EVENT_X);

        hr = SimConnect_SetInputGroupState(hSimConnect, INPUT_ZX, SIMCONNECT_STATE_OFF);

        // Sign up for notifications
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_ZX, EVENT_Z);
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_ZX, EVENT_X);
        
        // Request a simulation start event
        hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");

        // Subscribe to system events notifying the client that objects have been added or removed
        hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_ADDED_AIRCRAFT, "ObjectAdded");
        hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_REMOVED_AIRCRAFT, "ObjectRemoved");

        while( 0 == quit )
        {
            SimConnect_CallDispatch(hSimConnect, MyDispatchProcAI, NULL);
            Sleep(1);
        } 

        hr = SimConnect_Close(hSimConnect);
    }
}


int __cdecl _tmain(int argc, _TCHAR* argv[])
{
    testAIAircraft();
    return 0;
}