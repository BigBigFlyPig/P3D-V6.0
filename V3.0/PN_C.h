#pragma once
#pragma once
#ifndef _PNC_
#define _PNC_
#include<iostream>
#include"DataType.h"
#include"NeuronDataReader.h"
#include<windows.h>
#include<atlconv.h>
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtc/type_ptr.hpp"
#include"PN_C.h"
#include<vector>
#include "SimConnect.h"
#include <math.h>


//GKD GKD GKD TOU

using namespace std;

class PN_C {
private:
	const float PI = 3.14;

	float BIAS_X = (0.25); //(0.25)(1.05)(0.0)
	float BIAS_Y = (1.05);
	float BIAS_Z = (-0.1);

	const float ERROR_X = (-0.01); //
	const float ERROR_Y = (-0.01); //
	const float ERROR_Z = (0.01); //

	const float TARGET_X = 0.079;
	const float TARGET_Y = -0.184;
	const float TARGET_Z = -0.486;

public:
	bool first = false;
	bool init_flag = false;
	void Init_NYT();
	static void __stdcall bvhFrameDataReceived(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data);
	SOCKET_REF sockTCPRef;
	void showBvhBoneInfo(SOCKET_REF sender, BvhDataHeader* header, float* data, float* dataptr);
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
	vector<glm::mat4> Bone_Tran_Mat = vector<glm::mat4>(59, glm::mat4(1.0f));
	vector<glm::mat4> Bone_Rot_Mat = vector<glm::mat4>(59, glm::mat4(1.0f));
	vector<glm::mat4> BVH_Mat = vector<glm::mat4>(59, glm::mat4(1.0f));
	void Init_Data()
	{

		Sleep(5000);
		
		for (int i = 0; i < 59; i++)
		{
			glm::vec3 pos;
			for (int j = 0; j < 3; j++)
			{
				pos[j] = data[i * 6 + j];
			}
			glm::mat4 tra(1.0f);
			tra = glm::translate(tra, pos);
			Bone_Tran_Mat[i] = (tra);
		}
		cout << "GKD初始化完成" << endl;
		init_flag = true;
	}
	void computeXYZ()
	{
		for (int i = 0; i < 59; i++)
		{
			glm::mat4 tem(1.0f);
			glm::mat4 rotate(1.0f);
			if (i == 0)
			{
				glm::vec3 angle;
				for (int j = 0; j < 3; j++)
				{
					angle[j] = data[i * 6 + 3 + j];
				}
				tem = glm::rotate(tem, glm::radians(angle[1]), glm::vec3(0.0, 1.0f, 0.0));
				tem = glm::rotate(tem, glm::radians(angle[0]), glm::vec3(1.0, 0.0f, 0.0));
				tem = glm::rotate(tem, glm::radians(angle[2]), glm::vec3(0.0, 0.0f, 1.0));
				rotate = tem;
				tem = Bone_Tran_Mat[i] * tem;
				BVH_Mat[i] = tem;
				Bone_Rot_Mat[i] = rotate;

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
				rotate = tem;
				tem = Bone_Tran_Mat[i] * tem;
				BVH_Mat[i] = BVH_Mat[f_ptr[i]] * tem;
				Bone_Rot_Mat[i] = Bone_Rot_Mat[f_ptr[i]] * rotate;
			}
		}
	}
	SIMCONNECT_DATA_XYZ getXYZ(unsigned int ind,glm::vec4 XYZ = glm::vec4(0.0,0.0,0.0,1.0f))
	{
		XYZ = BVH_Mat[ind] * XYZ;
		SIMCONNECT_DATA_XYZ res;
		res.x = XYZ[0]*ERROR_X+BIAS_X;
		res.y = XYZ[1]*ERROR_Y+BIAS_Y;
		res.z = XYZ[2]*ERROR_Z+BIAS_Z;
		return res;
	}
	SIMCONNECT_DATA_PBH getPBH(unsigned int ind)
	{

		SIMCONNECT_DATA_PBH res;
		res.Bank = 0.0;
		res.Pitch = 0.0;
		res.Heading = 0.0;

		SIMCONNECT_DATA_XYZ forward = getXYZ(ind, glm::vec4(100.0, 0.0, 0.0f, 1.0f));
		SIMCONNECT_DATA_XYZ org = getXYZ(ind);
		glm::vec3 vec(forward.x - org.x, forward.y - org.y, forward.z - org.z);

		{
			res.Bank = asin(vec[1]);
		}
		

		if (vec[2] > 0.0)
		{
			res.Heading = acos(vec[0] / sqrtf(1 - vec[1] * vec[1]));
		}
		else
		{
				res.Heading = -acos(vec[0] / sqrtf(1 - vec[1] * vec[1]));
			
		}
		res.Heading = -res.Heading;
		return res;
	}
	void reSetBias() // 16 IS THE INDEX OF RIGHT HAND  
	{
		SIMCONNECT_DATA_XYZ hand_pos = getXYZ(16);
		glm::vec3 dir(TARGET_X - hand_pos.x, TARGET_Y - hand_pos.y, TARGET_Z - hand_pos.z);
		BIAS_X += (dir[0]+0.018);
		BIAS_Y += dir[1];
		BIAS_Z += dir[2];
		return;
	
	}
	PN_C()
	{
		this->Init_NYT();
		this->Init_Data();
	}
};

#endif // !_PNC_
