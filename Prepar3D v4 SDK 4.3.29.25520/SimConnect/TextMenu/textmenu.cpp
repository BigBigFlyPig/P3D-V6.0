//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//------------------------------------------------------------------------------
//
//  SimConnect Text Menu sample
//  
//	Description:
//				Ctrl F1 displays a menu on the screen
//				Ctrl F2 removes the menu from the screen
//				Selecting any menu option sends an event and removes the menu
//------------------------------------------------------------------------------

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "SimConnect.h"
#include <strsafe.h>
#include <string>

enum GROUP_ID {
    GROUP0,
};

enum EVENT_ID {
    EVENT1,
    EVENT2,
    EVENT_MENU_1,
    EVENT_TEXT_1,
    EVENT_MESSAGE_1,
    EVENT_TEXT_WINDOW_CREATED,
};

enum INPUT_ID {
    INPUT0,
};

int quit = 0;
HANDLE hSimConnect = NULL;
static const char Empty1[] = "";
static const char Menu1[] = "SimConnect Text Menu\0Choose which item:\0Item #1\0Item #2\0Item #3\0Item #4\0Item #5\0";
static const char Text1[] = "Sample scrolling text.";
static const char Message1[] = "This is a sample message window.\n\nMessage windows can display more than one line.";


const char* MenuText(SIMCONNECT_TEXT_RESULT result)
{
    switch (result)
    {
        case SIMCONNECT_TEXT_RESULT_MENU_SELECT_1:
            return "Item #1 Selected";
            break;
        case SIMCONNECT_TEXT_RESULT_MENU_SELECT_2:
            return "Item #2 Selected";
            break;
        case SIMCONNECT_TEXT_RESULT_MENU_SELECT_3:
            return "Item #3 Selected";
            break;
        case SIMCONNECT_TEXT_RESULT_MENU_SELECT_4:
            return "Item #4 Selected";
            break;
        case SIMCONNECT_TEXT_RESULT_MENU_SELECT_5:
            return "Item #5 Selected";
            break;
        case SIMCONNECT_TEXT_RESULT_MENU_SELECT_6:
            return "Item #6 Selected";
            break;
        case SIMCONNECT_TEXT_RESULT_MENU_SELECT_7:
            return "Item #7 Selected";
            break;
        case SIMCONNECT_TEXT_RESULT_MENU_SELECT_8:
            return "Item #8 Selected";
            break;
        case SIMCONNECT_TEXT_RESULT_MENU_SELECT_9:
            return "Item #9 Selected";
            break;
        case SIMCONNECT_TEXT_RESULT_MENU_SELECT_10:
            return "Item #10 Selected";
            break;
        case SIMCONNECT_TEXT_RESULT_DISPLAYED:
            return "Displayed";
            break;
        case SIMCONNECT_TEXT_RESULT_QUEUED:
            return "Queued";
            break;
        case SIMCONNECT_TEXT_RESULT_REMOVED:
            return "Removed from Queue";
            break;
        case SIMCONNECT_TEXT_RESULT_REPLACED:
            return "Replaced in Queue";
            break;
        case SIMCONNECT_TEXT_RESULT_TIMEOUT:
            return "Timeout";
            break;
        default:
            return "<unknown>";
            break;
    }
}

void CALLBACK MySignalProc(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
    switch (pData->dwID)
    {
        case SIMCONNECT_RECV_ID_OPEN:
        {
            SIMCONNECT_RECV_OPEN *pOpen = (SIMCONNECT_RECV_OPEN*)pData;
            printf("Open: AppName=\"%s\"  AppVersion=%d.%d.%d.%d  SimConnectVersion=%d.%d.%d.%d\n", pOpen->szApplicationName,
                pOpen->dwApplicationVersionMajor, pOpen->dwApplicationVersionMinor, pOpen->dwApplicationBuildMajor, pOpen->dwApplicationBuildMinor,
                pOpen->dwSimConnectVersionMajor, pOpen->dwSimConnectVersionMinor, pOpen->dwSimConnectBuildMajor, pOpen->dwSimConnectBuildMinor
                );
            break;
        }

        case SIMCONNECT_RECV_ID_EVENT:
        {
            SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;

            switch (evt->uEventID)
            {
                case EVENT1:
                    // Display menu
                    SimConnect_Text(hSimConnect, SIMCONNECT_TEXT_TYPE_MENU, 0, EVENT_MENU_1, sizeof(Menu1), (void*)Menu1);
                    SimConnect_Text(hSimConnect, SIMCONNECT_TEXT_TYPE_SCROLL_BLUE, 0, EVENT_TEXT_1, sizeof(Text1), (void*)Text1);
                    SimConnect_Text(hSimConnect, SIMCONNECT_TEXT_TYPE_MESSAGE_WINDOW, 0, EVENT_MESSAGE_1, sizeof(Message1), (void*)Message1);
                    break;

                case EVENT2:
                    // Stop displaying menu
                    SimConnect_Text(hSimConnect, SIMCONNECT_TEXT_TYPE_MENU, 0, EVENT_MENU_1, sizeof(Empty1), (void*)Empty1);
                    SimConnect_Text(hSimConnect, SIMCONNECT_TEXT_TYPE_SCROLL_BLUE, 0, EVENT_TEXT_1, sizeof(Empty1), (void*)Empty1);
                    SimConnect_Text(hSimConnect, SIMCONNECT_TEXT_TYPE_MESSAGE_WINDOW, 0, EVENT_MESSAGE_1, sizeof(Empty1), (void*)Empty1);
                    break;

                case EVENT_MENU_1:
                    // Respond to the users menu selection
                    printf("\n");
                    printf(MenuText((SIMCONNECT_TEXT_RESULT)evt->dwData));
                    break;

                default:
                    printf("\nSIMCONNECT_RECV_EVENT: 0x%08X 0x%08X 0x%X", evt->uEventID, evt->dwData, cbData);
                    break;
            }
            break;
        }

        case SIMCONNECT_RECV_ID_EVENT_TEXT:
        {
            SIMCONNECT_RECV_EVENT_TEXT* textData = (SIMCONNECT_RECV_EVENT_TEXT*)pData;

            if (textData->eTextType == SIMCONNECT_TEXT_TYPE_MENU)
            {
                void* message = textData->rgMessage;
                int remainingSize = textData->dwUnitSize;

                for (int itemPosition = 0; remainingSize > 0; ++itemPosition)
                {
                    int itemLength = strnlen((char*)message, remainingSize) + sizeof(char);

                    if (itemLength <= sizeof(char))
                    {
                        break;
                    }

                    remainingSize -= itemLength;

                    std::string messageItem = reinterpret_cast<char*> (message);

                    switch (itemPosition)
                    {
                    case 0:
                        printf("\nSIMCONNECT_RECV_ID_EVENT_TEXT: SIMCONNECT_TEXT_TYPE_MENU\nTitle: \"%s\"\n", messageItem.c_str());
                        break;
                    case 1:
                        printf("Prompt: \"%s\"\n", messageItem.c_str());
                        break;
                    default:
                        printf("Option: \"%s\"\n", messageItem.c_str());
                    }

                    message = (BYTE*)((UINT_PTR)message + itemLength);
                }
            }
            else
            {
                std::string message = reinterpret_cast<char*> (textData->rgMessage);
                printf("\n\nSIMCONNECT_RECV_ID_EVENT_TEXT: \n\"%s\"\n", message.c_str());
                break;
            }

            break;
        }

        case SIMCONNECT_RECV_ID_QUIT:
        {
            printf("\n***** SIMCONNECT_RECV_ID_QUIT *****\n");
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

    if (hEventHandle == NULL)
    {
        printf("Error: Event creation failed!  Quiting");
        return GetLastError();
    }

    if (FAILED(SimConnect_Open(&hSimConnect, "SimConnect Text", NULL, 0, hEventHandle, 0)))
    {
        printf("\nConnection to Prepar3D failed! Quiting");
        return 1;
    }

    printf("\nConnected to Prepar3D");

    HRESULT hr;

    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT1);
    hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT2);

    hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT1, TRUE);
    hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT2, TRUE);

    hr = SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP0, SIMCONNECT_GROUP_PRIORITY_HIGHEST);

    hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "Ctrl+F1", EVENT1);
    hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "Ctrl+F2", EVENT2);

    hr = SimConnect_SetInputGroupState(hSimConnect, INPUT0, SIMCONNECT_STATE_ON);

    hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_TEXT_WINDOW_CREATED, "TextEventCreated");

    while (0 == quit && ::WaitForSingleObject(hEventHandle, INFINITE) == WAIT_OBJECT_0)
    {
        hr = SimConnect_CallDispatch(hSimConnect, MySignalProc, NULL);
        if (FAILED(hr))
        {
            break;
        }
    }

    hr = SimConnect_Close(hSimConnect);
    CloseHandle(hEventHandle);

    return 0;
}

