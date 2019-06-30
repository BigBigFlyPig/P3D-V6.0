//==========================================================================================================
//  Copyright (C) Lockheed Martin 2017
//==========================================================================================================
//  WindowCallback.cpp
//==========================================================================================================

#include "WorldDrawWindowCallback.h"
#include "PdkServices.h"
using namespace P3D;

//==========================================================================================================

WorldDrawWindowCallback::WorldDrawWindowCallback()
{
}

//==========================================================================================================

WorldDrawWindowCallback::~WorldDrawWindowCallback()
{
    ShutDown();
}

//==========================================================================================================

void WorldDrawWindowCallback::ShutDown()
{
    if (m_Window)
    {
        m_Window->RemovePlugin(this);
        m_Window = nullptr;
    }
}

//==========================================================================================================

void WorldDrawWindowCallback::OnPreCameraUpdate(IWindowV400* pWindow, ICameraSystemV400* pCamera)
{
    //Doesn't get hit for panel windows.
}

//==========================================================================================================

void WorldDrawWindowCallback::OnPostCameraUpdate(IWindowV400 * pWindow, ICameraSystemV400* pCamera)
{
    //Doesn't get hit for panel windows.
}

//==========================================================================================================

void WorldDrawWindowCallback::OnAdd(IWindowV400* pWindow, ICameraSystemV400* pCamera)
{

//#if _DEBUG
//    //This should never change, unless the window is closed.
//    //This confirms this is the case.
//    if (m_Window)
//    {
//        assert(m_Window == pWindow);
//    }
//#endif

    //Store the window we are added to.
    m_Window = pWindow;

    //Look up the active camera we care about, for panels Windows pCamera should be nullptr,
    //so look up the camera of the current active 3D View.
    //TODO: Make sure this is always the left or right eye camera for purposes of placing in 
    //VR window views.
    ICameraSystem* pActiveCamera = static_cast<ICameraSystem*>(pCamera);
    if (pActiveCamera == nullptr)
    {
        IWindow* pCurrentViewWindow = static_cast<IWindow*>(P3D::PdkServices::GetWindowPluginSystem()->GetCurrentWindow());
        if (pCurrentViewWindow && pCurrentViewWindow->GetCameraSystem())
        {
            pActiveCamera = static_cast<ICameraSystem*>(pCurrentViewWindow->GetCameraSystem());
        }
    }

    if (m_Window && pActiveCamera)
    {
        IWindow* pLatestWindowVersion = static_cast<IWindow*>(m_Window.p);

        static float fXoffset = 0.0f;
        static float fYoffset = 0.0f;
        static float fZoffset = 1.5f;

        static float width = 1.0f;
        static float height = 1.0f;

        ObjectWorldTransform drawLocation;

        //Offset vs origin will help us keep the object attached or not attached to the cameras
        //pbh axis.
        pActiveCamera->GetWorldTransform(drawLocation, CAMERA_REFERENCE_OFFSET);

        ObjectLocalTransform drawOffset(fXoffset, fYoffset, fZoffset, 0, 0, 0);

        PdkServices::GetObjectRenderer()->ApplyBodyRelativeOffset(drawLocation, drawOffset, drawLocation);

        pLatestWindowVersion->InitWorldSpaceDraw(drawLocation, width, height);
        pLatestWindowVersion->SetWorldSpaceFacingFlags(false, false, false);
        pLatestWindowVersion->SetWorldSpaceDepthMode(false, false);
    }
}

//==========================================================================================================

void WorldDrawWindowCallback::OnRemove(IWindowV400* pWindow, ICameraSystemV400* pCamera)
{
//#if _DEBUG
//    //Make sure these are the same.
//    assert(m_Window == pWindow);
//#endif
//
    //Our callback was removed from the window we were added to,
    //make sure to clean up our world space draw.
    if (m_Window != nullptr)
    {
        IWindow* pLatestWindowVersion = static_cast<IWindow*>(m_Window.p);
        pLatestWindowVersion->DeinitWorldSpaceDraw();
        m_Window = nullptr;
    }
}

//==========================================================================================================

void WorldDrawWindowCallback::OnViewChange(IWindowV400* pWindow, ICameraSystemV400* pCamera)
{
}

//==========================================================================================================

void WorldDrawWindowCallback::OnClose(IWindowV400* pWindow, ICameraSystemV400* pCamera)
{

//#if _DEBUG
//    //Make sure these are the same.
//    assert(m_Window == pWindow);
//#endif

    ShutDown();
}

//==========================================================================================================

void WorldDrawWindowCallback::UpdateWorldLocation(ICameraSystem* pCamera)
{
    IWindow* pLatestWindowVersion = static_cast<IWindow*>(m_Window.p);

    if (pLatestWindowVersion && pCamera && pCamera->IsHMDView() && pCamera->IsVirtualCockpit())
    {
        static float fXoffset = 0.0f;
        static float fYoffset = -0.5f;
        static float fZoffset = 0.5f;

        ObjectWorldTransform drawLocation;

        //Offset vs origin will help us keep the object attached or not attached to the cameras
        //pbh axis.
        pCamera->GetWorldTransform(drawLocation, CAMERA_REFERENCE_EYE);

        ObjectLocalTransform drawOffset(fXoffset, fYoffset, fZoffset, 0, 0, 0);

        PdkServices::GetObjectRenderer()->ApplyBodyRelativeOffset(drawLocation, drawOffset, drawLocation);

        pLatestWindowVersion->SetWorldSpacePosition(drawLocation);
    }
}

//==========================================================================================================

bool WorldDrawWindowCallback::IsStillValid() const
{
    return m_Window != nullptr;
}

//==========================================================================================================

IWindow* WorldDrawWindowCallback::GetWindow() const
{
    return static_cast<IWindow*>(m_Window.p);
}

//==========================================================================================================


void VRPanelRenderPlugin::OnPanelOpened(P3D::IParameterListV400* pParams)
{
    IUnknown* pIUnknown = (IUnknown*)pParams->GetParameter(1).Value;
    if (pIUnknown)
    {
        CComPtr<IWindow> panelWindow;
        if (SUCCEEDED(pIUnknown->QueryInterface(IID_IWindow, (void**)&panelWindow)))
        {
            UINT32 ident = 0;
            if (panelWindow->IsPanelWindow(&ident)) //&& ident == 25000)
            {
                LPCWSTR name = panelWindow->GetWindowName();
                if (name[0] && name[0] == '$' &&
                    name[1] && name[1] == 'H' &&
                    name[2] && name[2] == 'M' &&
                    name[3] && name[3] == 'D')
                {
                    //Create the world draw window call back to monitor and update our world space draw.
                    m_spPanelWindowPlugin = nullptr;
                    m_spPanelWindowPlugin.Attach(new WorldDrawWindowCallback());
                    panelWindow->AddPlugin(m_spPanelWindowPlugin);
                }
            }
        }
    }
}


void VRPanelRenderPlugin::OnCustomRender(P3D::IParameterListV400 * pParams)
{
    if (m_spPanelWindowPlugin != nullptr)
    {
        P3D::IWindowV400* pWindow = PdkServices::GetWindowPluginSystem()->GetCurrentWindow();
        if (pWindow)
        {
            P3D::ICameraSystemV400* pCamV400 = pWindow->GetCameraSystem();
            CComPtr<P3D::ICameraSystem> spCamV410;
            if (pCamV400)
            {
                if (SUCCEEDED(pCamV400->QueryInterface(IID_ICameraSystemV410, (void**)&spCamV410)))
                {
                    m_spPanelWindowPlugin->UpdateWorldLocation(spCamV410);
                }
            }
        }

    }
}
