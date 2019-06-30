/************************************************************************************************************** 
    DLLMain.cpp

    Copyright (C) Lockheed Martin Corporation.  All rights reserved.
    
    Description: Countermeasure sample ISimObject DLL entry.
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

CComPtr<ISimObjectManagerV400> s_spSimObjectMgr;  //Smart pointer reference to SimObject Manager

const ISimObjectManagerV400* GetSimObjectManager() {return s_spSimObjectMgr;}

/***********************************************************************************************
***********************************************************************************************/
STDMETHODIMP_(void) DLLStart(__in __notnull IPdk* pPdk)
{      
    HRESULT hr = E_FAIL;
    
    if (!pPdk)
    {
        goto Error;
    }

    //Get the SimObject manager to register a simobject class and its create factory callback
    if (FAILED(pPdk->QueryService(SID_SimObjectManager, IID_ISimObjectManagerV400, (void**)&s_spSimObjectMgr)))
    {
        goto Error;
    }

    //Register class and factory callback function
    if (FAILED(s_spSimObjectMgr->RegisterSimulationCategory(GetClassId(),     //Unique class Guid
                                                          TEXT("Countermeasure"), //Friendly classification
                                                          New)))            //Factory function
    {
        goto Error;
    }  

    if (FAILED(RegisterProperties(s_spSimObjectMgr)))
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
