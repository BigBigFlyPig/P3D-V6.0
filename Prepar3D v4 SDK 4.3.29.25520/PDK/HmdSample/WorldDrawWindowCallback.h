//==========================================================================================================
//  Copyright (C) Lockheed Martin 2017
//==========================================================================================================
//  WorldDrawWindowCallback.h
//==========================================================================================================

#pragma once

//==========================================================================================================

#include <atlcomcli.h>
#include "Helpers\IUnknownHelper.h"
#include "IWindowPluginSystem.h"
#include "Helpers\WindowPlugin.h"
#include "IWindowPluginSystem.h"
#include "PdkPlugin.h"
class WorldDrawWindowCallback;
class VRPanelRenderPlugin : public P3D::PdkPlugin
{
public:
    VRPanelRenderPlugin() : PdkPlugin() {}
private:
    void OnPanelOpened(P3D::IParameterListV400* pParams);
    void OnCustomRender(P3D::IParameterListV400* pParams);
    void OnMessage(P3D::IParameterListV400* pParams)
    {
        UINT64 messageID = pParams->GetParameter(0).Value;
        switch (messageID)
        {
        case EVENT_MESSAGE_PANEL_OPENED:
            OnPanelOpened(pParams);
            break;
        }

        P3D::PdkPlugin::OnMessage(pParams);
    }
    CComPtr<WorldDrawWindowCallback> m_spPanelWindowPlugin;
};

//==========================================================================================================
/**
Window callback which handles logic for updating window draw object each frame
*/
class WorldDrawWindowCallback : public P3D::WindowPlugin
{
public:

    WorldDrawWindowCallback();

    virtual ~WorldDrawWindowCallback();

    //Shutdown the callback, remove itself from window.
    void ShutDown();

    //See if the callbacks window is still active.
    bool IsStillValid() const;

    //Get the associated window.
    P3D::IWindow* GetWindow() const;

    virtual void OnPreCameraUpdate(P3D::IWindowV400* pWindow, P3D::ICameraSystemV400* pCamera)  override;
    virtual void OnPostCameraUpdate(P3D::IWindowV400* pWindow, P3D::ICameraSystemV400* pCamera)  override;
    virtual void OnAdd(P3D::IWindowV400* pWindow, P3D::ICameraSystemV400* pCamera) override;
    virtual void OnRemove(P3D::IWindowV400* pWindow, P3D::ICameraSystemV400* pCamera) override;
    virtual void OnViewChange(P3D::IWindowV400* pWindow, P3D::ICameraSystemV400* pCamera) override;
    virtual void OnClose(P3D::IWindowV400* pWindow, P3D::ICameraSystemV400* pCamera) override;

    void UpdateWorldLocation(P3D::ICameraSystem* pCamera);

private:

    //Window current associated with this callback.
    CComPtr<P3D::IWindowV400> m_Window;

    /** Prevent copy construction and assignment */
    NO_COPY_IUNKNOWN_IMPL(WorldDrawWindowCallback)
};

//==========================================================================================================