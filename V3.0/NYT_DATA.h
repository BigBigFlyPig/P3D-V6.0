#pragma once
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


class GKD {
public:
	float data[59 * 6];
	int f_ptr[59] = {
		-1, //hip
		0,1,2, //right leg
		0,4,5, //left leg
		0,7,8,9,10,11,//body
		10,13,14,15,//right hand
		16,17,18,
		16,20,21,22,
		16,24,25,26,
		16,28,29,30,
		16,32,33,34, //right fingers
		10,36,37,38, //left hand
		39,40,41,
		39,43,44,45,
		39,47,48,49,
		39,51,52,53,
		39,55,56,57
	};
	PN_C* myPNC;
	glm::vec4 startpoint = glm::vec4(0.0, 0.0, 0.0, 1.0);
	vector<glm::mat4> Bone_Tran_Mat;
	vector<glm::mat4> BVH_Mat;
	GKD(PN_C* PNC)
	{
		myPNC = PNC;
		myPNC->Init(data);
		while (myPNC->first == true)
		{
			Sleep(10);
		}
		for (int i = 0; i < 59; i++)
		{
			glm::vec3 pos;
			for (int j = 0; j < 3; j++)
			{
				pos[j] = data[i * 6 + j];
			}
			glm::mat4 tra(1.0f);
			tra = glm::translate(tra, pos);
			Bone_Tran_Mat.push_back(tra);
		}
		BVH_Mat = vector<glm::mat4>(59, glm::mat4(1.0f));
		cout << Bone_Tran_Mat.size() << endl;
		cout << "GKD初始化完成" << endl;
	}
	glm::vec4 computeXYZ()
	{
		for (int i = 0; i < 59; i++)
		{
			glm::mat4 tem(1.0f);
			if (i==0)
			{
				glm::vec3 angle;
				for (int j = 0; j < 3; j++)
				{
					angle[j] = data[i * 6 + 3 + j];
				}
				tem = glm::rotate(tem, glm::radians(angle[1]), glm::vec3(0.0, 1.0f, 0.0));
				tem = glm::rotate(tem, glm::radians(angle[0]), glm::vec3(1.0, 0.0f, 0.0));
				tem = glm::rotate(tem, glm::radians(angle[2]), glm::vec3(0.0, 0.0f, 1.0));
				tem = Bone_Tran_Mat[i] * tem;
				BVH_Mat[i] = tem;
			}
			else
			{
				glm::vec3 angle;
				for (int j = 0; j < 3; j++)
				{
					angle[j] = data[i * 6 + 3 + j];
				}
				tem = glm::rotate(tem, glm::radians(angle[1]), glm::vec3(0.0, 1.0f, 0.0));
				tem = glm::rotate(tem, glm::radians(angle[0]), glm::vec3(1.0, 0.0f, 0.0));
				tem = glm::rotate(tem, glm::radians(angle[2]), glm::vec3(0.0, 0.0f, 1.0));
				tem = Bone_Tran_Mat[i] * tem;
				BVH_Mat[i] = BVH_Mat[f_ptr[i]]*tem;
			}
		}
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
		float x_angle = 0;//angle == radins
		float y_angle = 0;
		float z_angle = 0;
		//z_angle = atan((R[1][0] / R[1][1]));
		//y_angle = atan((R[0][2] / R[2][2]));
		//x_angle = asin(-R[1][2]);

		////cout << "x:"<<x_angle * 180.0 / PI << endl;
		////cout << "y:"<<y_angle * 180.0 / PI << endl;
		////cout << "z:"<<z_angle * 180.0 / PI << endl;
		////getchar();
		SIMCONNECT_DATA_PBH res;
		res.Bank = 90.0*PI / 180.0 - z_angle;
		res.Heading = 0.0 - y_angle;
		res.Pitch = 0.0 + x_angle;
		//res.Bank = z_angle;
		//res.Heading = y_angle;
		//res.Pitch = x_angle;
		return res;
	}
};


#endif // !__GKD__
