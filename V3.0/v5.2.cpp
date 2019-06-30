//#define DEBUG
#ifdef DEBUG

#include <windows.h> 
#include <tchar.h>
#include <stdio.h>
#include <WinDef.h> 
#include "SimConnect.h"
#include <iostream>
#include "GKD.h"

#define XR -100.0;
#define YR 100.0;
#define ZR -100.0;
//0.15,  0.10,   -0.05
#define XE 0.25
#define YE 1.05
#define ZE -0.1

const float X_BUTTON = 100;
const float Y_BUTTON = 100;
const float Z_BUTTON = 100;

using namespace std;

PN_C PNC;
GKD gkd(&PNC);

SIMCONNECT_DATA_XYZ attachxyz;
SIMCONNECT_DATA_PBH attachpbh;
SIMCONNECT_DATA_XYZ temLeft,temHip,temRight, temLeftPinky, temLeftRing, temLeftMiddle, temLeftIndex, temLeftThumb, temRightPinky, temRightRing, temRightMiddle, temRightIndex, temRightThumb;
SIMCONNECT_DATA_PBH PBHlist[59];
int Display_list[12] = { 16,19,23,27,31,35,39,42,46,50,54,58 };
int Hint_Point_Length = 20;
int status[2] = {-1,-1};
SIMCONNECT_DATA_XYZ hintpointxyz;
SIMCONNECT_DATA_PBH hintpointpbh;
vector<DWORD> Hint_Point_ID;
vector<vector<SIMCONNECT_DATA_PBH>> Hint_Point_PBH;
vector<vector<SIMCONNECT_DATA_XYZ>> Hint_Point_XYZ;
vector<DWORD> Hint_Point_HIGHLIGHT_ID;
vector<vector<SIMCONNECT_DATA_PBH>> Hint_Point_HIGHLIGHT_PBH;
vector<vector<SIMCONNECT_DATA_XYZ>> Hint_Point_HIGHLIGHT_XYZ;
vector<SIMCONNECT_DATA_XYZ> temVectorXYZ;
vector<SIMCONNECT_DATA_PBH> temVectorPBH;

int     quit = 0;
HANDLE  hSimConnect = NULL;

float datalist[59 * 6];

static enum EVENT_ID {
	EVENT_SIM_START,
	EVENT_Z,
	EVENT_X,
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

	REQUEST_LEFTHANDPINKY,
	REQUEST_LEFTHANDRING,
	REQUEST_LEFTHANDMIDDLE,
	REQUEST_LEFTHANDINDEX,
	REQUEST_LEFTHANDTHUMB,

	REQUEST_RIGHTHANDPINKY,
	REQUEST_RIGHTHANDRING,
	REQUEST_RIGHTHANDMIDDLE,
	REQUEST_RIGHTHANDINDEX,
	REQUEST_RIGHTHANDTHUMB,

	REQUEST_HINTPOINT,
	REQUEST_HINTPOINT_HIGHLIGHT,

};

static enum GROUP_ID {
	GROUP_ZX,
};

static enum INPUT_ID {
	INPUT_ZX,
};


/*************************************************************************
 *	lyy模块
 *	状态机及操作状态
 *	v1.0
 *	2019-3-12
 ************************************************************************/
int flag_target = 19;

 /*键位1*/
void allLights() {//灯光控制--开/关--所有灯(机翼及尾灯，头顶灯，面板灯,频闪灯，着陆灯)
	keybd_event('L', 0, 0, 0);//L
	Sleep(10);
	keybd_event('L', 0, 2, 0);
	Sleep(50); 
}
/*键位2*/
void strobeLights() {//频闪灯开关----需要设置！！！！！！
	keybd_event('O', 0, 0, 0);//O   
	Sleep(10);
	keybd_event('O', 0, 2, 0);
	Sleep(50);
}
/*键位3*/
void panelLights() {//头灯
	keybd_event(VK_SHIFT, 0, 0, 0);//Shift
	keybd_event('L', 0, 0, 0);//L
	Sleep(10);
	keybd_event('L', 0, 2, 0);
	keybd_event(VK_SHIFT, 0, 2, 0);//Shift
	Sleep(50);
}
/*键位4*/
void landingLights() {//着陆灯
	keybd_event(VK_CONTROL, 0, 0, 0);//Ctrl
	keybd_event('L', 0, 0, 0);//L
	Sleep(10);
	keybd_event('L', 0, 2, 0);
	keybd_event(VK_CONTROL, 0, 2, 0);
	Sleep(50);
}
/*键位5*/
void headingIndicatorReset() {//指南针仪表盘重置
	keybd_event('D', 0, 0, 0);//D
	Sleep(10);
	keybd_event('D', 0, 2, 0);
	Sleep(50);
}
/*键位6*/
void altimeterReset() {//高度仪重置
	keybd_event('B', 0, 0, 0);//B
	Sleep(10);
	keybd_event('B', 0, 2, 0);
	Sleep(50);
}
/*键位7*/
void pitotHeat() {//空速管加热
	keybd_event(VK_SHIFT, 0, 0, 0);//Shift
	keybd_event('H', 0, 0, 0);//H
	Sleep(10);
	keybd_event('H', 0, 2, 0);
	keybd_event(VK_SHIFT, 0, 2, 0);//Shift
	Sleep(50);
}
/*键位8*/
void throttlePull() {//油门拉出--减油门
	for (int i = 0; i < 8; i++) {
		keybd_event(VK_F2, 0, 0, 0);//F2
		Sleep(50);
		keybd_event(VK_F2, 0, 2, 0);
		Sleep(20);
	}
}
/*键位9*/
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

int statementControl(int buttonNum) {//状态机--传入按钮号
	switch (buttonNum)//需要进行异常处理
	{
	case 1://开所有灯键位
		allLights();//L
		return 666;
		break;
	case 2://频闪灯开关键位
		strobeLights();//O
		return 666;
		break;
	case 3://头灯键位
		panelLights();//Shift+L
		return 666;
		break;
	case 4://着陆灯键位
		landingLights();//Ctrl+L
		return 666;
		break;
	case 5://指南针仪表盘重置键位
		headingIndicatorReset();//D
		return 666;
		break;
	case 6://高度仪重置键位
		altimeterReset();//B
		return 666;
		break;
	case 7://空速管加热键位
		pitotHeat();//Shift+H
		return 666;
		break;
	case 8://油门拉出--减油门键位
		throttlePull();//F2
		return 666;
		break;
	case 9://油门推入--加油门键位
		throttlePush();//F3
		return 666;
		break;
	case 10://升降舵向下调整--圆盘向上转键位
		elevatorTrimDown();//Home
		return 666;
		break;
	case 11://升降舵向上调整--圆盘向下转键位
		elevatorTrimUp();//End
		return 666;
		break;
	case 12://襟翼（递增收缩）键位
		flapsRetractIncrementally();//F6
		return 666;
		break;
	case 13://襟翼（递增展开）键位
		flapsExtendIncrementally();//F7
		return 666;
		break;
	case 14://混合比减少--红拉杆拉出键位
		mixtureLeanQuickly();//Ctrl+Shift+F2
		return 666;
		break;
	case 15://混合比增加--红拉杆压入键位
		mixtureEnrich();//Ctrl+Shift+F3
		return 666;
		break;
	case 16://主电池/交流发电机（开关）键位
		masterBatteryAndAlternatorSelect();//Shift+M
		return 666;
		break;
	case 17://发动机自动启动键位
		engineAutostart();//Ctrl+E
		return 666;
		break;
	case 18://驻机把手开关键位
		brakeSetParking();//Ctrl+.
		return 666;
		break;
	case 19://舱门键位
		doorControl();//Shift+E
		return 666;
		break;
	default:
		return 99;
		break;
	}
}


/************************************************************************
 *	lyy模块
 *	状态机及操作状态
 ************************************************************************/



 ///////////////////////////////////////////////////////////////////////////////////////////
 ///////////////////////////////******//////////////////////////////////////////////////////
 ///////////////////////////////sj模块//////////////////////////////////////////////////////
 ///////////////////////////////******/////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////////////////////



const double CHUFAJULI = 0.001;
const double LIKAIJULI = 0.003;

bool button_Flag_left = false;
bool button_Flag_right = false;

//double button_position[][4] = { {-0.01158,-0.05537,-0.62664},{100,100,100} };
double button_position[][4] = { 
				   {X_BUTTON,Y_BUTTON,Z_BUTTON,1},               //0
				   {X_BUTTON,Y_BUTTON,Z_BUTTON,1},					 //1
				   {X_BUTTON,Y_BUTTON,Z_BUTTON,1},						 //2
				   {X_BUTTON,Y_BUTTON,Z_BUTTON,0				},//{100,0,0,0				}
				   {X_BUTTON,Y_BUTTON,Z_BUTTON,1},						 //4
				   {X_BUTTON,Y_BUTTON,Z_BUTTON,0				},
				   {X_BUTTON,Y_BUTTON,Z_BUTTON,1 },					//6
				   {X_BUTTON,Y_BUTTON,Z_BUTTON,1 },
				   {0.0235,-0.053,-0.566,2 },					//8
				   {0.0245,-0.0483,-0.638,2 },
				   {0.0482,0.035,-0.685,2 },					//10
				   {0.052,0.100,-0.602,2 },						//11
				   {-0.159,-0.089,-0.661,1 },
				   {-0.168,-0.052,-0.655,1 },
				   {-0.032,-0.050,-0.558,2 },
				   {-0.029,-0.048,-0.634,2 },
				   {0.455,-0.249,-0.685,1 },
				   {0.478,-0.054,-0.679,2 },
				   {0.277,0.037,-0.677,1 },
				   {0.481,-0.107,-0.291,1 },
				   {100,100,100,100}, };
double x = 0.0, y = 0.0, z = 0.0;
//double previous_position_x= 0.0,previous_position_y= 0.0,previous_position_z= 0.0;
int direction = 0;
//void record_previous_position(double x,double y,double z)
//{
//	previous_position_x=x;
//	previous_position_y=y;
//	previous_position_z=z;
//}
//double min = 1000;

int record_flag_left = -1;//record——flag用来记录当前左手和右手按了几号键
int record_flag_right = -1;
int count_distance_left(double x, double y, double z)//此函数找到当前位置距离最近的按钮，如果全部在触发距离外，则返回-1，否则返回按钮的代号
{
	int length = sizeof(button_position) / 32;
	double min = 1000.0;
	for (size_t i = 1; i < length; i++)
	{

		double dis = (x - button_position[i][0])*(x - button_position[i][0]) + (y - button_position[i][1])*(y - button_position[i][1]) + (z - button_position[i][2])*(z - button_position[i][2]);
		//printf("dis %f\n", dis);
		min = dis > min ? min : dis;
		if (dis <= CHUFAJULI)
		{
			status[0] = i;
			//cout << button_Flag_left << endl;
			if (button_position[i][3] == 1)//1号按键只按一次，如果已经在按键则跳出循环
			{
				if (button_Flag_left == true) {
					//cout << "1类按键，判断当前未离开触发器，只生效一次" << endl;
					return -1;
				}
				else
				{
					//cout << "进入按键区" << endl;
					button_Flag_left = true;
					statementControl(i);
					//printf("按下了");
					//cout << i << endl;
					return i;
				}
			}
			if (button_position[i][3] == 2)
			{
				statementControl(i);
				//cout << "正在启用2类按键" << i << endl;
			}

		}

	}
	if (min > LIKAIJULI)button_Flag_left = false;

	return -1;
}


int count_distance_right(double x, double y, double z)//此函数找到当前位置距离最近的按钮，如果全部在触发距离外，则返回-1，否则返回按钮的代号
{
	int length = sizeof(button_position) / 32;
	double min = 1000.0;
	for (size_t i = 1; i < length; i++)
	{

		double dis = (x - button_position[i][0])*(x - button_position[i][0]) + (y - button_position[i][1])*(y - button_position[i][1]) + (z - button_position[i][2])*(z - button_position[i][2]);
		//printf("dis %f\n", dis);
		min = dis > min ? min : dis;
		if (dis <= CHUFAJULI)
		{
			status[1] = i;
			//cout << button_Flag_right << endl;
			if (button_position[i][3] == 1)//1号按键只按一次，如果已经在按键则跳出循环
			{
				if (button_Flag_right == true) {
					//cout << "1类按键，判断当前未离开触发器，只生效一次" << endl;
					return -1;
				}
				else
				{
					//cout << "进入按键区" << endl;
					button_Flag_right = true;
					statementControl(i);
					//printf("按下了");
					cout << i << endl;
					return i;
				}
			}
			if (button_position[i][3] == 2)
			{
				statementControl(i);
				//cout << "正在启用2类按键" << i << endl;
			}

		}

	}
	if (min > LIKAIJULI)button_Flag_right = false;

	return -1;
}


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
	BoneList[37].fatherptr = 36;
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

	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_4", Init, REQUEST_HIP);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_4", Init, REQUEST_SPINE);
	//hr = SimConnect_AICreateSimulatedObject(hSimConnect, "AK_47", Init, REQUEST_SPINE1);
	//hr = SimConnect_AICreateSimulatedObject(hSimConnect, "AK_47", Init, REQUEST_SPINE2);
	//hr = SimConnect_AICreateSimulatedObject(hSimConnect, "AK_47", Init, REQUEST_SPINE3);
	//hr = SimConnect_AICreateSimulatedObject(hSimConnect, "AK_47", Init, REQUEST_RIGHTSHOULDER);
	//hr = SimConnect_AICreateSimulatedObject(hSimConnect, "AK_47", Init, REQUEST_RIGHTFOREARM);
	
	//hr = SimConnect_AICreateSimulatedObject(hSimConnect, "AK_47", Init, REQUEST_LEFTSHOULDER);
	//hr = SimConnect_AICreateSimulatedObject(hSimConnect, "AK_47", Init, REQUEST_LEFTFOREARM);
	
	//hr = SimConnect_AICreateSimulatedObject(hSimConnect, "AK_47", Init, REQUEST_HEAD);
	//hr = SimConnect_AICreateSimulatedObject(hSimConnect, "AK_47", Init, REQUEST_NECK);
	//hr = SimConnect_AICreateSimulatedObject(hSimConnect, "AK_47", Init, REQUEST_RIGHTARM);
	//hr = SimConnect_AICreateSimulatedObject(hSimConnect, "AK_47", Init, REQUEST_LEFTARM);

	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_4", Init, REQUEST_RIGHTHAND);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_4", Init, REQUEST_LEFTHAND);

	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_3", Init, REQUEST_LEFTHANDPINKY);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_3", Init, REQUEST_LEFTHANDRING);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_3", Init, REQUEST_LEFTHANDMIDDLE);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_3", Init, REQUEST_LEFTHANDINDEX);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_3", Init, REQUEST_LEFTHANDTHUMB);

	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_3", Init, REQUEST_RIGHTHANDPINKY);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_3", Init, REQUEST_RIGHTHANDRING);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_3", Init, REQUEST_RIGHTHANDMIDDLE);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_3", Init, REQUEST_RIGHTHANDINDEX);
	hr = SimConnect_AICreateSimulatedObject(hSimConnect, "Left_Finger_3", Init, REQUEST_RIGHTHANDTHUMB);
	for(int i = 0;i<Hint_Point_Length;i++)
		hr = SimConnect_AICreateSimulatedObject(hSimConnect, "DefaultPrompt", Init, REQUEST_HINTPOINT);
	for(int i = 0; i < Hint_Point_Length; i++)
		hr = SimConnect_AICreateSimulatedObject(hSimConnect, "HighLight", Init, REQUEST_HINTPOINT_HIGHLIGHT);


	//VEH_jetTruck Left_Finger_4 REQUEST_HINTPOINT_HIGHLIGHT DefaultPrompt HighLight
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

		case REQUEST_LEFTHANDPINKY:
			BoneList[58].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_LEFTHANDRING:
			BoneList[54].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_LEFTHANDMIDDLE:
			BoneList[50].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_LEFTHANDINDEX:
			BoneList[46].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_LEFTHANDTHUMB:
			BoneList[42].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_RIGHTHANDPINKY:
			BoneList[35].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_RIGHTHANDRING:
			BoneList[31].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_RIGHTHANDMIDDLE:
			BoneList[27].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_RIGHTHANDINDEX:
			BoneList[23].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_RIGHTHANDTHUMB:
			BoneList[19].setID(pObjData->dwObjectID);
			printf("\nCreated rightarm id = %d");
			break;
		case REQUEST_HINTPOINT:
			Hint_Point_ID.push_back(pObjData->dwObjectID);
			cout << "\nCreated HintPoint " << Hint_Point_ID.size();
			temVectorXYZ.clear();
			temVectorPBH.clear();
			hintpointxyz.x = button_position[Hint_Point_XYZ.size()][0];
			hintpointxyz.y = button_position[Hint_Point_XYZ.size()][1];
			hintpointxyz.z = button_position[Hint_Point_XYZ.size()][2];
			temVectorXYZ.push_back(hintpointxyz);
			hintpointxyz.x = 0.0;
			hintpointxyz.y = 0.0;
			hintpointxyz.z = 0.0;
			temVectorXYZ.push_back(hintpointxyz);
			temVectorPBH.push_back(attachpbh);
			temVectorPBH.push_back(attachpbh);
			Hint_Point_XYZ.push_back(temVectorXYZ);
			Hint_Point_PBH.push_back(temVectorPBH);
			break;
		case REQUEST_HINTPOINT_HIGHLIGHT:
			Hint_Point_HIGHLIGHT_ID.push_back(pObjData->dwObjectID);
			cout << "\nCreated HintPointHighLight " << Hint_Point_HIGHLIGHT_ID.size();
			temVectorXYZ.clear();
			temVectorPBH.clear();
			hintpointxyz.x = button_position[Hint_Point_HIGHLIGHT_XYZ.size() ][0];
			hintpointxyz.y = button_position[Hint_Point_HIGHLIGHT_XYZ.size() ][1];
			hintpointxyz.z = button_position[Hint_Point_HIGHLIGHT_XYZ.size() ][2];
			temVectorXYZ.push_back(hintpointxyz);
			hintpointxyz.x = 0.0;
			hintpointxyz.y = 0.0;
			hintpointxyz.z = 0.0;
			temVectorXYZ.push_back(hintpointxyz);
			temVectorPBH.push_back(attachpbh);
			temVectorPBH.push_back(attachpbh);
			Hint_Point_HIGHLIGHT_XYZ.push_back(temVectorXYZ);
			Hint_Point_HIGHLIGHT_PBH.push_back(temVectorPBH);
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
	SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[0].ID);
	SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[58].ID);
	SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[54].ID);
	SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[50].ID);
	SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[46].ID);
	SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[42].ID);
	SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[39].ID);
	SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[35].ID);
	SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[31].ID);
	SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[27].ID);
	SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[23].ID);
	SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[19].ID);
	SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, BoneList[16].ID);

	for (int i = Hint_Point_Length-1; i>=8 ; i--)
	{
		SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, Hint_Point_ID[i]);
	}
	for (int i = Hint_Point_Length-1; i >= 8; i--)
	{
		SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, Hint_Point_HIGHLIGHT_ID[i]);
	}
}

void ALeft(float a,float b,float c,int flag)
{//temLeftPinky, temLeftRing, temLeftMiddle, temLeftIndex, temLeftThumb,
	if(flag ==0){
		temLeft.x = temLeft.x + a;
		temLeft.y = temLeft.y + b;
		temLeft.z = temLeft.z + c;
	}
	if (flag == 1) {
		temLeftThumb.x = temLeftThumb.x + a;
		temLeftThumb.y = temLeftThumb.y + b;
		temLeftThumb.z = temLeftThumb.z + c;
	}
	if (flag == 2) {
		temLeftIndex.x = temLeftIndex.x + a;
		temLeftIndex.y = temLeftIndex.y + b;
		temLeftIndex.z = temLeftIndex.z + c;
	}
	if (flag == 3) {
		temLeftMiddle.x = temLeftMiddle.x + a;
		temLeftMiddle.y = temLeftMiddle.y + b;
		temLeftMiddle.z = temLeftMiddle.z + c;
	}
	if (flag == 4) {
		temLeftRing.x = temLeftRing.x + a;
		temLeftRing.y = temLeftRing.y + b;
		temLeftRing.z = temLeftRing.z + c;
	}
	if (flag == 5) {
		temLeftPinky.x = temLeftPinky.x + a;
		temLeftPinky.y = temLeftPinky.y + b;
		temLeftPinky.z = temLeftPinky.z + c;
	}
}

void ARight(float a, float b, float c, int flag)
{//temRightPinky, temRightRing, temRightMiddle, temRightIndex, temRightThumb
	if (flag == 0) {
		temRight.x = temRight.x + a;
		temRight.y = temRight.y + b;
		temRight.z = temRight.z + c;

	}
	if (flag == 1) {
		temRightThumb.x = temRightThumb.x + a;
		temRightThumb.y = temRightThumb.y + b;
		temRightThumb.z = temRightThumb.z + c;
	}
	if (flag == 2) {
		temRightIndex.x = temRightIndex.x + a;
		temRightIndex.y = temRightIndex.y + b;
		temRightIndex.z = temRightIndex.z + c;
	}
	if (flag == 3) {
		temRightMiddle.x = temRightMiddle.x + a;
		temRightMiddle.y = temRightMiddle.y + b;
		temRightMiddle.z = temRightMiddle.z + c;
	}
	if (flag == 4) {
		temRightRing.x = temRightRing.x + a;
		temRightRing.y = temRightRing.y + b;
		temRightRing.z = temRightRing.z + c;
	}
	if (flag == 5) {
		temRightPinky.x = temRightPinky.x + a;
		temRightPinky.y = temRightPinky.y + b;
		temRightPinky.z = temRightPinky.z + c;
	}
}

void calBone(float a, float b, float c) {

	temHip.x = gkd.data[0 + 0]*0.01+a;
	temHip.y = -gkd.data[0 + 1]*0.01+b;
	temHip.z = -gkd.data[0 + 2]*0.01+c;

	//for (int i = 0; i < 12; i++)
	//{
	//	PBHlist[Display_list[i]] = gkd.getPBH(Display_list[i]);
	//}
	//计算右手位置
	glm::vec4 righthand = gkd.resRightHandPos();

	temRight.x = righthand[0] / XR;
	temRight.y = -righthand[1] / YR;
	temRight.z = -righthand[2] / ZR;
	ARight(a, b, c,0);

	
	//计算右手拇指位置
	//temRightPinky, temRightRing, temRightMiddle, temRightIndex, temRightThumb
	
	glm::vec4 righthandThumb = gkd.resRightHandThumbPos();

	temRightThumb.x = righthandThumb[0] / XR;
	temRightThumb.y = -righthandThumb[1] / YR;
	temRightThumb.z = -righthandThumb[2] / ZR;
	ARight(a, b, c, 1);

	//计算右手食指位置
	//temLeftPink, temLeftRing, temLeftMiddle, temLeftIndex, temLeftThumb,
	glm::vec4 righthandIndex = gkd.resRightHandIndexPos();

	temRightIndex.x = righthandIndex[0] / XR;
	temRightIndex.y = -righthandIndex[1] / YR;
	temRightIndex.z = -righthandIndex[2] / ZR;
	ARight(a, b, c, 2);

	//计算右手中指位置
	//temLeftPink, temLeftRing, temLeftMiddle, temLeftIndex, temLeftThumb,
	glm::vec4 righthandMiddle = gkd.resRightHandMiddlePos();

	temRightMiddle.x = righthandMiddle[0] / XR;
	temRightMiddle.y = -righthandMiddle[1] / YR;
	temRightMiddle.z = -righthandMiddle[2] / ZR;
	ARight(a, b, c, 3);

	//计算右手无名指位置
	//temLeftPink, temLeftRing, temLeftMiddle, temLeftIndex, temLeftThumb,
	glm::vec4 righthandRing = gkd.resRightHandRingPos();

	temRightRing.x = righthandRing[0] / XR;
	temRightRing.y = -righthandRing[1] / YR;
	temRightRing.z = -righthandRing[2] / ZR;
	ARight(a, b, c, 4);

	//计算右手小指位置
	//temLeftPink, temLeftRing, temLeftMiddle, temLeftIndex, temLeftThumb,
	glm::vec4 righthandPinky = gkd.resRightHandPinkyPos();

	temRightPinky.x = righthandPinky[0] / XR;
	temRightPinky.y = -righthandPinky[1] / YR;
	temRightPinky.z = -righthandPinky[2] / ZR;
	ARight(a, b, c, 5);

	////////////////////////////////////

	//计算左手位置
	glm::vec4 lefthand = gkd.resLeftHandPos();

	temLeft.x = lefthand[0] / XR;
	temLeft.y = -lefthand[1] / YR;
	temLeft.z = -lefthand[2] / ZR;
	ALeft(a, b, c,0);

	//计算左手拇指位置
	//temLeftPink, temLeftRing, temLeftMiddle, temLeftIndex, temLeftThumb,
	glm::vec4 lefthandThumb = gkd.resLeftHandThumbPos();

	temLeftThumb.x = lefthandThumb[0] / XR;
	temLeftThumb.y = -lefthandThumb[1] / YR;
	temLeftThumb.z = -lefthandThumb[2] / ZR;
	ALeft(a, b, c, 1);

	//计算左手食指位置
	//temLeftPink, temLeftRing, temLeftMiddle, temLeftIndex, temLeftThumb,
	glm::vec4 lefthandIndex = gkd.resLeftHandIndexPos();

	temLeftIndex.x = lefthandIndex[0] / XR;
	temLeftIndex.y = -lefthandIndex[1] / YR;
	temLeftIndex.z = -lefthandIndex[2] / ZR;
	ALeft(a, b, c, 2);

	//计算左手中指位置
	//temLeftPink, temLeftRing, temLeftMiddle, temLeftIndex, temLeftThumb,
	glm::vec4 lefthandMiddle = gkd.resLeftHandMiddlePos();

	temLeftMiddle.x = lefthandMiddle[0] / XR;
	temLeftMiddle.y = -lefthandMiddle[1] / YR;
	temLeftMiddle.z = -lefthandMiddle[2] / ZR;
	ALeft(a, b, c, 3);

	//计算左手无名指位置
	//temLeftPink, temLeftRing, temLeftMiddle, temLeftIndex, temLeftThumb,
	glm::vec4 lefthandRing = gkd.resLeftHandRingPos();

	temLeftRing.x = lefthandRing[0] / XR;
	temLeftRing.y = -lefthandRing[1] / YR;
	temLeftRing.z = -lefthandRing[2] / ZR;
	ALeft(a, b, c, 4);

	//计算左手小指位置
	//temLeftPink, temLeftRing, temLeftMiddle, temLeftIndex, temLeftThumb,
	glm::vec4 lefthandPinky = gkd.resLeftHandPinkyPos();

	temLeftPinky.x = lefthandPinky[0] / XR;
	temLeftPinky.y = -lefthandPinky[1] / YR;
	temLeftPinky.z = -lefthandPinky[2] / ZR;
	ALeft(a, b, c, 5);

}


void attachBone()
{
	SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, BoneList[0].ID,  temHip, PBHlist[0]);
	SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, BoneList[16].ID, temRight, PBHlist[16]);
	SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, BoneList[19].ID, temRightThumb, PBHlist[19]);
	SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, BoneList[23].ID, temRightIndex, PBHlist[23]);
	SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, BoneList[27].ID, temRightMiddle, PBHlist[27]);
	SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, BoneList[31].ID, temRightRing, PBHlist[31]);
	SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, BoneList[35].ID, temRightPinky, PBHlist[35]);
	SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, BoneList[39].ID, temLeft, PBHlist[39]);
	SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, BoneList[42].ID, temLeftThumb, PBHlist[42]);
	SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, BoneList[46].ID, temLeftIndex, PBHlist[46]);
	SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, BoneList[50].ID, temLeftMiddle, PBHlist[50]);
	SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, BoneList[54].ID, temLeftRing, PBHlist[54]);
	SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, BoneList[58].ID, temLeftPinky, PBHlist[58]);
	for (int i = 8; i < Hint_Point_Length; i++)
	{
		if ((status[0] == i) || (status[1] == i))
			SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, Hint_Point_ID[i], Hint_Point_XYZ[i][1], Hint_Point_PBH[i][1]);
		else
			SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, Hint_Point_ID[i], Hint_Point_XYZ[i][0], Hint_Point_PBH[i][1]);
	}
	for (int i = 8; i < Hint_Point_Length; i++)
	{
		if ((status[0] == i) || (status[1] == i))
			SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, Hint_Point_HIGHLIGHT_ID[i], Hint_Point_HIGHLIGHT_XYZ[i][0], Hint_Point_HIGHLIGHT_PBH[i][1]);
		else
			SimConnect_AttachSimObjectToSimObject(hSimConnect, SIMCONNECT_OBJECT_ID_USER, attachxyz, attachpbh, Hint_Point_HIGHLIGHT_ID[i], Hint_Point_HIGHLIGHT_XYZ[i][1], Hint_Point_HIGHLIGHT_PBH[i][1]);
	}
}



int useind[12] = {
	16,19,23,27,31,35,39,42,46,50,54,58
};

bool noCreated()
{
	for (int i = 0; i < 12; i++)
	{
		if (!BoneList[useind[i]].isCreated())
		{
			return true;
		}

	}
	return false;
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
				if (noCreated()||(Hint_Point_ID.size()!=Hint_Point_Length)||(Hint_Point_HIGHLIGHT_ID.size()!=Hint_Point_Length))
				{
					IDNULL = true;
				}
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
			Sleep(15);
		}

		hr = SimConnect_Close(hSimConnect);
	}
}

DWORD WINAPI  calBoneObject(LPVOID pM)
{
	while (1)
	{
		calBone(XE, YE, ZE);
	}
	return 0;
}


DWORD WINAPI  hand_control(LPVOID pM)
{
	while (1)
	{

		record_flag_left = count_distance_right(temRightIndex.x, temRightIndex.y, temRightIndex.z);
		record_flag_right = count_distance_left(temLeftIndex.x, temLeftIndex.y, temLeftIndex.z);
		//cout << record_flag_left << endl;
		//cout << record_flag_right << endl;
		status[0] = record_flag_left;
		status[1] = record_flag_right;
		//status[0] = flag_target;
		Sleep(15);
	}
	return 0;
}


int __cdecl _tmain(int argc, _TCHAR* argv[])
{
	                                                                                                                                      
	HANDLE handle2 = CreateThread(NULL, 0, calBoneObject, NULL, 0, NULL);
	HANDLE handle1 = CreateThread(NULL, 0, hand_control, NULL, 0, NULL);
	testSimObjects();
	return 0;
}
#endif // DEBUG