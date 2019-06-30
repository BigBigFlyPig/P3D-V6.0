///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//
// DLLMain.cpp
//
///////////////////////////////////////////////////////////////////////////////
#define _CRT_RAND_S
#include "initpdk.h"
#include <ISimObject.h>
#include <IControllableCamera.h>
#include <ISimObjectAttachments.h>
#include "gauges.h"
#include "PDK.h"
#include "PdkPlugin.h"


#pragma once

using namespace P3D;

static CComPtr<IControllableCameraV400> g_spControllableCamera;

static const std::wstring s_name(L"TargetingPodView");
static const std::wstring s_whiteHotGreenName(L"WhiteHotGreenColorizer");
static const std::wstring s_blackHotGreenName(L"BlackHotGreenColorizer");
static unsigned int s_uPostProcessMode = 0;
static bool s_bChangeSensor = true;

static const double DEG_PER_RAD = 57.2958;
static const double FEET_PER_METER = 3.28084;
static const float MAX_MOVE_PAN_DEGREES = 10.0f;
static const float MAX_MOVE_HEADING_DEGREES = 10.0f;
static const unsigned int MAX_TARGET_ENTITIES = 10;
static const float TARGET_RADIUS_FEET = 1000.0f;

#define M_PI 3.1415926535897932384626433832795  // PI
#define TWO_PI (2*M_PI)
#define RADIANS_TO_DEGREES(rad)         ((rad) * (360/TWO_PI))
#define DEGREES_TO_RADIANS(degrees)     ((degrees) * (M_PI/180))

///////////////////////////////////////////////////////////////////////////////
//
// GetAxisPercent
//
// Helper function used to calculate the percentage the joystick is moved
//
///////////////////////////////////////////////////////////////////////////////
float GetAxisPercent(int sign, UINT32 evdata)
{
    if (evdata & 0xFFFF0000)
    {
        sign *= -1;
        evdata = 0xFFFFFFFF - evdata;
    }
    evdata = evdata & 0x0000FFFF;
    float axisPercent = (evdata / 65535.0f) * sign;

    return axisPercent;
}

///////////////////////////////////////////////////////////////////////////////
//
// KeyEventHandler
//
// Function to handle incoming key/joystick events
//
///////////////////////////////////////////////////////////////////////////////
void FSAPI KeyEventHandler(ID32 event, UINT32 evdata, PVOID userdata)
{
    if (!g_spControllableCamera || PdkServices::GetGlobalData()->IsPaused())
    {
        return;
    }

    switch (event)
    {
        // Rotate camera (x axis)
    case KEY_HOTAS_SLEW_AXIS_X:
    {
        g_spControllableCamera->DeactivatePositionTracking();
        g_spControllableCamera->DeactivateEntityTracking();

        CComPtr<IBaseObjectV400> pBaseObject = nullptr;
        CComPtr<IFireControlSystem> pFireControlSystem = nullptr;
        if (PdkServices::GetSimObjectManager() != nullptr)
        {
            PdkServices::GetSimObjectManager()->GetUserObject(&pBaseObject);
            pBaseObject->QueryService(SID_FireControlSystem, IID_IFireControlSystem, (void**)&pFireControlSystem);
            pFireControlSystem->SetSelectedTargetID(0);
        }

        float axisPercent = GetAxisPercent(-1, evdata);
        g_spControllableCamera->SetContinuousHeading(MAX_MOVE_HEADING_DEGREES * axisPercent);
        break;
    }

    // Rotate camera (y axis)
    case KEY_HOTAS_SLEW_AXIS_Y:
    {
        g_spControllableCamera->DeactivatePositionTracking();
        g_spControllableCamera->DeactivateEntityTracking();

        CComPtr<IBaseObjectV400> pBaseObject = nullptr;
        CComPtr<IFireControlSystem> pFireControlSystem = nullptr;
        if (PdkServices::GetSimObjectManager() != nullptr)
        {
            PdkServices::GetSimObjectManager()->GetUserObject(&pBaseObject);
            pBaseObject->QueryService(SID_FireControlSystem, IID_IFireControlSystem, (void**)&pFireControlSystem);
            pFireControlSystem->SetSelectedTargetID(0);
        }

        float axisPercent = GetAxisPercent(1, evdata);
        g_spControllableCamera->SetContinuousPitch(MAX_MOVE_PAN_DEGREES * axisPercent);
        break;
    }

    // Cycles between sensor modes and post process modes
    case KEY_HOTAS_KEY_A0:
    {
        g_spControllableCamera->RemovePostProcess(s_whiteHotGreenName.c_str());
        g_spControllableCamera->RemovePostProcess(s_blackHotGreenName.c_str());

        if (s_bChangeSensor)
        {
            unsigned int sensorMode = g_spControllableCamera->GetSensorMode();
            sensorMode++;
            if (sensorMode > 2)
            {
                sensorMode = 0;
                s_bChangeSensor = false;
            }
            g_spControllableCamera->SetSensorMode(sensorMode);
        }

        if (!s_bChangeSensor)
        {
            s_uPostProcessMode++;
            if (s_uPostProcessMode == 1)
            {
                g_spControllableCamera->AddPostProcess(s_whiteHotGreenName.c_str());
            }
            else if (s_uPostProcessMode == 2)
            {
                g_spControllableCamera->AddPostProcess(s_blackHotGreenName.c_str());
            }
            else if (s_uPostProcessMode > 2)
            {
                s_uPostProcessMode = 0;
                s_bChangeSensor = true;
            }
        }

        break;
    }

    // Lock target.  Will first attempt to target a nearby entity, if none are found the ground location is targetted.
    case KEY_HOTAS_KEY_A1:
    {
        if (PdkServices::GetSimObjectManager() != nullptr)
        {
            CComPtr<IBaseObjectV400> pBaseObject = nullptr;
            if (SUCCEEDED(PdkServices::GetSimObjectManager()->GetUserObject(&pBaseObject)))
            {
                CComPtr<IFireControlSystem> pFireControlSystem = nullptr;
                if (SUCCEEDED(pBaseObject->QueryService(SID_FireControlSystem, IID_IFireControlSystem, (void**)&pFireControlSystem)))
                {
                    double lat; // Degrees
                    double lon; // Degrees
                    double alt; // Meters

                    // Is the camera looking at a valid ground location?
                    if (g_spControllableCamera->GetCameraLookAtLLA(lat, lon, alt))
                    {
                        DXYZ camLLA; // Radians, Radians, Meters
                        camLLA.dZ = lat / DEG_PER_RAD; // Radians
                        camLLA.dX = lon / DEG_PER_RAD; // Radians
                        camLLA.dY = alt; // Meters

                        unsigned int objects = MAX_TARGET_ENTITIES;
                        unsigned int objectIds[MAX_TARGET_ENTITIES];
                        if (SUCCEEDED(PdkServices::GetSimObjectManager()->GetObjectsInRadius(camLLA, TARGET_RADIUS_FEET, objects, objectIds))
                            && objects > 0)
                        {
                            unsigned int targetId = 0;
                            for (unsigned int i = 0; i < objects; ++i)
                            {
                                unsigned int currentId = objectIds[i];

                                // Filter user.
                                if (currentId == pBaseObject->GetId())
                                {
                                    continue;
                                }

                                CComPtr<IBaseObjectV400> spTarget;
                                if (SUCCEEDED(PdkServices::GetSimObjectManager()->GetObject(currentId, &spTarget)))
                                {
                                    // Filter objects without health.
                                    if (spTarget->GetHealthPoints() <= 0.0)
                                    {
                                        continue;
                                    }

                                    // Filter based on mode (attached Avatar is not visible).
                                    int mode = spTarget->GetMode();
                                    static const int filter = (SIMOBJECT_MODE_ATTACHED |
                                                               SIMOBJECT_MODE_DISABLED |
                                                               SIMOBJECT_MODE_DEACTIVATED |
                                                               SIMOBJECT_MODE_INVISIBLE);
                                    if ((mode & filter) != 0)
                                    {
                                        continue;
                                    }

                                    // Filter weapons, countermeasures, and pylons.
                                    CComPtr<IWeaponServiceV400> spWeapon;
                                    CComPtr<ICountermeasureService> spCountermeasure;
                                    CComPtr<IPylonService> spPylon;
                                    if (SUCCEEDED(spTarget->QueryService(SID_WeaponService, IID_IWeaponServiceV400, (void**)&spWeapon)) ||
                                        SUCCEEDED(spTarget->QueryService(SID_CountermeasureService, IID_ICountermeasureService, (void**)&spCountermeasure)) ||
                                        SUCCEEDED(spTarget->QueryService(SID_PylonService, IID_IPylonService, (void**)&spCountermeasure)))
                                    {
                                        continue;
                                    }

                                    targetId = currentId;
                                    break;
                                }
                            }

                            if (targetId > 0)
                            {
                                g_spControllableCamera->ActivateEntityTracking();
                                g_spControllableCamera->SetTargetContainerId(targetId);

                                pFireControlSystem->SetSelectedTargetID(targetId);
                            }
                        }
                        else
                        {
                            g_spControllableCamera->ActivatePositionTracking();
                            g_spControllableCamera->SetTargetLla(lat, lon, alt); // Degrees, Degrees, Meters

                            P3D::P3DDXYZ lla; // Radians, Radians, Feet
                            lla.dX = lat / DEG_PER_RAD; // Radians
                            lla.dY = lon / DEG_PER_RAD; // Radians
                            lla.dZ = alt * FEET_PER_METER; // Feet
                            pFireControlSystem->SetTargetLLA(lla); // Radians, Radians, Feet
                        }
                    }
                }
            }
        }
        break;
    }

    // Zoom camera in
    case KEY_HOTAS_KEY_A2:
    {
        g_spControllableCamera->ZoomIn();
        break;
    }

    // Zoom camera out
    case KEY_HOTAS_KEY_A3:
    {
        g_spControllableCamera->ZoomOut();
        break;
    }

    default:
        break;
    }
}

// This is the gauge's import table definition 
PPANELS Panels = NULL;
GAUGESIMPORT    ImportTable =
{
    { 0x00000FFF, (PPANELS)NULL },
    { 0x00000000, NULL }
};

///////////////////////////////////////////////////////////////////////////////
//
// TargetingPodPlugin
//
// Utilizes controllable camera to provide generic targeting pod functionality
//
///////////////////////////////////////////////////////////////////////////////
class TargetingPodPlugin : public PdkPlugin
{
public:
    ///////////////////////////////////////////////////////////////////////////////
    //
    // TargetingPodPlugin
    //
    // Constructor
    // Hook up key event handler and initialize the controllable camera
    //
    ///////////////////////////////////////////////////////////////////////////////
    TargetingPodPlugin()
    {
        g_spControllableCamera = nullptr;

        if (Panels != nullptr)
        {
            ImportTable.PANELSentry.fnptr = (PPANELS)Panels;
            register_key_event_handler((GAUGE_KEY_EVENT_HANDLER)KeyEventHandler, NULL);
        }

        PdkServices::GetPdk()->QueryService(SID_ControllableCamera, IID_IControllableCameraV400, (void**)&g_spControllableCamera);

        InitializeCamera();
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    // ~TargetingPodPlugin
    //
    // Destructor
    // Unregister key event handler and deinitialize the controllable camera
    //
    ///////////////////////////////////////////////////////////////////////////////
    ~TargetingPodPlugin()
    {
        unregister_key_event_handler((GAUGE_KEY_EVENT_HANDLER)KeyEventHandler, NULL);

        if (g_spControllableCamera != nullptr)
        {
            g_spControllableCamera->DeInit();
            g_spControllableCamera = nullptr;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    // OnVehicleChanged
    //
    // Callback that's hit when a vehicle change occurs
    // Attempt to initialize the camera
    //
    ///////////////////////////////////////////////////////////////////////////////
    virtual void OnVehicleChanged(IParameterListV400* pParams) override
    {
        InitializeCamera();
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    // OnLoadComplete
    //
    // Callback that's hit when loading completes
    // Attempt to initialize the camera
    //
    ///////////////////////////////////////////////////////////////////////////////
    virtual void OnLoadComplete(IParameterListV400* pParams) override
    {
        InitializeCamera();
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    // OnViewChanged
    //
    // Callback that's hit when a view change occurs
    // Attempt to initialize the camera
    //
    ///////////////////////////////////////////////////////////////////////////////
    virtual void OnViewChanged(IParameterListV400* pParams) override
    {
        InitializeCamera();
    }

private:
    ///////////////////////////////////////////////////////////////////////////////
    //
    // InitializeCamera
    //
    // Hook up the controllable camera
    // Set up initial values including global rotation (not relative to the vehicle)
    // and initial rotation (matching the view direction of the vehicle)
    //
    ///////////////////////////////////////////////////////////////////////////////
    void InitializeCamera()
    {
        if (g_spControllableCamera)
        {
            if (!g_spControllableCamera->IsInitialized())
            {
                g_spControllableCamera->Init(s_name.c_str());

                // Rotate the camera globally.  This prevents the camera from rotating with the vehicle.
                g_spControllableCamera->SetGlobalRotate(true);
            }

            if (!g_spControllableCamera->IsRegistered())
            {
                g_spControllableCamera->Register();
            }

            CComPtr<IBaseObjectV400> pBaseObject = nullptr;
            if (PdkServices::GetSimObjectManager() != nullptr)
            {
                PdkServices::GetSimObjectManager()->GetUserObject(&pBaseObject);

                if (pBaseObject != nullptr)
                {
                    P3D::P3DDXYZ vLLA;
                    P3D::P3DDXYZ vPBH;
                    P3D::P3DDXYZ vVel;
                    P3D::P3DDXYZ vPHBVel;
                    pBaseObject->GetPosition(vLLA, vPBH, vVel, vPHBVel);

                    // Start the camera pointing toward the front of the vehicle
                    g_spControllableCamera->SetGlobalPbh(
                        static_cast<float>(vPBH.dX * DEG_PER_RAD),
                        0.0f,
                        static_cast<float>(vPBH.dY * DEG_PER_RAD));
                }
            }
        }
    }
};

static TargetingPodPlugin* s_pTargetingPodPlugin;
BOOL WINAPI DllMain(HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
    return TRUE;
}

extern "C"
{
    ///////////////////////////////////////////////////////////////////////////////
    //
    // DLLStart
    //
    // Entry point of DLL
    //
    ///////////////////////////////////////////////////////////////////////////////
    void __stdcall DLLStart(__in __notnull P3D::IPdk* pPdk)
    {
        PdkServices::Init(pPdk);
        s_pTargetingPodPlugin = new TargetingPodPlugin();
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    // DLLStop
    //
    // Exit point of DLL
    //
    ///////////////////////////////////////////////////////////////////////////////
    void __stdcall DLLStop(void)
    {
        if (s_pTargetingPodPlugin != nullptr)
        {
            delete s_pTargetingPodPlugin;
            s_pTargetingPodPlugin = nullptr;
        }
        PdkServices::Shutdown();
    }
}

// This is the module's export table.
GAUGESLINKAGE   Linkage =
{
    0x00000123,
    0,
    0,
    0,
    0,

    FS9LINK_VERSION,{ 0 }
};
