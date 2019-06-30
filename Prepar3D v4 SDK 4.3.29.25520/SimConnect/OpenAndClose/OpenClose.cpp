//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//------------------------------------------------------------------------------
//
//  SimConnect Open and Close Sample
//  
//	Description:
//				Opens and immediately Closes SimConnect
//------------------------------------------------------------------------------

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "SimConnect.h"
#include <strsafe.h>

HANDLE  hSimConnect = NULL;

void testOpenClose()
{
    HRESULT hr;

    if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Open and Close", NULL, 0, 0, 0)))
    {
        printf("\nConnected to Prepar3D!");   
    
        hr = SimConnect_Close(hSimConnect);

        printf("\nDisconnected from Prepar3D");
    } else
		printf("\nFailed to connect to Prepar3D");
}

int __cdecl _tmain(int argc, _TCHAR* argv[])
{
    testOpenClose();

    return 0;
}





