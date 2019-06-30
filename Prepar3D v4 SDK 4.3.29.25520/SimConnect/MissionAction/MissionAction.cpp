//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//------------------------------------------------------------------------------
//
//  SimConnect Mission Action Sample
//  
//	Description:
//				Link SimConnect client with a Mission.
//------------------------------------------------------------------------------

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "SimConnect.h"
#include <strsafe.h>

int     quit = 0;
HANDLE  hSimConnect = NULL;

// Need copies of all the GUIDs used in the mission file
// See "Mission Action Sample" add-on in the project's source directory.

// {B9566244-7047-4C72-BD57-13AE4FBE4FEC}
static const GUID g_guidCustomAction1 =
{ 0xb9566244, 0x7047, 0x4c72,{ 0xbd, 0x57, 0x13, 0xae, 0x4f, 0xbe, 0x4f, 0xec } };

// {031152BB-2F7A-471F-BB10-C67BEB5D1D68}
static const GUID g_guidCustomAction2 =
{ 0x31152bb, 0x2f7a, 0x471f,{ 0xbb, 0x10, 0xc6, 0x7b, 0xeb, 0x5d, 0x1d, 0x68 } };

// {593F90B3-EAA3-4C1B-A863-145CAFE7C3B4}
static const GUID g_guidMissionAction1 =
{ 0x593f90b3, 0xeaa3, 0x4c1b,{ 0xa8, 0x63, 0x14, 0x5c, 0xaf, 0xe7, 0xc3, 0xb4 } };

// {E863C0ED-AE38-474B-80C8-4BE46EB14B5A}
static const GUID g_guidMissionAction2 =
{ 0xe863c0ed, 0xae38, 0x474b,{ 0x80, 0xc8, 0x4b, 0xe4, 0x6e, 0xb1, 0x4b, 0x5a } };


enum EVENT_ID {
	EVENT_MISSION_ACTION,
	EVENT_MISSION_COMPLETED
};

void CALLBACK MyDispatchProcMA(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
	HRESULT hr;

    switch(pData->dwID)
    {
        case SIMCONNECT_RECV_ID_EVENT:
        {
            SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;

			switch(evt->uEventID)
            {
                case EVENT_MISSION_COMPLETED:

					printf("\nMission completed, result = %d", evt->dwData);

					// Perform any additional processing to complete the mission
					// Perhaps writing out a status file that can be read in by another
					// mission.

                    break;

                default:
                   break;
            }
            break;
        }
        
		case SIMCONNECT_RECV_ID_CUSTOM_ACTION:
        {
			SIMCONNECT_RECV_CUSTOM_ACTION *pCustomAction = (SIMCONNECT_RECV_CUSTOM_ACTION *)pData;
 
			if (pCustomAction->guidInstanceId == g_guidCustomAction1)
			{
				printf("\nCustom Action 1, payload: %s", pCustomAction->szPayLoad);

				// Custom actions can include calls to actions within the mission xml file, though
				// if this is done we cannot know if the actions have been completed within this
				// section of code (the actions may initiate triggers and it may be some time
				// before the sequence is ended).

				hr = SimConnect_ExecuteMissionAction(hSimConnect, g_guidMissionAction1);
				hr = SimConnect_ExecuteMissionAction(hSimConnect, g_guidMissionAction2);
			
			}
            else if (pCustomAction->guidInstanceId == g_guidCustomAction2)
			{
				printf("\nCustom Action 2, payload: %s", pCustomAction->szPayLoad);

				// This action simply notifies the Mission system that the first action
				// is complete
				hr = SimConnect_CompleteCustomMissionAction(hSimConnect, g_guidCustomAction1);

			}
            else
			{
				printf("\nUnknown custom action: %p", pCustomAction->guidInstanceId);
			}

			break;
		}

        case SIMCONNECT_RECV_ID_QUIT:
        {
            quit = 1;
            break;
        }

        default:
            break;
    }
}

void testMissionAction()
{
    HRESULT hr;

    HANDLE hEventHandle = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    
	if(hEventHandle == NULL)
    {
        printf("Error: Event creation failed!");
        return;
    }
    
	if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Mission Action", NULL, 0, hEventHandle, 0)))
    {
        printf("\nConnected to Prepar3D!");   

		// Subscribe to the mission completed event
        hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_MISSION_COMPLETED, "MissionCompleted");
        
		// Subscribe to a notification when a custom action executes
		hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_MISSION_ACTION, "CustomMissionActionExecuted");
 
		// Check for messages only when a Windows event has been received

		while( 0 == quit && ::WaitForSingleObject(hEventHandle, INFINITE) == WAIT_OBJECT_0)
        {
            SimConnect_CallDispatch(hSimConnect, MyDispatchProcMA, NULL);
            Sleep(1);
        }

		CloseHandle(hEventHandle);
        
		hr = SimConnect_Close(hSimConnect);
    }
}

int __cdecl _tmain(int argc, _TCHAR* argv[])
{
	testMissionAction();

	return 0;
}
