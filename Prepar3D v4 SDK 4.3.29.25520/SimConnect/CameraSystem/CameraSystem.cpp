//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//------------------------------------------------------------------------------
//
//  SimConnect Camera System Sample
//  
//  Description:
//              Press the 'E' key to create a camera through SimConnect.
//              After the camera is created use the 'J', 'K', ',', and '.' 
//              keys to modify the camera.
//
//              'J' Undocks the view
//              'K' Docks the view
//              ',' Changes the view's position
//              '.' Changes the view's size
//
//              These controls are also printed to the console when the sample
//              application starts.
//
//------------------------------------------------------------------------------

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "SimConnect.h"
#include <strsafe.h>

// SimConnect globals
static bool G_QUIT = false;
static HANDLE hSimConnect = NULL;
static const int SIMCONNECT_SLEEP_TIME = 1;

// Sample project globals
static const char* PROJECT_NAME = "Camera System";
static const char* CONTROLS = "\n\n\nCamera System Sample\n\nControls:\n'E' - Create Camera\n'J' - Undock view \n'K' - Dock view \n',' - Toggle view position \n'.' - Toggle view size\n";
static const char* CAMERA_NAME = "SimconnectCamera";

// Used to change window position and size values
static const int X1 = 200;
static const int Y1 = 200;
static const int X2 = 50;
static const int Y2 = 50;
static const int W1 = 512;
static const int H1 = 256;
static const int W2 = 256; 
static const int H2 = 512;

// Keep track of current states
static bool WINDOW_POS_TOGGLE = true;
static bool WINDOW_SIZE_TOGGLE = true;
static bool CAMERA_CREATED = false;

enum GROUP_ID
{
    GROUP0
};

enum EVENT_ID
{
    EVENT_CAMERA_CREATE,
    EVENT_CAMERA_UNDOCK,
    EVENT_CAMERA_DOCK,
    EVENT_CAMERA_WINDOW_POS,
    EVENT_CAMERA_WINDOW_SIZE
};

enum DATA_REQUEST_ID {
    REQUEST_ID_0
};
HRESULT CreateCamera()
{
    HRESULT hr;

    SIMCONNECT_DATA_XYZ position;
    position.x = 0;
    position.y = 0;
    position.z = 0;

    SIMCONNECT_DATA_PBH rotation;
    rotation.Pitch = 0;
    rotation.Bank = 0;
    rotation.Heading = 0;

    GUID guid;
    CoCreateGuid(&guid);
    SIMCONNECT_CAMERA_TYPE cameraType = SIMCONNECT_CAMERA_TYPE_OBJECT_CENTER;

    hr = SimConnect_CreateCameraDefinition(hSimConnect, guid, cameraType, CAMERA_NAME, position, rotation);
    if (hr == S_OK)
    {
        hr = SimConnect_CreateCameraInstance(hSimConnect, guid, CAMERA_NAME, SIMCONNECT_OBJECT_ID_USER, REQUEST_ID_0);
        if (hr == S_OK)
        {
            hr = SimConnect_OpenView(hSimConnect, CAMERA_NAME);
            if (hr == S_OK)
            {
                CAMERA_CREATED = true;
            }
        }
    }
    return hr;
}

// Used to cycle between two different positions on the screen
void GetWindowPos(int& x, int&y)
{
    if (WINDOW_POS_TOGGLE)
    {
        x = X1;
        y = Y1;
    }
    else
    {
        x = X2;
        y = Y2;
    }
    WINDOW_POS_TOGGLE = !WINDOW_POS_TOGGLE;
}

// Used to cycle the width and height between two different values
void GetWindowSize(int& width, int& height)
{
    if (WINDOW_SIZE_TOGGLE)
    {
        width = W1;
        height = H1;
    }
    else
    {
        width = W2;
        height = H2;
    }
    WINDOW_SIZE_TOGGLE = !WINDOW_SIZE_TOGGLE;
}

void CALLBACK MyDispatchProcCC(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
    if (!pData)
    {
        printf("SimConnect callback error.");
        return;
    }

    HRESULT hr;

    switch(pData->dwID)
    {
        case SIMCONNECT_RECV_ID_EVENT:
        {
            SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;

            switch(evt->uEventID)
            {
                case EVENT_CAMERA_CREATE:

                    if (!CAMERA_CREATED)
                    {
                        hr = CreateCamera();
                        if (hr == S_OK)
                        {
                            printf("\nCamera Created");
                        }
                        else
                        {
                            printf("\nSimConnect error - Create Camera");
                        }
                    }

                    break;

                case EVENT_CAMERA_UNDOCK:

                    if (CAMERA_CREATED)
                    {
                        hr = SimConnect_UndockView(hSimConnect, CAMERA_NAME);
                        if (hr == S_OK)
                        {
                            printf("\nView Undocked");
                        }
                        else
                        {
                            printf("\nSimConnect error - Undock Camera");
                        }
                    }
                    else
                    {
                        printf("\nCreate camera first! (press 'E')");
                    }

                    break;
                    
                case EVENT_CAMERA_DOCK:

                    if (CAMERA_CREATED)
                    {
                        hr = SimConnect_DockView(hSimConnect, CAMERA_NAME);
                        if (hr == S_OK)
                        {
                            printf("\nView Docked");
                        }
                        else
                        {
                            printf("\nSimConnect error - Dock Camera");
                        }
                    }
                    else
                    {
                        printf("\nCreate camera first! (press 'E')");
                    }

                    break;
                    
                case EVENT_CAMERA_WINDOW_POS:

                    if (CAMERA_CREATED)
                    {
                        int x;
                        int y;
                        GetWindowPos(x, y);
                        hr = SimConnect_SetCameraWindowPosition(hSimConnect, CAMERA_NAME, x, y);
                        if (hr == S_OK)
                        {
                            printf("\nWindow position updated");
                        }
                        else
                        {
                            printf("\nSimConnect error - Window Position");
                        }
                    }
                    else
                    {
                        printf("\nCreate camera first! (press 'E')");
                    }

                    break;

                case EVENT_CAMERA_WINDOW_SIZE:

                    if (CAMERA_CREATED)
                    {
                        int width;
                        int height;
                        GetWindowSize(width, height);
                        hr = SimConnect_SetCameraWindowSize(hSimConnect, CAMERA_NAME, width, height);
                        if (hr == S_OK)
                        {
                            printf("\nWindow size updated");
                        }
                        else
                        {
                            printf("\nSimConnect error - Window Size");
                        }
                    }
                    else
                    {
                        printf("\nCreate camera first! (press 'E')");
                    }

                    break;

                default:

                    break;
            }

            break;
        }

        case SIMCONNECT_RECV_ID_EXCEPTION:
        {
            SIMCONNECT_RECV_EXCEPTION *except = (SIMCONNECT_RECV_EXCEPTION*)pData;
            
            switch (except->dwException)
            {
                case SIMCONNECT_EXCEPTION_ERROR:

                    printf("\nCamera error");
                    break;

                default:

                    printf("\nException");
                    break;
            }
            break;
        }

        case SIMCONNECT_RECV_ID_QUIT:
        {
            G_QUIT = true;
            break;
        }

        default:
            break;
    }
}

void TestCameraSystem()
{
    HRESULT hr;

    if (SUCCEEDED(SimConnect_Open(&hSimConnect, PROJECT_NAME, NULL, 0, 0, 0)))
    {
        printf("\nConnected to Prepar3D!");   
        
        // Define private events
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CAMERA_CREATE);
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CAMERA_UNDOCK);
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CAMERA_DOCK);
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CAMERA_WINDOW_POS);
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CAMERA_WINDOW_SIZE);

        // Group inputs
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_CAMERA_CREATE);
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_CAMERA_UNDOCK);
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_CAMERA_DOCK);
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_CAMERA_WINDOW_POS);
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_CAMERA_WINDOW_SIZE);

        // Set priorities
        hr = SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP0, SIMCONNECT_GROUP_PRIORITY_HIGHEST);

        // Map the keys , and . keys to the private events
        hr = SimConnect_MapInputEventToClientEvent(hSimConnect, GROUP0, "E", EVENT_CAMERA_CREATE);
        hr = SimConnect_MapInputEventToClientEvent(hSimConnect, GROUP0, "J", EVENT_CAMERA_UNDOCK);
        hr = SimConnect_MapInputEventToClientEvent(hSimConnect, GROUP0, "K", EVENT_CAMERA_DOCK);
        hr = SimConnect_MapInputEventToClientEvent(hSimConnect, GROUP0, "VK_COMMA", EVENT_CAMERA_WINDOW_POS);
        hr = SimConnect_MapInputEventToClientEvent(hSimConnect, GROUP0, "VK_PERIOD", EVENT_CAMERA_WINDOW_SIZE);

        // Set group 0 to be active on startup
        hr = SimConnect_SetInputGroupState(hSimConnect, GROUP0, SIMCONNECT_STATE_ON);
        
        // Show controls to user
        printf(CONTROLS);

        while (!G_QUIT)
        {
            SimConnect_CallDispatch(hSimConnect, MyDispatchProcCC, NULL);
            Sleep(SIMCONNECT_SLEEP_TIME);
        } 

        hr = SimConnect_Close(hSimConnect);
    }
    else
    {
        printf("\nFailed to Connect");
    }
}

int __cdecl _tmain(int argc, _TCHAR* argv[])
{
    TestCameraSystem();

    return 0;
}
