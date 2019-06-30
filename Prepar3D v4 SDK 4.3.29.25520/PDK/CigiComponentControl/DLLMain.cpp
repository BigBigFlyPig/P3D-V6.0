///----------------------------------------------------------------------------
///
///  Copyright 2018 Lockheed Martin Corporation 
///  Lockheed Martin Proprietary Information
///  
///  Author: Prepar3D Dev.
///  Description: This Example will use the CIGI PDK interface to create
///               sample Component Control packets
///                
///----------------------------------------------------------------------------
#include "initpdk.h"
#include "PdkPlugin.h"

using namespace P3D;

enum COMPONENT_ID
{
    COMPONENT_ID_HEALTH = 0x1000,
    COMPONENT_ID_DOORS,
};

///----------------------------------------------------------------------------
/// Component Control Callback
///----------------------------------------------------------------------------
class CompCtrlCallback : public IComponentControlCallbackV430
{
public:
    CompCtrlCallback() :
        m_RefCount(1),
        m_bDoorState(true), // Default to open.
        m_dLastHealthUpdate(0),
        m_dLastDoorUpdate(0)
    {
    }

    virtual HRESULT OnSend(IComponentControlV430& CompCtrl) override
    {
        double dTime = 0;

        // Send packets once every 5 seconds.
        int iUnitSeconds = 0;        
        if (SUCCEEDED(PdkServices::GetGlobalData()->GetUnitCode(L"Seconds", iUnitSeconds)) &&
            SUCCEEDED(PdkServices::GetGlobalData()->GetAbsoluteTime(dTime, iUnitSeconds)))
        {
            bool bUpdateHealth = (dTime - m_dLastHealthUpdate > 1.0);
            bool bUpdateDoors = (dTime - m_dLastDoorUpdate > 5.0);
            if (bUpdateHealth || bUpdateDoors)
            {
                // Gather entities around the user.
                CComPtr<IBaseObjectV400> spUser;
                if (SUCCEEDED(PdkServices::GetSimObjectManager()->GetUserObject(&spUser)))
                {
                    DXYZ dxyzPos, dxyzDummy;
                    if (SUCCEEDED(spUser->GetPosition(dxyzPos, dxyzDummy, dxyzDummy, dxyzDummy)))
                    {
                        UINT nObjects = 1000;
                        UINT aObjectIDs[1000];
                        if (SUCCEEDED(PdkServices::GetSimObjectManager()->GetObjectsInRadius(dxyzPos, 100000.0f, nObjects, aObjectIDs)))
                        {
                            // Send door component control packets.
                            if (bUpdateDoors)
                            {
                                CompCtrl.SetComponentID(COMPONENT_ID_DOORS);
                                CompCtrl.SetComponentClass(0);      // Entity Class
                                CompCtrl.SetComponentState(0);      // Unused for this sample.
                                CompCtrl.SetShortComponent(true);   // Set to Short Component Control. Doors will only use Word 0.

                                for (UINT i = 0; i < nObjects; ++i)
                                {
                                    UINT uObjectID = aObjectIDs[i];

                                    USHORT usEntityID;
                                    if (SUCCEEDED(PdkServices::GetCigiService()->GetEntityID(uObjectID, usEntityID)))
                                    {
                                        // Entity ID (based on Entity Class above).
                                        CompCtrl.SetInstanceID(usEntityID);

                                        // Each door is represented as one byte. All door states stored in the first word.
                                        CompCtrl.SetUCharData(m_bDoorState, 0, BYTE_POS_3);   // Door 1, most significant byte.
                                        CompCtrl.SetUCharData(m_bDoorState, 0, BYTE_POS_2);   // ...
                                        CompCtrl.SetUCharData(m_bDoorState, 0, BYTE_POS_1);   // ...
                                        CompCtrl.SetUCharData(m_bDoorState, 0, BYTE_POS_0);   // Door 4, least significant byte.

                                        // Inform P3D to queue the current component control packet.
                                        // Note: We are not clearing the data here, so the next object
                                        // iteration will only need to update door states and entity ID.
                                        CompCtrl.SendData();
                                    }
                                }

                                // Switch door state, reset timer.
                                m_bDoorState = !m_bDoorState;
                                m_dLastDoorUpdate = dTime;
                            }

                            // Send health/association ID component control packets.
                            if (bUpdateHealth)
                            {
                                // Clear previous door data if it was sent.
                                if (bUpdateDoors)
                                {
                                    CompCtrl.ClearData();
                                }

                                CompCtrl.SetComponentID(COMPONENT_ID_HEALTH);
                                CompCtrl.SetComponentClass(0);      // Entity Class
                                CompCtrl.SetComponentState(0);      // Unused for this sample.
                                CompCtrl.SetShortComponent(false);  // Set to use regular Component Control packet instead of Short.

                                for (UINT i = 0; i < nObjects; ++i)
                                {
                                    UINT uObjectID = aObjectIDs[i];

                                    USHORT usEntityID;
                                    if (SUCCEEDED(PdkServices::GetCigiService()->GetEntityID(uObjectID, usEntityID)))
                                    {
                                        // Entity ID (based on Entity Class above).
                                        CompCtrl.SetInstanceID(usEntityID);

                                        CComPtr<IBaseObjectV400> spObject;
                                        if (SUCCEEDED(PdkServices::GetSimObjectManager()->GetObject(uObjectID, &spObject)))
                                        {
                                            double dHealth = 1.0;
                                            double dAssociationID = 0;

                                            spObject->GetProperty(L"HEALTH POINTS", L"number", dHealth, 0);
                                            spObject->GetProperty(L"ASSOCIATION ID", L"number", dAssociationID, 0);
                                            
                                            CompCtrl.SetDoubleData(dHealth, 0);     // Double fills Words 0 and 1.
                                            CompCtrl.SetIntData((int)dAssociationID, 2); // Association ID stored in Word 2.

                                            // Inform P3D to queue the current component control packet.
                                            // Note: We are not clearing the data here, so the next object
                                            // iteration will only need to update health and association ID.
                                            CompCtrl.SendData();
                                        }
                                    }
                                }

                                // Reset timer.
                                m_dLastHealthUpdate = dTime;
                            }
                        }
                    }
                }
            }
        }

        return S_OK;
    }

    virtual HRESULT OnReceive(const IComponentControlV430& CompCtrl) override
    {
        // Determine Component Control packet type based on Component ID and Component Class.
        switch (CompCtrl.GetComponentID())
        {
            case COMPONENT_ID_HEALTH:
            {
                // Entity Class
                if (CompCtrl.GetComponentClass() == 0)
                {
                    // Lookup the ObjectID based on the packet provided CIGI EntityID.
                    UINT uObjectID;
                    if (SUCCEEDED(PdkServices::GetCigiService()->GetObjectID(CompCtrl.GetInstanceID(), uObjectID)))
                    {
                        CComPtr<IBaseObjectV400> spObject;
                        if (SUCCEEDED(PdkServices::GetSimObjectManager()->GetObject(uObjectID, &spObject)))
                        {
                            // Set received health points and association ID.
                            spObject->TriggerProperty(L"HEALTH POINTS", L"number", CompCtrl.GetDoubleData(0), 0);   // Double fills Words 0 and 1.
                            spObject->TriggerProperty(L"ASSOCIATION ID", L"number", CompCtrl.GetIntData(2), 0);     // Association ID stored in Word 2.
                        }
                    }
                }
                break;
            }
            case COMPONENT_ID_DOORS:
            {
                // Entity Class
                if (CompCtrl.GetComponentClass() == 0)
                {
                    // Lookup the ObjectID based on the packet provided CIGI EntityID.
                    UINT uObjectID;
                    if (SUCCEEDED(PdkServices::GetCigiService()->GetObjectID(CompCtrl.GetInstanceID(), uObjectID)))
                    {
                        CComPtr<IBaseObjectV400> spObject;
                        if (SUCCEEDED(PdkServices::GetSimObjectManager()->GetObject(uObjectID, &spObject)))
                        {
                            // Set received door states.
                            spObject->TriggerProperty(L"EXIT OPEN:0", L"bool", CompCtrl.GetUCharData(0, BYTE_POS_3), 0);
                            spObject->TriggerProperty(L"EXIT OPEN:1", L"bool", CompCtrl.GetUCharData(0, BYTE_POS_2), 1);
                            spObject->TriggerProperty(L"EXIT OPEN:2", L"bool", CompCtrl.GetUCharData(0, BYTE_POS_1), 2);
                            spObject->TriggerProperty(L"EXIT OPEN:3", L"bool", CompCtrl.GetUCharData(0, BYTE_POS_0), 3);
                        }
                    }
                }
                break;
            }
        }

        return S_OK;
    }

    DEFAULT_REFCOUNT_INLINE_IMPL()
    DEFAULT_IUNKNOWN_QI_INLINE_IMPL(IComponentControlCallbackV430, IID_IComponentControlCallbackV430)

private:

    bool m_bDoorState;
    double m_dLastDoorUpdate;
    double m_dLastHealthUpdate;

};

///----------------------------------------------------------------------------
/// Component Control Plugin
///----------------------------------------------------------------------------
class CompCtrlPlugin
{
public:

    void Init()
    {
        m_spCallback.Attach(new CompCtrlCallback());
        PdkServices::GetCigiService()->RegisterComponentControlCallback(m_spCallback);
    }

    void Deinit()
    {
        PdkServices::GetCigiService()->UnregisterComponentControlCallback(m_spCallback);
    }

private:

    CComPtr<CompCtrlCallback> m_spCallback;

};

///----------------------------------------------------------------------------
/// Prepar3D DLL start and end entry points
///----------------------------------------------------------------------------
static CompCtrlPlugin* s_pCompCtrlPlugin = nullptr;

extern "C" __declspec(dllexport) void __stdcall DLLStart(__in __notnull IPdk* pPdk)
{
    PdkServices::Init(pPdk);

    s_pCompCtrlPlugin = new CompCtrlPlugin();
    s_pCompCtrlPlugin->Init();
}

extern "C" __declspec(dllexport) void __stdcall DLLStop(void)
{
    if (s_pCompCtrlPlugin != nullptr)
    {
        s_pCompCtrlPlugin->Deinit();
        s_pCompCtrlPlugin = nullptr;
    }

    PdkServices::Shutdown();
}
