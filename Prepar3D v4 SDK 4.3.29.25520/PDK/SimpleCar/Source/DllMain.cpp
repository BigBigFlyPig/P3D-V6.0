/************************************************************************************************************** 
    DLLMain.cpp

    Copyright (C) Lockheed Martin Corporation.  All rights reserved.
    
    Description: SimpleCar sample ISimObject DLL entry.
***************************************************************************************************************/

#include <atlcomcli.h>
#include <InitGuid.h>

//From Prepar3D SDK
#include <Pdk.h>   
#include <ISimObject.h>

using namespace P3D;

extern REFGUID GetClassId();
extern HRESULT New(__in __notnull IBaseObjectV400* pBaseObject, __out __notnull ISimObject** ppThisSim);
extern HRESULT RegisterProperties(__in __notnull ISimObjectManagerV400* pSimObjectMgr);


/***********************************************************************************************
***********************************************************************************************/
STDMETHODIMP_(void) DLLStart(__in __notnull IPdk* pPdk)
{      
    CComPtr<ISimObjectManagerV400> spSimObjectMgr;  //Smart pointer reference to SimObject Manager

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

    //Register class and factory callback function
    if (FAILED(spSimObjectMgr->RegisterSimulationCategory(GetClassId(),     //Unique class Guid
                                                          TEXT("GroundVehicle"),  //Friendly classification
                                                          New)))            //Factory function
    {
        goto Error;
    }  

    if (FAILED(RegisterProperties(spSimObjectMgr)))
    {
        goto Error;
    }

Error:

    return;
}

/***********************************************************************************************
***********************************************************************************************/

STDMETHODIMP_(void) DLLStop()
{
    //Clean up statics on module unload as necessary
}

/***********************************************************************************************
***********************************************************************************************/