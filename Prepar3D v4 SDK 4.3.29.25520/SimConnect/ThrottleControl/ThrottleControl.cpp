//Copyright (c) Lockheed Martin Corporation.  All rights reserved.  
//------------------------------------------------------------------------------
//
//  SimConnect	Throttle Control sample
// 
//	Description:
//				Press 1 to increase the throttle
//				Press 2 to decrease the throttle
//------------------------------------------------------------------------------

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "SimConnect.h"
#include <strsafe.h>

int     quit = 0;
HANDLE  hSimConnect = NULL;

enum GROUP_ID{
    GROUP_KEYS,
};
enum INPUT_ID {
    INPUT_KEYS,
};

enum EVENT_ID {
    EVENT_SIM_START,
    EVENT_1,
	EVENT_2
};

enum DATA_DEFINE_ID {
    DEFINITION_THROTTLE,
};

enum DATA_REQUEST_ID {
    REQUEST_THROTTLE,
};

struct structThrottleControl 
{
	double throttlePercent;
};

structThrottleControl		tc;

void CALLBACK MyDispatchProcTC(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
    HRESULT hr;
    
    switch(pData->dwID)
    {
        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
        {
            SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
            
            switch(pObjData->dwRequestID)
            {
                case REQUEST_THROTTLE:
                {
					// Read and set the initial throttle control value
					structThrottleControl *pS = (structThrottleControl*)&pObjData->dwData;

					tc.throttlePercent	= pS->throttlePercent;
					
					printf("\nREQUEST_USERID received, throttle = %2.1f", pS->throttlePercent);

					// Now turn the input events on
					hr = SimConnect_SetInputGroupState(hSimConnect, INPUT_KEYS, SIMCONNECT_STATE_ON);
                }

                default:
                   break;
            }
            break;
        }

        case SIMCONNECT_RECV_ID_EVENT:
        {
            SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;

            switch(evt->uEventID)
            {

		        case EVENT_SIM_START:
                    {
			            // Send this request to get the user aircraft id
		                hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST_THROTTLE, DEFINITION_THROTTLE, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
                    }
			        break;
						
			    case EVENT_1:
                    {
					    // Increase the throttle
					    if (tc.throttlePercent <= 95.0f)
						    tc.throttlePercent	+= 5.0f;

					    hr = SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_THROTTLE, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(tc), &tc);
                    }
                    break;

                case EVENT_2:
                    {
					    // Decrease the throttle
					    if (tc.throttlePercent >= 5.0f)
						    tc.throttlePercent	-= 5.0f;

					    hr = SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_THROTTLE, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(tc), &tc);
                    }
                    break;

                default:
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

void testThrottleControl()
{
    HRESULT hr;

    if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Throttle Control", NULL, 0, 0, 0)))
    {
        printf("\nConnected to Prepar3D!");   
           
        // Set up a data definition for the throttle control
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_THROTTLE,
			"GENERAL ENG THROTTLE LEVER POSITION:1", "percent");

        // Request a simulation started event
        hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");

        // Create two private key events to control the throttle
		hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_1);
		hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_2);

        // Link the events to some keyboard keys
        hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT_KEYS, "1", EVENT_1);
        hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT_KEYS, "2", EVENT_2);

		// Ensure the input events are off until the sim is up and running
        hr = SimConnect_SetInputGroupState(hSimConnect, INPUT_KEYS, SIMCONNECT_STATE_OFF);

        // Sign up for notifications
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_KEYS, EVENT_1);
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_KEYS, EVENT_2);

        // Set a high priority for the group
        hr = SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP_KEYS, SIMCONNECT_GROUP_PRIORITY_HIGHEST);
    
        while( 0 == quit )
        {
            SimConnect_CallDispatch(hSimConnect, MyDispatchProcTC, NULL);
            Sleep(1);
        } 

        hr = SimConnect_Close(hSimConnect);
    }
}

int __cdecl _tmain(int argc, _TCHAR* argv[])
{
    testThrottleControl();

	return 0;
}
