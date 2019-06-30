///----------------------------------------------------------------------------
///
///  Copyright 2010 Lockheed Martin Corporation 
///  Lockheed Martin Proprietary Information
///  
///  Author: Prepar3D Dev.
///  Description: This Example will use the Camera PDK interface to create
///               new camera view and manipulate using the PDK functionalities
///                
///----------------------------------------------------------------------------
#include "initpdk.h"
#include "PdkPlugin.h"

#include <string>

using namespace P3D;

static const float s_fMinPitch = -90.0f;
static const float s_fMaxPitch = 90.0f;
static const float s_fMinHeading = -180.0f;
static const float s_fMaxHeading = 180.0f;
static const float s_fMinPosOffset = -0.5f;
static const float s_fMaxPosOffset = 0.5f;
static const float s_fMinZ = -1.0f;
static const float s_fMaxZ = 1.0f;
static const float s_fMinZoom = 0.3f;
static const float s_fMaxZoom = 50.3f;
static const float s_fMoveStep = 0.05f;
static const double s_dLlaStep = 0.001;
static const UINT s_uMaxTargets = 10;
static const float s_fEntityRadius = 20000.0f;

enum TARGET_TYPE
{
    TARGET_TYPE_NONE,
    TARGET_TYPE_LOC,
    TARGET_TYPE_AI,
};

///----------------------------------------------------------------------------
/// KeyHandler CLASS
/// Helper class to manage key states.
///----------------------------------------------------------------------------
class KeyHandler
{
public:

    KeyHandler(int iKey) :
        m_iKey(iKey),
        m_bIsDown(false),
        m_bIsRepeat(false)
    {
    }

    bool IsDown() const { return m_bIsDown; }
    bool IsRepeat() const { return m_bIsRepeat; }

    void Update()
    {
        bool bIsDownNow = ((GetAsyncKeyState(m_iKey) & 0x8000) != 0);

        if (m_bIsDown && bIsDownNow)
        {
            m_bIsRepeat = true;
        }
        else if (bIsDownNow)
        {
            m_bIsDown = true;
        }
        else
        {
            m_bIsDown = false;
            m_bIsRepeat = false;
        }
    }

private:

    int m_iKey;
    bool m_bIsDown;
    bool m_bIsRepeat;
};

///----------------------------------------------------------------------------
/// Window Update CALLBACK CLASS
/// This callback class gets invoked everytime a window is updated.
///----------------------------------------------------------------------------
class WindowCallback : public IWindowPluginV400
{
public:

    WindowCallback() :
        m_RefCount(1),
        m_fPitch(0.0f),
        m_fBank(0.0f),
        m_fHeading(0.0f),
        m_fX(0.0f),
        m_fY(0.0f),
        m_fZ(0.0f),
        m_fZoom(0.3f),
        m_dLat(0.0),
        m_dLon(0.0),
        m_dAlt(0.0),
        m_uTargetId(0),
        m_uTargetIndex(0),
        m_uNumNearbyEntities(0),
        m_eTargetType(TARGET_TYPE_NONE),
        m_bPanCamera(false),
        m_TabKey(VK_TAB),
        m_ReturnKey(VK_RETURN)
    {
        for (int i = 0; i < s_uMaxTargets; i++)
        {
            m_entities[i] = 0;
        }
    }

    /* IWindowPluginV400 */
    virtual void OnAdd(IWindowV400* pWindow, ICameraSystemV400* pCamera) override { }
    virtual void OnRemove(IWindowV400* pWindow, ICameraSystemV400* pCamera) override { }
    virtual void OnPreCameraUpdate(IWindowV400* pWindow, ICameraSystemV400* pCamera) override;
    virtual void OnPostCameraUpdate(IWindowV400* pWindow, ICameraSystemV400* pCamera) override { }
    virtual void OnViewChange(IWindowV400* pWindow, ICameraSystemV400* pCamera) override { }
    virtual void OnClose(IWindowV400* pWindow, ICameraSystemV400* pCamera) override { }
    virtual bool OnUserInput(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam) override { return false; }

    DEFAULT_REFCOUNT_INLINE_IMPL()
    DEFAULT_IUNKNOWN_QI_INLINE_IMPL(IWindowPlugin, IID_IWindowPlugin)

private:

    float m_fPitch;
    float m_fBank;
    float m_fHeading;

    float m_fX;
    float m_fY;
    float m_fZ;

    float m_fZoom;

    double m_dLat;
    double m_dLon;
    double m_dAlt;

    bool m_bPanCamera;

    KeyHandler m_TabKey;
    KeyHandler m_ReturnKey;

    UINT m_uTargetId;
    UINT m_uTargetIndex;
    UINT m_uNumNearbyEntities;
    UINT m_entities[s_uMaxTargets];

    TARGET_TYPE m_eTargetType;

    void GetNearbyEntities(ICameraSystemV400* pCamera);
    void TargetEntity(ICameraSystemV400* pCamera);
};

void WindowCallback::GetNearbyEntities(ICameraSystemV400* pCamera)
{
    if (!pCamera)
    {
        return;
    }

    DXYZ curLLA;

    pCamera->GetLLARadians(curLLA.dZ, curLLA.dX, curLLA.dY);
    m_uNumNearbyEntities = s_uMaxTargets;

    PdkServices::GetSimObjectManager()->GetObjectsInRadius(curLLA, s_fEntityRadius, m_uNumNearbyEntities, m_entities);
}

void WindowCallback::TargetEntity(ICameraSystemV400* pCamera)
{
    if (!pCamera)
    {
        return;
    }

    if (m_uNumNearbyEntities > 0 && m_uNumNearbyEntities > m_uTargetIndex)
    {
        pCamera->SetTargetContainerId(m_entities[m_uTargetIndex]);
    }
}

/// Use the WindowPdk interface to resize the current window
void WindowCallback::OnPreCameraUpdate(IWindowV400* pWindow, ICameraSystemV400* pCamera)
{
    if (!pWindow || !pCamera)
    {
        return;
    }

    m_TabKey.Update();
    m_ReturnKey.Update();

    bool bShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
    bool bControlPressed = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
    
    bool bUpPressed = (GetAsyncKeyState(VK_UP) & 0x8000) != 0;
    bool bDownPressed = (GetAsyncKeyState(VK_DOWN) & 0x8000) != 0;
    bool bLeftPressed = (GetAsyncKeyState(VK_LEFT) & 0x8000) != 0;
    bool bRightPressed = (GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0;

    if (m_TabKey.IsDown() && !m_TabKey.IsRepeat())
    {
        pCamera->DeactivatePositionTracking();
        pCamera->DeactivateEntityTracking();

        switch (m_eTargetType)
        {
            // Switching to position targeting
            case TARGET_TYPE_NONE:
            {
                pCamera->ActivatePositionTracking();
                pCamera->GetLLA(m_dLat, m_dLon, m_dAlt);
                m_dLat += 0.001;
                m_dLon += 0.001;
                pCamera->SetTargetLatLonAltDegrees(m_dLat, m_dLon, m_dAlt);
                m_eTargetType = TARGET_TYPE_LOC;
                break;
            }
            // Switching to entity targeting
            case TARGET_TYPE_LOC:
            {
                GetNearbyEntities(pCamera);
                if (m_uNumNearbyEntities > 0)
                {
                    pCamera->ActivateEntityTracking();
                    TargetEntity(pCamera);
                }
                m_eTargetType = TARGET_TYPE_AI;
                break;
            }
            // Switching to standard mode
            case TARGET_TYPE_AI:
            {
                m_eTargetType = TARGET_TYPE_NONE;
                break;
            }
        }
    }

    if (m_eTargetType == TARGET_TYPE_NONE)
    {
        if (m_ReturnKey.IsDown() && !m_ReturnKey.IsRepeat())
        {
            m_bPanCamera = !m_bPanCamera;
        }

        if (m_bPanCamera)
        {
            m_fHeading++;

            if (m_fHeading > 360.0f)
            {
                m_fHeading -= 360.0f;
            }
        }

        if (bUpPressed && !bControlPressed)
        {
            if (bShiftPressed)
            {
                m_fZ += s_fMoveStep;
                if (m_fZ > s_fMaxPosOffset)
                {
                    m_fZ = s_fMaxPosOffset;
                }
            }
            else
            {
                m_fPitch--;
                if (m_fPitch < s_fMinPitch)
                {
                    m_fPitch = s_fMinPitch;
                }
            }
        }
        if (bDownPressed && !bControlPressed)
        {
            if (bShiftPressed)
            {
                m_fZ -= s_fMoveStep;
                if (m_fZ < s_fMinPosOffset)
                {
                    m_fZ = s_fMinPosOffset;
                }
            }
            else
            {
                m_fPitch++;
                if (m_fPitch > s_fMaxPitch)
                {
                    m_fPitch = s_fMaxPitch;
                }
            }
        }
        if (bLeftPressed)
        {
            if (bShiftPressed)
            {
                m_fX -= s_fMoveStep;
                if (m_fX < s_fMinPosOffset)
                {
                    m_fX = s_fMinPosOffset;
                }
            }
            else
            {
                m_fHeading--;
                if (m_fHeading < s_fMinHeading)
                {
                    m_fHeading = s_fMinHeading;
                }
            }
        }
        if (bRightPressed)
        {
            if (bShiftPressed)
            {
                m_fX += s_fMoveStep;
                if (m_fX > s_fMaxPosOffset)
                {
                    m_fX = s_fMaxPosOffset;
                }
            }
            else
            {
                m_fHeading++;
                if (m_fHeading > s_fMaxHeading)
                {
                    m_fHeading = s_fMaxHeading;
                }
            }
        }
        pCamera->SetRelative6DOF(m_fX, m_fY, m_fZ, m_fPitch, m_fBank, m_fHeading);
    }
    else if (m_eTargetType == TARGET_TYPE_LOC)
    {
        if (bUpPressed && !bControlPressed)
        {
            m_dLat += s_dLlaStep;
        }
        if (bDownPressed && !bControlPressed)
        {
            m_dLat -= s_dLlaStep;
        }
        if (bLeftPressed)
        {
            m_dLon -= s_dLlaStep;
        }
        if (bRightPressed)
        {
            m_dLon += s_dLlaStep;
        }
        pCamera->SetTargetLatLonAltDegrees(m_dLat, m_dLon, m_dAlt);
    }
    else // TARGET_TYPE_AI
    {
        if (bLeftPressed)
        {
            if (m_uTargetIndex > 0)
            {
                m_uTargetIndex--;
                GetNearbyEntities(pCamera);
                if (m_uNumNearbyEntities > 0)
                {
                    pCamera->ActivateEntityTracking();
                    TargetEntity(pCamera);
                }
            }
        }
        if (bRightPressed)
        {
            if (m_uTargetIndex < s_uMaxTargets)
            {
                m_uTargetIndex++;
                GetNearbyEntities(pCamera);
                if (m_uNumNearbyEntities > 0)
                {
                    pCamera->ActivateEntityTracking();
                    TargetEntity(pCamera);
                }
            }
        }
    }

    if (bUpPressed && bControlPressed)
    {
        if (m_fZoom < 1.0f)
        {
            m_fZoom += 0.1f;
        }
        else
        {
            m_fZoom += 1.0f;
        }

        if (m_fZoom > s_fMaxZoom)
        {
            m_fZoom = s_fMaxZoom;
        }
    }
    else if (bDownPressed && bControlPressed)
    {
        if (m_fZoom <= 1.0f)
        {
            m_fZoom -= 0.1f;
        }
        else
        {
            m_fZoom -= 1.0f;
        }

        if (m_fZoom < s_fMinZoom)
        {
            m_fZoom = s_fMinZoom;
        }
    }

    pCamera->SetZoomGoal(m_fZoom);
}

///----------------------------------------------------------------------------
/// Prepar3D Loaded Handler CLASS
///----------------------------------------------------------------------------
class SystemReadyCallback : public ICallbackV400
{
public:
    SystemReadyCallback() :
        m_RefCount(1)
    {
    }

    virtual void Invoke(IParameterListV400* pParams) override;

    DEFAULT_REFCOUNT_INLINE_IMPL()
    DEFAULT_IUNKNOWN_QI_INLINE_IMPL(ICallbackV400, IID_ICallbackV400)

private:

    CComPtr<WindowCallback> m_spWindowCallback;
};

/// Use the Plugin Pdk to register for windows callback or create a new Window.
void SystemReadyCallback::Invoke(IParameterListV400* pParams)
{
    if (!pParams)
    {
        return;
    }

    const static wchar_t* S_NAME = L"Virtual Cockpit - View 00";

    // This callback indicate that Prepar3D finish loading
    UINT64 messageID = pParams->GetParameter(0).Value;
    if (messageID == EVENT_MESSAGE_LOADING_COMPLETE)
    {
        // Get the window plugin system 
        CComPtr<IWindowPluginSystem> spPluginSystem = PdkServices::GetWindowPluginSystem();

        if (spPluginSystem != NULL)
        {
            m_spWindowCallback.Attach(new WindowCallback());

            // Check if the current flight already have this camera open
            if (spPluginSystem->HasWindow(S_NAME))
            {
                // Register for window update callback
                spPluginSystem->RegisterInternalWindow(S_NAME, m_spWindowCallback);
            }
            else
            {
                // Create new window and register for callback
                spPluginSystem->CreateInternalWindow(S_NAME,       // Window's Name
                                                     100,          // Starting x
                                                     100,          // Starting y
                                                     200,          // Width
                                                     200,          // Height
                                                     m_spWindowCallback);    // On update Cb
            }
        }
    }
    else if (messageID == EVENT_MESSAGE_APPLICATION_SHUTDOWN)
    {
        PdkServices::GetEventService()->UnregisterCallback(EVENTID_Message, this);

        PdkServices::GetWindowPluginSystem()->UnRegisterInternalWindow(S_NAME, m_spWindowCallback);
        m_spWindowCallback = NULL;
    }
}

///----------------------------------------------------------------------------
/// Prepar3D DLL start and end entry points
///----------------------------------------------------------------------------
static SystemReadyCallback* s_pSystemReadyCallback = nullptr;

extern "C" __declspec(dllexport) void __stdcall DLLStart(__in __notnull IPdk* pPdk)
{
    PdkServices::Init(pPdk);

    s_pSystemReadyCallback = new SystemReadyCallback();

    PdkServices::GetEventService()->RegisterCallback(EVENTID_Message, s_pSystemReadyCallback);
}

extern "C" __declspec(dllexport) void __stdcall DLLStop(void)
{
    if (s_pSystemReadyCallback != nullptr)
    {
        s_pSystemReadyCallback = nullptr;
    }

    PdkServices::Shutdown();
}
