///----------------------------------------------------------------------------
///
///  Copyright 2016 Lockheed Martin Corporation 
///  Lockheed Martin Proprietary Information
///  
///  Author: Prepar3D Dev.
///  Description: This example uses the PDK to render custom PDK Objects.
///                
///----------------------------------------------------------------------------
#include "initpdk.h"
#include <WinError.h>
#include "PdkPlugin.h"

#include <string>

#define PI                  3.141592653589793238462643
#define PI_OVER_2           (PI / 2)
#define TWO_PI              (2*PI)
#define RadToDeg(rad)       ((rad) * (360/TWO_PI))
#define FeetToMeters(feet)  ((feet)*(0.3048))

using namespace P3D;

class CustomPDKObjectsPlugin : public PdkPlugin
{
public:

    CustomPDKObjectsPlugin() : PdkPlugin()
    {
        m_bDrawGroundObjectGrid = true;
        m_bDrawUserSimObjects = true;
        m_uOneSecondTick = 0;

        m_spMenuTop.Attach(P3D::PdkServices::GetMenuService()->CreateMenuItem());
        m_spMenuTop->SetType(P3D::MenuTypePdk::MENU_ITEM);
        m_spMenuTop->SetText(L"Custom PDK Objects");
        P3D::PdkServices::GetMenuService()->AddItem(m_spMenuTop->GetId(), NO_PARENT, 0);

        m_spMenuDrawGrid.Attach(P3D::PdkServices::GetMenuService()->CreateMenuItem());
        m_spMenuDrawGrid->SetType(P3D::MenuTypePdk::MENU_CHECK_ITEM);
        m_spMenuDrawGrid->SetChecked(true);
        m_spMenuDrawGrid->SetText(L"Draw Ground Object Grid");
        CustomPDKObjectsPlugin::MenuCallback * callback1 = new CustomPDKObjectsPlugin::MenuCallback(DRAW_GRID);
        m_spMenuDrawGrid->RegisterCallback(callback1);
        P3D::PdkServices::GetMenuService()->AddItem(m_spMenuDrawGrid->GetId(), m_spMenuTop->GetId(), 0);

        m_spMenuDrawObjs.Attach(P3D::PdkServices::GetMenuService()->CreateMenuItem());
        m_spMenuDrawObjs->SetType(P3D::MenuTypePdk::MENU_CHECK_ITEM);
        m_spMenuDrawObjs->SetChecked(true);
        m_spMenuDrawObjs->SetText(L"Draw User Sim Objects");
        CustomPDKObjectsPlugin::MenuCallback * callback2 = new CustomPDKObjectsPlugin::MenuCallback(DRAW_OBJS);
        m_spMenuDrawObjs->RegisterCallback(callback2);
        P3D::PdkServices::GetMenuService()->AddItem(m_spMenuDrawObjs->GetId(), m_spMenuTop->GetId(), 0);

    }

    ///----------------------------------------------------------------------------
    ///  PdkPlugin Callback overrides
    ///----------------------------------------------------------------------------

    virtual void OnCustomRender(IParameterListV400* pParams) override
    {
        // Get the Object Renderer service from the callback params
        CComPtr<IObjectRendererV400>    spRenderService = NULL;
        if (SUCCEEDED(pParams->GetServiceProvider()->QueryService(
            SID_ObjectRenderer, IID_IObjectRendererV400, (void**)&spRenderService)))
        {
            if (m_bDrawUserSimObjects)
            {
                DrawUserSimObjects(spRenderService, pParams->GetServiceProvider());
            }
            if (m_bDrawGroundObjectGrid)
            {
                DrawGroundObjectGrid(spRenderService);
            }
        }
    }

    virtual void OnOneHz(IParameterListV400* pParams) override
    {
        // keep track count of seconds. This is used for alternating object color.
        m_uOneSecondTick++;
    }

protected:

    ///----------------------------------------------------------------------------
    ///  Object drawing functions
    ///----------------------------------------------------------------------------

    void DrawUserSimObjects(IObjectRendererV400* pRenderer, IServiceProvider* pServiceProvider)
    {
        CComPtr<IWindowV400> spWindow = NULL;
        CComPtr<IBaseObjectV400> spUserObject = NULL;
        ObjectWorldTransform objTrans, cameraTrans;

        // Get curent window
        spWindow = PdkServices::GetWindowPluginSystem()->GetCurrentWindow();
        if (spWindow != nullptr && spWindow->GetCameraSystem() != nullptr)
        {
            // get lla and pbh of camera (degrees/meters)
            spWindow->GetCameraSystem()->GetLLA(cameraTrans.LLA.Latitude, cameraTrans.LLA.Longitude, cameraTrans.LLA.Altitude);
            spWindow->GetCameraSystem()->GetPBH(cameraTrans.PBH.Pitch, cameraTrans.PBH.Bank, cameraTrans.PBH.Heading);
        }

        // Get user sim from sim object manager
        PdkServices::GetSimObjectManager()->GetUserObject((IBaseObjectV400**)&spUserObject);
        if (spUserObject != nullptr)
        {
            // get position and orientation and convert from radians/feet to degrees/meters
            P3D::P3DDXYZ objLal, objPhb, lalVel, pbhVel = { 0 };
            spUserObject->GetPosition(objLal, objPhb, lalVel, pbhVel);
            objTrans.LLA.Latitude = RadToDeg(objLal.dZ);
            objTrans.LLA.Longitude = RadToDeg(objLal.dX);
            objTrans.LLA.Altitude = FeetToMeters(objLal.dY);
            objTrans.PBH.Pitch = (float)RadToDeg(objPhb.dX);
            objTrans.PBH.Bank = (float)RadToDeg(objPhb.dZ);
            objTrans.PBH.Heading = (float)RadToDeg(objPhb.dY);
        }

        //Draw a few objects around and near user.
        DrawSimObjects(pRenderer, objTrans, cameraTrans);
    }

    void DrawGroundObjectGrid(IObjectRendererV400* pRenderer)
    {
        // coordinates from v3.4 default flight ( Eglin AFB - KVPS )
        ObjectWorldTransform transform(30.499298038645236,
            -86.511296988903155,
            29.508712768554688,
            0.0f, 0.0f, 0.0f);

        // draw a 10x10x5 grid of objects
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                for (int k = 0; k < 5; k++)
                {
                    //The color and size will change based on one second tick.
                    int group = m_uOneSecondTick % 10;
                    float size = 5;

                    //Opaque objects will instance together for better performance.
                    ARGBColor color(255, 255, 0, 0);
                    if (i == group)
                    {
                        size = 30;
                        color = ARGBColor(64, 0, 255, 0);
                    }

                    // grid is 2km by 2km by 100m
                    float x = -1000.0f + i * 2000.0f / 10.0f;
                    float y = k * 100.0f / 5.0f;
                    float z = -1000.0f + j * 2000.0f / 10.0f;

                    ObjectLocalTransform locationOffset(x, y, z, 0, 0, 0);
                    ObjectWorldTransform location;
                    pRenderer->ApplyBodyRelativeOffset(transform, locationOffset, location);

                    pRenderer->DrawSphere(location, size, color);
                }
            }
        }
    }

    void DrawSimObjects(IObjectRendererV400* pRenderer, ObjectWorldTransform& objTrans, ObjectWorldTransform& cameraTrans)
    {
        //Apply body relative offsets for recntangle placement.
        ObjectLocalTransform toRight(10, 0, 0, 0, 0, 0);
        ObjectLocalTransform toLeft(-10, 0, 0, 0, 0, 0);
        ObjectLocalTransform toLeftTop(-10, 5, 0, 0, 0, 0);
        ObjectLocalTransform toRightTop(10, 5, 0, 0, 0, 0);
        ObjectWorldTransform rightOfObject;
        ObjectWorldTransform leftOfObject;
        ObjectWorldTransform leftTopOfObject;
        ObjectWorldTransform rightTopOfObject;
        pRenderer->ApplyBodyRelativeOffset(objTrans, toRight, rightOfObject);
        pRenderer->ApplyBodyRelativeOffset(objTrans, toLeft, leftOfObject);
        pRenderer->ApplyBodyRelativeOffset(objTrans, toLeftTop, leftTopOfObject);
        pRenderer->ApplyBodyRelativeOffset(objTrans, toRightTop, rightTopOfObject);

        //Draw a sphere around user object.
        ARGBColor colorSphere(64, 0, 125, 125);
        pRenderer->DrawSphere(objTrans, 5, colorSphere);

        //Draw a rectangle to the left of the user object.
        ARGBColor colorRectangleLeft(64, 0, 0, 125);
        pRenderer->DrawRectangle(leftOfObject, 4, 4, 4, colorRectangleLeft);

        //Draw a cylinder to the right of the user object.
        ARGBColor colorCylinder(64, 125, 0, 125);
        pRenderer->DrawCylinder(rightOfObject, 4, 4, colorCylinder);

        //Draw a line between top left and top right of user object.
        ARGBColor colorLine(64, 0, 125, 0);
        pRenderer->DrawLine(leftTopOfObject.LLA, rightTopOfObject.LLA, 4, 4, colorLine);

        //Draw a triangle above the rectangle object
        ARGBColor colorTriangle(64, 125, 125, 0);
        pRenderer->DrawTriangle(leftTopOfObject, 4, 4, 4, colorTriangle);
    }


    bool m_bDrawGroundObjectGrid;
    bool m_bDrawUserSimObjects;
    int m_uOneSecondTick;

    ///----------------------------------------------------------------------------
    ///  Menu Items
    ///----------------------------------------------------------------------------
    CComPtr<P3D::IMenuItemV410> m_spMenuTop;
    CComPtr<P3D::IMenuItemV410> m_spMenuDrawGrid;
    CComPtr<P3D::IMenuItemV410> m_spMenuDrawObjs;

    enum CALLBACK_IDS {DRAW_GRID, DRAW_OBJS};

private:
    class MenuCallback : public P3D::ICallbackV400
    {
    public:
        MenuCallback(CALLBACK_IDS eventID)
            : m_EventID(eventID), m_RefCount(1)
        {

        }


        virtual void Invoke(P3D::IParameterListV400* pParams) override;

        CALLBACK_IDS m_EventID;

        DEFAULT_REFCOUNT_INLINE_IMPL()
            DEFAULT_IUNKNOWN_QI_INLINE_IMPL(MenuCallback, P3D::IID_ICallbackV400)
    };
};


///----------------------------------------------------------------------------
/// Prepar3D DLL start and end entry points
///----------------------------------------------------------------------------
static CustomPDKObjectsPlugin* s_pCustomObjectsPlugin = nullptr;

void __stdcall DLLStart(__in __notnull IPdk* pPdk)
{
    PdkServices::Init(pPdk);
    s_pCustomObjectsPlugin = new CustomPDKObjectsPlugin();
}

void __stdcall DLLStop(void)
{
    if (s_pCustomObjectsPlugin != nullptr)
    {
        delete s_pCustomObjectsPlugin;
    }
    PdkServices::Shutdown();
}

void CustomPDKObjectsPlugin::MenuCallback::Invoke(P3D::IParameterListV400 * pParams)
{
    switch (m_EventID)
    {
    case DRAW_GRID:
    {
        s_pCustomObjectsPlugin->m_bDrawGroundObjectGrid = !s_pCustomObjectsPlugin->m_bDrawGroundObjectGrid;
        s_pCustomObjectsPlugin->m_spMenuDrawGrid->SetChecked(s_pCustomObjectsPlugin->m_bDrawGroundObjectGrid);
        break;
    }
    case DRAW_OBJS:
    {
        s_pCustomObjectsPlugin->m_bDrawUserSimObjects = !s_pCustomObjectsPlugin->m_bDrawUserSimObjects;
        s_pCustomObjectsPlugin->m_spMenuDrawObjs->SetChecked(s_pCustomObjectsPlugin->m_bDrawUserSimObjects);
        break;
    }
    default:
        break;
    }
}
