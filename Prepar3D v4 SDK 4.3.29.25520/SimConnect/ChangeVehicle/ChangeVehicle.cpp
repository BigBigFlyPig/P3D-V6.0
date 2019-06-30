//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//----------------------------------------------------------------------------
//
//  SimConnect Change Vehicle sample
//
//  
//  Description:  Sample code for changing the Vehicle Prepar3D is running
//                through SimConnect.
//
//-----------------------------------------------------------------------------

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include "SimConnect.h"
#include <strsafe.h>

HANDLE hSimConnect = NULL;

void ChangeVehicle()
{
	HRESULT hr;

	if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Change Vehicle", NULL, 0, 0, 0)))
	{
		// Sending the title of the vehicle
		SimConnect_ChangeVehicle(hSimConnect, "Maule M7 260C paint2");

		hr = SimConnect_Close(hSimConnect);
	}
}

int __cdecl _tmain(int argc, _TCHAR* argv[])
{
	ChangeVehicle();

	return 0;
}
