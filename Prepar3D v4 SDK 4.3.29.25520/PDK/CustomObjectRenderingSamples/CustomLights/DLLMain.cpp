///----------------------------------------------------------------------------
///
///  Copyright 2016 Lockheed Martin Corporation 
///  Lockheed Martin Proprietary Information
///  
///  Author: Prepar3D Dev.
///  Description: This example uses the PDK to render custom lights.
///                
///----------------------------------------------------------------------------
#include "initpdk.h"
#include <WinError.h>
#include "PdkPlugin.h"

#include <string>

#define BLUE_COLOR 0xFF0000FF
#define GREEN_COLOR 0xFF00FF00
#define PI                  3.141592653589793238462643
#define PI_OVER_2           (PI / 2)
#define TWO_PI              (2*PI)
#define RadToDeg(rad)         ((rad) * (360/TWO_PI))
#define FeetToMeters(feet)    ((feet)*(0.3048))

using namespace P3D;

class CustomLightsPlugin : public PdkPlugin
{
public:

    CustomLightsPlugin() : PdkPlugin()
    {
        m_bDrawGroundLightGrid = true; 
        m_bDrawUserSimLightGrid = true;
        m_uOneSecondTick = 0;
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
            if (m_bDrawUserSimLightGrid)
            {
                DrawUserSimLightGrid(spRenderService);
            }
            if (m_bDrawGroundLightGrid)
            {
                DrawGroundLightGrid(spRenderService);
            }
        }
    }

    virtual void OnOneHz(IParameterListV400* pParams) override
    {
        // keep track count of seconds. This is used for alternating light color.
        m_uOneSecondTick++;
    }

protected:

    ///----------------------------------------------------------------------------
    ///  Light drawing functions
    ///----------------------------------------------------------------------------

    void DrawUserSimLightGrid(IObjectRendererV400* pRenderer)
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
        // draw block of lights
        DrawSimLightGrid(pRenderer, objTrans, cameraTrans);
    }

    void DrawGroundLightGrid(IObjectRendererV400* pRenderer)
    {
        // coordinates from v3.4 default flight ( Eglin AFB - KVPS )
        ObjectWorldTransform transform( 30.499298038645236,
                                       -86.511296988903155,
                                        29.508712768554688,
                                        0.0f, 0.0f, 0.0f);

        // Begin a light group at this LLA
        pRenderer->BeginLightGroup(transform);

        unsigned int color = BLUE_COLOR;
        // draw a 40x40x5 grid of lights
        for (int i = 0; i < 40; i++)
        {
            for (int j = 0; j < 40; j++)
            {
                for (int k = 0; k < 5; k++)
                {
                    // 1/4 of the lights are green.  The secon of green lights changes
                    // once per second.
                    int group = m_uOneSecondTick % 4;
                    color = BLUE_COLOR;
                    if (i >= 10 * group && i < 10 * (group + 1))
                    {
                        color = GREEN_COLOR;
                    }

                    // grid is 2km by 2km by 100m
                    float x = -1000.0f + i * 2000.0f / 40.0f;
                    float y = k * 100.0f / 5.0f;
                    float z = -1000.0f + j * 2000.0f / 40.0f;

                    // add a light to the group
                    pRenderer->AddLight(x, y, z, LIGHT_TYPE, color, LIGHT_WIDTH, 4000.0f, false);
                }
            }
        }
        // End the light group.  Do not sort this group since it is on the ground.
        pRenderer->EndLightGroup(false);
    }

    void DrawSimLightGrid(IObjectRendererV400* pRenderer, ObjectWorldTransform& objTrans, ObjectWorldTransform& cameraTrans)
    {
        // get body relative offset of camera and light box
        ObjectLocalTransform objToCameraTrans;
        pRenderer->CalculateBodyRelativeOffset(objTrans, cameraTrans, objToCameraTrans);
        // calculate heading offset
        int headingOffset = int(abs(objToCameraTrans.PBH.Heading)) % 360;
        if (headingOffset > 180)
        {
            headingOffset -= 180;
        }
        // Add red color in based on heading offset
        unsigned int color = BLUE_COLOR | (headingOffset << 16);

        // draw a 3x3x3 grid of lights
        pRenderer->BeginLightGroup(objTrans);
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    // grid is 2m x 2m
                    float x = -1.0f + i * 1.0f;
                    float y = 1.0f + j * 1.0f;
                    float z = -1.0f + k * 1.0f;

                    // add a light to the group
                    pRenderer->AddLight(x, y, z, LIGHT_TYPE, color, LIGHT_WIDTH, 4000.0f, true);
                }
            }
        }
        // End the light group.  Because it's attached to a sim object, sort this group 
        // to prevent drawing behind other transparent objects like clouds
        pRenderer->EndLightGroup(true);
    }

    bool m_bDrawGroundLightGrid;
    bool m_bDrawUserSimLightGrid;
    int m_uOneSecondTick;

    // default light type and width
    const float  LIGHT_WIDTH = 2.0f;
    const unsigned int LIGHT_TYPE = 0;
};


///----------------------------------------------------------------------------
/// Prepar3D DLL start and end entry points
///----------------------------------------------------------------------------
static CustomLightsPlugin* s_pCustomLightsPlugin = nullptr;

void __stdcall DLLStart(__in __notnull IPdk* pPdk)
{
    PdkServices::Init(pPdk);
    s_pCustomLightsPlugin = new CustomLightsPlugin();
}

void __stdcall DLLStop(void)
{
    if (s_pCustomLightsPlugin != nullptr)
    {
        delete s_pCustomLightsPlugin;
    }
    PdkServices::Shutdown();
}


