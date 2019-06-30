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

#pragma once

#include <atlcomcli.h>
#include <ISimObject.h>
#include <ISimObjectAttachments.h>

#include "ContactReaction.h"
#include "Engine.h"
#include "TankCannon.h"

#include "IUnknownHelper.h"

#include <vector>

#define PI 3.1415926535897932384626433832795
#define DEG_TO_RAD(deg) ((deg) * PI / 180.0)
#define RAD_TO_DEG(rad) ((rad) * 180.0 / PI)

#define FEET_TO_LATITUDE(ft) (ft) * (1.0 / 6077) /*Ft->Nm*/ * (1.0 / 60) /*Nm->Deg*/ * (PI / 180) /*Deg->Rad*/
#define FEET_TO_LONGITUDE(ft, dLatRad) FEET_TO_LATITUDE(ft) * cos(dLatRad)

using namespace P3D;

class WheeledTankFixedData;
class TankCannon;

/******************************************************************************************************
Mass
******************************************************************************************************/
class MassProperties
{
public:
    MassProperties()
        :   m_fMass(41380/32.17f) // stones
    {
        m_vfMoi.fX = 5970;
        m_vfMoi.fY = 1540;
        m_vfMoi.fZ = 5924;
    }

    float GetMass()     const {return m_fMass;}
    float GetPitchMoi() const {return m_vfMoi.fX;}
    float GetYawMoi()   const {return m_vfMoi.fY;}
    float GetRollMoi()  const {return m_vfMoi.fZ;}

private:
    float m_fMass;
    FXYZ  m_vfMoi;
};

/******************************************************************************************************
Simulation System Subclass
******************************************************************************************************/
class Simulation : public ISimulationV310
{
    static const int MAX_ACCELERATION = 25; // ft/s2
    static const int MAX_DECELERATION = 50; // ft/s2 (for braking)

    static const int MAX_SPEED = 90;  // feet p/s


public:
    Simulation(IBaseObjectV400* pBaseObject) 
        : m_RefCount(1),
          m_spBaseObject(pBaseObject), 
          m_fTargetSpeed(0.0),
          m_fCurrentSpeed(0.0),
          m_fGroundEffectDelay(0.35f),
          m_fGroundEffectTimer(0.0)
    {
    }

    HRESULT Init(WheeledTank* pWheeledTank);
    HRESULT DeInit();

    void IncSpeed()
    {
        // hitting the brakes
        if(m_fCurrentSpeed < 0.0)
        {
            m_fTargetSpeed = min(m_fCurrentSpeed + MAX_DECELERATION, MAX_SPEED);
        }
        else // moving forward
        {
            m_fTargetSpeed = min(m_fCurrentSpeed + MAX_ACCELERATION, MAX_SPEED);
        }
    }

    void DecSpeed()
    {
        // hitting the brakes
        if(m_fCurrentSpeed > 0.0)
        {
            m_fTargetSpeed = max(m_fCurrentSpeed - MAX_DECELERATION, -MAX_SPEED);
        }
        else // moving in reverse
        {
            // slightly slower reverse speed
            m_fTargetSpeed = max(m_fCurrentSpeed - MAX_ACCELERATION * 0.75f, -MAX_SPEED * 0.75f);
        }
    }

    // called when releasing key inputs
    void ResetSpeed()
    {
        m_fTargetSpeed = 0.0;
    }

    void SetSpeedPct(double dPct) {m_fTargetSpeed = (float)(MAX_SPEED * dPct);}

private:
    CComPtr<IBaseObjectV400> m_spBaseObject;
    CComPtr<WheeledTank> m_spWheeledTank;

    volatile LONG   m_RefCount;
    STDMETHOD_(ULONG, AddRef)()   override {return InterlockedIncrement(&m_RefCount) ;}
    STDMETHOD_(ULONG, Release)()  override 
    {
        ULONG   RetVal = InterlockedDecrement(&m_RefCount);
        if      (RetVal == 0)         {delete this;}
        else if (RetVal & 0x80000000) {__debugbreak();}
        return RetVal;
    }

    STDMETHOD (QueryInterface)(__in REFIID riid, void **ppv)  override 
    {
        HRESULT hr = E_NOINTERFACE;

        if (!ppv)
        {
            goto Error;
        }
    
        *ppv = NULL;

        if (IsEqualIID(riid , IID_ISimulationV310))
        {
            *ppv = static_cast<ISimulationV310*>(this);
        }        
        else if (IsEqualIID(riid, IID_ISimulationV01))
        {
            *ppv = static_cast<ISimulationV01*>(this);
        }
        else if (IsEqualIID(riid, IID_ISimulation))
        {
            *ppv = static_cast<ISimulation*>(this);
        }
        else if (IsEqualIID(riid, IID_IUnknown))
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

    STDMETHOD (SaveLoadState)(__in __notnull PSaveLoadCallback, __in const BOOL bSave) override;
    STDMETHOD (Update)(double dDeltaT);
    STDMETHOD (Serialize)(__in NetOutPublic& netOut);
    STDMETHOD (Deserialize)(__in NetInPublic& netIn);

    BOOL IsNormalMode() const 
    {
        return (0 == (m_spBaseObject->GetMode() & ( SIMOBJECT_MODE_PAUSE | SIMOBJECT_MODE_DISABLED | SIMOBJECT_MODE_SLEW | SIMOBJECT_MODE_CRASH | SIMOBJECT_MODE_FREEZE_POSITION
                                                    | SIMOBJECT_MODE_FREEZE_ATTITUDE | SIMOBJECT_MODE_FREEZE_ALTITUDE | SIMOBJECT_MODE_FREEZE_LATLON)));
    }

    float m_fTargetSpeed;
    float m_fCurrentSpeed;

    float m_fGroundEffectDelay;
    float m_fGroundEffectTimer;

    static WorldConstants  ms_WorldConstants;
};


static const double MIN_STEER_ANGLE = DEG_TO_RAD(30.0);
static const double MAX_STEER_RATE = DEG_TO_RAD(30.0);


/***********************************************************************************************
Doors (Hatches)
***********************************************************************************************/
class Doors : public IDoorServiceV01
{

    DECLARE_IUNKNOWN()

public:

    Doors() :
        m_RefCount(1)
    {
    }
    ~Doors() {};

    HRESULT Init(WheeledTank* pWheeledTank);

    STDMETHOD_(float, GetDoorPercentOpen)(__in int doorIndex)                           const override       //on-disk data.  should point to loaded data, and return NULL
    {
        return 0.0;
    }

private:
};


/***********************************************************************************************
***********************************************************************************************/
// Ensure this GUID matches the CategoryId in the sim.cfg
class __declspec(uuid("{41A7D829-A12E-4550-AF70-868E3BF69F50}")) WheeledTank : public ISimObjectV02
{
    DECLARE_IUNKNOWN()

public:
    ~WheeledTank();

    /* Class Factory Function */
    static STDMETHODIMP New(__in __notnull IBaseObjectV400* pBaseObject, __out __notnull ISimObject** ppSimImp);
    static HRESULT RegisterProperties(__in __notnull ISimObjectManagerV400* pSimObjectMgr);

    STDMETHOD (QueryService)(__in REFGUID guidService, __in REFIID riid, void **ppvObject) override;

    STDMETHOD (LoadConstantData)(__out void** ppConstantData)       override;                   //on-disk data
    STDMETHOD (UnloadConstantData)(__inout void** ppConstantData)   override;                   //on-disk data
    STDMETHOD (LoadDynamicData)()                                   override;                   //e.g. create subsystems based on disk data
    STDMETHOD (Init)()           override;
    STDMETHOD (DeInit)()         override;
    STDMETHOD_(BOOL, SupportsLabels)()                      const   override {return (m_bSupportsLabel != false);}                      //Does this simobject support label display?
    STDMETHOD (SetSupportsLabels)(BOOL bOn)                         override {m_bSupportsLabel = (bOn != FALSE); return S_OK;}     //Should this simobject support label display?
    STDMETHOD_(void, OnModeChange)(int bfMode)                      override {}                 //Called when a mode has changed, with the new mode bitfield
    STDMETHOD_(void, OnPositionInit)()                              override {}                 //Called upon change in non-realtime object positions   (e.g. UI, slew)
    STDMETHOD_(void, OnSpeedInit)(float fSpeed)                     override {}                 //(Feet/Second) Called upon init in speed   (e.g. UI)

    IBaseObjectV400* GetBaseObject() const         {return m_spBaseObject;}

    const DXYZ& GetPosition()    const {return m_vdPosition;}
    const DXYZ& GetOrientation() const {return m_vdOrientation;}
    const DXYZ& GetVelocity()       const {return m_vdVel;}
    const DXYZ& GetRotVelocity()    const {return m_vdRotVel;}

    void GetPosition(DXYZ& vdPos, DXYZ& vdOrient, DXYZ& vdVel, DXYZ& vdRotVel) 
    {
        m_spBaseObject->GetPosition(m_vdPosition, m_vdOrientation, m_vdVel, m_vdRotVel);

        vdPos       = m_vdPosition;
        vdOrient    = m_vdOrientation;
        vdVel       = m_vdVel;
        vdRotVel    = m_vdRotVel;
    }

    void SetPosition(const DXYZ& vdPos, const DXYZ& vdOrient, const DXYZ& vdVel, const DXYZ& vdRotVel, double dDeltaT)
    {
        m_vdPosition    = vdPos;
        m_vdOrientation = vdOrient;
        m_vdVel         = vdVel;
        m_vdRotVel      = vdRotVel;

        m_spBaseObject->SetPosition(m_vdPosition, m_vdOrientation, m_vdVel, m_vdRotVel, m_ContactReaction.IsOnGround(), dDeltaT);
    }

    void SyncPosition() {m_spBaseObject->GetPosition(m_vdPosition, m_vdOrientation, m_vdVel, m_vdRotVel);}
        
    const ContactReaction& GetContactReaction() const {return m_ContactReaction;}
    const MassProperties&  GetMassProperties() const {return m_MassProperties;}

    void IncSpeed() {m_Simulation.IncSpeed();}
    void DecSpeed() {m_Simulation.DecSpeed();}
    void SetSpeedPct(double dPct) {m_Simulation.SetSpeedPct(dPct);}
    void ResetSpeed() {m_Simulation.ResetSpeed();}

    HRESULT UpdateWeapons(double dDeltaT) {return m_spBaseObject->UpdateServiceInstance(SID_WeaponsSystem, dDeltaT);}
    HRESULT UpdateGuns(double dDeltaT)       { return m_spBaseObject->UpdateServiceInstance(SID_GunSystem, dDeltaT); }
    void UpdateEngine(double dDeltaT)     {m_Engine.Update(*this, dDeltaT);}
    Engine& GetEngine() {return m_Engine;}
    const Engine& GetEngine() const {return m_Engine;}

    HRESULT CalculateContactReactionForceMoment(__out DXYZ& vdForce, __out DXYZ& vdMoment, __in double dDeltaT)  {return m_ContactReaction.CalculateForceMoment(vdForce, vdMoment, dDeltaT);}

    BOOL IsNormalMode() const 
    {
        return (0 == (m_spBaseObject->GetMode() & ( SIMOBJECT_MODE_PAUSE | SIMOBJECT_MODE_DISABLED | SIMOBJECT_MODE_SLEW | SIMOBJECT_MODE_CRASH | SIMOBJECT_MODE_FREEZE_POSITION
                                                    | SIMOBJECT_MODE_FREEZE_ATTITUDE | SIMOBJECT_MODE_FREEZE_ALTITUDE | SIMOBJECT_MODE_FREEZE_LATLON)));
    }

    static STDMETHODIMP IncSpeed(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
    {
        static_cast<WheeledTank&>(Sim).IncSpeed(); 
        return S_OK;
    }
    static STDMETHODIMP DecSpeed(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
    {
        static_cast<WheeledTank&>(Sim).DecSpeed(); 
        return S_OK;
    }
    static STDMETHODIMP SetSpeedPct(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
    {
        static_cast<WheeledTank&>(Sim).SetSpeedPct(dProperty); 
        return S_OK;
    }
    static STDMETHODIMP ResetSpeed(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
    {
        static_cast<WheeledTank&>(Sim).ResetSpeed();
        return S_OK;
    }
    /******************************************************************************************************
    Doors (Hatches)
    ******************************************************************************************************/
    static STDMETHODIMP OpenHatch(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
    {
        static_cast<WheeledTank&>(Sim).OpenHatch(); 
        return S_OK;
    }
    static STDMETHODIMP CloseHatch(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
    {
        static_cast<WheeledTank&>(Sim).CloseHatch(); 
        return S_OK;
    }
    static STDMETHODIMP GetDoorPercent(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const WheeledTank&>(Sim).GetHatchPercent(); 
        return S_OK;
    }
    /******************************************************************************************************
    Turret
    ******************************************************************************************************/
    static STDMETHODIMP GetTurretPitch(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const WheeledTank&>(Sim).GetTurretPitch(); 
        return S_OK;
    }
    static STDMETHODIMP TurretPitchUp(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
    {
        static_cast<WheeledTank&>(Sim).TurretPitchUp(); 
        return S_OK;
    }
    static STDMETHODIMP TurretPitchDown(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
    {
        static_cast<WheeledTank&>(Sim).TurretPitchDown(); 
        return S_OK;
    }
    static STDMETHODIMP GetTurretRotation(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const WheeledTank&>(Sim).GetTurretRotation();
        return S_OK;
    }
    static STDMETHODIMP TurretRotateRight(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
    {
        static_cast<WheeledTank&>(Sim).TurretRotateRight(); 
        return S_OK;
    }
    static STDMETHODIMP TurretRotateLeft(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
    {
        static_cast<WheeledTank&>(Sim).TurretRotateLeft(); 
        return S_OK;
    }

    static STDMETHODIMP SetTurretPitchSpeedPct(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
    {
        static_cast<WheeledTank&>(Sim).SetTurretPitchSpeedPct((float)dProperty);
        return S_OK;
    }

    static STDMETHODIMP SetTurretRotationSpeedPct(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
    {
        static_cast<WheeledTank&>(Sim).SetTurretRotationSpeedPct((float)dProperty);
        return S_OK;
    }

    /******************************************************************************************************
    Steering
    ******************************************************************************************************/
    static STDMETHODIMP GetSteeringDirection(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const WheeledTank&>(Sim).GetSteeringDirection(); 
        return S_OK;
    }
    static STDMETHODIMP GetSteerAngle(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const WheeledTank&>(Sim).GetSteerAngle(); 
        return S_OK;
    }
    static STDMETHODIMP IncSteeringDirection(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
    {
        static_cast<WheeledTank&>(Sim).IncSteeringDirection(); 
        return S_OK;
    }
    static STDMETHODIMP DecSteeringDirection(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
    {
        static_cast<WheeledTank&>(Sim).DecSteeringDirection(); 
        return S_OK;
    }
    static STDMETHODIMP ResetSteeringDirection(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
    {
        static_cast<WheeledTank&>(Sim).ResetSteeringDirection();
        return S_OK;
    }
    static STDMETHODIMP SetSteeringDirectionPct(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
    {
        static_cast<WheeledTank&>(Sim).SetSteeringDirectionPct(dProperty); 
        return S_OK;
    }
    static STDMETHODIMP GetWheelRotationRate(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const WheeledTank&>(Sim).GetWheelRotationRate(); 
        return S_OK;
    }
    static STDMETHODIMP GetWheelRotationAngle(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const WheeledTank&>(Sim).GetWheelRotationAngle(); 
        return S_OK;
    }

    float GetHatchPercent() const {return m_fHatchPercent;}
    void  OpenHatch()             {if(m_fHatchPercent < 1.0f){m_fHatchPercent += 0.05f;};}
    void  CloseHatch()            {if(m_fHatchPercent > 0.0f){m_fHatchPercent -= 0.05f;};}

    float GetSteeringDirection() const {return m_fSteerAngle;}
    void  IncSteeringDirection() { m_fSteerKeyInput = (float)MIN_STEER_ANGLE; }
    void  DecSteeringDirection() { m_fSteerKeyInput = (float)-MIN_STEER_ANGLE; }
    void  ResetSteeringDirection() { m_fSteerKeyInput = 0.0f; }
    void  SetSteeringDirectionPct(double dDirectionPct) { m_fSteerAxisInput = (float)dDirectionPct; }

    float GetSteerAngle()                       const {return m_fSteerAngle;}
    void  SetSteerAngle(float angle)                  { m_fSteerAngle = angle; }

    void  UpdateSteering(double dDeltaT);
    void  UpdateWheelRotation(double dVel, double dDeltaT);

    float GetWheelRotationRate()  const {return m_fWheelRotationRate;}

    float GetWheelRotationAngle()               const { return m_fWheelRotationAngle; }

    float GetTurretPitch()                      const { return m_fTurretPitch; }
    void  SetTurretPitch(float pitch)                 { m_fTurretPitch = pitch; }

    float GetTurretRotation()                   const { return m_fTurretRotation; }
    void  SetTurretRotation(float rotation)           { m_fTurretRotation = rotation; }

    void  UpdateTurretOrientation(double dDeltaT);

    void TurretPitchUp()        
    {
        m_fTurretPitch += 0.01f;

        //Limit upward angle
        m_fTurretPitch = (float)min(m_fTurretPitch, DEG_TO_RAD(15.0));

        //No matter what, never exceed 90 degrees
        m_fTurretPitch = (float)min(m_fTurretPitch, DEG_TO_RAD(90.0));

    }

    void TurretPitchDown()      
    {
        m_fTurretPitch -= 0.01f;

        //Limit downward angle
        m_fTurretPitch = (float)max(m_fTurretPitch, DEG_TO_RAD(-6.0));

        //No matter what, never exceed 90 degrees
        m_fTurretPitch = (float)max(m_fTurretPitch, DEG_TO_RAD(-90.0));
    }

    void TurretRotateRight()    
    {
        m_fTurretRotation += 0.01f;

        //Wrap if passing 180 degrees
        if (m_fTurretRotation > PI)
        {
            m_fTurretRotation -= (float)(2.0 * PI);
        }
    }

    void TurretRotateLeft()     
    {
        m_fTurretRotation -= 0.01f;

        //Wrap if passing -180 degrees
        if (m_fTurretRotation < -PI)
        {
            m_fTurretRotation += (float)(2.0 * PI);
        }
    }

    // change the pitch speed of the turret based on axis input
    void SetTurretPitchSpeedPct(float input)
    {
        m_fTurretPitchSpeed = 0.25f * input;
    }

    // change the rotation speed of the turret based on axis input
    void SetTurretRotationSpeedPct(float input)
    {
        m_fTurretRotationSpeed = 0.25f * input;
    }

    WorldConstants  GetWorldConstants() const {return ms_WorldConstants;}

    void InitGuns();

private:
    WheeledTank::WheeledTank(IBaseObjectV400* pBaseObject)
        :   m_RefCount(1), 
            m_bSupportsLabel(false),
            m_Engine(pBaseObject),
            m_spBaseObject(pBaseObject),
            m_Simulation(pBaseObject),
            m_pFixedData(NULL),
            m_ContactReaction(pBaseObject),
            m_fSteerAngle(0.0f),
            m_fTargetSteerAngle(0.0f),
            m_fSteerAxisInput(0.0f),
            m_fSteerKeyInput(0.0f),
            m_fWheelRotationRate(0.0),
            m_fWheelRotationAngle(0.0),
            m_fHatchPercent(0.0),
            m_fTurretPitch(0.0),
            m_fTurretRotation(0.0),
            m_fTurretPitchSpeed(0.0),
            m_fTurretRotationSpeed(0.0),
            m_pTankCannon(NULL),
            m_pGunSystem(NULL),
            m_bGunsInitialized(false)
    {
        memset(&m_vdPosition,    0, sizeof(m_vdPosition));
        memset(&m_vdOrientation, 0, sizeof(m_vdOrientation));
        memset(&m_vdVel,         0, sizeof(m_vdVel));
        memset(&m_vdRotVel,      0, sizeof(m_vdRotVel));

        m_Doors = new Doors();
    }

    HRESULT Load();

    /******************************************************************************************************
    Doors (Hatches)
    ******************************************************************************************************/
    float m_fHatchPercent;

    /******************************************************************************************************
    Steering
    ******************************************************************************************************/       
    float m_fSteerAngle;   // the local steering angle
    float m_fTargetSteerAngle;  // the local target steering angle
    float m_fSteerAxisInput;    // target steer angle from axis input
    float m_fSteerKeyInput;

    bool  m_bSteerInput;            // received steering input from user

    /******************************************************************************************************
    Wheel Rotation
    ******************************************************************************************************/
    float m_fWheelRotationRate;
    float m_fWheelRotationAngle;

    /******************************************************************************************************
    Turret
    ******************************************************************************************************/
    float   m_fTurretPitch;
    float   m_fTurretRotation;
    float   m_fTurretPitchSpeed;
    float   m_fTurretRotationSpeed;

    /******************************************************************************************************
    Member vars
    ******************************************************************************************************/
    CComPtr<IBaseObjectV400> m_spBaseObject;
    const WheeledTankFixedData* m_pFixedData;

    MassProperties m_MassProperties;

    Doors* m_Doors;
    Simulation m_Simulation;
    Engine m_Engine;
    ContactReaction m_ContactReaction;
    TankCannon* m_pTankCannon;
    IGunSystemV400* m_pGunSystem;

    static WorldConstants  ms_WorldConstants;

    DXYZ  m_vdPosition;
    DXYZ  m_vdOrientation;
    DXYZ  m_vdVel;
    DXYZ  m_vdRotVel;

    bool m_bGunsInitialized;
    bool m_bSupportsLabel;
};


class WheeledTankFixedData
{
public:
    WheeledTankFixedData()  {}
    ~WheeledTankFixedData() {}

    HRESULT Load(__in __notnull IBaseObjectV400* pBaseObject) const;
    HRESULT Unload(__in __notnull IBaseObjectV400* pBaseObject) const;

private:
   
};



#define CLSID_WheeledTank __uuidof(WheeledTank)

