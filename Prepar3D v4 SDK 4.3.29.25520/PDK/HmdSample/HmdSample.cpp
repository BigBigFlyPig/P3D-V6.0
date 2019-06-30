///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) Lockheed Martin Corporation 2017
// All rights reserved
//
// HmdSample.cpp
//
///////////////////////////////////////////////////////////////////////////////

#include "HmdSample.h"
#include "WorldDrawWindowCallback.h"
///////////////////////////////////////////////////////////////////////////////
//
// HmdPanelCallback
//
///////////////////////////////////////////////////////////////////////////////
DEFINE_PANEL_CALLBACK_REFCOUNT(HmdPanelCallback);

///////////////////////////////////////////////////////////////////////////////
//
// HmdPanelCallback
// Constructor
//
///////////////////////////////////////////////////////////////////////////////
HmdPanelCallback::HmdPanelCallback()
    : m_RefCount(1)
{
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdPanelCallback
// ConvertStringToProperty
//
///////////////////////////////////////////////////////////////////////////////
bool HmdPanelCallback::ConvertStringToProperty(LPCSTR keyword, SINT32* pID)
{
    if (!keyword)
    {
        return false;
    }
    if (!pID)
    {
        return false;
    }

    UINT uNumProperties;
    const PROPERTY_TABLE *parPropertyTable = GetPropertyTable(uNumProperties);

    for (UINT i = 0; i < uNumProperties; i++)
    {
        if (_stricmp(parPropertyTable[i].szPropertyName, keyword) == 0)
        {
            *pID = i;
            return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdPanelCallback
// ConvertPropertyToString
//
///////////////////////////////////////////////////////////////////////////////
bool HmdPanelCallback::ConvertPropertyToString(SINT32 id, LPCSTR* pKeyword)
{
    if (!pKeyword)
    {
        return false;
    }

    UINT uNumProperties;
    const PROPERTY_TABLE *parPropertyTable = GetPropertyTable(uNumProperties);

    if (id < 0 || id >= (SINT32)uNumProperties)
    {
        return false;
    }
    *pKeyword = parPropertyTable[id].szPropertyName;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdPanelCallback
// GetPropertyUnits
//
///////////////////////////////////////////////////////////////////////////////
bool HmdPanelCallback::GetPropertyUnits(SINT32 id, ENUM* pEnum)
{
    if (!pEnum)
    {
        return false;
    }

    UINT uNumProperties;
    const PROPERTY_TABLE *parPropertyTable = GetPropertyTable(uNumProperties);

    if (id < 0 || id >= (SINT32)uNumProperties)
    {
        return false;
    }

    *pEnum = parPropertyTable[id].units;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdPanelCallback
// CreateAircraftCCallback
//
///////////////////////////////////////////////////////////////////////////////
IAircraftCCallback * HmdPanelCallback::CreateAircraftCCallback(UINT32 ContainerID)
{
    return new HmdAircraftCallback(ContainerID);
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdPanelCallback
// GetPropertyTable
//
///////////////////////////////////////////////////////////////////////////////
const PROPERTY_TABLE *HmdPanelCallback::GetPropertyTable(UINT &uLength)
{
    uLength = LENGTHOF(HMD_PROPERTY_TABLE);
    return HMD_PROPERTY_TABLE;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdAircraftCallback
//
///////////////////////////////////////////////////////////////////////////////
DEFINE_PANEL_CALLBACK_REFCOUNT(HmdAircraftCallback);

///////////////////////////////////////////////////////////////////////////////
//
// HmdAircraftCallback
// Constructor
//
///////////////////////////////////////////////////////////////////////////////
HmdAircraftCallback::HmdAircraftCallback(UINT32 containerId) :
    m_containerId(containerId), 
    m_RefCount(1)
{
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdAircraftCallback
// CreateGaugeCCallback
//
///////////////////////////////////////////////////////////////////////////////
IGaugeCCallback* HmdAircraftCallback::CreateGaugeCCallback()
{
    return new HmdGaugeCallback(GetContainerId());
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
//
///////////////////////////////////////////////////////////////////////////////
DEFINE_PANEL_CALLBACK_REFCOUNT(HmdGaugeCallback)

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// Constructor
//
///////////////////////////////////////////////////////////////////////////////
HmdGaugeCallback::HmdGaugeCallback(UINT32 containerId) :
    m_RefCount(1), 
    m_containerId(containerId),
    m_bActive(true),
    m_fCameraPitch(0),
    m_fCameraBank(0),
    m_fCameraHeading(0),
    m_fHFov(0.0f),
    m_fVFov(0.0f),
    m_fZoom(0.0f),
    m_bHasTarget(false),
    m_fTargetX(0.0f),
    m_fTargetY(0.0f),
    m_fTargetAlt(0.0f),
    m_fTargetAspectAngle(0.0f),
    m_fTargetDistance(0.0f)
{
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// Destructor
//
///////////////////////////////////////////////////////////////////////////////
HmdGaugeCallback::~HmdGaugeCallback()
{
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// Update
//
///////////////////////////////////////////////////////////////////////////////
void HmdGaugeCallback::Update()
{
    UpdateVisibility();
    if (m_bActive)
    {
        UpdatePbh();
        UpdateTarget();
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// UpdateVisibility
//
///////////////////////////////////////////////////////////////////////////////
void HmdGaugeCallback::UpdateVisibility()
{
    bool bVisible = false;
    WindowList windowList;
    PdkServices::GetWindowPluginSystem()->GetWindowList(windowList);
    for (size_t i = 0; i < windowList.Items.size(); i++)
    {
        CComPtr<IWindowV400> curWindow = windowList.Items[i];
        if (curWindow->IsMainAppWindow() && 
            curWindow->GetCameraSystem() && 
            curWindow->GetCameraSystem()->IsVirtualCockpit())
        {
            bVisible = true;
            break;
        }
    }
    m_bActive = bVisible;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// UpdatePbh
//
///////////////////////////////////////////////////////////////////////////////
void HmdGaugeCallback::UpdatePbh()
{
    WindowList windowList;

    PdkServices::GetWindowPluginSystem()->GetWindowList(windowList);
    for (size_t i = 0; i < windowList.Items.size(); i++)
    {
        CComPtr<IWindowV400> curWindow = windowList.Items[i];
        if (curWindow->IsMainAppWindow())
        {
            if (curWindow)
            {
                P3D::ICameraSystemV400* pCamV400 = curWindow->GetCameraSystem();
                CComPtr<P3D::ICameraSystemV410> spCamV410;
                if (pCamV400)
                {
                    if (SUCCEEDED(pCamV400->QueryInterface(IID_ICameraSystemV410, (void**)&spCamV410)))
                    {
                        ObjectWorldTransform origin;
                        spCamV410->GetWorldTransform(origin, CAMERA_REFERENCE_ORIGIN);
                        spCamV410->GetWorldTransform(m_CameraTransform, CAMERA_REFERENCE_EYE);
                        ObjectLocalTransform offset;
                        PdkServices::GetObjectRenderer()->CalculateBodyRelativeOffset(origin, m_CameraTransform, offset);
                        m_fCameraPitch = offset.PBH.Pitch;
                        m_fCameraBank = offset.PBH.Bank;
                        m_fCameraHeading = offset.PBH.Heading;
                    }
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// UpdateTarget
//
///////////////////////////////////////////////////////////////////////////////
void HmdGaugeCallback::UpdateTarget()
{
    CComPtr<IBaseObjectV400> pUserObject;
    PdkServices::GetSimObjectManager()->GetUserObject(&pUserObject);

    if (pUserObject != nullptr)
    {
        CComPtr<IFireControlSystem> spFireControlSystem;
        pUserObject->QueryService(SID_FireControlSystem, IID_IFireControlSystem, (void**)&spFireControlSystem);

        if (spFireControlSystem != nullptr)
        {
            double lat;
            double lon;
            double alt;

            UINT uTargetId = spFireControlSystem->GetSelectedTargetID();
            if (uTargetId != 0)
            {
                m_bHasTarget = true;

                P3DDXYZ vUserLLA;
                P3DDXYZ vUserPBH;
                P3DDXYZ vUserLLAvel;
                P3DDXYZ vUserPBHvel;

                pUserObject->GetPosition(vUserLLA, vUserPBH, vUserLLAvel, vUserPBHvel);

                CComPtr<IBaseObjectV400> pTargetObject;
                PdkServices::GetSimObjectManager()->GetObject(uTargetId, &pTargetObject);

                P3DDXYZ vTargetLLA;
                P3DDXYZ vTargetPBH;
                P3DDXYZ vTargetLLAvel;
                P3DDXYZ vTargetPBHvel;

                pTargetObject->GetPosition(vTargetLLA, vTargetPBH, vTargetLLAvel, vTargetPBHvel);
                
                m_TargetTransform.LLA.Latitude = RAD_TO_DEG(vTargetLLA.dZ);
                m_TargetTransform.LLA.Longitude = RAD_TO_DEG(vTargetLLA.dX);
                m_TargetTransform.LLA.Altitude = FEET_TO_METERS(vTargetLLA.dY);
                m_TargetTransform.PBH.Pitch = (float)RAD_TO_DEG(vTargetPBH.dX);
                m_TargetTransform.PBH.Bank = (float)RAD_TO_DEG(vTargetPBH.dZ);
                m_TargetTransform.PBH.Heading = (float)RAD_TO_DEG(vTargetPBH.dY);


                lat = vTargetLLA.dZ;
                lon = vTargetLLA.dX;
                alt = vTargetLLA.dY;

                lat = RAD_TO_DEG(lat);
                lon = RAD_TO_DEG(lon);
                m_fTargetAlt = (float)alt;
                m_fTargetAspectAngle = RAD_TO_DEG(vTargetPBH.dY - vUserPBH.dY);

                double deltaLatitude_radians = vTargetLLA.dZ - vUserLLA.dZ;
                double deltaLongitude_radians = vTargetLLA.dX - vUserLLA.dX;
                double deltaAltitude_feet = vTargetLLA.dY - vUserLLA.dY;

                P3D::DXYZ relativePosition;
                relativePosition.dX = deltaLongitude_radians * RADIUS_OF_EARTH_AT_EQUATOR * cos(vTargetLLA.dZ);
                relativePosition.dY = deltaAltitude_feet;//lonAltLat_radians.dY;
                relativePosition.dZ = deltaLatitude_radians  * RADIUS_OF_EARTH_AT_EQUATOR;
                double distance = 0.0;

                distance = sqrt((relativePosition.dX * relativePosition.dX) + (relativePosition.dY * relativePosition.dY) + (relativePosition.dZ * relativePosition.dZ));
                m_fTargetDistance = (float)distance;


                IWindowV400* pWindow = PdkServices::GetWindowPluginSystem()->GetCurrentWindow();
                if (pWindow->IsMainAppWindow())
                {
                    if (pWindow->GetCameraSystem()->IsHMDView())
                    {
                        ObjectLocalTransform transToTarget;
                        PdkServices::GetObjectRenderer()->CalculateBodyRelativeOffset(m_CameraTransform, m_TargetTransform, transToTarget);
                        m_fTargetX = 0.5f + transToTarget.XYZ.X * 0.5f / transToTarget.XYZ.Z;
                        m_fTargetY = 0.5f - transToTarget.XYZ.Y * 0.5f / transToTarget.XYZ.Z;
                    }
                    else
                    {
                        ScreenCoord sc;
                        PdkServices::GetWindowPluginSystem()->GetCurrentWindow()->GetCameraSystem()->GetScreenCoord(m_TargetTransform.LLA, sc);
                        unsigned int w, h;
                        
                        pWindow->GetSize(w, h);
                        if (h != 0 && w != 0)
                        {
                            if (w > h)
                            {
                                m_fTargetX = (sc.XPixels - ((w - h) / 2)) / (float)h;
                                m_fTargetY = sc.YPixels / (float)h;
                            }
                            else
                            {
                                m_fTargetX = sc.XPixels / (float)w;
                                m_fTargetY = (sc.YPixels - ((h - w) / 2)) / (float)w;
                            }
                        }
                    }
                }
            }
            else
            {
                m_bHasTarget = false;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// IsActive
//
///////////////////////////////////////////////////////////////////////////////
bool HmdGaugeCallback::IsActive()
{
    return m_bActive;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// SetActive
//
///////////////////////////////////////////////////////////////////////////////
void HmdGaugeCallback::SetActive(bool bActive)
{
    m_bActive = bActive;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// GetPitch
//
///////////////////////////////////////////////////////////////////////////////
float HmdGaugeCallback::GetPitch()
{
    return m_fCameraPitch;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// GetBank
//
///////////////////////////////////////////////////////////////////////////////
float HmdGaugeCallback::GetBank()
{
    return m_fCameraBank;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// GetHeading
//
///////////////////////////////////////////////////////////////////////////////
float HmdGaugeCallback::GetHeading()
{
    return m_fCameraHeading;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// GetHFov
//
///////////////////////////////////////////////////////////////////////////////
float HmdGaugeCallback::GetHFov()
{
    return m_fHFov;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// GetVFov
//
///////////////////////////////////////////////////////////////////////////////
float HmdGaugeCallback::GetVFov()
{
    return m_fVFov;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// GetZoom
//
///////////////////////////////////////////////////////////////////////////////
float HmdGaugeCallback::GetZoom()
{
    return m_fZoom;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// HasTarget
//
///////////////////////////////////////////////////////////////////////////////
bool HmdGaugeCallback::HasTarget()
{
    return m_bHasTarget;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// GetTargetX
//
///////////////////////////////////////////////////////////////////////////////
float HmdGaugeCallback::GetTargetX()
{
    return m_fTargetX;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// GetTargetY
//
///////////////////////////////////////////////////////////////////////////////
float HmdGaugeCallback::GetTargetY()
{
    return m_fTargetY;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// GetTargetAlt
//
///////////////////////////////////////////////////////////////////////////////
float HmdGaugeCallback::GetTargetAlt()
{
    return m_fTargetAlt;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// GetTargetAspectAngle
//
///////////////////////////////////////////////////////////////////////////////
float HmdGaugeCallback::GetTargetAspectAngle()
{
    return m_fTargetAspectAngle;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// GetTargetDistance
//
///////////////////////////////////////////////////////////////////////////////
float HmdGaugeCallback::GetTargetDistance()
{
    return m_fTargetDistance;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// GetPropertyValue
//
///////////////////////////////////////////////////////////////////////////////
bool HmdGaugeCallback::GetPropertyValue(SINT32 id, FLOAT64* pValue)
{
    if (!pValue)
    {
        return false;
    }

    *pValue = 1.0;      // Start with a reasonable default

    HMD_VAR eHmdVar = (HMD_VAR)id;
    switch (eHmdVar)
    {
        case HmdActive:
            *pValue = IsActive();
            break;
        case HmdPitch:
            *pValue = GetPitch();
            break;
        case HmdBank:
            *pValue = GetBank();
            break;
        case HmdHeading:
            *pValue = GetHeading();
            break;
        case HmdHFov:
            *pValue = GetHFov();
            break;
        case HmdVFov:
            *pValue = GetVFov();
            break;
        case HmdZoom:
            *pValue = GetZoom();
            break;
        case HmdTarget:
            *pValue = HasTarget();
            break;
        case HmdTargetX:
            *pValue = GetTargetX();
            break;
        case HmdTargetY:
            *pValue = GetTargetY();
            break;
        case HmdTargetAlt:
            *pValue = GetTargetAlt();
            break;
        case HmdTargetAspectAngle:
            *pValue = GetTargetAspectAngle();
            break;
        case HmdTargetDistance:
            *pValue = GetTargetDistance();
            break;
        default:
            return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// SetPropertyValue
//
///////////////////////////////////////////////////////////////////////////////
bool HmdGaugeCallback::SetPropertyValue(SINT32 id, FLOAT64 value)
{
    HMD_VAR eHmdVar = (HMD_VAR)id;
    switch (eHmdVar)
    {
        case HmdActive:
            SetActive(value != 0.0);
            break;
        default:
            return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// HmdGaugeCallback
// QueryInterface
//
///////////////////////////////////////////////////////////////////////////////
IGaugeCCallback* HmdGaugeCallback::QueryInterface(LPCSTR pszInterface)
{
    if (strncmp(pszInterface, ISERIALIZABLE_GAUGECCALLBACK_NAME, strlen(ISERIALIZABLE_GAUGECCALLBACK_NAME)) == 0)
    {
        return this;
    }
    return NULL;
}

static VRPanelRenderPlugin* pVRPanelRenderPlugin = nullptr;

///////////////////////////////////////////////////////////////////////////////
//
// DLLStart
//
///////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport) void __stdcall DLLStart(__in __notnull IPdk* pPdk)
{
    if (pPdk != nullptr)
    {
        // create panel callback passing in the radar service pointer
        HmdPanelCallback * pPanelCallback = new HmdPanelCallback();

        PdkServices::Init(pPdk);
        PdkServices::GetPanelSystem()->PanelRegisterCCallback(HMD_SAMPLE_CALLBACK_NAME, pPanelCallback);
        pVRPanelRenderPlugin = new VRPanelRenderPlugin();
        // clean up local ref counted resources
        pPanelCallback->Release();
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////
//
// DLLStop
//
///////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport) void __stdcall DLLStop(void)
{
    // unregister the callback
    PdkServices::GetPanelSystem()->PanelRegisterCCallback(HMD_SAMPLE_CALLBACK_NAME, nullptr);
    delete pVRPanelRenderPlugin;
    PdkServices::Shutdown();
}
