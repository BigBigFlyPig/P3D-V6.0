#ifdef __GKD__
#define __GKD__

#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtc/type_ptr.hpp"
#include"PN_C.h"
#include<vector>
#include<windows.h>
#include "SimConnect.h"
#include <math.h>
#define PI 3.14
using namespace std;


//同步问题
class GKD {
public:
	float data[59*6];
	PN_C* myPNC;
	glm::vec4 startpoint = glm::vec4(0.0, 0.0, 0.0, 1.0);

	int testlength = 2;
	int TestBoneList[2] = { 7,0 };

	int testlength1 = 1;
	int TestBoneList1[1] = { 0 };

	int BoneListLength = 7;
	int BoneList[7] = {
	12,11,10,9,8,7,0
	};

	//左手小指Pinky
	int LeftHandPinkyListLength = 13;
	int LeftHandPinkyList[13] = {
		58,57,56,55,39,38,37,36,10,9,8,7,0
	};

	//左手无名指Ring
	int LeftHandRingListLength = 13;
	int LeftHandRingList[13] = {
		54,53,52,51,39,38,37,36,10,9,8,7,0
	};

	//左手中指Middle
	int LeftHandMiddleListLength = 13;
	int LeftHandMiddleList[13] = {
		50,49,48,47,39,38,37,36,10,9,8,7,0
	};

	//左手食指Index
	int LeftHandIndexListLength = 13;
	int LeftHandIndexList[13] = {
		46,45,44,43,39,38,37,36,10,9,8,7,0
	};

	//左手拇指Thumb
	int LeftHandThumbListLength = 12;
	int LeftHandThumbList[12] = {
		42,41,40,39,38,37,36,10,9,8,7,0
	};


	int LeftHandListLength = 9;
	int LeftHandList[9] = {

		39,38,37,36,10,9,8,7,0
	};


	int RightHandListLength = 9;
	int RightHandList[9] = {
	
		16,15,14,13,10,9,8,7,0
	};


	//右手小指Pinky
	int RightHandPinkyListLength = 13;
	int RightHandPinkyList[13] = {
		35,34,33,32,16,15,14,13,10,9,8,7,0
	};

	//右手无名指Ring
	int RightHandRingListLength = 13;
	int RightHandRingList[13] = {
		31,30,29,28,16,15,14,13,10,9,8,7,0
	};

	//右手中指Middle
	int RightHandMiddleListLength = 13;
	int RightHandMiddleList[13] = {
		27,26,25,24,16,15,14,13,10,9,8,7,0
	};

	//右手食指Index
	int RightHandIndexListLength = 13;
	int RightHandIndexList[13] = {
		23,22,21,20,16,15,14,13,10,9,8,7,0
	};

	//右手拇指Thumb
	int RightHandThumbListLength = 12;
	int RightHandThumbList[12] = {
		19,18,17,16,15,14,13,10,9,8,7,0
	};




	GKD(PN_C* PNC)
	{
		myPNC = PNC;
		myPNC->Init(data);
		cout << "GKD初始化完成" << endl;
	}
	glm::vec4 computeXYZ(int ind,glm::vec4 posn)
	{
		
		glm::mat4 rotate = glm::mat4(1.0f);
		glm::mat4 transform = glm::mat4(1.0f);
		glm::vec3 transpos = glm::vec3(0.0);
		glm::vec4 pos = posn;
		glm::vec4 XYZ = glm::vec4(0.0f);


		float angle[3];


		for (int i = 0; i < 3; i++)
		{
			//if (i == 0)
				//cout << data[ind * 6 + 3 + i] << endl;;
			transpos[i] = data[ind * 6 + i];

			angle[i] = data[ind * 6 + 3 + i];
		}
		
		transform = glm::translate(transform, transpos);

		rotate = glm::rotate(rotate, glm::radians(angle[1]), glm::vec3(0.0f, 1.0f, 0.0f));
		rotate = glm::rotate(rotate, glm::radians(angle[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		rotate = glm::rotate(rotate, glm::radians(angle[2]), glm::vec3(0.0f, 0.0f, 1.0f));
		XYZ = transform * rotate *  pos;


		return XYZ;
	}
	glm::vec4 resHeadPos()
	{
		glm::vec4 res = this->startpoint;


		for (int i = 0; i<BoneListLength; i++)
		{	
			res = computeXYZ(BoneList[i], res);
		}
		return res;
	
	}
	//计算左手小指
	glm::vec4 resLeftHandPinkyPos()
	{
		glm::vec4 res = this->startpoint;

		for (int i = 0; i < 13; i++)
		{
			res = computeXYZ(LeftHandPinkyList[i], res);
		}
		return res;

	}

	//计算左手无名指
	glm::vec4 resLeftHandRingPos()
	{
		glm::vec4 res = this->startpoint;

		for (int i = 0; i < 13; i++)
		{
			res = computeXYZ(LeftHandRingList[i], res);
		}
		return res;

	}

	glm::vec4 resLeftHandMiddlePos()
	{
		glm::vec4 res = this->startpoint;

		for (int i = 0; i < 13; i++)
		{
			res = computeXYZ(LeftHandMiddleList[i], res);
		}
		return res;

	}

	glm::vec4 resLeftHandIndexPos()
	{
		glm::vec4 res = this->startpoint;

		for (int i = 0; i < 13; i++)
		{
			res = computeXYZ(LeftHandIndexList[i], res);
		}
		return res;

	}

	glm::vec4 resLeftHandThumbPos()
	{
		glm::vec4 res = this->startpoint;

		for (int i = 0; i < 12; i++)
		{
			res = computeXYZ(LeftHandThumbList[i], res);
		}
		return res;

	}



	glm::vec4 resLeftHandPos()
	{
		glm::vec4 res = this->startpoint;

		for (int i = 0; i <9 ; i++)
		{
			res = computeXYZ(LeftHandList[i], res);
		}
		return res;

	}
	glm::vec4 resRightHandPos()
	{
		glm::vec4 res = this->startpoint;

		for (int i = 0; i < 9 ; i++)
		{
			res = computeXYZ(RightHandList[i], res);
		}
		return res;

	}

	///
	//计算右手小指
	glm::vec4 resRightHandPinkyPos()
	{
		glm::vec4 res = this->startpoint;

		for (int i = 0; i < 13; i++)
		{
			res = computeXYZ(RightHandPinkyList[i], res);
		}
		return res;

	}

	//计算右手无名指
	glm::vec4 resRightHandRingPos()
	{
		glm::vec4 res = this->startpoint;

		for (int i = 0; i < 13; i++)
		{
			res = computeXYZ(RightHandRingList[i], res);
		}
		return res;

	}

	glm::vec4 resRightHandMiddlePos()
	{
		glm::vec4 res = this->startpoint;

		for (int i = 0; i < 13; i++)
		{
			res = computeXYZ(RightHandMiddleList[i], res);
		}
		return res;

	}

	glm::vec4 resRightHandIndexPos()
	{
		glm::vec4 res = this->startpoint;

		for (int i = 0; i < 13; i++)
		{
			res = computeXYZ(RightHandIndexList[i], res);
		}
		return res;

	}

	glm::vec4 resRightHandThumbPos()
	{
		glm::vec4 res = this->startpoint;

		for (int i = 0; i < 12; i++)
		{
			res = computeXYZ(RightHandThumbList[i], res);
		}
		return res;

	}


	///

	glm::vec4 TestPos()
	{
		glm::vec4 res = this->startpoint;

		//res = computeXYZ(BoneList[6], res);

		for (int i = 0; i < testlength; i++)
		{
			res = computeXYZ(TestBoneList[i], res);
		}
		return res;

	}
	glm::vec4 TestPos1()
	{
		glm::vec4 res = this->startpoint;

		//res = computeXYZ(BoneList[6], res);

		for (int i = 0; i < testlength1; i++)
		{
			res = computeXYZ(TestBoneList1[i], res);
		}
		return res;

	}
	SIMCONNECT_DATA_PBH getPBH(int ind)
	{
		//int LeftHandIndexListLength = 13;
		//int LeftHandIndexList[13] = {
		//	46,45,44,43,39,38,37,36,10,9,8,7,0
		//};
		glm::mat4 R(1.0);
		for (int i = 0; i < LeftHandIndexListLength; i++)
		{
			glm::mat4 rot(1.0f);
			int finger_index = LeftHandIndexList[i] * 6;
			rot = glm::rotate(rot, glm::radians(data[finger_index + 4]), glm::vec3(0.0, 1.0, 0.0));
			rot = glm::rotate(rot, glm::radians(data[finger_index + 3]), glm::vec3(1.0, 0.0, 0.0));
			rot = glm::rotate(rot, glm::radians(data[finger_index + 5]), glm::vec3(0.0, 0.0, 1.0));
			R = rot * R;
		}
		float x_angle=0;//angle == radins
		float y_angle=0;
		float z_angle=0;
		//z_angle = atan((R[1][0] / R[1][1]));
		//y_angle = atan((R[0][2] / R[2][2]));
		//x_angle = asin(-R[1][2]);

		////cout << "x:"<<x_angle * 180.0 / PI << endl;
		////cout << "y:"<<y_angle * 180.0 / PI << endl;
		////cout << "z:"<<z_angle * 180.0 / PI << endl;
		////getchar();
		SIMCONNECT_DATA_PBH res;
		res.Bank = 90.0*PI / 180.0 -z_angle;
		res.Heading = 0.0-y_angle;
		res.Pitch = 0.0+x_angle;
		//res.Bank = z_angle;
		//res.Heading = y_angle;
		//res.Pitch = x_angle;
		return res;
	}
};
/*
vector<vector<float>> build(GKD &gkd)
{
	vector<glm::vec4> BoneList(59, glm::vec4(0.0f));



}
*/
/*
int main()
{
	PN_C pnc;
	GKD gkd(&pnc);
	while (1)
	{
		//for (int i = 0; i < 6; i++)
		//	cout << gkd.data[12 * 6 + i] << endl;
		//cout << "head" << endl;
		glm::vec4 resr = gkd.resRightHandPos();
		//glm::vec4 res = gkd.computeXYZ(12,gkd.startpoint);
		cout << "当前坐标右手：" << endl;
		cout << resr[0] << endl;
		cout << resr[1] << endl;
		cout << resr[2] << endl;
		glm::vec4 resl = gkd.resLeftHandPos();
		//glm::vec4 res = gkd.computeXYZ(12,gkd.startpoint);
		cout << "当前坐标左手：" << endl;
		cout << resl[0] << endl;
		cout << resl[1] << endl;
		cout << resl[2] << endl;
		//for (int i = 0; i < 4; i++)
		//{
			
		//	cout << res[i] << endl;
		//}
		Sleep(5000);
	}
	
	getchar();
	return 0;

}
*/

#endif // !__GKD__
