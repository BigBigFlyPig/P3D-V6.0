

//#define __RE__
#ifdef __RE__



#include <windows.h> 
#include <tchar.h>
#include <stdio.h>
#include <WinDef.h> 
#include "SimConnect.h"
#include <iostream>
#include "PN_C.h"



using namespace std;


int     quit = 0;
HANDLE  hSimConnect = NULL;

float datalist[59 * 6];

static enum EVENT_ID {
	EVENT_SIM_START,
	EVENT_Z,
	EVENT_X,
};
int useindlength = 15;

int useind[15] = {
	0,7,8,9,10,11,12,13,14,15,16,36,37,38,39

};

static enum DATA_REQUEST_ID7 {

	REQUEST_HIP,
	REQUEST_SPINE,
	REQUEST_SPINE1,
	REQUEST_SPINE2,
	REQUEST_SPINE3,
	REQUEST_NECK,
	REQUEST_RIGHTSHOULDER,
	REQUEST_RIGHTFOREARM,
	REQUEST_RIGHTHAND,
	REQUEST_LEFTSHOULDER,
	REQUEST_LEFTFOREARM,
	REQUEST_LEFTHAND,
	REQUEST_LEFTARM,
	REQUEST_RIGHTARM,
	REQUEST_HEAD,

};

static enum GROUP_ID {
	GROUP_ZX,
};

static enum INPUT_ID {
	INPUT_ZX,
};


// Set up flags so these operations only happen once 

static bool objectsCreated = false;

class AttachData {
public:
	int fatherptr;
	DWORD ID;
	SIMCONNECT_DATA_XYZ attachxyz[2];
	SIMCONNECT_DATA_PBH attachpbh[2];
	AttachData()
	{
		fatherptr = -1;
		ID = SIMCONNECT_OBJECT_ID_USER;
	}
	void setID(DWORD id)
	{
		this->ID = id;
	}
	bool isCreated()
	{
		if (ID == SIMCONNECT_OBJECT_ID_USER)
			return false;
		else
			return true;
	}
};


AttachData BoneList[60];


void initFaPtr()
{

	BoneList[0].fatherptr = 59;
	BoneList[7].fatherptr = 0;
	BoneList[8].fatherptr = 7;
	BoneList[9].fatherptr = 8;
	BoneList[10].fatherptr = 9;
	BoneList[11].fatherptr = 10;
	BoneList[12].fatherptr = 11;

	BoneList[13].fatherptr = 11;
	BoneList[14].fatherptr = 13;
	BoneList[15].fatherptr = 14;
	BoneList[16].fatherptr = 15;


	BoneList[36].fatherptr = 11;
	//BoneList[37].fatherptr = 36;
	BoneList[37].fatherptr = 0;
	//
	BoneList[38].fatherptr = 37;
	BoneList[39].fatherptr = 38;

	BoneList[59].ID = SIMCONNECT_OBJECT_ID_USER;
}

void setUpSimObjects()
{
	SIMCONNECT_DATA_INITPOSITION Init;
	HRESULT hr;

	Init.Altitude = 433.0;                // Altitude of Sea-tac is 433 feet 
	Init.Latitude = 47 + (25.91 / 60);        // Convert from 47 25.90 N 
	Init.Longitude = -122 - (18.47 / 60);    // Convert from 122 18.48 W 
	Init.Pitch = 0.0;
	Init.Bank = 0.0;
	Init.Heading = 360.0;
	Init.OnGround = 1;
	Init.Airspeed = 0;

	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "yz10", Init, REQUEST_HIP);

	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "yz10", Init, REQUEST_SPINE);
	/*
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_4", Init, REQUEST_SPINE1);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_4", Init, REQUEST_SPINE2);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_4", Init, REQUEST_SPINE3);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_4", Init, REQUEST_RIGHTSHOULDER);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_4", Init, REQUEST_RIGHTFOREARM);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_4", Init, REQUEST_RIGHTHAND);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_4", Init, REQUEST_LEFTSHOULDER);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_4", Init, REQUEST_LEFTFOREARM);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_4", Init, REQUEST_LEFTHAND);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_4", Init, REQUEST_HEAD);
	*/
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_4", Init, REQUEST_NECK);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_4", Init, REQUEST_RIGHTARM);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_4", Init, REQUEST_LEFTARM);


	//VEH_jetTruck Left_Finger_4
}

void CALLBACK MyDispatchProcSO(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
	HRESULT hr;

	switch (pData->dwID)
	{
	case SIMCONNECT_RECV_ID_EVENT:
	{
		SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;

		switch (evt->uEventID)
		{

		case EVENT_Z:
			if (!objectsCreated)
			{
				setUpSimObjects();
				objectsCreated = true;
			}
			break;

		default:
			printf("\nUnknown event: %d", evt->uEventID);
			break;
		}
		break;
	}

	case SIMCONNECT_RECV_ID_ASSIGNED_OBJECT_ID:
	{
		SIMCONNECT_RECV_ASSIGNED_OBJECT_ID *pObjData = (SIMCONNECT_RECV_ASSIGNED_OBJECT_ID*)pData;

		switch (pObjData->dwRequestID)
		{

		case REQUEST_HIP:
			BoneList[0].setID(pObjData->dwObjectID);
			printf("\nCreated hip id = %d");
			break;
		case REQUEST_NECK:
			BoneList[11].setID(pObjData->dwObjectID);
			printf("\nCreated neck id = %d");
			break;
		case REQUEST_LEFTARM:
			BoneList[37].setID(pObjData->dwObjectID);
			printf("\nCreated leftarm id = %d");
			break;
		case REQUEST_RIGHTARM:
			BoneList[14].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_SPINE:
			BoneList[7].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_SPINE1:
			BoneList[8].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_SPINE2:
			BoneList[9].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_SPINE3:
			BoneList[10].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_RIGHTSHOULDER:
			BoneList[13].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_LEFTSHOULDER:
			BoneList[36].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_RIGHTFOREARM:
			BoneList[15].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_LEFTFOREARM:
			BoneList[38].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_LEFTHAND:
			BoneList[39].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_RIGHTHAND:
			BoneList[16].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_HEAD:
			BoneList[12].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
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
		printf("\nReceived:%d", pData->dwID);
		break;
	}
}
void releaseBone()
{
	//SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[0].ID);
	//SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, BoneList[BoneList[37].fatherptr].ID, BoneList[37].ID);
	SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, BoneList[0].ID, BoneList[7].ID);
	SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[0].ID);
	/*
	int i = 0;
	for (int j = 0; j < useindlength; j++)
	{
		i = useind[j];
		SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, BoneList[BoneList[i].fatherptr].ID, BoneList[i].ID);
	}
	*/
	//for (int i = 0; i < 59; i++)
	//{
		// || (i == 37)

		//if ((i == 0) || (i == 37))
		//{
		//	if (i == 0)
		//		SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[0].ID);
			//else
			//	SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, BoneList[BoneList[i].fatherptr].ID, BoneList[i].ID);
		//}


		//if (BoneList[i].fatherptr != -1)
		//{
		//	SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, BoneList[BoneList[i].fatherptr].ID, BoneList[i].ID);
		//}
	//}

}
void attachBone()
{
	SIMCONNECT_DATA_PBH tem;
	tem.Heading = 90.0;
	tem.Bank = 45;
	tem.Pitch = 45;
	SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[0].attachxyz[0], BoneList[0].attachpbh[0], BoneList[0].ID, BoneList[0].attachxyz[1], BoneList[7].attachpbh[1]);
	SimConnect_AttachSimObjectToSimObject(hSimConnect, BoneList[0].ID, BoneList[7].attachxyz[0], BoneList[7].attachpbh[0], BoneList[7].ID, BoneList[7].attachxyz[1], BoneList[7].attachpbh[0]);

	//SimConnect_AttachSimObjectToSimObject(hSimConnect, BoneList[0].ID, BoneList[7].attachxyz[0], BoneList[7].attachpbh[0], BoneList[7].ID, BoneList[7].attachxyz[1], BoneList[7].attachpbh[0]);



	//SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[0].attachxyz[0], BoneList[0].attachpbh[0], BoneList[0].ID, BoneList[0].attachxyz[1], BoneList[0].attachpbh[1]);
	//SimConnect_AttachSimObjectToSimObject(hSimConnect, BoneList[(BoneList[37].fatherptr)].ID, BoneList[37].attachxyz[0], BoneList[37].attachpbh[0], BoneList[37].ID, BoneList[37].attachxyz[1], BoneList[37].attachpbh[1]);
	//int i = 0;
	//for (int j = 0; j < useindlength; j++)
	//{
	//	i = useind[j];
	//	SimConnect_AttachSimObjectToSimObject(hSimConnect, BoneList[BoneList[i].fatherptr].ID, BoneList[i].attachxyz[0], BoneList[i].attachpbh[0], BoneList[i].ID, BoneList[i].attachxyz[1], BoneList[i].attachpbh[1]);
	//}

	//for (int i = 0; i < 59; i++)
	//{


		//if ((i == 0) || (i == 37))
		//{
		//	if(i==0)
		//		SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[0].attachxyz[0], BoneList[0].attachpbh[0], BoneList[0].ID, BoneList[0].attachxyz[1], BoneList[0].attachpbh[1]);
			//else
			//SimConnect_AttachSimObjectToSimObject(hSimConnect, BoneList[BoneList[i].fatherptr].ID, BoneList[i].attachxyz[0], BoneList[i].attachpbh[0], BoneList[i].ID, BoneList[i].attachxyz[1], BoneList[i].attachpbh[1]);
		//}
		/*
		if (BoneList[i].fatherptr != -1)
		{
			SimConnect_AttachSimObjectToSimObject(hSimConnect, BoneList[BoneList[i].fatherptr].ID, BoneList[i].attachxyz[0], BoneList[i].attachpbh[0], BoneList[i].ID, BoneList[i].attachxyz[1], BoneList[i].attachpbh[1]);
		}*/
		//}
}
void testSimObjects()
{
	HRESULT hr;

	if (SUCCEEDED(SimConnect_Open(&hSimConnect, "AI Objects and Waypoints", NULL, 0, 0, 0)))
	{

		printf("\nConnected!");

		// Create some private events 
		hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_Z);
		hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_X);

		// Link the private events to keyboard keys, and ensure the input events are off 
		hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT_ZX, "Z", EVENT_Z);
		hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT_ZX, "X", EVENT_X);

		hr = SimConnect_SetInputGroupState(hSimConnect, INPUT_ZX, SIMCONNECT_STATE_OFF);

		// Sign up for notifications 
		hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_ZX, EVENT_Z);
		hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_ZX, EVENT_X);
		hr = SimConnect_SetInputGroupState(hSimConnect, INPUT_ZX, SIMCONNECT_STATE_ON);

		bool flag = false;
		bool IDNULL = true;
		bool initPtr = false;

		while (0 == quit)
		{

			SimConnect_CallDispatch(hSimConnect, MyDispatchProcSO, NULL);
			if (IDNULL)
			{
				IDNULL = false;
				if (!BoneList[0].isCreated() || !BoneList[7].isCreated())
				{
					IDNULL = true;
				}

				/*
				for (int i = 0; i < useindlength; i++)
				{
					int j = useind[i];
					if (!BoneList[j].isCreated())
					{
						IDNULL = true;
						break;
					}
				}
				*/
			}
			if (!IDNULL)
			{
				if (initPtr == false)
				{
					void initFaPtr();
					initPtr = true;
				}
				if (flag == true)
				{
					releaseBone();
				}
				attachBone();
				if (flag == false)
					flag = true;
			}
			Sleep(1);
		}

		hr = SimConnect_Close(hSimConnect);
	}
}

DWORD WINAPI ThreadFunc(LPVOID p)
{

	while (true)
	{
		static float count = 0.0;
		//count -= 0.000001;
		for (int i = 0; i < 59; i++)
		{
			if (i != 0)
			{
				BoneList[i].attachxyz[0].x = datalist[i * 6 + 0] / 100.0;
				BoneList[i].attachxyz[0].y = datalist[i * 6 + 1] / 100.0;
				BoneList[i].attachxyz[0].z = datalist[i * 6 + 2] / 100.0;
				BoneList[i].attachpbh[1].Bank = -datalist[i * 6 + 5] / 50.0;

				BoneList[i].attachpbh[1].Heading = datalist[i * 6 + 3] / 50.0;;
				BoneList[i].attachpbh[1].Pitch = datalist[i * 6 + 4] / 50.0;;

				//BoneList[i].attachpbh[1].Heading = datalist[i * 6 + 3]/50.0;
				//BoneList[i].attachpbh[1].Pitch = datalist[i * 6 + 3]/50.0;
			}
			else
			{
				BoneList[i].attachxyz[0].x = -count + datalist[i * 6 + 0] / 100.0 + (-0.25);
				BoneList[i].attachxyz[0].y = datalist[i * 6 + 1] / 100.0 + (-0.75);
				BoneList[i].attachxyz[0].z = datalist[i * 6 + 2] / 100.0 + (0.2);
				BoneList[i].attachpbh[1].Bank = datalist[i * 6 + 3] / 50.0;
				BoneList[i].attachpbh[1].Heading = datalist[i * 6 + 3] / 50.0;
				BoneList[i].attachpbh[1].Pitch = datalist[i * 6 + 3] / 50.0;
				//cout << "refresh" << BoneList[0].attachxyz[0].x << endl;
			}
			/*
			if((i==0)||(i==37))
			for (int j = 3; j < 6; j++)
				cout << j << "       " << datalist[i * 6 + j] << endl;
			*/
		}
		//cout << count << endl;
	}
	return 0;
}

int __cdecl _tmain(int argc, _TCHAR* argv[])
{

	PN_C PNC;
	PNC.Init(datalist);
	HANDLE hThread;
	DWORD  threadId;
	hThread = CreateThread(NULL, 0, ThreadFunc, 0, 0, &threadId); // 创建线程
	testSimObjects();
	return 0;
}




#endif // !__RE__
