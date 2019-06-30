#include <windows.h>
#include "SimConnect.h"
#include <stdio.h>

HANDLE hSimConnect;
enum EVENTS
{
	EVENT_SIM_PAUSED,
	EVENT_SIM_START,
	EVENT_SIM_STOP
};

enum PAUSESTATE
{
	UNPAUSED=0,
	PAUSED
};

bool RUNSIMCONNECT;

void CALLBACK Dispatch(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext);

int main (int argc, char* argv)
{
	HRESULT hr;
	RUNSIMCONNECT=true;
	if(SUCCEEDED(SimConnect_Open(&hSimConnect, "PauseMonitor", NULL, 0, 0, 0)))
	{
		printf("Connected to SimConnect!\n");
		
		hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_PAUSED, "Pause");
		hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");
		hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_STOP, "SimStop");

		while(RUNSIMCONNECT)
		{
			SimConnect_CallDispatch(hSimConnect, Dispatch, NULL);
			Sleep(1);
		}

		printf("Closing connection to SimConnect\n");
		hr = SimConnect_Close(hSimConnect);

	}
	else
		printf("Failed to connect to SimConnect!\n");

}

void CALLBACK Dispatch(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext)
{
	switch(pData->dwID)
	{
		case SIMCONNECT_RECV_ID_EVENT:
		{
			SIMCONNECT_RECV_EVENT* myEvent = (SIMCONNECT_RECV_EVENT*)pData;
			switch(myEvent->uEventID)
			{
				case EVENT_SIM_START:
					printf("SimConnect SimStart has occurred\n");
				break;

				case EVENT_SIM_PAUSED:
					printf("SimConnect believes the simulation is currently ");
					switch(myEvent->dwData)
					{
						case UNPAUSED:
							printf("UNPAUSED\n");
						break;
						
						case PAUSED:
							printf("PAUSED\n");
						break;
					}
				break;

				case EVENT_SIM_STOP:
					printf("SimConnect SimStop has occurred\n");
				break;
			}
		}
		break;

		case SIMCONNECT_RECV_ID_QUIT:
		{
			printf("SimConnect Quit Received\n");
			RUNSIMCONNECT=false;
		}
		break;
	}

}