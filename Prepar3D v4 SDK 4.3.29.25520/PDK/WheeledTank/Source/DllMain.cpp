//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//------------------------------------------------------------------------------
//
//  WheeledTank ISimObject (SimObject API) Example
//  DllMain.cpp
//  
//  Description:
//              DllMain
//------------------------------------------------------------------------------


/***********************************************************************************************
DLLMain
***********************************************************************************************/
#include <atlcomcli.h>
#include <InitGuid.h>

#include <Pdk.h>   //From Prepar3D SDK

#include "WheeledTank.h"

using namespace P3D;


/***********************************************************************************************
    Start
***********************************************************************************************/
void __stdcall DLLStart( __in __notnull IPdk* pPdk )
{  
    CComPtr<ISimObjectManagerV400> spSimObjectMgr;

    HRESULT hr = E_FAIL;
    
    if (!pPdk)
    {
        goto Error;
    }

    //Get the SimObject manager to register a simobject class and its create factory callback
    if (FAILED(pPdk->QueryService(SID_SimObjectManager, IID_ISimObjectManagerV400, (void**)&spSimObjectMgr)))
    {
        goto Error;
    }

    //Register class and factory function
    if (FAILED(spSimObjectMgr->RegisterSimulationCategory(CLSID_WheeledTank, TEXT("GroundVehicle"), WheeledTank::New)))
    {
        goto Error;
    } 

    if (FAILED(WheeledTank::RegisterProperties(spSimObjectMgr)))
    {
        goto Error;
    }

Error:

    return;
}

/***********************************************************************************************
    Stop
***********************************************************************************************/
void __stdcall DLLStop( void )
{
    //Clean up statics on module unload as necessary
}
