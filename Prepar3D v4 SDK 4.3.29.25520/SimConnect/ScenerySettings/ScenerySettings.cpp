//Copyright (c) Lockheed Martin Corporation.  All rights reserved.  
//------------------------------------------------------------------------------
//
//  SimConnect Scenery Complexity and Shadow Flag sample
//
//  Description:
//              If the key combination U+C is typed, a request is sent to request
//              the current scenery complexity.
//
//              If the key combination U+F is typed, a request is sent to request
//              the current shadow flags.
//------------------------------------------------------------------------------

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "SimConnect.h"
#include <strsafe.h>

int     quit = 0;
HANDLE  hSimConnect = NULL;



enum GROUP_ID {
    GROUP0,
};

enum EVENT_ID {
    EVENT0,
    EVENT1,
};

enum INPUT_ID {
    INPUT0,
};

enum REQUEST_ID {
    REQUEST0,
    REQUEST1,
};


void CALLBACK MyDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
    HRESULT hr;

    switch(pData->dwID)
    {
        case SIMCONNECT_RECV_ID_EVENT:
        {
            SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;

            switch(evt->uEventID)
            {
                case EVENT0:
                    printf("\nEVENT0: %d", evt->dwData);

                    // Send a request for the scenery complexity
                    hr = SimConnect_RequestSceneryComplexity( hSimConnect, REQUEST0);
                    break;

                case EVENT1:
                    printf("\nEVENT1: %d", evt->dwData);

                    // Send a request for the shadow flags
                    hr = SimConnect_RequestShadowFlags( hSimConnect, REQUEST1);
                    break;

                default:
                    printf("\nSIMCONNECT_RECV_EVENT: 0x%08X 0x%08X 0x%X", evt->uEventID, evt->dwData, cbData);
                    break;
            }
            break;
        }

        case SIMCONNECT_RECV_ID_SCENERY_COMPLEXITY:
        {
            SIMCONNECT_RECV_SCENERY_COMPLEXITY *pComplexity = (SIMCONNECT_RECV_SCENERY_COMPLEXITY*)pData;

            printf("\nSIMCONNECT_RECV_SCENERY_COMPLEXITY RequestID=%d  dwSceneryComplexity=%d\n", pComplexity->dwRequestID, pComplexity->dwSceneryComplexity);
            break;
        }

        case SIMCONNECT_RECV_ID_SHADOW_FLAGS:
        {
            SIMCONNECT_RECV_SHADOW_FLAGS *pShadows = (SIMCONNECT_RECV_SHADOW_FLAGS*)pData;

            printf("\nSIMCONNECT_RECV_SHADOW_FLAGS RequestID=%d  dwShadowFlags=%d\n", pShadows->dwRequestID, pShadows->dwShadowFlags);
            break;
        }

        case SIMCONNECT_RECV_ID_QUIT:
        {
            quit = 1;
            break;
        }

        case SIMCONNECT_RECV_ID_EXCEPTION:
        {
            SIMCONNECT_RECV_EXCEPTION *except = (SIMCONNECT_RECV_EXCEPTION*)pData;
            printf("\n\n***** EXCEPTION=%d  SendID=%d  uOffset=%d  cbData=%d\n", except->dwException, except->dwSendID, except->dwIndex, cbData);
            break;
        }

        default:
            printf("\nUNKNOWN DATA RECEIVED: pData=%p cbData=%d\n", pData, cbData);
            break;
    }
}

//------------------------------------------------------------------------------
//  main
//------------------------------------------------------------------------------
int __cdecl main(int argc, char* argv[])
{
    HANDLE hEventHandle = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    if(hEventHandle == NULL)
    {
        printf("Error: Event creation failed!  Bailing");
        return 1;
    }

    if (FAILED(SimConnect_Open(&hSimConnect, "Scenery Complexity and Shadow Flag Sample", NULL, 0, hEventHandle, 0)))
    {
        printf("\nConnection to Prepar3D failed!");

    } else
    {
        printf("\nConnected to Prepar3D!");

        HRESULT hr;

        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT0);
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT1);
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT0);
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT1);
        hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "U+C", EVENT0);
        hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "U+F", EVENT1);
        hr = SimConnect_SetInputGroupState(hSimConnect, INPUT0, SIMCONNECT_STATE_ON);

        while( 0 == quit && hr==S_OK && ::WaitForSingleObject(hEventHandle, INFINITE) == WAIT_OBJECT_0)
        {
            hr = SimConnect_CallDispatch(hSimConnect, MyDispatchProc, NULL);
        }
        hr = SimConnect_Close(hSimConnect);
        CloseHandle(hEventHandle);
    }

    return 0;
}
