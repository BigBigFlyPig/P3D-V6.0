#ifndef __V60__
#define __V60__

#define FINGERS_MODEL
//#define FULL_BODY_MODEL


#ifdef FINGERS_MODEL
#define Display_Num (38)
int Display_Ind[Display_Num] = {

	17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,
	40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58
};
#endif // FINGERS_MODEL

#ifdef FULL_BODY_MODEL
#define Display_Num (59)
int Display_Ind[Display_Num] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,
	13,14,15,
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,
	36,37,38,
	39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58
};
#endif // FULL_BODY_MODEL

#include <windows.h> 
#include <tchar.h>
#include <stdio.h>
#include <WinDef.h> 
#include "SimConnect.h"
#include <iostream>
#include "PN_C.h"
#include "Button.h"

using namespace std;


/*

*/

/****************关于button的全局变量****************/

Button buttons[12];
//const double CHUFAJULI = 0.001;
const double LIKAIJULI = 0.002;
bool button_Flag_left;
bool button_Flag_right;
SIMCONNECT_DATA_XYZ last_left_position;
SIMCONNECT_DATA_XYZ last_right_position;
SIMCONNECT_DATA_XYZ current_left_position;
SIMCONNECT_DATA_XYZ current_right_position;
//
//double button_position[][4] = {
//				   {0.0235,-0.053,-0.566,2, },					//8
//				   {0.0245,-0.0483,-0.638,2 },
//				   {0.0482,0.035,-0.685,2 },					//10
//				   {0.052,0.100,-0.602,2 },						//11
//				   {-0.159,-0.089,-0.661,1 },//12
//				   {-0.168,-0.052,-0.655,1 },
//				   {-0.032,-0.050,-0.558,2 },//14
//				   {-0.029,-0.048,-0.634,2 },
//				   {0.455,-0.249,-0.685,1 },
//				   {0.478,-0.054,-0.679,2 },//17
//				   {0.277,0.037,-0.677,1 },
//				   {0.481,-0.107,-0.291,1 },
//				   {100,100,100,100}, };
double a_test[2] = { 0.0 };
//double mixturepencent = 0.0;
struct structMixtureControl
{
	double mixturePercent;
};

structMixtureControl		mixturecontrol[10];
enum DATA_DEFINE_ID {
	DEFINITION_THROTTLE,
	DEFINITION_MIXTURE,
};

enum DATA_REQUEST_ID {
	REQUEST_THROTTLE,
	REQUEST_MIXTURE,
};

struct structThrottleControl
{
	double throttlePercent;
};

structThrottleControl		tc;

//



/****************************************************/


int     quit = 0;
HANDLE  hSimConnect = NULL;
bool objectsCreated = false;
vector<DWORD> Bone_ID;
vector<DWORD> Hint_Point_ID;
SIMCONNECT_DATA_XYZ attachxyz;
SIMCONNECT_DATA_PBH attachpbh, attachpbh1;

static enum EVENT_ID {
	EVENT_SIM_START,
	EVENT_Z,
	EVENT_X
};

static enum DATA_REQUEST_ID7 {

	REQUEST_BONE,

	REQUEST_HINTPOINT,
	REQUEST_HINTPOINT_HIGHLIGHT

};

static enum GROUP_ID {
	GROUP_ZX
};

static enum INPUT_ID {
	INPUT_ZX
};


void setUpSimObjects()
{
	cout << "setup" << endl;
	SIMCONNECT_DATA_INITPOSITION Init;
	HRESULT hr;

	Init.Altitude = 433.0;
	Init.Latitude = 47 + (25.91 / 60);
	Init.Longitude = -122 - (18.47 / 60);
	Init.Pitch = 0.0;
	Init.Bank = 0.0;
	Init.Heading = 360.0;
	Init.OnGround = 1;
	Init.Airspeed = 0;
	for (int i = 0; i < Display_Num; i++)
	{
		if ((Display_Ind[i] < 36) && (Display_Ind[i] > 12))
		{
			if (Display_Ind[i] == 23)
				hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_1", Init, REQUEST_BONE);
			else if ((Display_Ind[i] == 19) || (Display_Ind[i] == 27) || (Display_Ind[i] == 31) || (Display_Ind[i] == 35))
				hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_2", Init, REQUEST_BONE);
			else
				hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_3", Init, REQUEST_BONE);
			continue;
		}
		else if ((Display_Ind[i] < 59) && (Display_Ind[i] > 35))
		{
			if (Display_Ind[i] == 46)
				hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_1", Init, REQUEST_BONE);
			else if ((Display_Ind[i] == 42) || (Display_Ind[i] == 50) || (Display_Ind[i] == 54) || (Display_Ind[i] == 58))
				hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_2", Init, REQUEST_BONE);
			else
				hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_3", Init, REQUEST_BONE);
			continue;
		}
		hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_5", Init, REQUEST_BONE);
	}
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_1", Init, REQUEST_HINTPOINT);

}
PN_C NYT_Data_Reader;
// 1 red 2 ball 3 right 4 left 5 front
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

		case EVENT_X:
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

		case REQUEST_BONE:
			Bone_ID.push_back(pObjData->dwObjectID);
			printf("\nCreated bone id = %d", pObjData->dwObjectID);
			break;
		case REQUEST_HINTPOINT:
			Hint_Point_ID.push_back(pObjData->dwObjectID);
			printf("\nCreated bone id = %d", pObjData->dwObjectID);
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
	case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
	{
		SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
		switch (pObjData->dwRequestID) {
			case REQUEST_THROTTLE:
			{
				structThrottleControl *pS = (structThrottleControl*)&pObjData->dwData;
				//hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST_MIXTURE, DEFINITION_MIXTURE, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
				
				tc.throttlePercent = pS->throttlePercent;

				//printf("\nREQUEST_USERID received, throttle = %2.1f", pS->throttlePercent);
				break;
			}
			case REQUEST_MIXTURE:
			{
				structMixtureControl *pS = (structMixtureControl*)&pObjData->dwData;
				//hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST_THROTTLE, DEFINITION_THROTTLE, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
				mixturecontrol[0].mixturePercent = pS->mixturePercent;

				//printf("\nREQUEST_USERID received, mixture = %2.1f", pS->mixturePercent);
			}
			
		}
	}

	default:
		//printf("\nReceived:%d", pData->dwID);
		break;
	}
}

bool is_Created()
{
	if ((Bone_ID.size() != Display_Num)||(Hint_Point_ID.size()!=1))
	{
		return false;
	}
	else
	{
		return true;
	}

}



void loop_Attach_Release()
{
	static bool flag = false;
	if (flag == false)
	{
		for (int i = 0; i < Display_Num; i++)
		{
			SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, Bone_ID[i], NYT_Data_Reader.getXYZ(Display_Ind[i]), attachpbh1);
		}
		SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, Hint_Point_ID[0], buttons[11].getPosition(), attachpbh);
		flag = true;
	}
	else
	{
		for (int i = 0; i < Display_Num; i++)
		{
			SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, Bone_ID[i]);
			SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, Bone_ID[i], NYT_Data_Reader.getXYZ(Display_Ind[i]), NYT_Data_Reader.getPBH(Display_Ind[i]));
		}
		SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, Hint_Point_ID[0]);
		SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, Hint_Point_ID[0], buttons[11].getPosition(), attachpbh);
	}
	current_left_position = NYT_Data_Reader.getXYZ(Display_Ind[23]);
	current_right_position = NYT_Data_Reader.getXYZ(Display_Ind[46]);
}

void Init_Var()
{
	attachpbh.Bank = 0.0;
	attachpbh.Heading = 0.0;
	attachpbh.Pitch = 0.0;
	attachpbh1.Bank = 0.0;
	attachpbh1.Heading = 0.0;
	attachpbh1.Pitch = 0.0;
	attachxyz.x = 0.0;
	attachxyz.y = 0.0;
	attachxyz.z = 0.0;
}


void testSimObjects()
{
	HRESULT hr;

	if (SUCCEEDED(SimConnect_Open(&hSimConnect, "VR_NYT_P3D_HYBRIDIZE", NULL, 0, 0, 0)))
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


		//add definition
		SimConnect_AddToDataDefinition(hSimConnect, REQUEST_THROTTLE, "GENERAL ENG THROTTLE LEVER POSITION:1", "percent");
		SimConnect_AddToDataDefinition(hSimConnect, REQUEST_MIXTURE, "GENERAL ENG MIXTURE LEVER POSITION:1", "percent");
		hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST_THROTTLE, DEFINITION_THROTTLE, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME);
		hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST_MIXTURE, DEFINITION_MIXTURE, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SIM_FRAME);
		bool flag = false;
		while (0 == quit)
		{

			SimConnect_CallDispatch(hSimConnect, MyDispatchProcSO, NULL);
			if (!flag && !is_Created())
			{
				//cout << "wait for init..." << endl;
			}
			else
			{
				flag = true;
				loop_Attach_Release();

			}
			Sleep(25);
		}
		hr = SimConnect_Close(hSimConnect);
	}
}

/*********************关于button控制的函数***********************/
//

void init_button() {
	buttons[1].setPosition(0.0245, -0.0483, -0.55);
	buttons[1].setbType(BUTTON_THROTTLE);
	buttons[2].setPosition(0.0482, 0.035, -0.685);
	buttons[2].setbType(BUTTON_ELEVATOR_UP);
	buttons[3].setPosition(0.052, 0.100, -0.602);
	buttons[3].setbType(BUTTON_ELEVATOR_DOWN);
	buttons[4].setPosition(-0.032, -0.050, -0.558);
	buttons[4].setbType(BUTTON_MIX);
	buttons[6].setPosition(-0.159, -0.089, -0.60);					//buttons[6].setPosition(-0.159, -0.089, -0.661);	原本位置
	buttons[6].setbType(BUTTON_FLAP_UP);
	buttons[5].setPosition(-0.168, -0.052, -0.60);
	buttons[5].setbType(BUTTON_FLAP_DOWN);
	buttons[7].setPosition(0.455, -0.249, -0.685);
	buttons[7].setbType(BUTTON_BATTARY);
	buttons[8].setPosition(0.478, -0.054, -0.679);
	buttons[8].setbType(BUTTON_ENGIEN_START);
	buttons[9].setPosition(0.277, 0.037, -0.677);
	buttons[9].setbType(BUTTON_BRAKE);
	buttons[10].setPosition(0.481, -0.107, -0.291);
	buttons[10].setbType(BUTTON_DOOR);
	buttons[11].setPosition(0.445, -0.693, -0.397);
	buttons[11].setbType(BUTTON_CALIBARATE);
	for (int i = 1; i < 12; i++)
	{
		if (i < 5)
			buttons[i].settType(TRIGGER_LAST);
		else
			buttons[i].settType(TRIGGER_ONCE);
	}
	buttons[5].setchufajuli(0.004);
	buttons[6].setchufajuli(0.004);
	buttons[4].setchufajuli(0.001);
	buttons[1].setchufajuli(0.001);

}
//
//
//
//
BUTTON_TYPE count_to_do(const SIMCONNECT_DATA_XYZ& fingerpos, bool & flag) {
	double min = 1000.0;
	for (auto button : buttons) {
		double dis = button.triggered_dis(fingerpos);
		
		min = dis > min ? min : dis;
		/*cout << dis<<endl;
		cout <<"fx:"<< fingerpos.x << endl;
		cout << "fy:" << fingerpos.y << endl;
		cout << "fz:" << fingerpos.z << endl;*/
		/*cout << "x:" << button.getPosition().x << endl;
		cout << "y:" << button.getPosition().y << endl;
		cout << "z:" << button.getPosition().z << endl;*/

		if (dis < button.getchufajuli()) {
			if (button.gettType() == TRIGGER_ONCE && flag == true)
				return BUTTON_NOTHING;
			else {
				flag = true;
				cout << "chufa"<<(int)button.getbType()<<endl;
				return button.getbType();
				
			}
		}
		
	}
	if (min > LIKAIJULI) { flag = false;
		//cout << "likai"<<endl;
	}
	return BUTTON_NOTHING;
}

void throttleControl() {
	//cout << "last position:" << last_left_position.z << endl;
	//cout << "current position " << NYT_Data_Reader.getXYZ(Display_Ind[23]).z << endl;
	//cout << "percent1" << tc.throttlePercent << endl;
	tc.throttlePercent = tc.throttlePercent + button_Flag_left * ( NYT_Data_Reader.getXYZ(46).z- last_left_position.z)*-1410 + button_Flag_right * (NYT_Data_Reader.getXYZ(23).z- last_right_position.z)*-1410;
	tc.throttlePercent = (tc.throttlePercent) > 100?100: tc.throttlePercent;
	tc.throttlePercent = (tc.throttlePercent) < 0 ? 0 : tc.throttlePercent;

	cout <<"percent2" <<tc.throttlePercent << endl;
	SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_THROTTLE, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(tc), &tc);
}

void throttleControl2() {
	if (button_Flag_left)
		tc.throttlePercent = (NYT_Data_Reader.getXYZ(46).z + 0.55) * -1410;
	else 
		tc.throttlePercent = (NYT_Data_Reader.getXYZ(23).z + 0.55) * -1410;
	tc.throttlePercent = (tc.throttlePercent) > 100 ? 100 : tc.throttlePercent;
	tc.throttlePercent = (tc.throttlePercent) < 0 ? 0 : tc.throttlePercent;
	SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_THROTTLE, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(tc), &tc);
}

void mixtureControl() {
	//cout << "last position:" << last_left_position.z << endl;
	//cout << "current position " << NYT_Data_Reader.getXYZ(Display_Ind[23]).z << endl;
	//cout << "percent1" << tc.throttlePercent << endl;
	mixturecontrol[0].mixturePercent = mixturecontrol[0].mixturePercent + button_Flag_left * (NYT_Data_Reader.getXYZ(46).z - last_left_position.z)*-1300 + button_Flag_right * (NYT_Data_Reader.getXYZ(23).z - last_right_position.z)*-1300;
	mixturecontrol[0].mixturePercent = (mixturecontrol[0].mixturePercent) > 100 ? 100 : mixturecontrol[0].mixturePercent;
	mixturecontrol[0].mixturePercent = (mixturecontrol[0].mixturePercent) < 0 ? 0 : mixturecontrol[0].mixturePercent;

	cout << "percent2" << mixturecontrol[0].mixturePercent << endl;
	SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_MIXTURE, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(mixturecontrol[0]), &mixturecontrol);
}

///*键位9*/
void throttlePush() {//油门推入--加油门
	for (int i = 0; i < 30; i++) {
		keybd_event(VK_F3, 0, 0, 0);//F3
		Sleep(50);
		keybd_event(VK_F3, 0, 2, 0);
		Sleep(20);
	}

}
/*键位10*/
void elevatorTrimDown() {//升降舵向下调整--圆盘向上转
	for (int i = 0; i < 10; i++) {
		keybd_event(VK_HOME, 0, 0, 0);//Home
		Sleep(10);
		keybd_event(VK_HOME, 0, 2, 0);
		Sleep(40);
	}
}
/*键位11*/
void elevatorTrimUp() {//升降舵向上调整--圆盘向下转
	for (int i = 0; i < 10; i++) {
		keybd_event(VK_END, 0, 0, 0);//End
		Sleep(5);
		keybd_event(VK_END, 0, 2, 0);
		Sleep(50);
	}
}
/*键位12*/
void flapsRetractIncrementally() {//襟翼（递增收缩）
	keybd_event(VK_F6, 0, 0, 0);//F6
	Sleep(5);
	keybd_event(VK_F6, 0, 2, 0);
	Sleep(40);
}
/*键位13*/
void flapsExtendIncrementally() {//襟翼（递增展开）
	keybd_event(VK_F7, 0, 0, 0);//F7
	Sleep(5);
	keybd_event(VK_F7, 0, 2, 0);
	Sleep(40);
}
/*键位14*/
void mixtureLeanQuickly() {//混合比减少--红拉杆拉出
	for (int i = 0; i < 15; i++) {
		keybd_event(VK_CONTROL, 0, 0, 0);//Ctrl
		keybd_event(VK_SHIFT, 0, 0, 0);//Shift
		keybd_event(VK_F2, 0, 0, 0);//F2
		Sleep(5);
		keybd_event(VK_F2, 0, 2, 0);
		keybd_event(VK_SHIFT, 0, 2, 0);
		keybd_event(VK_CONTROL, 0, 2, 0);
		Sleep(50);
	}
}
/*键位15*/
void mixtureEnrich() {//混合比增加--红拉杆压入
	for (int i = 0; i < 15; i++) {
		keybd_event(VK_CONTROL, 0, 0, 0);//Ctrl
		keybd_event(VK_SHIFT, 0, 0, 0);//Shift
		keybd_event(VK_F3, 0, 0, 0);//F3
		Sleep(5);
		keybd_event(VK_F3, 0, 2, 0);
		keybd_event(VK_SHIFT, 0, 2, 0);
		keybd_event(VK_CONTROL, 0, 2, 0);
		Sleep(50);
	}
}
/*键位16*/
void masterBatteryAndAlternatorSelect() {//主电池/交流发电机（开关）
	keybd_event(VK_SHIFT, 0, 0, 0);//SHIFT
	keybd_event('M', 0, 0, 0);//M
	Sleep(10);
	keybd_event('M', 0, 2, 0);
	keybd_event(VK_SHIFT, 0, 2, 0);
	Sleep(50);
}
/*键位17*/
void engineAutostart() {//发动机自动启动
	keybd_event(VK_CONTROL, 0, 0, 0);//Ctrl
	keybd_event('E', 0, 0, 0);//E
	Sleep(10);
	keybd_event('E', 0, 2, 0);
	keybd_event(VK_CONTROL, 0, 2, 0);
	Sleep(50);
}
/*键位18*/
void brakeSetParking() {//驻机把手开关
	keybd_event(VK_CONTROL, 0, 0, 0);//Ctrl
	keybd_event(VK_OEM_PERIOD, 0, 0, 0);//.
	Sleep(10);
	keybd_event(VK_OEM_PERIOD, 0, 2, 0);
	keybd_event(VK_CONTROL, 0, 2, 0);
	Sleep(50);
}
/*键位19*/
void doorControl() {//驾驶舱门控制 开/关
	keybd_event(16, 0, 0, 0);//shift
	keybd_event(69, 0, 0, 0);//E
	Sleep(10);
	keybd_event(16, 0, 2, 0);
	keybd_event(69, 0, 2, 0);
	Sleep(50);
}

void calibrate() {
	NYT_Data_Reader.reSetBias();
};

int statementControl(BUTTON_TYPE b) {//状态机--传入按钮号
	switch (b)//需要进行异常处理
	{
	case BUTTON_THROTTLE://油门拉出--减油门键位
		throttleControl2();//F2
		return 666;
		break;

	case BUTTON_ELEVATOR_UP://升降舵向下调整--圆盘向上转键位
		elevatorTrimDown();//Home
		return 666;
		break;
	case BUTTON_ELEVATOR_DOWN://升降舵向上调整--圆	盘向下转键位
		elevatorTrimUp();//End
		return 666;
		break;
	case BUTTON_FLAP_UP://襟翼（递增收缩）键位
		flapsRetractIncrementally();//F6
		return 666;
		break;
	case BUTTON_FLAP_DOWN://襟翼（递增展开）键位
		flapsExtendIncrementally();//F7
		return 666;
		break;
	case BUTTON_MIX:
		mixtureControl();//Ctrl+Shift+F3
		return 666;
		break;
	case BUTTON_BATTARY://主电池/交流发电机（开关）键位
		masterBatteryAndAlternatorSelect();//Shift+M
		return 666;
		break;
	case BUTTON_ENGIEN_START://发动机自动启动键位
		engineAutostart();//Ctrl+E
		return 666;
		break;
	case BUTTON_BRAKE://驻机把手开关键位
		brakeSetParking();//Ctrl+.
		return 666;
		break;
	case BUTTON_DOOR://舱门键位
		doorControl();//Shift+E
		return 666;
		break;
	case BUTTON_NOTHING:
		return 0;
		break;
	case BUTTON_CALIBARATE:
		calibrate();
		cout << "校正"<<endl;
		return 99;
		break;
	default:
		return 99;
		break;
	}
}


void update_button() {
	buttons[1].setPosition(0.0245, -0.0483, -tc.throttlePercent*0.07 *0.01 -0.55);
	buttons[4].setPosition(-0.032, -0.050, -mixturecontrol[0].mixturePercent*0.07 *0.01 - 0.55);
	last_left_position = NYT_Data_Reader.getXYZ(46);
	last_right_position = NYT_Data_Reader.getXYZ(23);
}


DWORD WINAPI check_for_state(LPVOID pM) {
	Sleep(5000);
	
	while (1)
	{
		BUTTON_TYPE current_button = count_to_do(NYT_Data_Reader.getXYZ(46), button_Flag_left);
		statementControl(current_button);
		BUTTON_TYPE current2_button = count_to_do(NYT_Data_Reader.getXYZ(23), button_Flag_right);
		statementControl(current2_button);
		update_button();

		//cout <<  "z:" <<buttons[1].getPosition().z << endl;
		//NYT_Data_Reader.getXYZ(Display_Ind[46]);
		/*cout << endl << "fz:" << NYT_Data_Reader.getXYZ(16).z << endl;
		cout << endl << "fx:" << NYT_Data_Reader.getXYZ(16).x << endl;
		cout << endl << "fy:" << NYT_Data_Reader.getXYZ(16).y << endl;*/
		Sleep(100);

	}
}












/*****************************************************************/
















int main()
{
	init_button();
	Init_Var();
	HANDLE handle = CreateThread(NULL, 0, check_for_state, NULL, 0, NULL);;
	testSimObjects();
	return 0;
}



#endif // !__V6.0__
