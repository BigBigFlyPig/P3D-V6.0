/************************************************************************************************************** 
    Missile.cpp

    Copyright (C) Lockheed Martin Corporation.  All rights reserved.
    
    Description: A sample demonstrating how to implement a Weapon ISimObject.
***************************************************************************************************************/

#include <atlcomcli.h>
#include <math.h>
#include <InitGuid.h>
#include <SimConnect.h>
#include <Pdk.h>
#include <ISimObject.h>
#include <ISimObjectAttachments.h>

#include "IMissile.h"

#include <strsafe.h>

using namespace P3D;

extern HANDLE  g_hSimConnect;
extern const ISimObjectManagerV400* GetSimObjectManager();

static double GetPolarRadius();
static double GetEquatorialRadius();
static double GetGravity();

//Body <-> World Matrix
class Matrix
{
public:
    Matrix()
    {
        memset(&m_dMatrix, 0, sizeof(m_dMatrix));
    }

    void BodyToWorld(__in const DXYZ& vBody, __out DXYZ& vWorld)
    {
        vWorld.dZ = m_dMatrix[0][0] * vBody.dZ + m_dMatrix[0][1] * vBody.dX + m_dMatrix[0][2] * vBody.dY;  //l1 m1  n1
        vWorld.dX = m_dMatrix[1][0] * vBody.dZ + m_dMatrix[1][1] * vBody.dX + m_dMatrix[1][2] * vBody.dY;  //l2 m2  n2
        vWorld.dY = m_dMatrix[2][0] * vBody.dZ + m_dMatrix[2][1] * vBody.dX + m_dMatrix[2][2] * vBody.dY;  //l3 m3  n3
    }

    void WorldToBody(__in const DXYZ& vWorld, __out DXYZ& vBody)
    {
        vBody.dZ = m_dMatrix[0][0] * vWorld.dZ + m_dMatrix[1][0] * vWorld.dX + m_dMatrix[2][0] * vWorld.dY;  //l1 l2  l3
        vBody.dX = m_dMatrix[0][1] * vWorld.dZ + m_dMatrix[1][1] * vWorld.dX + m_dMatrix[2][1] * vWorld.dY;  //m1 m2  m3
        vBody.dY = m_dMatrix[0][2] * vWorld.dZ + m_dMatrix[1][2] * vWorld.dX + m_dMatrix[2][2] * vWorld.dY;  //n1 n2  n3
    }

    void Update(const DXYZ& vdOrient)
    {
        double dCosPitch   = cos(vdOrient.dX);
        double dSinPitch   = sin(vdOrient.dX);
        double dCosBank    = cos(vdOrient.dZ);
        double dSinBank    = sin(vdOrient.dZ);
        double dCosHeading = cos(vdOrient.dY);
        double dSinHeading = sin(vdOrient.dY);

        m_dMatrix[0][0] = dCosPitch * dCosHeading;  // l1
        m_dMatrix[1][0] = dCosPitch * dSinHeading;  // l2
        m_dMatrix[2][0] = -dSinPitch;  // l3

        m_dMatrix[0][1] = dSinBank * dSinPitch * dCosHeading - dCosBank * dSinHeading; // m1
        m_dMatrix[1][1] = dSinBank * dSinPitch * dSinHeading + dCosBank * dCosHeading; // m2
        m_dMatrix[2][1] = dSinBank * dCosPitch;  // m3
                            
        m_dMatrix[0][2] = dCosBank * dSinPitch * dCosHeading + dSinBank * dSinHeading;  // n1
        m_dMatrix[1][2] = dCosBank * dSinPitch * dSinHeading - dSinBank * dCosHeading;  // n2
        m_dMatrix[2][2] = dCosBank * dCosPitch;  // n3
    }

    double m_dMatrix[3][3];
};

/***********************************************
"Fixed" data constants
***********************************************/
struct 
{
    float fAttachPointX;
    float fAttachPointY;
    float fAttachPointZ;
    float fMass;
    FXYZ  vMoi;
    float fMaxThrust;
    float fTimeToArm;

}FixedData = 
{
    0.0f,               //X Offset in which it attaches to aircraft pylon
    0.3f,               //Y Offset in which it attaches to aircraft pylon
    0.0f,               //Z Offset in which it attaches to aircraft pylon
    6.0f,               //Mass
    5.0f,5.0f,2.5f,     //MOI
    2000.0f,            //Max Thrust
    2.0f,               //Time to arm
};

/************************************************************************************************************
************************************************************************************************************
GuidanceSystem - sample guidance system implementation
************************************************************************************************************
************************************************************************************************************/
class MassProperties : public IMassPropertiesV01
{
public:
    MassProperties() : m_RefCount(1) {}
    ~MassProperties() {}

    /*******************************************************
    *******************************************************/
    STDMETHOD_(ULONG, AddRef)()   override {return InterlockedIncrement(&m_RefCount) ;}
    STDMETHOD_(ULONG, Release)()  override 
    {
        ULONG   RetVal = InterlockedDecrement(&m_RefCount);
        if      (RetVal == 0)         {delete this;}
        else if (RetVal & 0x80000000) {__debugbreak();}
        return RetVal;
    }

    /*******************************************************
    *******************************************************/
    STDMETHOD (QueryInterface)(__in REFIID riid, void **ppv)  override 
    {
        HRESULT hr = E_NOINTERFACE;

        if (!ppv)
        {
            goto Error;
        }
    
        *ppv = NULL;

        if (IsEqualIID(riid , IID_IUnknown))
        {
            *ppv = static_cast<IUnknown*>(this);
        }        
        else if (IsEqualIID(riid , IID_IMassProperties))
        {
            *ppv = static_cast<IMassProperties*>(this);
        }        
        else if (IsEqualIID(riid , IID_IMassPropertiesV01))
        {
            *ppv = static_cast<IMassPropertiesV01*>(this);
        }        
            
        if (*ppv)
        {
            hr = S_OK;
            AddRef();
        }

    Error:

        return hr;
    }

private:

    STDMETHOD_ (float,  GetWeight())                                        const override {return (float)(FixedData.fMass * GetGravity());}
    STDMETHOD_ (BOOL,   RegisterMass(__in const IMassElement* pElement))          override {return TRUE;}
    STDMETHOD_ (BOOL,   UnRegisterMass(__in const IMassElement* pElement))        override {return TRUE;}
    STDMETHOD_ (void,   ForceUpdate())                                            override {return;}

    volatile LONG   m_RefCount;
};

/************************************************************************************************************
************************************************************************************************************
GuidanceSystem - sample guidance system implementation
************************************************************************************************************
************************************************************************************************************/
class GuidanceSystem : public IGuidanceSystemV01
{
public:
    GuidanceSystem() : m_RefCount(1), m_uTargetId(0)  {}
    ~GuidanceSystem() {}

    /*******************************************************
    *******************************************************/
    STDMETHOD_(ULONG, AddRef)()   override {return InterlockedIncrement(&m_RefCount) ;}
    STDMETHOD_(ULONG, Release)()  override 
    {
        ULONG   RetVal = InterlockedDecrement(&m_RefCount);
        if      (RetVal == 0)         {delete this;}
        else if (RetVal & 0x80000000) {__debugbreak();}
        return RetVal;
    }

    /*******************************************************
    *******************************************************/
    STDMETHOD (QueryInterface)(__in REFIID riid, void **ppv)  override 
    {
        HRESULT hr = E_NOINTERFACE;

        if (!ppv)
        {
            goto Error;
        }
    
        *ppv = NULL;

        if (IsEqualIID(riid , IID_IUnknown))
        {
            *ppv = static_cast<IUnknown*>(this);
        }        
        else if (IsEqualIID(riid , IID_IGuidanceSystem))
        {
            *ppv = static_cast<IGuidanceSystem*>(this);
        }        
        else if (IsEqualIID(riid , IID_IGuidanceSystemV01))
        {
            *ppv = static_cast<IGuidanceSystemV01*>(this);
        }        
            
        if (*ppv)
        {
            hr = S_OK;
            AddRef();
        }

    Error:

        return hr;
    }

    /*******************************************************
    *******************************************************/
    STDMETHOD_(void,   SetTargetObjectID)(UINT uTargetId)        override {m_uTargetId = uTargetId;}
    STDMETHOD_(UINT,   GetTargetObjectID)()                const override {return m_uTargetId;}
    STDMETHOD_(void,   SetTargetLLA)(__in const DXYZ& vLLA)      override {}
    STDMETHOD_(BOOL,   GetTargetLLA)(__out DXYZ& vLLA)     const override {return FALSE;}

    /*******************************************************
    *******************************************************/
    HRESULT Update(double dDeltaT)
    {
        HRESULT hr = S_OK;

        if (m_uTargetId != 0)  //Valid id?  
        {
            //Get ref to target
            CComPtr<IBaseObjectV400> spTarget;
            if (FAILED(GetSimObjectManager()->GetObject(m_uTargetId, &spTarget)))
            {
                hr = E_FAIL;
            }
            else
            {
                //Do some interesting tracking, flight control algorithms here based on target position
            }
        }

        return hr;
    }

private:

    volatile LONG   m_RefCount;
    UINT            m_uTargetId;    //Prepar3D target ID
};


class Simulation : public ISimulationV01
{
public:
    Simulation(IBaseObjectV400* pBaseObject) 
        :   m_RefCount(1), 
            m_spBaseObject(pBaseObject), 
            m_bIsAttached(FALSE),
            m_uOwnerId(0),  //0 = invalid
            m_bOnGround(FALSE),
            m_fThrust(0),
            m_fTimeSinceFired(0)
    {
        memset(&m_SurfaceInfo, 0, sizeof(m_SurfaceInfo));
        memset(&m_WeatherInfo, 0, sizeof(m_WeatherInfo));

        memset(&m_vdPos,     0, sizeof(m_vdPos));
        memset(&m_vdOrient,  0, sizeof(m_vdOrient));
        memset(&m_vdVel,    0, sizeof(m_vdVel));
        memset(&m_vdRotVel, 0, sizeof(m_vdRotVel));

        memset(&m_vBodyVelocity,         0, sizeof(m_vBodyVelocity));
        memset(&m_vBodyRotationVelocity, 0, sizeof(m_vBodyRotationVelocity));
    }

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

        if (IsEqualIID(riid , IID_IUnknown))
        {
            *ppv = static_cast<IUnknown*>(this);
        }        
        else if (IsEqualIID(riid , IID_ISimulation))
        {
            *ppv = static_cast<ISimulation*>(this);
        }        
        else if (IsEqualIID(riid , IID_ISimulationV01))
        {
            *ppv = static_cast<ISimulationV01*>(this);
        }        
            
        if (*ppv)
        {
            hr = S_OK;
            AddRef();
        }

    Error:

        return hr;
    }


    //Get owner's object id
    BOOL CanWeaponBeReleased()      const {return TRUE;}
    UINT GetOwnerId()               const {return m_uOwnerId;}
    BOOL IsAttachedToOwner()        const {return m_bIsAttached;}

    double GetSpeed()                const {return m_vBodyVelocity.dZ;}

    STDMETHOD (SetIsAttachedToOwner)(BOOL bAttached, UINT uOwnerId)
    {
        HRESULT hr = E_FAIL;

        if (bAttached != m_bIsAttached)
        {
            m_bIsAttached = bAttached;

            //If being detached...
            if (!m_bIsAttached)
            {
                CComPtr<IBaseObjectV400> spOwner;

                //Get a reference to the Prepar3d SDK interface AND the SimObjectManager
                if (SUCCEEDED(GetSimObjectManager()->GetObject(m_uOwnerId, &spOwner)))  //Get a reference to parent object that's releasing us
                {
                    DXYZ vOwnerPos;
                    DXYZ vOwnerOrient;
                    DXYZ vOwnerVel;
                    DXYZ vOwnerRotationVel;

                    //Get owner's world pos/vel
                    spOwner->GetPosition(vOwnerPos, vOwnerOrient, vOwnerVel, vOwnerRotationVel);

                    //For simplicity, this assumes this weapon orientation aligned with parent    
                    m_Matrix.WorldToBody(vOwnerVel, m_vBodyVelocity);
                }

                //For simplicity, zero rotational vel
                m_vBodyRotationVelocity.dX = 0;
                m_vBodyRotationVelocity.dY = 0;
                m_vBodyRotationVelocity.dZ = 0;
            }

            hr = S_OK;
        }

        if(m_bIsAttached)
        {
            m_uOwnerId = uOwnerId;

            hr = S_OK;
        }

        return hr;
    }

    GuidanceSystem& GetGuidanceSystem()  {return m_GuidanceSystem;}
    MassProperties& GetMassProperties()  {return m_MassProperties;}

private:
    volatile LONG   m_RefCount;

    CComPtr<IBaseObjectV400> m_spBaseObject;

    STDMETHOD (SaveLoadState)(__in __notnull PSaveLoadCallback pfnCallback, __in const BOOL bSave) override {return S_OK;}  

    STDMETHOD (Update)(double dDeltaT)   override 
    {
        BOOL bShouldSimulate = (0 == (m_spBaseObject->GetMode() & ( SIMOBJECT_MODE_PAUSE | SIMOBJECT_MODE_DISABLED | SIMOBJECT_MODE_SLEW | SIMOBJECT_MODE_CRASH | SIMOBJECT_MODE_FREEZE_POSITION
                                                    | SIMOBJECT_MODE_FREEZE_ATTITUDE | SIMOBJECT_MODE_FREEZE_ALTITUDE | SIMOBJECT_MODE_FREEZE_LATLON)));

        if (bShouldSimulate)   //i.e. No flags
        {
            static const float MAX_SPEED = 50.0f;


            //Update surface and weather info
            m_spBaseObject->GetSurfaceInformation(m_SurfaceInfo, NULL);
            m_spBaseObject->GetWeatherInformation(m_WeatherInfo);

            //Get current position
            m_spBaseObject->GetPosition(m_vdPos, m_vdOrient, m_vdVel, m_vdRotVel);
            //Update matrix
            m_Matrix.Update(m_vdOrient);

            //Simulation subsystems
            m_GuidanceSystem.Update(dDeltaT);
            DoContactReaction(dDeltaT);
            DoThrust(dDeltaT);

            //Total body force
            DXYZ vdBodyForce;
            vdBodyForce.dX = 0;
            vdBodyForce.dY = 0;
            vdBodyForce.dZ = m_fThrust;

            //Total Body Moment
            DXYZ vdBodyMoment;
            vdBodyMoment.dX = 0;
            vdBodyMoment.dY = 0;
            vdBodyMoment.dZ = 0;

            if (m_bIsAttached)
            {
                m_Matrix.WorldToBody(m_vdVel, m_vBodyVelocity);
                m_vBodyRotationVelocity.dX = 0;
                m_vBodyRotationVelocity.dY = 0;
                m_vBodyRotationVelocity.dZ = 0;

                //The following function is simply to demonstrate querying a property from another ISimObject
                double dParentVel = GetParentTotalVelocity();
            }
            else
            {
                m_fTimeSinceFired += (float)dDeltaT;

                //Total body acceleration
                DXYZ vdBodyAcceleration;
                vdBodyAcceleration.dX = (vdBodyForce.dX / FixedData.fMass) - GetGravity() * cos(m_vdOrient.dX) * sin(m_vdOrient.dZ);
                vdBodyAcceleration.dY = (vdBodyForce.dY / FixedData.fMass) - GetGravity() * cos(m_vdOrient.dX) * cos(m_vdOrient.dZ);
                vdBodyAcceleration.dZ = (vdBodyForce.dZ / FixedData.fMass) + GetGravity() * sin(m_vdOrient.dX);

                //Integrate body velocity
                m_vBodyVelocity.dX += vdBodyAcceleration.dX  * dDeltaT;
                m_vBodyVelocity.dY += vdBodyAcceleration.dY  * dDeltaT;
                m_vBodyVelocity.dZ += vdBodyAcceleration.dZ  * dDeltaT;

                //Transform to World velocity
                m_Matrix.BodyToWorld(m_vBodyVelocity, m_vdVel);

                //Integrate world postion
                m_vdPos.dX += m_vdVel.dX * cos(m_vdPos.dZ) * dDeltaT / GetEquatorialRadius();
                m_vdPos.dY += m_vdVel.dY * dDeltaT;
                m_vdPos.dZ += m_vdVel.dZ * dDeltaT / GetPolarRadius();


                //Total Body Rotation Acceleration
                DXYZ vdBodyRotaionAcceleration;
                vdBodyRotaionAcceleration.dX = vdBodyMoment.dX / FixedData.vMoi.fX;
                vdBodyRotaionAcceleration.dY = vdBodyMoment.dY / FixedData.vMoi.fY;
                vdBodyRotaionAcceleration.dZ = vdBodyMoment.dZ / FixedData.vMoi.fZ;

                //Body Rotation Velocity
                m_vBodyRotationVelocity.dX += vdBodyRotaionAcceleration.dX  * dDeltaT;
                m_vBodyRotationVelocity.dY += vdBodyRotaionAcceleration.dY  * dDeltaT;
                m_vBodyRotationVelocity.dZ += vdBodyRotaionAcceleration.dZ  * dDeltaT;

                //World Rotation Velocity
                m_vdRotVel.dX = m_vBodyRotationVelocity.dX * cos(m_vdOrient.dZ) - m_vBodyRotationVelocity.dY * sin(m_vdOrient.dZ);
                m_vdRotVel.dY = m_vBodyRotationVelocity.dY * cos(m_vdOrient.dZ) / cos(m_vdOrient.dX) + m_vBodyRotationVelocity.dX * sin(m_vdOrient.dZ) / cos(m_vdOrient.dX);
                m_vdRotVel.dZ = m_vBodyRotationVelocity.dZ + m_vBodyRotationVelocity.dX * sin(m_vdOrient.dZ) * tan(m_vdOrient.dX) + m_vBodyRotationVelocity.dY * cos(m_vdOrient.dZ) * tan(m_vdOrient.dX);

                m_vdOrient.dX += m_vdRotVel.dX * dDeltaT;
                m_vdOrient.dY += m_vdRotVel.dY * dDeltaT;
                m_vdOrient.dZ += m_vdRotVel.dZ * dDeltaT;
            }

            //Update Prepar3D object
            m_spBaseObject->SetPosition(m_vdPos, m_vdOrient, m_vdVel, m_vdRotVel, m_bOnGround, dDeltaT);

            if (!m_bIsAttached)
            {
                //Check for collision
                CheckCollision();
            }
        }

        return S_OK;
    }

    /**********************************************************
    DoContactReaction
    **********************************************************/
    void DoContactReaction(double dDeltaT)
    {
        if (m_vdPos.dY <= m_SurfaceInfo.m_fElevation + 1.0)  //close to ground?
        {
            m_bOnGround = TRUE;
        }
        else
        {
            m_bOnGround = FALSE;
        }
    }

    /**********************************************************
    DoThrust
    **********************************************************/
    void DoThrust(double dDeltaT)
    {
        if (!m_bIsAttached)
        {
            m_fThrust = FixedData.fMaxThrust;
        }
        else
        {
            m_fThrust = 0;
        }
    }

    /******************************************************
    CheckCollision
    ******************************************************/
    void CheckCollision()
    {
        BOOL bDestroySelf = FALSE;

        if (IsArmed())
        {
            P3D::COLLISIONTYPE eCollision = COLLISIONTYPE_NONE;
            CComPtr<IUnknown> spUnkHitObject;
            
            if (SUCCEEDED(m_spBaseObject->CheckCollision(/*radius feet*/30.0f, eCollision, &spUnkHitObject)) 
                && spUnkHitObject
                && eCollision != COLLISIONTYPE_NONE)
            
            {
                CComPtr<IBaseObjectV400> spHitObject;
            
                if (SUCCEEDED(spUnkHitObject->QueryInterface(IID_IBaseObjectV400, (void**)&spHitObject)))
                {
                    CComPtr<IWeaponServiceV400>     spWeapon;
                    CComPtr<ICountermeasureService> spCountermeasure;
                    CComPtr<IPylonService>          spPylon;

                    if (  (FAILED(spHitObject->QueryService(SID_WeaponService,         IID_IWeaponServiceV400,     (void**)&spWeapon)))
                       && (FAILED(spHitObject->QueryService(SID_CountermeasureService, IID_ICountermeasureService, (void**)&spCountermeasure)))
                       && (FAILED(spHitObject->QueryService(SID_PylonService,          IID_IPylonService,          (void**)&spPylon))))
                    {
                        spHitObject->DecrementHealthPoints(1.0/*Damage Points*/);

                        //Report damage
                        SIMCONNECT_DATA_OBJECT_DAMAGED_BY_WEAPON WeaponDamageData = {m_spBaseObject->GetId(), GetOwnerId(), spHitObject->GetId()};
                        SimConnect_ReportWeaponDamage(g_hSimConnect, WeaponDamageData);
 
                        bDestroySelf = TRUE;
                    }
                }
            }

            //Splash damage
            static const UINT REQUESTED_OBJECT_COUNT = 30;
            UINT rgObjectIDs[REQUESTED_OBJECT_COUNT] = {0};
            UINT nObjects = REQUESTED_OBJECT_COUNT;
            if (SUCCEEDED(GetSimObjectManager()->GetObjectsInRadius(m_vdPos, 100.0f /* fRadiusFeet*/, nObjects, rgObjectIDs)))
            {
                for (UINT i = 0; i < nObjects; i++)
                {
                    CComPtr<IBaseObjectV400> spSplashObject;

                    if (SUCCEEDED(GetSimObjectManager()->GetObject(rgObjectIDs[i], &spSplashObject)))
                    {
                        //Don't splash self
                        if (GetOwnerId() == rgObjectIDs[i])
                        {
                            continue;
                        }

                        //Don't damage weapons
                        CComPtr<IWeaponServiceV400> spWeapon;
                        CComPtr<ICountermeasureService> spCountermeasure;
                        CComPtr<IPylonService>  spPylon;

                        if (  (SUCCEEDED(spSplashObject->QueryService(SID_WeaponService,         IID_IWeaponServiceV400,     (void**)&spWeapon)))
                           || (SUCCEEDED(spSplashObject->QueryService(SID_CountermeasureService, IID_ICountermeasureService, (void**)&spCountermeasure)))
                           || (SUCCEEDED(spSplashObject->QueryService(SID_PylonService,          IID_IPylonService,          (void**)&spPylon))))
                        {
                            continue;
                        }

                        spSplashObject->DecrementHealthPoints(100);

                        //Report damage
                        SIMCONNECT_DATA_OBJECT_DAMAGED_BY_WEAPON WeaponDamageData = {m_spBaseObject->GetId(), GetOwnerId(), spSplashObject->GetId()};
                        SimConnect_ReportWeaponDamage(g_hSimConnect, WeaponDamageData);
                    }
                }
            }
        }

        //If we hit the ground, spawn effect and destroy self.
        if (m_bOnGround)
        {
            m_spBaseObject->VisualEffectOn(TEXT("fx_explosionDestroyLandMedium"), NULL /*&vOffset*/, NULL /*&pEffect*/);
            bDestroySelf = TRUE;
        }

        if (bDestroySelf)
        {
            m_spBaseObject->Destroy();
        }
    }

    /******************************************************
    The following is for demonstrating querying a property
    of another ISimObject
    ******************************************************/
    double GetParentTotalVelocity() 
    {
        double dParentVel = 0;

        if (m_bIsAttached)
        {
            CComPtr<IBaseObjectV400> spOwner;

            if (SUCCEEDED(GetSimObjectManager()->GetObject(m_uOwnerId, &spOwner)))
            {
                //Method 1
                spOwner->GetProperty(TEXT("TotalVelocity"), TEXT("knots"), dParentVel, 0);

                //Method 2
                int iPropertyCode;
                int iIndex = -1;
                int iUnitCode;
                if (SUCCEEDED(spOwner->GetPropertyCodeAndIndex(PROPERTY_TYPE_DOUBLE, TEXT("TotalVelocity"), iPropertyCode, iIndex))
                    && SUCCEEDED(GetSimObjectManager()->GetUnitCode(TEXT("knots"), iUnitCode)))
                {
                    spOwner->GetProperty(iPropertyCode, iUnitCode, dParentVel, iIndex);
                }
            }
        }

        return dParentVel;
    }

    /******************************************************
    ******************************************************/
    BOOL IsArmed() const {return m_fTimeSinceFired > FixedData.fTimeToArm;}

    /******************************************************
    ******************************************************/
    float m_fThrust;
    float m_fTimeSinceFired;

    //World-relative state
    DXYZ m_vdPos;
    DXYZ m_vdOrient;
    DXYZ m_vdVel;
    DXYZ m_vdRotVel;

    DXYZ m_vBodyRotationVelocity;
    DXYZ m_vBodyVelocity;

    BOOL m_bOnGround;

    Matrix m_Matrix;

    BOOL m_bIsAttached;
    UINT m_uOwnerId;    //id of object in which this weapon is attached

    GuidanceSystem m_GuidanceSystem;
    MassProperties m_MassProperties;

    SurfaceInfoV400 m_SurfaceInfo;
    WeatherInfoV400 m_WeatherInfo;
};

// Ensure this GUID matches the CategoryId in the sim.cfg
class __declspec(uuid("{722F784D-1196-4C40-BCAA-F1D6482B1958}")) Missile : public ISimObjectV02, public IWeaponServiceV400, public IMissile
{
public:

    //IServiceProvider Implementation
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

        if (IsEqualIID(riid , IID_IUnknown))
        {
            *ppv = static_cast<Missile*>(this);
        }        
        else if (IsEqualIID(riid , IID_IServiceProvider))
        {
            *ppv = static_cast<IServiceProvider*>(this);
        }
        else if (IsEqualIID(riid, IID_ISimObject))
        {
            *ppv = static_cast<ISimObject*>(this);
        }
        else if (IsEqualIID(riid, IID_ISimObjectV01))
        {
            *ppv = static_cast<ISimObjectV01*>(this);
        }
        else if (IsEqualIID(riid, IID_ISimObjectV02))
        {
            *ppv = static_cast<ISimObjectV02*>(this);
        }
        else if (IsEqualIID(riid, IID_IWeaponServiceV400))
        {
            *ppv = static_cast<IWeaponServiceV400*>(this);
        }
        else if (IsEqualIID(riid, IID_IMissile))
        {
            *ppv = static_cast<IMissile*>(this);
        }
                

        if (*ppv)
        {
            hr = S_OK;
            AddRef();
        }

    Error:

        return hr;
    }
    
    STDMETHOD (QueryService)(  __in REFGUID guidService, __in REFIID riid, void **ppv) override 
    {
        HRESULT hr = E_NOINTERFACE;

        if (!ppv)
        {
            goto Error;
        }
    
        *ppv = NULL;
    
        if (IsEqualIID(guidService, SID_SimObject))
        {
            hr = QueryInterface(riid, ppv);
        }
        else if (IsEqualIID(guidService, SID_WeaponService))
        {
            hr = QueryInterface(riid, ppv);
        }
        else if (IsEqualIID(guidService, SID_Missile))
        {
            hr = QueryInterface(riid, ppv);
        }
        else if (IsEqualIID(guidService, SID_Simulation))
        {
            hr = m_Simulation.QueryInterface(riid, ppv);
        }
        else if (IsEqualIID(guidService, SID_GuidanceSystem))
        {
            hr = m_Simulation.GetGuidanceSystem().QueryInterface(riid, ppv);
        }
        else if (IsEqualIID(guidService, SID_MassProperties))
        {
            hr = m_Simulation.GetMassProperties().QueryInterface(riid, ppv);
        }

    Error:

        return hr;

    }

    //ISimObject implementations
    STDMETHOD (LoadConstantData)(__out void** ppConstantData)       override {return S_OK;}       //on-disk data.  return loaded data
    STDMETHOD (UnloadConstantData)(__inout void** ppConstantData)   override {return S_OK;}       //on-disk data.  should point to loaded data, and return NULL
    STDMETHOD (LoadDynamicData)()                                   override {return S_OK;}       //e.g. create runtime subsystems based on disk data
    STDMETHOD (Init)()                                              override {return S_OK;}       //e.g. init relationships between subsystems
    STDMETHOD (DeInit)()                                            override {return S_OK;}
    STDMETHOD_(BOOL, SupportsLabels)()                      const   override {return m_bSupportsLabel;}                      //Does this simobject support label display?
    STDMETHOD (SetSupportsLabels)(BOOL bOn)                         override {m_bSupportsLabel = (bOn != FALSE); return S_OK;}     //Should this simobject support label display?
    STDMETHOD_(void, OnModeChange)(int bfMode)                      override {}                   //Called when a mode has changed, with the new mode bitfield
    STDMETHOD_(void, OnPositionInit)()                              override {}                   //Called upon change in non-realtime object positions   (e.g. UI, slew)
    STDMETHOD_(void, OnSpeedInit)(float fSpeed)                     override {}                  //(Feet/Second) Called upon init in speed   (e.g. UI)

    //IWeaponService implementations
    STDMETHOD_(BOOL, CanWeaponBeReleased)()                         const override {return m_Simulation.CanWeaponBeReleased();}
    STDMETHOD_(float, GetAerodynamicsDragCoefficient)(float fMach)  const override {return 0;}
    STDMETHOD_(UINT, GetOwnerId)()                                  const override {return m_Simulation.GetOwnerId();}                   //ID of object in which weapon is attached
    STDMETHOD_(BOOL, IsAttachedToOwner)()                           const override {return m_Simulation.IsAttachedToOwner();}
    STDMETHOD (SetIsAttachedToOwner)(BOOL bAttached, UINT uOwnerId, BOOL bJettisoned) override {return m_Simulation.SetIsAttachedToOwner(bAttached, uOwnerId);}     //Called from weapon system when fired (0 = invalid id)
    STDMETHOD (GetAttachOffsetFeet(__out P3D::DXYZ& vOffset))       const override {vOffset.dX = FixedData.fAttachPointX, vOffset.dY = FixedData.fAttachPointY, vOffset.dZ = FixedData.fAttachPointZ; return S_OK;}

    //Dependent on the weapon implementation, but can be used for arbitrary categorization
    STDMETHOD (GetType)(__out LPWSTR pszType, __in unsigned int uLength)   const override
    {
        return StringCchCopy(pszType, uLength, TEXT("AAM"));
    }

    STDMETHOD_(double, GetSpeed())                const override {return m_Simulation.GetSpeed();} //Feet per second

    //Factory function
    static HRESULT New(__in __notnull IBaseObjectV400* pBaseObject, __out __notnull ISimObject** ppThisSim) 
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

        *ppThisSim = new Missile(spBaseObject);

        if (!*ppThisSim)
        {
            goto Error;
        }

        hr = spBaseObject->RegisterSimulation(&static_cast<Missile*>(*ppThisSim)->m_Simulation, 60.0f /*Hz*/);


    Error:
        return hr;
    }

    static WorldConstants  ms_WorldConstants;

private:
    Missile(IBaseObjectV400* pBaseObject) : m_Simulation(pBaseObject), m_bSupportsLabel(FALSE), m_RefCount(1)  {}

    volatile LONG   m_RefCount;
    Simulation m_Simulation;
    BOOL m_bSupportsLabel;
};

//Unique class Guid
#define CLSID_Missile __uuidof(Missile)

REFGUID GetClassId() {return CLSID_Missile;}

WorldConstants Missile::ms_WorldConstants;

//Factory function wrapper
HRESULT New(__in __notnull IBaseObjectV400* pBaseObject, __out __notnull ISimObject** ppThisSim)
{
    return Missile::New(pBaseObject, ppThisSim);
}

//Property Registration
HRESULT RegisterProperties(__in __notnull ISimObjectManagerV400* pSimObjectMgr)
{
    HRESULT hr = E_FAIL;

    hr = pSimObjectMgr->GetWorldConstants(Missile::ms_WorldConstants);

    return S_OK;
}

static double GetPolarRadius()       {return Missile::ms_WorldConstants.m_dPolarRadius;}
static double GetEquatorialRadius()  {return Missile::ms_WorldConstants.m_dEquatorialRadius;}
static double GetGravity()           {return Missile::ms_WorldConstants.m_fGravitySeaLevel;}
