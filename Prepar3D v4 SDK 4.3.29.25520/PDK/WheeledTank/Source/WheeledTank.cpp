//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//------------------------------------------------------------------------------
//
//  WheeledTank ISimObject (SimObject API) Example
//  WheeledTank.h
//  
//  Description:
//              Main source file of the WheeledTank ISimObject example.  This
//              WheeledTank example includes a SimObject of a Stryker MGS M1128 
//              that is preconfigured to work with this sample once compiled. 
//              For more information on setting up and using ISimObjects,
//              please see the Learning Center, Using the SimObject API Samples.
//------------------------------------------------------------------------------

#include "WheeledTank.h"
#include "IUnknownHelper.h"
#include "NetInOutPublic.h"

#include <math.h>

using namespace P3D;

/***********************************************************************************************
    Converts meters into feet.
***********************************************************************************************/
inline double METERS_TO_FEET(double meters)
{
    return meters * (1/0.3048);
}

/***********************************************************************************************
    Converts feet into meters.
***********************************************************************************************/
inline double FEET_TO_METERS(double feet)
{
    return feet * 0.304800;
}

/***********************************************************************************************
    Adds angle 1 to angle 0, and wraps the result to -PI < r < PI
***********************************************************************************************/
float AddWrapPi(float fAng0, float fAng1)
{
    float fAngResult = fAng0 + fAng1;

    if (fAngResult > PI)
    {
        fAngResult -= (float)(2*PI);
    }
    else if (fAngResult < -PI)
    {
        fAngResult += (float)(2*PI);
    }

    return fAngResult;
}

/***********************************************************************************************
    Adds angle 1 to angle 0, and wraps the result to 0 < r < 2PI
***********************************************************************************************/
float AddWrap2Pi(float fAng0, float fAng1)
{
    float fAngResult = fAng0 + fAng1;

    if (fAngResult > 2*PI)
    {
        fAngResult -= (float)(2*PI);
    }
    else if (fAngResult < 0)
    {
        fAngResult += (float)(2*PI);
    }

    return fAngResult;
}


/***********************************************************************************************
    Doors::QueryInterface
***********************************************************************************************/
STDMETHODIMP Doors::QueryInterface(__in REFIID riid,  // Interface ID 
                                   __out void **ppv)  // Interface Ptr
{
    HRESULT hr = E_NOINTERFACE;

    if (!ppv)
    {
        goto Error;
    }
    
    *ppv = NULL;
    
    if (IsEqualIID(riid , IID_IDoorService))
    {
        *ppv = static_cast<IDoorService*>(this);
    }
    else if (IsEqualIID(riid , IID_IDoorServiceV01))
    {
        *ppv = static_cast<IDoorServiceV01*>(this);
    }
    else if (IsEqualIID(riid , IID_IUnknown))
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    
    if (*ppv)
    {
        hr = S_OK;
        AddRef();
    }

Error:

    return hr;
}


/***********************************************************************************************
    WheeledTank Destructor
***********************************************************************************************/
WheeledTank::~WheeledTank()
{
    if(m_Doors)
    {
        delete m_Doors;
        m_Doors = NULL;
    }

    if(m_pTankCannon)
    {
        delete m_pTankCannon;
        m_pTankCannon = NULL;
    }

    if (m_pGunSystem)
    {
        m_pGunSystem->Release();
        m_pGunSystem = NULL;
    }
}

/***********************************************************************************************
    Class Factory Function
***********************************************************************************************/
/*static*/
STDMETHODIMP WheeledTank::New(__in  __notnull IBaseObjectV400* pBaseObject,      // Ptr to base object
                              __out __notnull ISimObject** ppThisSim)        // Ptr to new implementation
{
    HRESULT hr = E_FAIL;
    CComPtr<IBaseObjectV400> spBaseObject;

    if (!ppThisSim)
    {
        goto Error;
    }

    if (FAILED(pBaseObject->QueryInterface(IID_IBaseObjectV400, (void**)&spBaseObject)))
    {
        goto Error;
    }

    *ppThisSim = new WheeledTank(spBaseObject);

    if (!*ppThisSim)
    {
        goto Error;
    }

    // Registers the main simulation loop of the object
    hr = spBaseObject->RegisterSimulation(&static_cast<WheeledTank*>(*ppThisSim)->m_Simulation, 60.0f /*Hz*/);

Error:
    return hr;
}

/***********************************************************************************************
    WheeledTank Initialization
***********************************************************************************************/
/*override*/
STDMETHODIMP WheeledTank::Init()
{
    HRESULT hr = E_FAIL;

    //Sync with host
    SyncPosition();

    if (FAILED(m_Simulation.Init(this)))
    {
        goto Error;
    }

    if (FAILED(m_ContactReaction.Init(this)))
    {
        goto Error;
    }

    hr = S_OK;

Error:
    
    return hr;
}

/***********************************************************************************************
    WheeledTank Deinitialization
***********************************************************************************************/
/*override*/
STDMETHODIMP WheeledTank::DeInit()   
{
    HRESULT hr = E_FAIL;

    if (SUCCEEDED(m_Simulation.DeInit()))
    {
        HRESULT hrWeapons = m_spBaseObject->DestroyServiceInstance(SID_WeaponsSystem);
        HRESULT hrGuns    = m_spBaseObject->DestroyServiceInstance(SID_GunSystem);

        hr = S_OK;
    }

    return hr;
}

/***********************************************************************************************
    Loads the on-disk constant/fixed data 
***********************************************************************************************/
/*override*/
STDMETHODIMP WheeledTank::LoadConstantData(__out void** ppConstantData)
{
    HRESULT hr = E_FAIL;

    if (!ppConstantData)
    {
        return E_POINTER;
    }

    if (*ppConstantData) // Do we already have a ref to loaded data due to a previous instance?
    {
        m_pFixedData = static_cast<WheeledTankFixedData*>(*ppConstantData);

        hr = S_OK;
    }
    else // Load the data, return a ref to it to be cached for subsequent instances
    {
        m_pFixedData = new WheeledTankFixedData();

        hr = m_pFixedData->Load(m_spBaseObject);

        if (FAILED(hr))
        {
            delete m_pFixedData;
            m_pFixedData = NULL;
        }
            
        *ppConstantData = const_cast<void*>(static_cast<const void*>(m_pFixedData));
    }

    return hr;
}

/***********************************************************************************************
    Unloads the on-disk constant/fixed data 
***********************************************************************************************/
/*override*/
STDMETHODIMP WheeledTank::UnloadConstantData(__in void** ppConstantData)
{
    HRESULT hr = E_FAIL;

    if (*ppConstantData == m_pFixedData)
    {
        hr = m_pFixedData->Unload(m_spBaseObject);

        delete m_pFixedData;
        m_pFixedData = NULL;

        *ppConstantData = reinterpret_cast<void*>(const_cast<WheeledTankFixedData*>(m_pFixedData));
    }

    return hr;
}

/***********************************************************************************************
    Creates subsystems based on disk data
***********************************************************************************************/
/*override*/
STDMETHODIMP WheeledTank::LoadDynamicData()
{
    HRESULT hr = E_FAIL;

    m_spBaseObject->CreateServiceInstance(SID_WeaponsSystem);
    m_spBaseObject->CreateServiceInstance(SID_GunSystem);

    hr = S_OK;

    return hr;
}

/***********************************************************************************************
    WheeledTank::QueryInterface
***********************************************************************************************/
STDMETHODIMP WheeledTank::QueryInterface(__in REFIID riid,  //Interface ID 
                                         __out void **ppv)  //Interface Ptr
{
    HRESULT hr = E_NOINTERFACE;

    if (!ppv)
    {
        goto Error;
    }
    
    *ppv = NULL;
    
    if (IsEqualIID(riid , CLSID_WheeledTank))
    {
        *ppv = this;
    }        
    else if (IsEqualIID(riid , IID_ISimObject))
    {
        *ppv = static_cast<ISimObject*>(this);
    }        
    else if (IsEqualIID(riid , IID_ISimObjectV02))
    {
        *ppv = static_cast<ISimObjectV02*>(this);
    }        
    else if (IsEqualIID(riid , IID_ISimObjectV01))
    {
        *ppv = static_cast<ISimObjectV01*>(this);
    }        
    else if (IsEqualIID(riid , IID_IServiceProvider))
    {
        *ppv = static_cast<IServiceProvider*>(this);
    }        
    else if (IsEqualIID(riid , IID_IUnknown))
    {
        *ppv = static_cast<IUnknown*>(this);
    }


    if (*ppv)
    {
        hr = S_OK;
        AddRef();
    }

Error:

    return hr;
}

/***********************************************************************************************
    WheeledTank::QueryService
***********************************************************************************************/
STDMETHODIMP WheeledTank::QueryService(__in REFGUID guidService,    //Service ID
                                       __in REFIID riid,            //Interface ID
                                       __out void **ppv)            //Interface Ptr
{
    HRESULT hr = E_NOINTERFACE;

    if (!ppv)
    {
        goto Error;
    }  
    
    *ppv = NULL;

    if (IsEqualIID(guidService, SID_DoorService))
    {
        if (m_Doors)
        {
            hr = m_Doors->QueryInterface(riid, ppv);
        }
    }
    else if (IsEqualIID(guidService, SID_SimObject))
    {
        hr = QueryInterface(riid, ppv);
    }

Error:

    return hr;

}

WorldConstants WheeledTank::ms_WorldConstants;

/***********************************************************************************************
    Simulation Initialization
***********************************************************************************************/
HRESULT Simulation::Init(WheeledTank* pWheeledTank)
{
    m_spWheeledTank = pWheeledTank; 

    return S_OK;
}

/***********************************************************************************************
    Simulation Deinitialization
***********************************************************************************************/
HRESULT Simulation::DeInit()
{
    m_spWheeledTank = NULL;
    
    return S_OK;
}

/***********************************************************************************************
    Main simulation loop.
***********************************************************************************************/
STDMETHODIMP Simulation::Update(double dDeltaT)        
{
    // Initialize guns, called only once
    m_spWheeledTank->InitGuns();

    if (IsNormalMode())
    {
        DXYZ vdPos;
        DXYZ vdOrient;
        DXYZ vdVel;
        DXYZ vdRotVel;

        //Get current position
        m_spBaseObject->GetPosition(vdPos, vdOrient, vdVel, vdRotVel);
        
        //Calc ground reaction forces
        DXYZ vdContactReactionForce;
        DXYZ vdContactReactionMoment;
        m_spWheeledTank->CalculateContactReactionForceMoment(vdContactReactionForce, vdContactReactionMoment, dDeltaT);

        //Sum totoal force (currently only for altitude)
        double dTotalAccel = ((vdContactReactionForce.dY / m_spWheeledTank->GetMassProperties().GetMass())  + -m_spWheeledTank->GetWorldConstants().m_fGravitySeaLevel);
        //Integrate new velocitcy
        vdVel.dY += dTotalAccel * dDeltaT;

        //Get the target speed based on the state of the engine and user input
        if (m_spWheeledTank->GetEngine().IsOn())
        {
            m_spWheeledTank->GetEngine().SetPctRpm(abs(m_fCurrentSpeed) / MAX_SPEED);
        }
        else
        {
            m_fTargetSpeed = 0.0f;
            m_spWheeledTank->GetEngine().SetPctRpm(0);
        }
        //Update current target speed to move toward speed goal
        m_fCurrentSpeed += (m_fTargetSpeed - m_fCurrentSpeed) * (float)dDeltaT;

        //Calculate new position (kinematics only for lat/lon)
        vdVel.dZ = m_fCurrentSpeed * cos(vdOrient.dY);
        vdVel.dX = m_fCurrentSpeed * sin(vdOrient.dY);

        //Integrate postion
        double latRadians = cos(vdPos.dZ);
        if(latRadians != 0.0)
        {
            vdPos.dX += (vdVel.dX * dDeltaT) / (latRadians * m_spWheeledTank->GetWorldConstants().m_dEquatorialRadius);
        }  
        vdPos.dY += vdVel.dY * dDeltaT;
        vdPos.dZ += (vdVel.dZ * dDeltaT) / m_spWheeledTank->GetWorldConstants().m_dPolarRadius;

        //Update steering angle
        m_spWheeledTank->UpdateSteering(dDeltaT);

        //Calc heading velocity based on current velocity and steering angle
        vdRotVel.dY = m_fCurrentSpeed * tan(m_spWheeledTank->GetSteerAngle()) * dDeltaT;

        //Integrate new pitch/roll vel (currently only kinematics for heading)
        vdRotVel.dX = (vdContactReactionMoment.dX /  m_spWheeledTank->GetMassProperties().GetPitchMoi()) * dDeltaT;
        vdRotVel.dZ = (vdContactReactionMoment.dZ /  m_spWheeledTank->GetMassProperties().GetRollMoi()) * dDeltaT;

        //Integrate new orientation
        vdOrient.dX += vdRotVel.dX * dDeltaT;
        vdOrient.dZ += vdRotVel.dZ * dDeltaT;
        vdOrient.dY += vdRotVel.dY * dDeltaT;

        //Update new position
        m_spWheeledTank->SetPosition(vdPos, vdOrient, vdVel, vdRotVel, dDeltaT);

        //Update wheel rotation
        m_spWheeledTank->UpdateWheelRotation(m_fCurrentSpeed, dDeltaT);

        // update turret position
        m_spWheeledTank->UpdateTurretOrientation(dDeltaT);

        // create ground visual effect depending on vehicle speed            
        if(abs(m_fCurrentSpeed) > 40.0f && m_fGroundEffectTimer > m_fGroundEffectDelay)
        {
            // turn on the effect
            // offset to rear of vehicle
            FXYZ offset = {0};
            offset.fX = 0.0f;
            offset.fY = -8.007f;
            offset.fZ = -6.35f;
            // create effect
            void* pGroundEffect = NULL;
            m_spWheeledTank->GetBaseObject()->VisualEffectOn(TEXT("fx_dustcloud_vs"), &offset, &pGroundEffect);
            // reset timer
            m_fGroundEffectTimer = 0.0f;
        }
        else
        {
            // increase timer
            m_fGroundEffectTimer += (float)dDeltaT;
        }
    }

    // Update remaining systems
    if (m_spWheeledTank)
    {
        m_spWheeledTank->UpdateWeapons(dDeltaT);
        m_spWheeledTank->UpdateGuns(dDeltaT);
        m_spWheeledTank->UpdateEngine(dDeltaT);
    }

    return S_OK;
}

/*override*/
STDMETHODIMP Simulation::SaveLoadState(__in __notnull PSaveLoadCallback, __in const BOOL bSave) 
{
    if (!bSave) //i.e. load
    {
        m_spWheeledTank->SyncPosition();
    }

    return S_OK;
}

//Multiplayer serialize/deserialize
STDMETHODIMP Simulation::Serialize(__in NetOutPublic& netOut)
{
    float steerAngle = m_spWheeledTank->GetSteerAngle();
    float turretRotation = m_spWheeledTank->GetTurretRotation();
    float turretPitch = m_spWheeledTank->GetTurretPitch();

    netOut.WriteFloat(steerAngle);
    netOut.WriteFloat(turretRotation);
    netOut.WriteFloat(turretPitch);

    return S_OK;
}

STDMETHODIMP Simulation::Deserialize(__in NetInPublic& netIn)
{
    float steerAngle = netIn.ReadFloat();
    float turretRotation = netIn.ReadFloat();
    float turretPitch = netIn.ReadFloat();

    m_spWheeledTank->SetSteerAngle(steerAngle);
    m_spWheeledTank->SetTurretRotation(turretRotation);
    m_spWheeledTank->SetTurretPitch(turretPitch);

    return S_OK;
}

/***********************************************************************************************
    RegisterProperties
***********************************************************************************************/
/*static*/
HRESULT WheeledTank::RegisterProperties(__in __notnull ISimObjectManagerV400* pSimObjectMgr)
{
    HRESULT hr = E_FAIL;

    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("IncSpeed"),                      TEXT("Feet per Second"),      WheeledTank::IncSpeed,                  EVENTTYPE_NORMAL);
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("DecSpeed"),                      TEXT("Feet per Second"),      WheeledTank::DecSpeed,                  EVENTTYPE_NORMAL);
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("SetSpeedPct"),                   TEXT("Percent over 100"),     WheeledTank::SetSpeedPct,               EVENTTYPE_AXIS);
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("ResetSpeed"),                    TEXT("Feet per Second"),      WheeledTank::ResetSpeed,                EVENTTYPE_NORMAL);

    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("IncDirection"),                  TEXT("Percent over 100"),     WheeledTank::IncSteeringDirection,      EVENTTYPE_NORMAL);    
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("DecDirection"),                  TEXT("Percent over 100"),     WheeledTank::DecSteeringDirection,      EVENTTYPE_NORMAL);
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("ResetDirection"),                TEXT("Percent over 100"),     WheeledTank::ResetSteeringDirection,    EVENTTYPE_NORMAL);
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("SetDirectionPct"),               TEXT("Percent over 100"),     WheeledTank::SetSteeringDirectionPct,   EVENTTYPE_AXIS);
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("GetSteeringDirection"),          TEXT("Radians"),              WheeledTank::GetSteeringDirection);
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("SteerAngle"),                    TEXT("Radians"),              WheeledTank::GetSteerAngle);

    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("WheelRotationRate"),             TEXT("Radians per Second"),   WheeledTank::GetWheelRotationRate);
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("WheelRotationAngle"),            TEXT("Radians"),              WheeledTank::GetWheelRotationAngle);
    
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("TurretPitch"),                   TEXT("Radians"),              WheeledTank::GetTurretPitch);
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("TurretPitchUp"),                 TEXT("Radians"),              WheeledTank::TurretPitchUp,             EVENTTYPE_NORMAL);
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("TurretPitchDown"),               TEXT("Radians"),              WheeledTank::TurretPitchDown,           EVENTTYPE_NORMAL);
    
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("TurretRotation"),                TEXT("Radians"),              WheeledTank::GetTurretRotation);
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("TurretRotateRight"),             TEXT("Radians"),              WheeledTank::TurretRotateRight,         EVENTTYPE_NORMAL);
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("TurretRotateLeft"),              TEXT("Radians"),              WheeledTank::TurretRotateLeft,          EVENTTYPE_NORMAL);

    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("SetTurretRotationSpeedPct"),     TEXT("Percent over 100"),     WheeledTank::SetTurretRotationSpeedPct, EVENTTYPE_AXIS);
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("SetTurretPitchSpeedPct"),        TEXT("Percent over 100"),     WheeledTank::SetTurretPitchSpeedPct,    EVENTTYPE_AXIS);

    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("OpenHatch"),                     TEXT("Percent over 100"),     WheeledTank::OpenHatch,                 EVENTTYPE_NORMAL);
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("CloseHatch"),                    TEXT("Percent over 100"),     WheeledTank::CloseHatch,                EVENTTYPE_NORMAL);   
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("Door_0"),                        TEXT("Percent over 100"),     WheeledTank::GetDoorPercent); 

    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("PercentRpm"),                    TEXT("Percent over 100"),     Engine::GetPercentRpm);
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("Combustion"),                    TEXT("Bool"),                 Engine::GetCombustion);
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("StarterOn"),                     TEXT("Bool"),                 Engine::GetStarter);
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("EngineOffset"),                  TEXT("Feet"),                 Engine::GetEngineOffset);
    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("ToggleEngine"),                  TEXT("Bool"),                 Engine::Toggle,                         EVENTTYPE_NORMAL);

    pSimObjectMgr->RegisterProperty(CLSID_WheeledTank, TEXT("StrutCompression"),              TEXT("Feet"),                 ContactReaction::GetStrutCompressionLength);

    hr = pSimObjectMgr->GetWorldConstants(WheeledTank::ms_WorldConstants);

    hr = S_OK;

    return hr;
}

/***********************************************************************************************
    Steering Updates
***********************************************************************************************/
void WheeledTank::UpdateSteering(double dDeltaT)
{    
    // calculate steer angle from axis input percent
    if(m_fSteerAxisInput != 0.0 || m_fSteerKeyInput != 0.0)
    {
        if(m_fSteerAxisInput != 0.0)
        {
            m_fTargetSteerAngle = (float)(min(max(-1.0, m_fSteerAxisInput), 1.0) * MIN_STEER_ANGLE);
        }
        else
        {
            m_fTargetSteerAngle = m_fSteerKeyInput;
        }

        // steer toward target angle
        if(m_fSteerAngle < m_fTargetSteerAngle)
        {
            // increase steer angle
            m_fSteerAngle += (float)MAX_STEER_RATE * (float)dDeltaT;
            // clamp steer angle to target angle
            m_fSteerAngle = min(m_fSteerAngle, m_fTargetSteerAngle);
        }
        else if(m_fSteerAngle > m_fTargetSteerAngle)
        {
            // decrease steer angle
            m_fSteerAngle -= (float)MAX_STEER_RATE * (float)dDeltaT;
            // clamp steer angle to target angle
            m_fSteerAngle = max(m_fSteerAngle, m_fTargetSteerAngle);
        }

        // clamp the steer angle
        if(m_fSteerAngle < -MIN_STEER_ANGLE)
        {
            m_fSteerAngle = (float)-MIN_STEER_ANGLE;
        }
        else if(m_fSteerAngle > MIN_STEER_ANGLE)
        {
            m_fSteerAngle = (float)MIN_STEER_ANGLE;
        }
    }
    else
    {
        // steering is serialized in multiplayer, do not attempt to steer straight
        if (m_spBaseObject->GetNetworkMode() == NET_MODE_TYPE_NORMAL ||
            m_spBaseObject->GetNetworkMode() == NET_MODE_TYPE_MASTER)
        {
            if (m_fSteerAngle > 0.0)     // no input, start to steer straight
            {
                // steer left
                m_fSteerAngle -= (float)MAX_STEER_RATE * (float)dDeltaT;
            }
            else if (m_fSteerAngle < 0.0)
            {
                // steer right
                m_fSteerAngle += (float)MAX_STEER_RATE * (float)dDeltaT;
            }
        }

        // set steer angle to zero if with epsilon
        if(m_fSteerAngle < 0.01 && m_fSteerAngle > -0.01)
        {
            m_fSteerAngle = 0.0;
        }

        m_fTargetSteerAngle = 0.0;
    }
}

/***********************************************************************************************
    Updates the wheel's rotation angle based on the object's velocity and the wheel's radius.
***********************************************************************************************/
void WheeledTank::UpdateWheelRotation(double dVel, double dDeltaT)
{
    static const float WHEEL_RADIUS = 1.875f; //Feet

    m_fWheelRotationRate = (float)(dVel / WHEEL_RADIUS); //Radians / sec

    m_fWheelRotationAngle = AddWrap2Pi(m_fWheelRotationAngle, (float)(m_fWheelRotationRate * dDeltaT)); //Radians
}

/***********************************************************************************************
    Updates the orientation of the turret based on user inputs.
***********************************************************************************************/
void WheeledTank::UpdateTurretOrientation(double dDeltaT)
{
    // apply scaled pitch
    m_fTurretPitch += ((float)dDeltaT * m_fTurretPitchSpeed);

    if(m_fTurretPitchSpeed < 0)         //Limit downward angle
    {
        m_fTurretPitch = (float)max(m_fTurretPitch, DEG_TO_RAD(-6.0));

        //No matter what, never exceed 90 degrees
        m_fTurretPitch = (float)max(m_fTurretPitch, DEG_TO_RAD(-90.0));
    }
    else if(m_fTurretPitchSpeed > 0)    //Limit upward angle
    {    
        m_fTurretPitch = (float)min(m_fTurretPitch, DEG_TO_RAD(15.0));

        //No matter what, never exceed 90 degrees
        m_fTurretPitch = (float)min(m_fTurretPitch, DEG_TO_RAD(90.0));
    }

    // apply scaled pitch
    m_fTurretRotation += ((float)dDeltaT * m_fTurretRotationSpeed);

    if(m_fTurretRotationSpeed < 0)  //Wrap if passing -180 degrees
    {
        if (m_fTurretRotation < -PI)
        {
            m_fTurretRotation += (float)(2.0 * PI);
        }
    }
    else if(m_fTurretRotationSpeed > 0) //Wrap if passing 180 degrees
    {
        if (m_fTurretRotation > PI)
        {
            m_fTurretRotation -= (float)(2.0 * PI);
        }
    }
}

/***********************************************************************************************
    Creates a single IGun implementation. This function is only called once, and is placed in 
    the Simulation::Update() function. The reason for this is to allow the internal GunSystem 
    to completely load before attempting to add a gun.
***********************************************************************************************/
void WheeledTank::InitGuns()
{
    if(!m_bGunsInitialized)
    {
        m_pGunSystem = NULL;
        m_spBaseObject->QueryService(SID_GunSystem, IID_IGunSystemV400, (void**)&m_pGunSystem);
        if(m_pGunSystem)
        {
            if(!m_pGunSystem->GetIsGunPresentAtStation(0))
            {
                m_pTankCannon = new TankCannon(this, m_pGunSystem);
                m_pGunSystem->AddGun(m_pTankCannon, 0);
            }
        }

        m_bGunsInitialized = true;
    }
}

/***********************************************************************************************
    Functions to load information from the configuration and xml files.
***********************************************************************************************/
HRESULT WheeledTankFixedData::Load(__in __notnull IBaseObjectV400* pBaseObject) const
{
    //These will fail without Professional Plus License
    HRESULT hrWeapons = pBaseObject->LoadServiceConstantData(SID_WeaponsSystem);
    HRESULT hrGuns    = pBaseObject->LoadServiceConstantData(SID_GunSystem);

    return S_OK;
}

/***********************************************************************************************
    Functions to unload information from the configuration and xml files.
***********************************************************************************************/
HRESULT WheeledTankFixedData::Unload(__in __notnull IBaseObjectV400* pBaseObject) const
{
    //These will fail without Professional Plus License
    HRESULT hrWeapons = pBaseObject->UnloadServiceConstantData(SID_WeaponsSystem);
    HRESULT hrGuns    = pBaseObject->UnloadServiceConstantData(SID_GunSystem);

    return S_OK;
}
