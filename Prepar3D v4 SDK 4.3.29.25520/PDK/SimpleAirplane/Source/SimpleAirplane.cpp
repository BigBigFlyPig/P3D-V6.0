/************************************************************************************************************** 
    SimpleAirplane.cpp

    Copyright (C) Lockheed Martin Corporation.  All rights reserved.
    
    Description: A sample demonstrating how to implement a simple airplane ISimObject.
***************************************************************************************************************/

#include <atlcomcli.h>
#include <math.h>
#include <float.h>
#include <InitGuid.h>
#include <vector>

#include <Pdk.h>
#include <ISimObject.h>
#include <ISimObjectAI.h>
#include <ISimObjectAttachments.h>
#include <IDataLoader.h>
#include <IGlobalData.h>

#include "..\..\Missile\Source\IMissile.h"

#include <strsafe.h>  // Include Last!

#define MAKE_STATIC 1
#ifdef  MAKE_STATIC
#define STATIC_CONST static const
#else
#define STATIC_CONST 
#endif

#define USE_P3D_AIRPLANE_AI

static const float SIM_RATE = 60.0f; //Hz

using namespace P3D;

extern HRESULT GetPdk(__in REFIID riid, __out __notnull void** ppPdk);

static double GetPolarRadius();
static double GetEquatorialRadius();
static double GetGravity();
static double GetSpecificGasConstant();

#define LIMIT_VECTOR(V, LIM)  \
    if (_isnan(V.dX)) V.dX = 0; \
    if (_isnan(V.dY)) V.dY = 0; \
    if (_isnan(V.dZ)) V.dZ = 0; \
    V.dX = max(V.dX, -LIM); V.dY = max(V.dY, -LIM); V.dZ = max(V.dZ, -LIM); \
    V.dX = min(V.dX,  LIM); V.dY = min(V.dY,  LIM); V.dZ = min(V.dZ,  LIM); \

//Engine failure ID
DEFINE_GUID(ID_EngineFailure,      0xa1294329, 0x809c, 0x48a7, 0xb7, 0x28, 0x95, 0x92, 0x30, 0x2, 0xfa, 0x6e);

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

/**************************************************************************
IFuelService implementation example
**************************************************************************/
class FuelSystem : public IFuelServiceV400
{ 
public:
    //IServiceProvider Implementation
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

        if (IsEqualIID(riid , IID_IFuelServiceV400))
        {
            *ppv = static_cast<IFuelServiceV400*>(this);
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

    FuelSystem() : m_RefCount(1)
    {
        //Init max capacity
        m_aTank[0].Init(750.0f);  
        m_aTank[1].Init(750.0f);
    }
    ~FuelSystem() {}

    STDMETHOD_(UINT,  GetNumberOfTanks)()                                                    const override {return ARRAYSIZE(m_aTank);}
    STDMETHOD_(float, GetWeightPerGallon)()                                                  const override {return 6.7f;}   //Pounds / gallon

    STDMETHOD (GetTankName) (__in int iTankIndex, __out LPWSTR pszName,  __in UINT uLength) const    override 
    {
        HRESULT hr = E_FAIL;

        if (iTankIndex >= 0 && iTankIndex < ARRAYSIZE(m_aTank))
        {
            hr = StringCchPrintf(pszName, uLength, TEXT("Tank %d"), iTankIndex);
        }
        return hr;
    }

    //IFuelService methods
    STDMETHOD_(float, GetTankCapacityGallons) (__in int iTankIndex)  const    override 
    {
        return (iTankIndex >= 0 && iTankIndex < ARRAYSIZE(m_aTank)) ? m_aTank[iTankIndex].GetCapacity() : 0.0f;
    }

    STDMETHOD_(float, GetTankLevelPercent)    (__in int iTankIndex)  const    override
    {
        return (iTankIndex >= 0 && iTankIndex < ARRAYSIZE(m_aTank)) ? m_aTank[iTankIndex].GetLevelPercent() : 0.0f;
    }

    STDMETHOD (SetTankLevelPercent)(__in int iTankIndex, float fPct)  override
    {
        if (iTankIndex >= 0 && iTankIndex < ARRAYSIZE(m_aTank)) {m_aTank[iTankIndex].SetLevelPercent(fPct); return S_OK;} else {return E_FAIL;}
    }

    STDMETHOD_(float, GetTotalCapacityGallons) ()  const    override 
    {
        float fTotalCapacity = 0;

        for (UINT i = 0; i < ARRAYSIZE(m_aTank); i++)
        {
            fTotalCapacity += m_aTank[i].GetCapacity();
        }

        return fTotalCapacity;
    }

    STDMETHOD_(float, GetTotalLevelPercent) ()   const    override 
    {
        float fTotalCapacity = 0;
        float fTotalCurrent  = 0;

        for (UINT i = 0; i < ARRAYSIZE(m_aTank); i++)
        {
            fTotalCapacity += m_aTank[i].GetCapacity();
            fTotalCurrent  += m_aTank[i].GetLevelPercent() * m_aTank[i].GetCapacity();
        }

        return (fTotalCapacity > 0) ? (fTotalCurrent / fTotalCapacity) : 0.0f;
    }

    STDMETHOD  (SetTotalLevelPercent)(float fPct)  override
    {
        if (fPct < 0.0f || fPct > 1.0f)
        {
            return E_INVALIDARG;
        }

        for (UINT i = 0; i < ARRAYSIZE(m_aTank); i++)
        {
            m_aTank[i].SetLevelPercent(fPct);
        }

        return S_OK;
    }

    void Update(double dDeltaT, float fThrottle)
    {
        static const float MAX_FUEL_BURN_RATE = 0.05f; //Gallons per second

        for (UINT i = 0; i < ARRAYSIZE(m_aTank); i++)
        {
            double dDeltaPct = MAX_FUEL_BURN_RATE * fThrottle * dDeltaT / m_aTank[i].GetCapacity();
            double dNewLevel = max(m_aTank[i].GetLevelPercent() - dDeltaPct, 0);

            m_aTank[i].SetLevelPercent((float)dNewLevel);
        }
    }

private:

    static const UINT NUM_FUEL_TANKS = 2;

    /*****************************************
    Individual Tank class
    *****************************************/
    class Tank
    {
    public:
        Tank() : m_fCapacityGallons(0), m_fCurrentGallons(0) {}
        ~Tank() {}

        void Init(float fCapacity)
        {
            m_fCapacityGallons = fCapacity;
            m_fCurrentGallons  = fCapacity;
        }

        float GetCapacity()     const {return m_fCapacityGallons;}
        float GetLevelPercent() const {return (m_fCapacityGallons > 0) ? m_fCurrentGallons / m_fCapacityGallons : 0.0f;}

        void SetLevelPercent(float fPct) {if (fPct >= 0.0f && fPct <= 1.0f) {m_fCurrentGallons = fPct * m_fCapacityGallons;}}

    private:
        float m_fCapacityGallons;
        float m_fCurrentGallons;
    };
    /*****************************************
    *****************************************/

    Tank m_aTank[NUM_FUEL_TANKS];  //Tank list
};


class Simulation : public ISimulationV01
{
public:
    Simulation(IBaseObjectV400& BaseObject) 
        :   m_RefCount(1), 
            m_BaseObject(BaseObject), 
            m_fElevator(0),
            m_fAilerons(0),
            m_fRudder(0),
            m_fThrottle(0.0f),
            m_bAfterburnerActive(FALSE),
            m_fThrust(0.0f),
            m_hEngHealth(0),
            m_bGearHandleDown(TRUE),
            m_fGearExtended((m_bGearHandleDown) ? 1.0f : 0.0f),
            m_fMass(587.0f), //Slugs
            m_dAlpha(0.0),
            m_dBeta(0.0),
            m_dTotalVelocity(0.0),
            m_bOnGround(FALSE)
    {
        memset(&m_SurfaceInfo, 0, sizeof(m_SurfaceInfo));
        memset(&m_WeatherInfo, 0, sizeof(m_WeatherInfo));

        m_vfMOI.fX = 60000.0f;
        m_vfMOI.fY = 41000.0f;
        m_vfMOI.fZ = 50000.0f; 

        memset(&m_vdPos,     0, sizeof(m_vdPos));
        memset(&m_vdOrient,  0, sizeof(m_vdOrient));
        memset(&m_vdVel,    0, sizeof(m_vdVel));
        memset(&m_vdRotVel, 0, sizeof(m_vdRotVel));

        memset(&m_vBodyVelocity,         0, sizeof(m_vBodyVelocity));
        memset(&m_vBodyRotationVelocity, 0, sizeof(m_vBodyRotationVelocity));

        memset(&m_vAeroForce,  0, sizeof(m_vAeroForce));
        memset(&m_vAeroMoment, 0, sizeof(m_vAeroMoment));

        memset(&m_vGroundReactionForce,  0, sizeof(m_vGroundReactionForce));
        memset(&m_vGroundReactionMoment, 0, sizeof(m_vGroundReactionMoment));

        StringCchPrintf(m_szTest, ARRAYSIZE(m_szTest), TEXT("TestString"));
    }
    ~Simulation() {}

    HRESULT Init()
    {
        m_Matrix.Update(m_vdOrient);

        //Register engine failure with P3D
        m_hEngHealth = m_BaseObject.RegisterSystemMalfunction(ID_EngineFailure,         //Unique ID
                                                              TEXT("Engines"),                //UI Type/category - choices are: Instruments, Systems, Radios, Engines, Controls, Structural, Miscellaneous
                                                              TEXT("Engine"),                 //Base Failure name.  Used in mission files
                                                              TEXT("Total Engine Failure"),   //Malfunction instance name.  Used in UI
                                                              0);                       //Engine index

        return S_OK;
    }

    void InitPositionFromBase()
    {
        m_BaseObject.GetPosition(m_vdPos, m_vdOrient, m_vdVel, m_vdRotVel);
        m_Matrix.Update(m_vdOrient);
    }

    void   SetThrottle(float fThrottle)     {m_fThrottle = (float)max(min(fThrottle, 1.0), 0.0);}
    void   IncThrottle()                    {SetThrottle(m_fThrottle += 0.2f);}
    void   DecThrottle()                    {SetThrottle(m_fThrottle -= 0.2f);}
    float  GetThrottle() const              {return m_fThrottle;}
    BOOL   IsAfterburnerActive() const      {return m_bAfterburnerActive;}

    void   SetElevator(float fElevator)     {m_fElevator = (float)max(min(fElevator, 1.0), -1.0);}
    void   SetAilerons(float fAilerons)     {m_fAilerons = (float)max(min(fAilerons, 1.0), -1.0);}
    void   SetRudder  (float fRudder)       {m_fRudder   = (float)max(min(fRudder,   1.0), -1.0);}

    float  GetPctElevator()     const       {return m_fElevator;}
    float  GetPctAilerons()     const       {return m_fAilerons;}
    float  GetPctRudder()       const       {return m_fRudder;}

    void   IncGearLever()                   {m_bGearHandleDown = TRUE;}
    void   DecGearLever()                   {m_bGearHandleDown = FALSE;}
    void   SetGearUp()                      {m_bGearHandleDown = FALSE;}
    void   SetGearDown()                    {m_bGearHandleDown = TRUE;}
    void   ToggleGear()                     {m_bGearHandleDown = !m_bGearHandleDown;}
    void   GearLeverSet(float fPctGearHandle) {m_bGearHandleDown = (fPctGearHandle > 0.5);}
    double GetPctGearExtended() const       {return m_fGearExtended;}

    double GetAlpha()           const {return m_dAlpha;}
    double GetBeta()            const {return m_dBeta;}
    double GetTotalVelocity()   const {return m_dTotalVelocity;}
    BOOL   IsOnGround()         const {return m_bOnGround;}
    BOOL   IsOverPlatform()     const {return m_SurfaceInfo.m_bOnPlatform;}
    float  GetSurfaceElevation()const {return m_SurfaceInfo.m_fElevation;}

    float GetAirspeed() const {return (float)m_vBodyVelocity.dZ;}
    void InitAtAirspeed(float fAirspeed) {m_vBodyVelocity.dZ = fAirspeed;}
    void SetMFD_Message(LPCWSTR szMessage) {StringCchPrintf(m_szTest, ARRAYSIZE(m_szTest), szMessage);}
    void GetMFD_Message(LPWSTR szMessage, UINT uLength)const {StringCchPrintf(szMessage, uLength, m_szTest);}

    FuelSystem& GetFuelSystem() {return m_FuelSystem;}

private:
    IBaseObjectV400& m_BaseObject;

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

        if (IsEqualIID(riid , IID_IUnknown))
        {
            *ppv = static_cast<IUnknown*>(this);
        }        
        else if (IsEqualIID(riid , IID_ISimulationV01))
        {
            *ppv = static_cast<ISimulationV01*>(this);
        }        
        else if (IsEqualIID(riid , IID_ISimulation))
        {
            *ppv = static_cast<ISimulation*>(this);
        }        

        if (*ppv)
        {
            hr = S_OK;
            AddRef();
        }

    Error:

        return hr;
    }

    //Sample saving and loading string data
    STDMETHOD (SaveLoadState)(__in __notnull PSaveLoadCallback pfnCallback, __in const BOOL bSave) override
    {
        if (!pfnCallback)
        { 
            return E_FAIL;
        }

        //If that data is saved identical to how it is used locally, the same call can be used for both save and load
        return pfnCallback(TEXT("TestString"), 0, TEXT("State"), m_szTest, SAVED_DATA_TYPE_STRING);
    }  

    STDMETHOD (Update)(double dDeltaT)   override 
    {
        if (IsNormalMode())
        {        
            static const float MAX_VELOCITY          = 5000.0f; // ft/s
            static const float MAX_ROTATION_VELOCITY = 14.0f;   // Rad/s

            GetGlobalData();

            //Update the low-level "per simulation frame" whenever the aircraft updates
            CComPtr<ISimObjectAIV01> spAI;
            if (SUCCEEDED(m_BaseObject.QueryService(SID_SimObjectAI, IID_ISimObjectAIV01, (void**)&spAI)))
            {
                spAI->UpdateSimulationFrame(dDeltaT);
            }            

            //Update surface and weather info
            m_BaseObject.GetSurfaceInformation(m_SurfaceInfo, NULL);
            m_BaseObject.GetWeatherInformation(m_WeatherInfo);

            m_FuelSystem.Update(dDeltaT, m_fThrottle);

            //Get current position
            m_BaseObject.GetPosition(m_vdPos, m_vdOrient, m_vdVel, m_vdRotVel);
            //Update Subsystems
            UpdateLandingGear(dDeltaT);
            UpdateEngine(dDeltaT);
            UpdateAerodynamics(dDeltaT);
            UpdateGroundReaction(dDeltaT);

            //Total body force
            DXYZ vdBodyForce;
            vdBodyForce.dX = m_vAeroForce.dX + m_vGroundReactionForce.dX;
            vdBodyForce.dY = m_vAeroForce.dY + m_vGroundReactionForce.dY;
            vdBodyForce.dZ = m_vAeroForce.dZ + m_vGroundReactionForce.dZ + m_fThrust;

            //Total body acceleration
            DXYZ vdBodyAcceleration;
            vdBodyAcceleration.dX = (vdBodyForce.dX / m_fMass) - GetGravity() * cos(m_vdOrient.dX) * sin(m_vdOrient.dZ);
            vdBodyAcceleration.dY = (vdBodyForce.dY / m_fMass) - GetGravity() * cos(m_vdOrient.dX) * cos(m_vdOrient.dZ);
            vdBodyAcceleration.dZ = (vdBodyForce.dZ / m_fMass) + GetGravity() * sin(m_vdOrient.dX);

            //Integrate body velocity
            m_vBodyVelocity.dX += vdBodyAcceleration.dX  * dDeltaT;
            m_vBodyVelocity.dY += vdBodyAcceleration.dY  * dDeltaT;
            m_vBodyVelocity.dZ += vdBodyAcceleration.dZ  * dDeltaT;

            LIMIT_VECTOR(m_vBodyVelocity, MAX_VELOCITY)

            //Transform to World velocity
            m_Matrix.BodyToWorld(m_vBodyVelocity, m_vdVel);

            //Integrate world postion
            double latRadians = cos(m_vdPos.dZ);
            if(latRadians != 0.0)
            {
                m_vdPos.dX += (m_vdVel.dX * dDeltaT) / (latRadians * GetEquatorialRadius());
            }            
            m_vdPos.dY += m_vdVel.dY * dDeltaT;
            m_vdPos.dZ += (m_vdVel.dZ * dDeltaT) / GetPolarRadius();

            //Total Body Moment
            DXYZ vdBodyMoment;
            vdBodyMoment.dX = m_vAeroMoment.dX + m_vGroundReactionMoment.dX;
            vdBodyMoment.dY = m_vAeroMoment.dY + m_vGroundReactionMoment.dY;
            vdBodyMoment.dZ = m_vAeroMoment.dZ + m_vGroundReactionMoment.dZ;

            //Total Body Rotation Acceleration
            DXYZ vdBodyRotaionAcceleration;
            vdBodyRotaionAcceleration.dX = vdBodyMoment.dX / m_vfMOI.fX;
            vdBodyRotaionAcceleration.dY = vdBodyMoment.dY / m_vfMOI.fY;
            vdBodyRotaionAcceleration.dZ = vdBodyMoment.dZ / m_vfMOI.fZ;

            //Body Rotation Velocity
            m_vBodyRotationVelocity.dX += vdBodyRotaionAcceleration.dX  * dDeltaT;
            m_vBodyRotationVelocity.dY += vdBodyRotaionAcceleration.dY  * dDeltaT;
            m_vBodyRotationVelocity.dZ += vdBodyRotaionAcceleration.dZ  * dDeltaT;

            LIMIT_VECTOR(m_vBodyRotationVelocity, MAX_ROTATION_VELOCITY)

            //World Rotation Velocity
            m_vdRotVel.dX = m_vBodyRotationVelocity.dX * cos(m_vdOrient.dZ) - m_vBodyRotationVelocity.dY * sin(m_vdOrient.dZ);
            m_vdRotVel.dY = m_vBodyRotationVelocity.dY * cos(m_vdOrient.dZ) / cos(m_vdOrient.dX) + m_vBodyRotationVelocity.dX * sin(m_vdOrient.dZ) / cos(m_vdOrient.dX);
            m_vdRotVel.dZ = m_vBodyRotationVelocity.dZ + m_vBodyRotationVelocity.dX * sin(m_vdOrient.dZ) * tan(m_vdOrient.dX) + m_vBodyRotationVelocity.dY * cos(m_vdOrient.dZ) * tan(m_vdOrient.dX);

            m_vdOrient.dX += m_vdRotVel.dX * dDeltaT;
            m_vdOrient.dY += m_vdRotVel.dY * dDeltaT;
            m_vdOrient.dZ += m_vdRotVel.dZ * dDeltaT;

            m_Matrix.Update(m_vdOrient);

            //Update Prepar3D object
            m_BaseObject.SetPosition(m_vdPos, m_vdOrient, m_vdVel, m_vdRotVel, m_bOnGround, dDeltaT);
        }

        //WeaponSystem should be simulated after the sim's position is updated so that the "attached"
        //  weapons get the most current position.
        //  Continue to sim weapons in SLEW mode, as this allows attached weapons to remain attached as airplane moves
        //These will fail without Professional Plus License
        HRESULT hrWeapons        = m_BaseObject.UpdateServiceInstance(SID_WeaponsSystem, dDeltaT); 
        HRESULT hrGuns           = m_BaseObject.UpdateServiceInstance(SID_GunSystem, dDeltaT);
        HRESULT hrCountermeasure = m_BaseObject.UpdateServiceInstance(SID_CountermeasureSystem, dDeltaT); 
        
        /*********************************************************************************************************************
        The following code is simply to demonstrate querying for attached weapons and attaining references on them
        *********************************************************************************************************************/
        CComPtr<IWeaponsSystemV400> spWeaponSystem;
        if (SUCCEEDED(m_BaseObject.QueryService(SID_WeaponsSystem, IID_IWeaponsSystemV400, (void**)&spWeaponSystem)))
        {
            UINT nNumAttachedWeapons = 0;            

            for (UINT iStation = 0; iStation < spWeaponSystem->GetNumberOfStations(); iStation++)
            {
                for (UINT iPylon = 0; iPylon < spWeaponSystem->GetNumberOfPylonPoints(iStation); iPylon++)
                {
                    CComPtr<IWeaponServiceV400> spWeapon;

                    UINT uWeaponID = 0;

                    //For a given station and pylon, get the attached weapon
                    if (SUCCEEDED(spWeaponSystem->GetWeapon(iStation, iPylon, uWeaponID, &spWeapon)))
                    {
                        nNumAttachedWeapons++;

                        /*********************************************************************************
                        Below are two methods of getting to the SDK Sample Missile's IMissile interface.
                        **********************************************************************************/

                        //Method 1: Using the pointer ref attained above and querying for the interface.
                        //          This assumes the Missile implementation derives from IMissile, which it does
                        {
                            CComPtr<IMissile> spMissile;
                            if (SUCCEEDED(spWeapon->QueryInterface(IID_IMissile, (void**)&spMissile)))
                            {
                                double dMissileSpeed = spMissile->GetSpeed();
                            }
                        }

                        //Method 2:  Using the ID attained above, ask the SDK's SimObjectManager for the object.
                        //           This example demonstrates querying for the service, which is handled 
                        //           appropriately in the Missile's QueryService implemenation
                        {
                            //Get the PDK
                            CComPtr<IPdk>              spP3dSdk;       
                            if (SUCCEEDED(GetPdk(IID_IPdk, (void**)&spP3dSdk)))
                            {
                                //Get the SimObjectManager from the PDK
                                CComPtr<ISimObjectManagerV400> spSimObjectMgr;  //Smart pointer reference to SimObject Manager
                                if (SUCCEEDED(spP3dSdk->QueryService(SID_SimObjectManager, IID_ISimObjectManagerV400, (void**)&spSimObjectMgr)))
                                {
                                    //Get the object ref from the SimObjectManager using the ID
                                    CComPtr<IBaseObjectV400> spMissileBaseFromID;
                                    if (SUCCEEDED(spSimObjectMgr->GetObject(uWeaponID, &spMissileBaseFromID)))
                                    {
                                        //From the ref, query for the specific service and interface
                                        CComPtr<IMissile> spMissileFromId;
                                        if (SUCCEEDED(spMissileBaseFromID->QueryService(SID_Missile, IID_IMissile, (void**)&spMissileFromId)))
                                        {
                                            double dMissileSpeed = spMissileFromId->GetSpeed();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return S_OK;
    }

    void UpdateEngine(double dDeltaT)
    {
        STATIC_CONST float MAX_THRUST = 20000.0f;
        STATIC_CONST float THRUST_TC  = 1.3f;

        float fEngineHealth = m_BaseObject.GetSystemHealth(m_hEngHealth);

        if (fEngineHealth > 0)
        {
            m_fThrust += (float)(((MAX_THRUST * m_fThrottle) - m_fThrust) * THRUST_TC * dDeltaT);
        }
        else
        {
            m_fThrust = 0;
        }
               
        m_bAfterburnerActive = (m_fThrottle > 0.9f) ? TRUE : FALSE;
    }

    void UpdateLandingGear(double dDeltaT)
    {
        STATIC_CONST float GEAR_TC = 1.0f;
        STATIC_CONST float MAX_GEAR_RATE = 1.0f/3.0f;

        double dRate = (m_bGearHandleDown - m_fGearExtended) * GEAR_TC;
        dRate = max(min(dRate, MAX_GEAR_RATE), -MAX_GEAR_RATE);
        m_fGearExtended += (float)(dRate * dDeltaT);
    }

    void UpdateAerodynamics(double dDeltaT)
    {
        #define SQRD(x) (x) * (x)

        STATIC_CONST float WING_AREA = 300.0f;
        STATIC_CONST float WING_SPAN = 35.0f;
        STATIC_CONST float WING_MEAN_CHORD = 9.0f;

        /* AERODYNAMIC COEFFICIENTS */

        //Lift
        STATIC_CONST float CL0      = 0.01f;
        STATIC_CONST float CLALPHA  = 5.9f;

        //Drag
        STATIC_CONST float CD0      =  0.035f;

        //Side
        STATIC_CONST float CXBETA   = -0.03f;

        //Pitch
        STATIC_CONST float CMDE     = -0.08f;
        STATIC_CONST float CMQ      = -0.10f;
        STATIC_CONST float CMALPHA  =  0.0f;

        //Roll
        STATIC_CONST float CLDA     = -0.50f;
        STATIC_CONST float CLP      = -0.50f;

        //Yaw
        STATIC_CONST float CNDR     =  0.02f;
        STATIC_CONST float CNR      = -0.02f;
        STATIC_CONST float CNBETA   =  0.05f;

        double dTotalVelocitySqrd = SQRD(m_vBodyVelocity.dX)  + SQRD(m_vBodyVelocity.dY) + SQRD(m_vBodyVelocity.dZ);
        m_dTotalVelocity   = sqrt(dTotalVelocitySqrd);

        m_dAlpha = atan2(-m_vBodyVelocity.dY, m_vBodyVelocity.dZ);
        m_dBeta  = atan2( m_vBodyVelocity.dX, m_vBodyVelocity.dZ);

        //Rho = P/RT
        double dAmbientDensity = m_WeatherInfo.m_fAmbientPressure / (GetSpecificGasConstant() * m_WeatherInfo.m_fTemperature);
        double dDynamicPressure = 0.5 * dAmbientDensity * dTotalVelocitySqrd;
        double dDynamicForce = dDynamicPressure * WING_AREA;
        double dDynamicMomentPitch    = dDynamicForce * WING_MEAN_CHORD / 2.0;
        double dDynamicMomentLateral  = dDynamicForce * WING_SPAN / 2.0;

        DXYZ dCf;
        DXYZ dCmf;

        //Side Force
        dCf.dX = CXBETA * m_dBeta;

        //Lift
        double dLiftCf = CL0 + CLALPHA * m_dAlpha;
        //Drag
        double dDragCf = CD0;

        // Y - Z Force
        dCf.dZ = -dDragCf * cos(m_dAlpha) + dLiftCf * sin(m_dAlpha);
        dCf.dY =  dLiftCf * cos(m_dAlpha) + dDragCf * sin(m_dAlpha);

        //Pitch Moment
        dCmf.dX  = CMDE * m_fElevator;
        dCmf.dX += CMQ  * m_vBodyRotationVelocity.dX;

        dCmf.dY  = CNDR * m_fRudder;
        dCmf.dY += CNR  * m_vBodyRotationVelocity.dY;
        dCmf.dY += CNBETA  * m_dBeta;

        dCmf.dZ  = CLDA * m_fAilerons;
        dCmf.dZ += CLP  * m_vBodyRotationVelocity.dZ;

        //Cf -> Force
        m_vAeroForce.dX = dCf.dX * dDynamicForce;
        m_vAeroForce.dY = dCf.dY * dDynamicForce;
        m_vAeroForce.dZ = dCf.dZ * dDynamicForce;

        m_vAeroMoment.dX = dCmf.dX * dDynamicMomentPitch;
        m_vAeroMoment.dY = dCmf.dY * dDynamicMomentLateral;
        m_vAeroMoment.dZ = dCmf.dZ * dDynamicMomentLateral;
    }

    void UpdateGroundReaction(double dDeltaT)
    {
        STATIC_CONST float STATIC_HEIGHT_ABOVE_GROUND = 6.5f;

        memset(&m_vGroundReactionForce,  0, sizeof(m_vGroundReactionForce));
        memset(&m_vGroundReactionMoment, 0, sizeof(m_vGroundReactionMoment));

        double dHeightAboveGround = m_vdPos.dY - m_SurfaceInfo.m_fElevation - STATIC_HEIGHT_ABOVE_GROUND;
        
        if (dHeightAboveGround < 0)   // On ground
        {
            STATIC_CONST float SPRING_K = -19000.0f;
            STATIC_CONST float DAMPER_K = -4700.0f;

            STATIC_CONST float FRICTION_X   = -500.0f;
            STATIC_CONST float FRICTION_Z   = -100.0f;
            STATIC_CONST float FRICTION_YAW = -100.0f;

            STATIC_CONST float MAX_ROLLING_FRICTION_VEL = 60.0f;  //Limits friction

            STATIC_CONST float PITCH_K = -1500.0f;

            m_bOnGround = TRUE;

            //Spring
            m_vGroundReactionForce.dY = dHeightAboveGround * SPRING_K + m_vdVel.dY * DAMPER_K;
            
            //Rolling friction Velocity - limit to max speed
            double dRollingFrictionVel = min(m_vBodyVelocity.dZ, MAX_ROLLING_FRICTION_VEL);

            //Apply friction.  For simplicity this actually acts more like a drag resistance
            m_vGroundReactionForce.dX = m_vBodyVelocity.dX * FRICTION_X;
            m_vGroundReactionForce.dZ = m_vBodyVelocity.dZ * FRICTION_Z;

            //Rotating resistance
            m_vGroundReactionMoment.dY = m_vBodyRotationVelocity.dY * FRICTION_YAW;

            //Coax the airplane to level pitch and bank
            m_vGroundReactionMoment.dX = m_vdOrient.dX * PITCH_K; 
            m_vdOrient.dZ = 0;
            m_vGroundReactionMoment.dY = m_vBodyRotationVelocity.dY * FRICTION_YAW;

            //Safety net - If nose dive beneath ground, level plane so the ground reaction will push back up
            if (dHeightAboveGround < -10)
            {
                m_vdOrient.dX = 0;
                m_vdOrient.dZ = 0;
            }
        }
        else //In air
        {
             m_bOnGround = FALSE;
        }
    }

    void GetGlobalData()
    {
        CComPtr<IPdkV01> spPdk;
        CComPtr<IGlobalDataV400> spGlobalData;
        if (SUCCEEDED(GetPdk(IID_IPdkV01, (void**)&spPdk)))
        {
            if (SUCCEEDED(spPdk->QueryService(SID_GlobalData, IID_IGlobalDataV400, (void**)&spGlobalData)))
            {
                double dTimeSec, dTimeMinutes, dTimeHours, dTimeDays;
                int eSeconds, eMinutes, eHours, eDays;

                if (SUCCEEDED(spGlobalData->GetUnitCode(TEXT("seconds"), eSeconds)))      {spGlobalData->GetAbsoluteTime(dTimeSec,     eSeconds);}
                if (SUCCEEDED(spGlobalData->GetUnitCode(TEXT("minutes"), eMinutes)))      {spGlobalData->GetAbsoluteTime(dTimeMinutes, eMinutes);} 
                if (SUCCEEDED(spGlobalData->GetUnitCode(TEXT("hours"),   eHours)))        {spGlobalData->GetAbsoluteTime(dTimeHours,   eHours);}
                if (SUCCEEDED(spGlobalData->GetUnitCode(TEXT("days"),    eDays)))         {spGlobalData->GetAbsoluteTime(dTimeDays,    eDays); }

                UINT uLocalYear, uLocalMonth, uLocalDayOfMonth, uLocalDayofYear;
                double dLocalHours;
                spGlobalData->GetTime(dLocalHours, eHours, TIMEREF_LOCAL);
                spGlobalData->GetYear(uLocalYear, TIMEREF_LOCAL);
                spGlobalData->GetMonthOfTheYear(uLocalMonth, TIMEREF_LOCAL);
                spGlobalData->GetDayOfTheMonth(uLocalDayOfMonth, TIMEREF_LOCAL); 
                spGlobalData->GetDayOfTheYear(uLocalDayofYear, TIMEREF_LOCAL);
                
                UINT uZuluYear, uZuluMonth, uZuluDayOfMonth, uZuluDayofYear;
                double dZuluHours;
                spGlobalData->GetTime(dZuluHours, eHours, TIMEREF_ZULU);
                spGlobalData->GetYear(uZuluYear, TIMEREF_ZULU);
                spGlobalData->GetMonthOfTheYear(uZuluMonth, TIMEREF_ZULU);
                spGlobalData->GetDayOfTheMonth(uZuluDayOfMonth, TIMEREF_ZULU); 
                spGlobalData->GetDayOfTheYear(uZuluDayofYear, TIMEREF_ZULU);

                double dTimeZoneOfs;
                spGlobalData->GetTimeZoneOffset(dTimeZoneOfs, eHours);

                TIMEOFDAY eTOD = spGlobalData->GetGeneralTimeOfDay();

                BOOL bPaused = spGlobalData->IsPaused();

                float fSimSpeed = spGlobalData->GetSimRate();
            }
        }
    }

private:

    BOOL IsNormalMode() const 
    {
        return (0 == (m_BaseObject.GetMode() & ( SIMOBJECT_MODE_PAUSE | SIMOBJECT_MODE_DISABLED | SIMOBJECT_MODE_SLEW | SIMOBJECT_MODE_CRASH | SIMOBJECT_MODE_FREEZE_POSITION
                                                    | SIMOBJECT_MODE_FREEZE_ATTITUDE | SIMOBJECT_MODE_FREEZE_ALTITUDE | SIMOBJECT_MODE_FREEZE_LATLON)));
    }

    SurfaceInfoV400 m_SurfaceInfo;
    WeatherInfoV400 m_WeatherInfo;

    //World-relative state
    DXYZ m_vdPos;
    DXYZ m_vdOrient;
    DXYZ m_vdVel;
    DXYZ m_vdRotVel;

    DXYZ m_vBodyRotationVelocity;
    DXYZ m_vBodyVelocity;

    BOOL m_bOnGround;

    Matrix m_Matrix;

    //Mass Properties
    float m_fMass;
    FXYZ  m_vfMOI;

    //Flight Controls
    float m_fElevator;
    float m_fAilerons;
    float m_fRudder;

    //Engine
    float  m_fThrottle;
    float  m_fThrust;
    BOOL   m_bAfterburnerActive;
    HANDLE m_hEngHealth;

    //Landing Gear
    BOOL   m_bGearHandleDown;
    float  m_fGearExtended;

    //Aerodynamics
    DXYZ   m_vAeroForce;
    DXYZ   m_vAeroMoment;

    //Aerodynamics
    double m_dAlpha;
    double m_dBeta;
    double m_dTotalVelocity;

    DXYZ   m_vGroundReactionForce;
    DXYZ   m_vGroundReactionMoment;

    FuelSystem m_FuelSystem;

    TCHAR  m_szTest[128];

};

/**********************************************************************
**********************************************************************/
class SimpleAirplaneFixedData
{
public:

    /**********************************************************************
    Load - loads on-disk data
    **********************************************************************/
    HRESULT Load(__in __notnull IBaseObjectV400& BaseObject) const
    {
        HRESULT hr = S_OK;

        //These will fail without Professional Plus License
        HRESULT hrWeapons = BaseObject.LoadServiceConstantData(SID_WeaponsSystem);
        HRESULT hrGuns    = BaseObject.LoadServiceConstantData(SID_GunSystem);
        HRESULT hrCMs     = BaseObject.LoadServiceConstantData(SID_CountermeasureSystem);
        
        {//Sample xml data loading.  This data is only for the purpose of demonstrating how to 
         // use the SDK's DataLoadHelper service, and has absolutely no effect on this simulation.
            TCHAR szDir[260] = TEXT("");
            if (SUCCEEDED(BaseObject.GetCfgDir(szDir, ARRAYSIZE(szDir))))
            {
                TCHAR szDataFile[260] = TEXT("");

                if (SUCCEEDED(StringCchPrintf(szDataFile, ARRAYSIZE(szDataFile), TEXT("%s\\SampleData"), szDir)))
                {
                    CComPtr<IPdk> spP3dSdk;       
                    if (SUCCEEDED(GetPdk(IID_IPdk, (void**)&spP3dSdk)))
                    {
                        CComPtr<IDataLoadHelperV400> spDataLoader;       
                        if (SUCCEEDED(spP3dSdk->QueryService(SID_DataLoadHelper, IID_IDataLoadHelperV400, (void**)&spDataLoader)))
                        {
                            TCHAR szName[32] = TEXT("");

                            hr = spDataLoader->LoadXmlStringData(szDataFile, TEXT("Engine"), TEXT("EngineName"), szName, ARRAYSIZE(szName));
                            hr = spDataLoader->LoadXmlFloatData ((void*)this, szDataFile, TEXT("Engine"), TEXT("EngineData"),  EngineDataCallback);
                        }
                    }
                }
            }
        }

        return hr;
    }

    /***********************************************************************
    Sample data loading callback
    ***********************************************************************/
    static HRESULT STDMETHODCALLTYPE EngineDataCallback(__in const void* pID,           //id passed in by the caller. Can be used to identify specific object
                                                        __in const WCHAR* pszNameSet,    //Data set name passed in
                                                        __in const WCHAR* pszSubName,    //Subset name passed in
                                                        __in const float* rgFloat,      //array pointer to list of floats
                                                        __in UINT nFloats)              //number of floats loaded in array
    {
        HRESULT hr = E_FAIL;

        if (pID)
        {
            SimpleAirplaneFixedData* pThis = (SimpleAirplaneFixedData*)pID;

            for(UINT i = 0; i < nFloats; i++)
            {
                pThis->m_vEngineData.push_back(*rgFloat);  //Representative data

                rgFloat++;
            }
        }

        hr = S_OK;

        return hr;
    }

    /**********************************************************************
    Unload
    **********************************************************************/
    HRESULT Unload(__in __notnull IBaseObjectV400& BaseObject) const
    {
        HRESULT hr = S_OK;

        //These will fail without Professional Plus License
        HRESULT hrWeapons = BaseObject.UnloadServiceConstantData(SID_WeaponsSystem);
        HRESULT hrGuns    = BaseObject.UnloadServiceConstantData(SID_GunSystem);
        HRESULT hrCMs     = BaseObject.UnloadServiceConstantData(SID_CountermeasureSystem);
    
        return hr;
    }

private:
    //This data is used only for the data loading example code.  It has no effect on this simulation
    std::vector<float> m_vEngineData;  

};
/************************************************************************************************
End Simulation
************************************************************************************************/
class AirplaneAIService : public IAirplaneAIServiceV01
{
    //IUnknown Implementation
    volatile LONG   m_RefCount;
    STDMETHOD_(ULONG, AddRef)()   override {return InterlockedIncrement(&m_RefCount) ;}
    STDMETHOD_(ULONG, Release)()  override 
    {
        ULONG   RetVal = InterlockedDecrement(&m_RefCount);
        if      (RetVal == 0)         {delete this;}
        else if (RetVal & 0x80000000) {__debugbreak();}
        return RetVal;
    }

public:
    STDMETHOD (QueryInterface)(__in REFIID riid, void **ppv)  override 
    {
        HRESULT hr = E_NOINTERFACE;

        if (!ppv)
        {
            goto Error;
        }
    
        *ppv = NULL;

        if (IsEqualIID(riid , IID_IAirplaneAIServiceV01))
        {
            *ppv = static_cast<IAirplaneAIServiceV01*>(this);
        }        
        else if (IsEqualIID(riid , IID_IAircraftAIServiceV01))
        {
            *ppv = static_cast<IAircraftAIServiceV01*>(this);
        }        
        else if (IsEqualIID(riid , IID_IAIService))
        {
            *ppv = static_cast<IAIService*>(this);
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

    AirplaneAIService() : m_RefCount(1) {}
    ~AirplaneAIService() {}

    STDMETHOD_(BOOL,   IsEngineTypePiston)()                const override {return FALSE;}
    STDMETHOD_(UINT,   GetEngineCount)()                    const override {return 1;}
    STDMETHOD_(float,  GetGeneralEngineRPM)(int iEngine)    const override {return 0;}
    STDMETHOD_(float,  GetCruiseSpeedTrue)()                const override {return 0;}
    STDMETHOD_(float,  GetMaxIndicatedAirspeed)()           const override {return 0;}
    STDMETHOD_(float,  GetMaxMach)()                        const override {return 0;}
    STDMETHOD_(float,  GetEmptyRollingMomentOfInertia)()    const override {return 0;}
    STDMETHOD_(float,  GetDynamicPressure)()                const override {return 0;}
    STDMETHOD_(float,  GetTotalWeight)()                    const override {return 0;}
    STDMETHOD_(float,  GetLongitudinalAccelerationBody)()   const override {return 0;}
    STDMETHOD_(float,  GetLateralAccelerationBody)()        const override {return 0;}
    STDMETHOD_(float,  GetVerticalAccelerationBody)()       const override {return 0;}
    STDMETHOD_(float,  GetTotalFuelCapacity)()              const override {return 0;}
    STDMETHOD_(float,  GetFuelWeightPerGallon)()            const override {return 0;}
    STDMETHOD_(float,  GetEstimatedFuelFlowLbsPerHour)()    const override {return 0;}

    STDMETHOD_(float,  GetTotalLongitudinalThrust)()        const override {return 0;}
    STDMETHOD_(float,  GetLiftForce)()                      const override {return 0;}
    STDMETHOD_(double, CalculateDesiredBank)(double dHeadingError, double dDeltaT)  override {return 0;}
    STDMETHOD_(double, CalculateDeltaThrottle)(double dSpeedError, double dDeltaT)  override {return 0;}

    STDMETHOD_(float,  GetIncidenceAlpha)()                 const override {return 0;}
    STDMETHOD_(float,  GetIncidenceBeta)()                  const override {return 0;}
    STDMETHOD_(float,  GetBetaDot)()                        const override {return 0;}
    STDMETHOD_(float,  GetRollRate)()                       const override {return 0;}
    STDMETHOD_(float,  GetSigmaSqrt)()                      const override {return 0;}
    STDMETHOD_(float,  GetIndicatedAirspeed)()              const override {return 0;}
    STDMETHOD_(float,  GetTrueAirspeed)()                   const override {return 0;}
    STDMETHOD_(float,  GetMach)()                           const override {return 0;}
    STDMETHOD_(float,  GetIndicatedAltitude)()              const override {return 0;}

    STDMETHOD_(BOOL, IsEngineRunning)(int iEngine)          const override {return FALSE;}
    STDMETHOD_(void, SetBrakePosition)(int iBrake,float fPos)     override {}  // none 0->1 full  
    STDMETHOD_(BOOL, GetParkingBrake)()                     const override {return FALSE;}
    STDMETHOD_(void, SetParkingBrake)(BOOL bOn)                   override {}
    STDMETHOD_(void, ExtendLandingGear)()                         override {}
    STDMETHOD_(void, RetractLandingGear)()                        override {}
    STDMETHOD_(void, TapBrakes)()                                 override {} 
    STDMETHOD_(void, StartEngines)()                              override {}
    STDMETHOD_(void, ShutdownEngines)()                           override {}

    STDMETHOD_(void, SetNavLights)         (BOOL bOn)    override {}
    STDMETHOD_(void, SetBeaconLights)      (BOOL bOn)    override {}
    STDMETHOD_(void, SetLogoLights)        (BOOL bOn)    override {}
    STDMETHOD_(void, SetRecognitionLights) (BOOL bOn)    override {}
    STDMETHOD_(void, SetTaxiLights)        (BOOL bOn)    override {}
    STDMETHOD_(void, SetLandingLights)     (BOOL bOn)    override {}
    STDMETHOD_(void, SetStrobeLights)      (BOOL bOn)    override {}
    STDMETHOD_(void, SetPanelLights)       (BOOL bOn)    override {}
    STDMETHOD_(void, SetCabinLights)       (BOOL bOn)    override {}

    //Airplane Specific
    STDMETHOD_(float,  GetStallSpeedDirty)()         const override {return 0;}
    STDMETHOD_(float,  GetStallSpeedClean)()         const override {return 0;}
    STDMETHOD_(float,  GetMinDragSpeed)()            const override {return 0;}
    STDMETHOD_(double, GetThrottlePercent)()         const override {return 0;}
    STDMETHOD_(double, GetElevatorPercent)()        const override {return 0;}
    STDMETHOD_(double, GetAileronPercent)()         const override {return 0;}
    STDMETHOD_(double, GetRudderPercent)()          const override {return 0;}
    STDMETHOD_(double, GetSpoilersPercent)()        const override {return 0;}
    STDMETHOD_(double, GetFlapsPercent)()           const override {return 0;}
    STDMETHOD_(float,  GetWingArea)()                const override {return 0;}
    STDMETHOD_(float,  GetWingSpan)()                const override {return 0;}
    STDMETHOD_(float,  GetZeroLiftAngleOfAttack)()   const override {return 0;}
    STDMETHOD_(float,  GetCriticalAngleOfAttack)()   const override {return 0;}
    STDMETHOD_(float,  GetLinearCLAlpha)()           const override {return 0;}
    STDMETHOD_(BOOL,   IsTailDragger)()              const override {return 0;}
    STDMETHOD_(BOOL,   IsSeaPlane)()                 const override {return 0;}
    STDMETHOD_(float,  GetMinTakeoffLength)()        const override {return 0;}
    STDMETHOD_(float,  GetMinLandingLength)()        const override {return 0;}

    STDMETHOD_(void, SetThrottlePercent)(double dPct)     override {}  
    STDMETHOD_(void, SetElevatorPercent)(double dPct)     override {}  // pitch down <-0-> pitch up
    STDMETHOD_(void, SetAileronPercent) (double dPct)     override {}  // roll left  <-0-> roll right
    STDMETHOD_(void, SetRudderPercent)  (double dPct)     override {}  // yaw left   <-0-> yaw right
    STDMETHOD_(void, SetFlapsPercent)   (double dPct)     override {}  // none 0-> full
    STDMETHOD_(void, SetSpoilersPercent)(double dPct)     override {}  // retracted 0-> extended

    STDMETHOD_(void, SetTaxiHeading)(float fHeading)       override {}
    STDMETHOD_(void, SetTaxiSpeed)(float fSpeed)           override {}
    STDMETHOD_(void, StopTaxi)()                           override {}
    STDMETHOD_(void, SetPushBack)(BOOL bOn)                override {}
};

/************************************************************************************************
AI Implementation
************************************************************************************************/
class AICombatBehaviors : public IAIBehaviorAttackerV400
{
public:
    AICombatBehaviors() : m_RefCount(1)  {}
    ~AICombatBehaviors() {}

    // AddRef implementation
    volatile LONG   m_RefCount;
    STDMETHOD_(ULONG, AddRef)()   override {return InterlockedIncrement(&m_RefCount) ;}
    STDMETHOD_(ULONG, Release)()  override 
    {
        ULONG   RetVal = InterlockedDecrement(&m_RefCount);
        if      (RetVal == 0)         {delete this;}
        else if (RetVal & 0x80000000) {__debugbreak();}
        return RetVal;
    }

    //QueryInterface
    STDMETHOD (QueryInterface)(__in REFIID riid, void **ppv)  override 
    {
        HRESULT hr = E_NOINTERFACE;

        if (!ppv)
        {
            goto Error;
        }
    
        *ppv = NULL;

        if (IsEqualIID(riid , IID_IAIBehaviorAttackerV400))
        {
            *ppv = static_cast<IAIBehaviorAttackerV400*>(this);
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

    //Interface implementations
    STDMETHOD_(GUID,  GetBehaviorGuid)()                            const override {return GUID_NULL;}
    STDMETHOD_(void,  SetFireRadiusMin)(float radiusFeet)                 override {}           //Feet
    STDMETHOD_(float, GetFireRadiusMin)()                           const override {return 0;}  //Feet
    STDMETHOD_(void,  SetFireRadiusMax)(float radiusFeet)                 override {}           //Feet
    STDMETHOD_(float, GetFireRadiusMax)()                           const override {return 0;}  //Feet
    STDMETHOD_(void,  SetFireFOVDegrees)(float degrees)                   override {}           //Degrees
    STDMETHOD_(float, GetFireFOVDegrees)()                          const override {return 0;}  //Degrees

    STDMETHOD_(void,  SetWeaponTitle)(WCHAR* weaponTitle)                  override {}
    STDMETHOD_(const WCHAR*, GetWeaponTitle)()                       const override {return 0;}
    STDMETHOD_(void,  SetWeaponType)(WCHAR* weaponType)                    override {}
    STDMETHOD_(const WCHAR*, GetWeaponType)()                        const override {return 0;}

    STDMETHOD_(void,  SetGunTitle)(WCHAR* gunTitle)                        override {}
    STDMETHOD_(const WCHAR*, GetGunTitle)()                          const override {return 0;}
    STDMETHOD_(void,  SetGunType)(WCHAR* gunType)                          override {}
    STDMETHOD_(const WCHAR*, GetGunType)()                           const override {return 0;}

    STDMETHOD_(void,  SetAttackDelay)(float delaySeconds)                 override {}           //Seconds
    STDMETHOD_(float, GetAttackDelay)()                             const override {return 0;}  //Seconds

    STDMETHOD_(void,  SetGunBurstDuration)(float durationSeconds)         override {}           //Seconds
    STDMETHOD_(float, GetGunBurstDuration)()                        const override {return 0;}  //Seconds

    STDMETHOD_(BOOL,  IsWithinFireZone)()                           const override {return 0;}
};

/************************************************************************************************
AI Implementation
************************************************************************************************/
class AirplaneAI : public ISimObjectAIV01
{
    //IServiceProvider Implementation
    volatile LONG   m_RefCount;
    STDMETHOD_(ULONG, AddRef)()   override {return InterlockedIncrement(&m_RefCount) ;}
    STDMETHOD_(ULONG, Release)()  override 
    {
        ULONG   RetVal = InterlockedDecrement(&m_RefCount);
        if      (RetVal == 0)         {delete this;}
        else if (RetVal & 0x80000000) {__debugbreak();}
        return RetVal;
    }

public:
    STDMETHOD (QueryInterface)(__in REFIID riid, void **ppv)  override 
    {
        HRESULT hr = E_NOINTERFACE;

        if (!ppv)
        {
            goto Error;
        }
    
        *ppv = NULL;

        if (IsEqualIID(riid , IID_ISimObjectAI))
        {
            *ppv = static_cast<ISimObjectAI*>(this);
        }        
        if (IsEqualIID(riid , IID_ISimObjectAIV01))
        {
            *ppv = static_cast<ISimObjectAIV01*>(this);
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

    STDMETHOD (QueryService)(  __in REFGUID guidService, __in REFIID riid, void **ppv) override 
    {
        HRESULT hr = E_NOINTERFACE;

        if (!ppv)
        {
            goto Error;
        }
    
        *ppv = NULL;
    
        if (IsEqualIID(guidService, SID_SimObjectAI))
        {
            hr = QueryInterface(riid, ppv);
        }
        else if (IsEqualIID(guidService, SID_AIBehaviorAttacker))
        {
            hr = m_CombatBehaviors.QueryInterface(riid, ppv);
        }

    Error:

        return hr;

    }

    AirplaneAI()
        : m_RefCount(1)
    {
    }

    AICombatBehaviors m_CombatBehaviors;

    STDMETHOD (UpdateSimulationFrame)(__in double dDeltaT)          override {return S_OK;}
    STDMETHOD_(void, Deactivate)()                                  override {}  //Deactivate AI runtime
    STDMETHOD_(void, Activate)()                                    override {}  //Activate AI runtime
    STDMETHOD_(UNITMODE, GetPilotMode)()                    const   override {return UNITMODE_WAITING;}
    STDMETHOD_(void, SetPilotMode)(UNITMODE eMode, BOOL bOn = TRUE) override {}
    STDMETHOD (SetWaypoint)(__in BasicWaypoint)                     override {return E_FAIL;}

    STDMETHOD (SetDesiredHeading)(double dTrueHeading)              override {return E_FAIL;}   //Radians
    STDMETHOD (SetDesiredPitch)(double dPitch)                      override {return E_FAIL;}   //Radians
    STDMETHOD (SetDesiredSpeed)(double dSpeed)                      override {return E_FAIL;}   //Feet per second (Indicated airspeed for aircraft)
    STDMETHOD (SetDesiredAltitude)(double dAltitudeMSL)             override {return E_FAIL;}   //Feet

private:

};

/************************************************************************************************
End AI Implementation
************************************************************************************************/

/************************************************************************************************
ISimObject parent class
************************************************************************************************/
// Ensure this GUID matches the CategoryId in the sim.cfg
class __declspec(uuid("{85B2CDCD-8156-4C93-B069-61B2D0A96C12}")) SimpleAirplane : public ISimObjectV400, IAirplaneServiceV01, IAvatarAttachServiceV01
{

#define NUM_CHANNELS 4

public:

    //IServiceProvider Implementation
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

        if (IsEqualIID(riid, IID_ISimObject))
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
        else if (IsEqualIID(riid, IID_ISimObjectV03))
        {
            *ppv = static_cast<ISimObjectV03*>(this);
        }
        else if (IsEqualIID(riid, IID_ISimObjectV400))
        {
            *ppv = static_cast<ISimObjectV400*>(this);
        }
        else if (IsEqualIID(riid , IID_IAircraftService))
        {
            *ppv = static_cast<IAircraftService*>(this);
        }
        else if (IsEqualIID(riid , IID_IAircraftServiceV01))
        {
            *ppv = static_cast<IAircraftServiceV01*>(this);
        }
        else if (IsEqualIID(riid , IID_IAirplaneServiceV01))
        {
            *ppv = static_cast<IAirplaneServiceV01*>(this);
        }
        else if (IsEqualIID(riid , IID_IAvatarAttachServiceV01))
        {
            *ppv = static_cast<IAvatarAttachServiceV01*>(this);
        }        
        else if (IsEqualIID(riid , IID_IServiceProvider))
        {
            *ppv = static_cast<IServiceProvider*>(this);
        }
        else if (IsEqualIID(riid , IID_IUnknown))
        {
            *ppv = static_cast<SimpleAirplane*>(this);
        }        


        if (*ppv)
        {
            hr = S_OK;
            AddRef();
        }

    Error:

        return hr;
    }
    
    STDMETHOD (QueryBaseObject)( __in REFIID riid, void** ppv) override 
    {
        HRESULT hr = E_NOINTERFACE;

        if (!ppv)
        {
            goto Error;
        }    

        *ppv = NULL;
    
        hr = m_BaseObject.QueryInterface(riid, (void**)ppv);

    Error:

        return hr;
    }

    ///< Provide the main rate (Hz).  Typically the world position update rate.
    STDMETHOD(GetMainSimRate)(__out float& fSimRate)         const override
    {
        fSimRate = SIM_RATE;

        return S_OK;
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
        else if (IsEqualIID(guidService, SID_AirplaneService))
        {
            hr = QueryInterface(riid, ppv);
        }        
        else if (IsEqualIID(guidService, SID_AircraftService))
        {
            hr = QueryInterface(riid, ppv);
        }        
        else if (IsEqualIID(guidService , SID_AvatarAttachService))
        {
            hr = QueryInterface(riid, ppv);
        }        
        else if (IsEqualIID(guidService, SID_FuelService))
        {
            hr = m_Simulation.GetFuelSystem().QueryInterface(riid, ppv);
        }

#ifdef USE_P3D_AIRPLANE_AI
        else if (IsEqualIID(guidService, SID_AirplaneAIService) || IsEqualIID(guidService, SID_AircraftAIService) || IsEqualIID(guidService, SID_AIService))
        {
            hr = m_Airplane.QueryInterface(riid, ppv);
        }        
#else
        else if (IsEqualIID(guidService, SID_SimObjectAI))
        {
            hr = m_AI.QueryInterface(riid, ppv);
        }
#endif // USE_P3D_AIRPLANE_AI

    Error:

        return hr;

    }

    //ISimObject implementations

    /**********************************************************************
    LoadConstantData - called by base object to load on-disk data
    **********************************************************************/
    STDMETHOD (LoadConstantData)(__out void** ppConstantData)       override        //on-disk data.  return loaded data
    {
        HRESULT hr = E_FAIL;

        if (!ppConstantData)
        {
            return E_POINTER;
        }

        //Do we already have a ref to loaded data due to a previous instance?
        if (*ppConstantData)
        {
            m_pFixedData = static_cast<SimpleAirplaneFixedData*>(*ppConstantData);

            hr = S_OK;
        }
        else //Load the data, return a ref to it to be cached for subsequent instances
        {
            m_pFixedData = new SimpleAirplaneFixedData();

            hr = m_pFixedData->Load(m_BaseObject);

            if (FAILED(hr))
            {
                delete m_pFixedData;
                m_pFixedData = NULL;
            }

            *ppConstantData = const_cast<void*>(static_cast<const void*>(m_pFixedData));
        }

        return hr;
    }

    /**********************************************************************
    UnloadConstantData - called by base object to unload on-disk data
    **********************************************************************/
    STDMETHOD (UnloadConstantData)(__inout void** ppConstantData)   override       //on-disk data.  should point to loaded data, and return NULL
    {
        HRESULT hr = E_FAIL;

        if (*ppConstantData == m_pFixedData)
        {
            hr = m_pFixedData->Unload(m_BaseObject);

            delete m_pFixedData;
            m_pFixedData = NULL;

            *ppConstantData = reinterpret_cast<void*>(const_cast<SimpleAirplaneFixedData*>(m_pFixedData));
        }

        return hr;
    }
    
    STDMETHOD (LoadDynamicData)()                                   override //e.g. create runtime subsystems based on disk data
    {       
        //These will fail without Professional Plus License
        HRESULT hrWeapons = m_BaseObject.CreateServiceInstance(SID_WeaponsSystem);
        HRESULT hrGuns    = m_BaseObject.CreateServiceInstance(SID_GunSystem);
        HRESULT hrCMs     = m_BaseObject.CreateServiceInstance(SID_CountermeasureSystem);
        
        return S_OK;
    }

    STDMETHOD (Init)()                                              override {return m_Simulation.Init();}       //e.g. init relationships between subsystems
    STDMETHOD (DeInit)()                                            override 
    {
        //These will fail without Professional Plus License
        HRESULT hrWeapons = m_BaseObject.DestroyServiceInstance(SID_WeaponsSystem);
        HRESULT hrGuns    = m_BaseObject.DestroyServiceInstance(SID_GunSystem);
        HRESULT hrCMs     = m_BaseObject.DestroyServiceInstance(SID_CountermeasureSystem);

        return S_OK;
    }

    STDMETHOD_(BOOL, SupportsLabels)()                      const   override {return m_bSupportsLabel;}                      //Does this simobject support label display?
    STDMETHOD (SetSupportsLabels)(BOOL bOn)                         override {m_bSupportsLabel = (bOn != FALSE); return S_OK;}     //Should this simobject support label display?
    STDMETHOD_(void, OnModeChange)(int bfMode)                     override {}                   //Called when a mode has changed, with the new mode bitfield
    STDMETHOD_(void, OnPositionInit)()                             override {m_Simulation.InitPositionFromBase();}       //Called upon change in non-realtime object positions   (e.g. UI, slew)
    STDMETHOD_(void, OnSpeedInit)(float fSpeed)                    override {m_Simulation.InitAtAirspeed(fSpeed);}       //(Feet per Second) Called upon change in speed   (e.g. UI)
    STDMETHOD_(float, GetIndicatedAirspeed)()                const override {return m_Simulation.GetAirspeed();};  //Feet per second

    STDMETHOD_(BOOL, CanAvatarAttach()) const override {return m_Simulation.IsOnGround() && m_Simulation.GetTotalVelocity() < 1.0;}
    STDMETHOD_(BOOL, CanAvatarDetach()) const override {return m_Simulation.IsOnGround() && m_Simulation.GetTotalVelocity() < 1.0;}

    //Factory function
    static HRESULT New(__in __notnull IBaseObjectV400* pBaseObject, __out __notnull ISimObject** ppThisSim) 
    {
        HRESULT hr = E_FAIL;

        if (!ppThisSim)
        {
            goto Error;
        }

        *ppThisSim = new SimpleAirplane(*pBaseObject);

        if (!*ppThisSim)
        {
            goto Error;
        }

        hr = pBaseObject->RegisterSimulation(&static_cast<SimpleAirplane*>(*ppThisSim)->m_Simulation, SIM_RATE /*Hz*/);


    Error:
        return hr;
    }

    //Throttle triggers
    static STDMETHODIMP SetThrottle(__in ISimObject& Sim, __in double dProperty, __in int iIndex) 
    {
        //dProperty = (1.0 - dProperty) / 2.0;
        dProperty = 0.5 - 0.51 * dProperty;
        dProperty = max(dProperty, 0);
        dProperty = min(dProperty, 1.0);

        static_cast<SimpleAirplane&>(Sim).SetThrottle((float)(dProperty)); 
        return S_OK;
    }

    static STDMETHODIMP IncThrottle(__in ISimObject& Sim, __in double dProperty, __in int iIndex) 
    {
        static_cast<SimpleAirplane&>(Sim).IncThrottle(); 
        return S_OK;
    }

    static STDMETHODIMP DecThrottle(__in ISimObject& Sim, __in double dProperty, __in int iIndex) 
    {
        static_cast<SimpleAirplane&>(Sim).DecThrottle(); 
        return S_OK;
    }

    //Throttle property queries
    static STDMETHODIMP GetThrottle(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const SimpleAirplane&>(Sim).GetThrottle(); 
        return S_OK;
    }

    static STDMETHODIMP IsAfterburnerActive(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const SimpleAirplane&>(Sim).IsAfterburnerActive(); 
        return S_OK;
    }

    //Landing Gear trigger
    static STDMETHODIMP ToggleGear(__in ISimObject& Sim, __in double dProperty, __in int iIndex) 
    {
        static_cast<SimpleAirplane&>(Sim).ToggleGear(); 
        return S_OK;
    }

    //Dec Landing Gear lever
    static STDMETHODIMP IncGearLever(__in ISimObject& Sim, __in double dProperty, __in int iIndex) 
    {
        static_cast<SimpleAirplane&>(Sim).IncGearLever(); 
        return S_OK;
    }

    //Inc Landing Gear lever
    static STDMETHODIMP DecGearLever(__in ISimObject& Sim, __in double dProperty, __in int iIndex) 
    {
        static_cast<SimpleAirplane&>(Sim).DecGearLever(); 
        return S_OK;
    }

    //Set Landing Gear lever Up
    static STDMETHODIMP SetGearUp(__in ISimObject& Sim, __in double dProperty, __in int iIndex) 
    {
        static_cast<SimpleAirplane&>(Sim).SetGearUp(); 
        return S_OK;
    }

        //Set Landing Gear lever down
    static STDMETHODIMP SetGearDown(__in ISimObject& Sim, __in double dProperty, __in int iIndex) 
    {
        static_cast<SimpleAirplane&>(Sim).SetGearDown(); 
        return S_OK;
    }

    //Landing Gear trigger
    static STDMETHODIMP GearLeverSet(__in ISimObject& Sim, __in double dProperty, __in int iIndex) 
    {
        static_cast<SimpleAirplane&>(Sim).GearLeverSet((float)dProperty); 
        return S_OK;
    }

    //Landing gear extended property
    static STDMETHODIMP GetGearLever(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const SimpleAirplane&>(Sim).GetPctGearExtended(); 
        return S_OK;
    }

    //Landing gear extended property
    static STDMETHODIMP GetPctGearExtended(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const SimpleAirplane&>(Sim).GetPctGearExtended(); 
        return S_OK;
    }

    //Flight Controls
    static STDMETHODIMP SetElevator(__in ISimObject& Sim, __in double dProperty, __in int iIndex) 
    {
        static_cast<SimpleAirplane&>(Sim).SetElevator((float)(dProperty)); 
        return S_OK;
    }

    static STDMETHODIMP SetAilerons(__in ISimObject& Sim, __in double dProperty, __in int iIndex) 
    {
        static_cast<SimpleAirplane&>(Sim).SetAilerons((float)(dProperty)); 
        return S_OK;
    }

    static STDMETHODIMP SetRudder(__in ISimObject& Sim, __in double dProperty, __in int iIndex) 
    {
        static_cast<SimpleAirplane&>(Sim).SetRudder((float)(dProperty)); 
        return S_OK;
    }

    static STDMETHODIMP GetPctElevator(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const SimpleAirplane&>(Sim).GetPctElevator(); 
        return S_OK;
    }

    static STDMETHODIMP GetPctAilerons(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const SimpleAirplane&>(Sim).GetPctAilerons(); 
        return S_OK;
    }

    static STDMETHODIMP GetPctRudder(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const SimpleAirplane&>(Sim).GetPctRudder();
        return S_OK;
    }

    static STDMETHODIMP GetAlpha(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const SimpleAirplane&>(Sim).GetAlpha(); 
        return S_OK;
    }

    static STDMETHODIMP GetBeta(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const SimpleAirplane&>(Sim).GetBeta(); 
        return S_OK;
    }

    static STDMETHODIMP GetTotalVelocity(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const SimpleAirplane&>(Sim).GetTotalVelocity(); 
        return S_OK;
    }

    static STDMETHODIMP IsOnGround(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const SimpleAirplane&>(Sim).IsOnGround(); 
        return S_OK;
    }

    static STDMETHODIMP IsOverPlatform(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const SimpleAirplane&>(Sim).IsOverPlatform(); 
        return S_OK;
    }

    static STDMETHODIMP GetSurfaceElevation(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const SimpleAirplane&>(Sim).GetSurfaceElevation(); 
        return S_OK;
    }


    static STDMETHODIMP IsPaused(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const SimpleAirplane&>(Sim).IsPaused();
        return S_OK;
    }

    static STDMETHODIMP IsDisabled(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const SimpleAirplane&>(Sim).IsDisabled();
        return S_OK;
    }

    static STDMETHODIMP IsSlewMode(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const SimpleAirplane&>(Sim).IsSlewMode();
        return S_OK;
    }

    static STDMETHODIMP IsCrashMode(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex) 
    {
        dProperty = static_cast<const SimpleAirplane&>(Sim).IsCrashMode();
        return S_OK;
    }

    //String property samples
    static STDMETHODIMP GetCfgDir(__in const ISimObject& Sim, __out WCHAR* pszProperty, __in UINT uLength,  __in int iIndex)
    {
        return static_cast<const SimpleAirplane&>(Sim).GetBaseObject().GetCfgDir(pszProperty, uLength);
    }

    static STDMETHODIMP GetCfgFilePath(__in const ISimObject& Sim, __out WCHAR* pszProperty, __in UINT uLength,  __in int iIndex)
    {
        return static_cast<const SimpleAirplane&>(Sim).GetBaseObject().GetCfgFilePath(pszProperty, uLength);
    }

    static STDMETHODIMP GetCfgSectionName(__in const ISimObject& Sim, __out WCHAR* pszProperty, __in UINT uLength,  __in int iIndex)
    {
        return static_cast<const SimpleAirplane&>(Sim).GetBaseObject().GetCfgSectionName(pszProperty, uLength);
    }

    static STDMETHODIMP SetMFD_Message(__in ISimObject& Sim, __in LPCWSTR pszProperty,  __in int iIndex) 
    {
        static_cast<SimpleAirplane&>(Sim).SetMFD_Message(pszProperty);
        return S_OK;
    }

    static STDMETHODIMP GetMFD_Message(__in const ISimObject& Sim, __out LPWSTR pszProperty, __in UINT uLength,  __in int iIndex) 
    {
        static_cast<const SimpleAirplane&>(Sim).GetMFD_Message(pszProperty, uLength);
        return S_OK;
    }
    
    //Sample Trigger for setting value with secondary string input
    static STDMETHODIMP GetChannelValue(__in const ISimObject& Sim, __out double& dProperty, __in LPCWSTR pszSecondarySubstring, __in int iIndex)
    {
        static TCHAR* aChannels[] = { TEXT("A"), TEXT("B"), TEXT("C"), TEXT("D")};

        HRESULT hr = E_FAIL;

        dProperty = 0;

        if (pszSecondarySubstring)
        {
            for (UINT i = 0; i < ARRAYSIZE(aChannels); i++)
            {
                if (_tcsicmp(pszSecondarySubstring, aChannels[i]) == 0) 
                {
                    dProperty = static_cast<const SimpleAirplane&>(Sim).m_aChannelValues[i];
                    hr = S_OK;
                    break;
                }
            }
        }

        return hr;
    }

    //Sample property get for setting value with secondary string input
    static STDMETHODIMP SetChannelValue(__in ISimObject& Sim, __in double dProperty, __in LPCWSTR pszSecondaryName, __in int iIndex)
    {
        static TCHAR* aChannels[] = {TEXT("A"), TEXT("B"), TEXT("C"), TEXT("D")};

        HRESULT hr = E_FAIL;

        if (pszSecondaryName)
        {
            for (UINT i = 0; i < ARRAYSIZE(aChannels); i++)
            {
                if (_tcsicmp(pszSecondaryName, aChannels[i]) == 0) 
                {
                    static_cast<SimpleAirplane&>(Sim).m_aChannelValues[i] = (int)dProperty;

                    hr = S_OK;
                    break;
                }
            }
        }

        return hr;
    }

    //Sample property that supports an index
    static STDMETHODIMP GetMFD_ButtonId(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex)
    {
        HRESULT hr = E_FAIL;

        if( iIndex >= 0 &&
            iIndex < NUM_CHANNELS)
        {
            dProperty = static_cast<const SimpleAirplane&>(Sim).m_anMFD_ButtonIds[iIndex];
            
            hr = S_OK;
        }

        return hr;

    }

    //Sample trigger event that supports an index
    static STDMETHODIMP SetMFD_ButtonId(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
    {
        HRESULT hr = E_FAIL;

        if( iIndex >= 0 &&
            iIndex < NUM_CHANNELS)
        {
            static_cast<SimpleAirplane&>(Sim).m_anMFD_ButtonIds[iIndex] = (int)dProperty;
            
            hr = S_OK;
        }

        return hr;
    }

    const IBaseObjectV400& GetBaseObject() const {return m_BaseObject;}

    static WorldConstants  ms_WorldConstants;

private:

    SimpleAirplane(IBaseObjectV400& BaseObject) 
        :   m_RefCount(1),
            m_bSupportsLabel(TRUE),
            m_BaseObject(BaseObject), 
            m_Simulation(BaseObject)
    {
        for (UINT i = 0; i < NUM_CHANNELS; i++)
        {
            m_aChannelValues[i] = i;
            m_anMFD_ButtonIds[i] = i;
        }
    }
    
    ~SimpleAirplane() {}

    void   SetThrottle(float fThrottle)     {m_Simulation.SetThrottle(fThrottle);}
    void   IncThrottle()                    {m_Simulation.IncThrottle();}
    void   DecThrottle()                    {m_Simulation.DecThrottle();}
    double GetThrottle() const              {return m_Simulation.GetThrottle();}
    BOOL   IsAfterburnerActive() const      {return m_Simulation.IsAfterburnerActive();}

    void   SetElevator(float fElevator)     {m_Simulation.SetElevator(fElevator);}
    void   SetAilerons(float fAilerons)     {m_Simulation.SetAilerons(fAilerons);}
    void   SetRudder  (float fRudder)       {m_Simulation.SetRudder  (fRudder  );}

    double GetPctElevator() const           {return m_Simulation.GetPctElevator();}
    double GetPctAilerons() const           {return m_Simulation.GetPctAilerons();}
    double GetPctRudder()   const           {return m_Simulation.GetPctRudder();}

    void   IncGearLever()                   {m_Simulation.IncGearLever();}
    void   DecGearLever()                   {m_Simulation.DecGearLever();}
    void   SetGearUp()                      {m_Simulation.SetGearUp();}
    void   SetGearDown()                    {m_Simulation.SetGearDown();}
    void   ToggleGear()                     {m_Simulation.ToggleGear();}
    void   GearLeverSet(float fPctLever)    {m_Simulation.GearLeverSet(fPctLever);}
    double GetPctGearExtended() const       {return m_Simulation.GetPctGearExtended();}

    double GetAlpha()           const {return m_Simulation.GetAlpha();}
    double GetBeta()            const {return m_Simulation.GetBeta();}
    double GetTotalVelocity()   const {return m_Simulation.GetTotalVelocity();}
    BOOL   IsOnGround()         const {return m_Simulation.IsOnGround();}
    BOOL   IsOverPlatform()     const {return m_Simulation.IsOverPlatform();}
    double GetSurfaceElevation()const {return m_Simulation.GetSurfaceElevation();}

    BOOL IsPaused()    const {return (m_BaseObject.GetMode() & SIMOBJECT_MODE_PAUSE)    != 0;}
    BOOL IsDisabled()  const {return (m_BaseObject.GetMode() & SIMOBJECT_MODE_DISABLED) != 0;}
    BOOL IsSlewMode()  const {return (m_BaseObject.GetMode() & SIMOBJECT_MODE_SLEW)     != 0;}
    BOOL IsCrashMode() const {return (m_BaseObject.GetMode() & SIMOBJECT_MODE_CRASH)    != 0;}

    void SetMFD_Message(LPCWSTR szMessage)   {m_Simulation.SetMFD_Message(szMessage);}
    void GetMFD_Message(LPWSTR szMessage, UINT uLength)const  {m_Simulation.GetMFD_Message(szMessage, uLength);}

    IBaseObjectV400& m_BaseObject;
    Simulation m_Simulation;
    BOOL m_bSupportsLabel;

#ifdef USE_P3D_AIRPLANE_AI
    AirplaneAIService m_Airplane;
#else
    AirplaneAI m_AI;
#endif //USE_P3D_AIRPLANE_AI

    const SimpleAirplaneFixedData* m_pFixedData;

    //Sample "string-based" properties
    int  m_aChannelValues[NUM_CHANNELS];

    //Sample indexed property
    int m_anMFD_ButtonIds[NUM_CHANNELS];
};

//Unique class Guid
#define CLSID_Airplane __uuidof(SimpleAirplane)

REFGUID GetClassId() {return CLSID_Airplane;}

WorldConstants SimpleAirplane::ms_WorldConstants;

//Factory function wrapper
HRESULT New(__in __notnull IBaseObjectV400* pBaseObject, __out __notnull ISimObject** ppThisSim)
{
    CComPtr<IBaseObjectV400> spBaseObject;
    if (SUCCEEDED(pBaseObject->QueryInterface(IID_IBaseObjectV400, (void**)&spBaseObject)))
    {
        return SimpleAirplane::New(spBaseObject, ppThisSim);
    }
    else
    {
        return E_FAIL;
    }
}

//Property Registration
HRESULT RegisterProperties(__in __notnull ISimObjectManagerV400* pSimObjectMgr)
{
    HRESULT hr = E_FAIL;

    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("Throttle"),    TEXT("percent over 100"), SimpleAirplane::SetThrottle, EVENTTYPE_AXIS);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("IncThrottle"), TEXT("percent over 100"), SimpleAirplane::IncThrottle, EVENTTYPE_NORMAL);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("DecThrottle"), TEXT("percent over 100"), SimpleAirplane::DecThrottle, EVENTTYPE_NORMAL);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("Throttle"),    TEXT("percent over 100"), SimpleAirplane::GetThrottle);    
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("AfterburnerActive"), TEXT("Bool"),       SimpleAirplane::IsAfterburnerActive);
                                                    
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("Elevator"),    TEXT("percent over 100"), SimpleAirplane::SetElevator, EVENTTYPE_AXIS);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("Ailerons"),    TEXT("percent over 100"), SimpleAirplane::SetAilerons, EVENTTYPE_AXIS);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("Rudder"),      TEXT("percent over 100"), SimpleAirplane::SetRudder,   EVENTTYPE_AXIS);
                                                    
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("PctElevator"), TEXT("percent over 100"), SimpleAirplane::GetPctElevator);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("PctAilerons"), TEXT("percent over 100"), SimpleAirplane::GetPctAilerons);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("PctRudder"),   TEXT("percent over 100"), SimpleAirplane::GetPctRudder);
                                                    
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("Alpha"),            TEXT("Radians"),          SimpleAirplane::GetAlpha);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("Beta"),             TEXT("Radians"),          SimpleAirplane::GetBeta);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("TotalVelocity"),    TEXT("Feet Per Second"),  SimpleAirplane::GetTotalVelocity);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("IsOnGround"),       TEXT("Bool"),             SimpleAirplane::IsOnGround);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("IsOverPlatform"),   TEXT("Bool"),             SimpleAirplane::IsOverPlatform);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("SurfaceElevation"), TEXT("Feet"),             SimpleAirplane::GetSurfaceElevation);
                                                    
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("ToggleGear"),       TEXT("bool"),             SimpleAirplane::ToggleGear,  EVENTTYPE_NORMAL);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("GearLeverSet"),     TEXT("percent over 100"), SimpleAirplane::GearLeverSet, EVENTTYPE_AXIS);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("GearLeverSetPct"),  TEXT("percent over 100"), SimpleAirplane::GearLeverSet, EVENTTYPE_NORMAL);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("IncGearLever"),     TEXT("percent over 100"), SimpleAirplane::IncGearLever, P3D::EVENTTYPE_NORMAL);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("DecGearLever"),     TEXT("percent over 100"), SimpleAirplane::DecGearLever, P3D::EVENTTYPE_NORMAL);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("SetGearUp"),        TEXT("bool"),             SimpleAirplane::SetGearUp,    P3D::EVENTTYPE_NORMAL);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("SetGearDown"),      TEXT("bool"),             SimpleAirplane::SetGearDown,  P3D::EVENTTYPE_NORMAL);    
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("GearLever"),        TEXT("percent over 100"), SimpleAirplane::GetGearLever);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("PctGearExtended"),  TEXT("percent over 100"), SimpleAirplane::GetPctGearExtended);
                                                    
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("PctGearExtended"),  TEXT("percent over 100"), SimpleAirplane::GetPctGearExtended);
                                                    
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("IsPaused"),   TEXT("bool"), SimpleAirplane::IsPaused);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("IsDisabled"), TEXT("bool"), SimpleAirplane::IsDisabled);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("IsSlewMode"), TEXT("bool"), SimpleAirplane::IsSlewMode);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("IsCrashMode"), TEXT("bool"), SimpleAirplane::IsCrashMode);
                                                    
    //String properties                             
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("CfgDir"),           SimpleAirplane::GetCfgDir);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("CfgFilePath"),      SimpleAirplane::GetCfgFilePath);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("CfgSectionName"),   SimpleAirplane::GetCfgSectionName);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("MFD_Message"),      SimpleAirplane::SetMFD_Message);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("MFD_Message"),      SimpleAirplane::GetMFD_Message);
                                                    
    //Sample secondary string properties            
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("Channel"), TEXT("number"),  SimpleAirplane::GetChannelValue);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("Channel"), TEXT("number"),  SimpleAirplane::SetChannelValue);
                                                    
    //Sample indexed property                       
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("MFD_ButtonId"), TEXT("number"),  SimpleAirplane::GetMFD_ButtonId);
    pSimObjectMgr->RegisterProperty(CLSID_Airplane, TEXT("MFD_ButtonId"), TEXT("number"),  SimpleAirplane::SetMFD_ButtonId,  P3D::EVENTTYPE_NORMAL);


    hr = pSimObjectMgr->GetWorldConstants(SimpleAirplane::ms_WorldConstants);

    return S_OK;
}

static double GetPolarRadius()         {return SimpleAirplane::ms_WorldConstants.m_dPolarRadius;}
static double GetEquatorialRadius()    {return SimpleAirplane::ms_WorldConstants.m_dEquatorialRadius;}
static double GetGravity()             {return SimpleAirplane::ms_WorldConstants.m_fGravitySeaLevel;}
static double GetSpecificGasConstant() {return SimpleAirplane::ms_WorldConstants.m_fSpecificGasConstant;}

