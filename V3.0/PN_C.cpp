
#include"PN_C.h"

void PN_C::Init_NYT()
{
	USES_CONVERSION;
	char *add = (char*)"127.0.0.1";
	char *port = (char*)"7001";
	BRRegisterFrameDataCallback(this, bvhFrameDataReceived);
	sockTCPRef = BRConnectTo(add, atoi(port));
	cout << "NYT初始化完成" << endl;
}

void __stdcall PN_C::bvhFrameDataReceived(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data)
{
	//cout << "get data from NYT........." << endl;
	PN_C* pthis = (PN_C*)customedObj;
	pthis->showBvhBoneInfo(sender, header, data,pthis->data);
	if (pthis->first == true)
		pthis->first = false;
}


void PN_C::showBvhBoneInfo(SOCKET_REF sender, BvhDataHeader* header, float* data,float* dataptr)
{

	char strFrameIndex[60];

	int dataIndex = 0;

	int i = 0;

	////
	for (i = 0; i < 59; i++)
	{

		if (header->WithDisp)
		{

			dataIndex = i * 6;
			if (header->WithReference)
			{
				dataIndex += 6;
			}

			dataptr[dataIndex + 0] = data[dataIndex + 0];
			dataptr[dataIndex + 1] = data[dataIndex + 1];
			dataptr[dataIndex + 2] = data[dataIndex + 2];


			dataptr[dataIndex + 3] = data[dataIndex + 4];
			dataptr[dataIndex + 4] = data[dataIndex + 3];
			dataptr[dataIndex + 5] = data[dataIndex + 5];

		}
		else
		{

			if (i == 0)
			{
				dataIndex = 0;
				if (header->WithReference)
				{
					dataIndex += 6;
				}

				float dispX = data[dataIndex + 0];
				float dispY = data[dataIndex + 1];
				float dispZ = data[dataIndex + 2];

				float angX = data[dataIndex + 4];
				float angY = data[dataIndex + 3];
				float angZ = data[dataIndex + 5];

			}
			else
			{
				dataIndex = 3 + i * 3;
				if (header->WithReference)
				{
					dataIndex += 6;
				}

				float angX = data[dataIndex + 1];
				float angY = data[dataIndex + 0];
				float angZ = data[dataIndex + 2];

			}
		}
	}
	if (init_flag)
	{
		//cout << "cp" << endl;
		computeXYZ();
	}
}
