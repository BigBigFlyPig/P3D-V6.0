/**************************************************************************************************************
    Copyright (C) Lockheed Martin Corporation.  All rights reserved.

    Description: A sample demonstrating how to implement a simple helicopter ISimObject.
***************************************************************************************************************/

#include <atlcomcli.h>
#include <initguid.h>

#include "Simulation.h"

#include "Constants.h"
#include "Util.h"

#define MAKE_STATIC 1
#ifdef  MAKE_STATIC
#define STATIC_CONST static const
#else
#define STATIC_CONST 
#endif

using namespace P3D;

DEFINE_GUID(CLSID_Simulation, 0xdc89e031, 0x65a1, 0x4234, 0x9e, 0x62, 0x37, 0x62, 0x64, 0xaf, 0xe2, 0x18);

/****************************************************************
****************************************************************/
Simulation::Simulation(IBaseObjectV400& BaseObject) :
    m_RefCount(1),
    m_BaseObject(BaseObject),
    m_fCyclicPitch(0),
    m_fCyclicBank(0),
    m_fPedals(0),
    m_fCollective(0.0f),
    m_fMass(125.0f), //Slugs
    m_dTotalVelocity(0.0),
    m_bOnGround(FALSE),
    m_fMainRotorRotationPercent(0.0f),
    m_fTailRotorRotationPercent(0.0f)
{
    memset(&m_SurfaceInfo, 0, sizeof(m_SurfaceInfo));
    memset(&m_WeatherInfo, 0, sizeof(m_WeatherInfo));

    m_vfMOI.fX = 20000.0f;
    m_vfMOI.fY = 40000.0f;
    m_vfMOI.fZ = 20000.0f;

    memset(&m_vdPos, 0, sizeof(m_vdPos));
    memset(&m_vdOrient, 0, sizeof(m_vdOrient));
    memset(&m_vdVel, 0, sizeof(m_vdVel));
    memset(&m_vdRotVel, 0, sizeof(m_vdRotVel));

    memset(&m_vBodyVelocity, 0, sizeof(m_vBodyVelocity));
    memset(&m_vBodyRotationVelocity, 0, sizeof(m_vBodyRotationVelocity));

    memset(&m_vRotorForce, 0, sizeof(m_vRotorForce));
    memset(&m_vRotorMoment, 0, sizeof(m_vRotorMoment));

    memset(&m_vAeroForce, 0, sizeof(m_vAeroForce));
    memset(&m_vAeroMoment, 0, sizeof(m_vAeroMoment));

    memset(&m_vGroundReactionForce, 0, sizeof(m_vGroundReactionForce));
    memset(&m_vGroundReactionMoment, 0, sizeof(m_vGroundReactionMoment));
}

/****************************************************************
****************************************************************/
Simulation::~Simulation()
{
}

/****************************************************************
****************************************************************/
HRESULT Simulation::Init()
{
    m_Matrix.Update(m_vdOrient);

    return S_OK;
}

/****************************************************************
****************************************************************/
void Simulation::InitPositionFromBase()
{
    m_BaseObject.GetPosition(m_vdPos, m_vdOrient, m_vdVel, m_vdRotVel);
    m_Matrix.Update(m_vdOrient);
}

/****************************************************************
****************************************************************/
STDMETHODIMP Simulation::QueryInterface(__in REFIID riid, void **ppv)
{
    HRESULT hr = E_NOINTERFACE;

    if (!ppv)
    {
        goto Error;
    }

    *ppv = NULL;

    if (IsEqualIID(riid, IID_IUnknown))
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    else if (IsEqualIID(riid, IID_ISimulationV01))
    {
        *ppv = static_cast<ISimulationV01*>(this);
    }
    else if (IsEqualIID(riid, IID_ISimulation))
    {
        *ppv = static_cast<ISimulation*>(this);
    }
    else if (IsEqualIID(riid, CLSID_Simulation))
    {
        *ppv = static_cast<Simulation*>(this);
    }

    if (*ppv)
    {
        hr = S_OK;
        AddRef();
    }

Error:

    return hr;
}

/****************************************************************
****************************************************************/
STDMETHODIMP Simulation::Update(double dDeltaT)
{
    if (IsNormalMode())
    {
        static const float MAX_VELOCITY = 5000.0f; // ft/s
        static const float MAX_ROTATION_VELOCITY = 14.0f;   // Rad/s

        //Update surface and weather info
        m_BaseObject.GetSurfaceInformation(m_SurfaceInfo, NULL);
        m_BaseObject.GetWeatherInformation(m_WeatherInfo);

        //Get current position
        m_BaseObject.GetPosition(m_vdPos, m_vdOrient, m_vdVel, m_vdRotVel);

        //Update Subsystems
        UpdateRotors(dDeltaT);
        UpdateAerodynamics(dDeltaT);
        UpdateGroundReaction(dDeltaT);

        //Total body force
        DXYZ vdBodyForce;
        vdBodyForce.dX = m_vRotorForce.dX + m_vAeroForce.dX + m_vGroundReactionForce.dX;
        vdBodyForce.dY = m_vRotorForce.dY + m_vAeroForce.dY + m_vGroundReactionForce.dY;
        vdBodyForce.dZ = m_vRotorForce.dZ + m_vAeroForce.dZ + m_vGroundReactionForce.dZ;

        //Total body acceleration
        DXYZ vdBodyAcceleration;
        vdBodyAcceleration.dX = (vdBodyForce.dX / m_fMass) - GetGravitySeaLevel() * cos(m_vdOrient.dX) * sin(m_vdOrient.dZ);
        vdBodyAcceleration.dY = (vdBodyForce.dY / m_fMass) - GetGravitySeaLevel() * cos(m_vdOrient.dX) * cos(m_vdOrient.dZ);
        vdBodyAcceleration.dZ = (vdBodyForce.dZ / m_fMass) + GetGravitySeaLevel() * sin(m_vdOrient.dX);

        //Integrate body velocity
        m_vBodyVelocity.dX += vdBodyAcceleration.dX  * dDeltaT;
        m_vBodyVelocity.dY += vdBodyAcceleration.dY  * dDeltaT;
        m_vBodyVelocity.dZ += vdBodyAcceleration.dZ  * dDeltaT;

        LIMIT_VECTOR(m_vBodyVelocity, MAX_VELOCITY)

        //Transform to World velocity
        m_Matrix.BodyToWorld(m_vBodyVelocity, m_vdVel);

        //Integrate world postion
        double latRadians = cos(m_vdPos.dZ);
        if (latRadians != 0.0)
        {
            m_vdPos.dX += (m_vdVel.dX * dDeltaT) / (latRadians * GetEquatorialRadius());
        }
        m_vdPos.dY += m_vdVel.dY * dDeltaT;
        m_vdPos.dZ += (m_vdVel.dZ * dDeltaT) / GetPolarRadius();

        //Total Body Moment
        DXYZ vdBodyMoment;
        vdBodyMoment.dX = m_vRotorMoment.dX + m_vAeroMoment.dX + m_vGroundReactionMoment.dX;
        vdBodyMoment.dY = m_vRotorMoment.dY + m_vAeroMoment.dY + m_vGroundReactionMoment.dY;
        vdBodyMoment.dZ = m_vRotorMoment.dZ + m_vAeroMoment.dZ + m_vGroundReactionMoment.dZ;

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

    return S_OK;
}

/****************************************************************
****************************************************************/
void Simulation::UpdateRotors(double dDeltaT)
{
    // Main Rotor
    STATIC_CONST double MAIN_ROTOR_BLADE_COUNT = 2;
    STATIC_CONST double MAIN_ROTOR_BLADE_LENGTH = METERS_TO_FEET(4.0);                  // Feet
    STATIC_CONST double MAIN_ROTOR_BLADE_CHORD = METERS_TO_FEET(0.25);                  // Feet
    STATIC_CONST double MAIN_ROTOR_BLADE_LIFT_COEFF = 20.0;
    STATIC_CONST double MAIN_ROTOR_CIRCUMFERENCE = 2.0 * PI * MAIN_ROTOR_BLADE_LENGTH;  // Feet

    STATIC_CONST double MAIN_ROTOR_RPM = 500.0;                                         // Rotations per minute
    STATIC_CONST double MAIN_ROTOR_RPS = MAIN_ROTOR_RPM * ONE_OVER_60 * TWO_PI;         // Radians per second

    STATIC_CONST double MAIN_ROTOR_PITCH_MAX = DEGREES_TO_RADIANS(7.5);                 // Radians
    STATIC_CONST double MAIN_ROTOR_BANK_MAX = DEGREES_TO_RADIANS(10.0);                 // Radians

    STATIC_CONST DXYZ   MAIN_ROTOR_POSITION = { 0.000, 4.921, -0.137 };                 // Feet
    STATIC_CONST double MAIN_ROTOR_DISTANCE = sqrt(SQRD(MAIN_ROTOR_POSITION.dX) + SQRD(MAIN_ROTOR_POSITION.dY) + SQRD(MAIN_ROTOR_POSITION.dZ)); // Feet

    // Tail Rotor
    STATIC_CONST double TAIL_ROTOR_BLADE_COUNT = 2;
    STATIC_CONST double TAIL_ROTOR_BLADE_LENGTH = METERS_TO_FEET(0.75);                 // Feet
    STATIC_CONST double TAIL_ROTOR_BLADE_CHORD = METERS_TO_FEET(0.2);                   // Feet
    STATIC_CONST double TAIL_ROTOR_BLADE_LIFT_COEFF = 5.0;
    STATIC_CONST double TAIL_ROTOR_CIRCUMFERENCE = 2.0 * PI * TAIL_ROTOR_BLADE_LENGTH;  // Feet

    STATIC_CONST double TAIL_ROTOR_RATIO = 3.0;                                         // 3:1 Ratio
    STATIC_CONST double TAIL_ROTOR_RPM = MAIN_ROTOR_RPM * TAIL_ROTOR_RATIO;             // Rotations per minute
    STATIC_CONST double TAIL_ROTOR_RPS = TAIL_ROTOR_RPM * ONE_OVER_60 * TWO_PI;         // Radians per second

    STATIC_CONST DXYZ   TAIL_ROTOR_POSITION = { -0.738, 2.297, -14.737 };               // Feet
    STATIC_CONST double TAIL_ROTOR_DISTANCE = sqrt(SQRD(TAIL_ROTOR_POSITION.dX) + SQRD(TAIL_ROTOR_POSITION.dY) + SQRD(TAIL_ROTOR_POSITION.dZ)); // Feet

    // Blade Velocity (feet per second)
    double dMainBladeVelocity = dDeltaT * MAIN_ROTOR_RPS * ONE_OVER_TWO_PI * MAIN_ROTOR_CIRCUMFERENCE;
    double dTailBladeVelocity = dDeltaT * TAIL_ROTOR_RPS * ONE_OVER_TWO_PI * TAIL_ROTOR_CIRCUMFERENCE;

    // Lift
    double dAmbientDensity = m_WeatherInfo.m_fAmbientPressure / (GetSpecificGasConstant() * m_WeatherInfo.m_fTemperature);
    double dMainRotorThrust = 0.5 * dAmbientDensity * dMainBladeVelocity * dMainBladeVelocity * MAIN_ROTOR_BLADE_LIFT_COEFF * MAIN_ROTOR_BLADE_CHORD * MAIN_ROTOR_BLADE_LENGTH * MAIN_ROTOR_BLADE_COUNT;
    double dTailRotorThrust = 0.5 * dAmbientDensity * dTailBladeVelocity * dTailBladeVelocity * MAIN_ROTOR_BLADE_LIFT_COEFF * TAIL_ROTOR_BLADE_LIFT_COEFF * TAIL_ROTOR_BLADE_CHORD * TAIL_ROTOR_BLADE_LENGTH * TAIL_ROTOR_BLADE_COUNT;

    // Apply Rotor Pitch and Bank
    double dMainRotorPitch = m_fCyclicPitch * MAIN_ROTOR_PITCH_MAX;
    double dMainRotorBank = m_fCyclicBank * MAIN_ROTOR_BANK_MAX;

    // Lift Vector
    double dSinPitch = sin(dMainRotorPitch);
    double dCosPitch = cos(dMainRotorPitch);
    double dSinBank = sin(dMainRotorBank);
    double dCosBank = cos(dMainRotorBank);

    DXYZ dCf;
    DXYZ dCmf;

    STATIC_CONST double PITCH_THRUST_SCALAR = 2.0;
    STATIC_CONST double BANK_THRUST_SCALAR = 2.0;

    // Resultant Force
    DXYZ dResultantForce;
    dResultantForce.dZ = dMainRotorThrust * -dSinBank * PITCH_THRUST_SCALAR;
    dResultantForce.dX = -dMainRotorThrust * dSinPitch * dCosBank * BANK_THRUST_SCALAR;
    dResultantForce.dY = dMainRotorThrust * (dCosPitch * dSinBank + dCosPitch * dCosBank);
    
    // Force
    dCf.dZ = dResultantForce.dZ * m_fCollective;
    dCf.dX = dResultantForce.dX * m_fCollective;
    dCf.dY = dResultantForce.dY * m_fCollective;

    // Moment
    dCmf.dX = dResultantForce.dX * MAIN_ROTOR_DISTANCE;
    dCmf.dX += m_vBodyRotationVelocity.dX;

    dCmf.dZ = dResultantForce.dZ * MAIN_ROTOR_DISTANCE;
    dCmf.dZ += m_vBodyRotationVelocity.dZ;

    dCmf.dY = dTailRotorThrust * TAIL_ROTOR_DISTANCE * m_fPedals;
    dCmf.dY += m_vBodyRotationVelocity.dY;

    // Cf -> Force
    m_vRotorForce.dX = dCf.dX;
    m_vRotorForce.dY = dCf.dY;
    m_vRotorForce.dZ = dCf.dZ;

    m_vRotorMoment.dX = dCmf.dX;
    m_vRotorMoment.dY = dCmf.dY;
    m_vRotorMoment.dZ = dCmf.dZ;

    // Animations
    m_fMainRotorRotationPercent += (float)(dDeltaT * MAIN_ROTOR_RPS * ONE_OVER_TWO_PI);
    m_fTailRotorRotationPercent += (float)(dDeltaT * TAIL_ROTOR_RPS * ONE_OVER_TWO_PI);

    m_fMainRotorRotationPercent = (float)fmod(m_fMainRotorRotationPercent, 1.0f);
    m_fTailRotorRotationPercent = (float)fmod(m_fTailRotorRotationPercent, 1.0f);
}

/****************************************************************
****************************************************************/
void Simulation::UpdateAerodynamics(double dDeltaT)
{
    /* AERODYNAMIC COEFFICIENTS */

    //Drag
    STATIC_CONST DXYZ DRAG_COEFF = { 0.82, 0.82, 0.42 };
    STATIC_CONST DXYZ SURFACE_AREA = { 50.0, 40.0, 20.0 };
    
    //Pitch
    STATIC_CONST float CMDE = -0.50f;
    STATIC_CONST float CMQ = -0.50f;

    //Roll
    STATIC_CONST float CLDA = -0.50f;
    STATIC_CONST float CLP = -0.50f;

    //Yaw
    STATIC_CONST float CNDR = -0.50f;
    STATIC_CONST float CNR = -0.50f;

    double dTotalVelocitySqrd = SQRD(m_vBodyVelocity.dX) + SQRD(m_vBodyVelocity.dY) + SQRD(m_vBodyVelocity.dZ);
    m_dTotalVelocity = sqrt(dTotalVelocitySqrd);

    double dAmbientDensity = m_WeatherInfo.m_fAmbientPressure / (GetSpecificGasConstant() * m_WeatherInfo.m_fTemperature);

    DXYZ dCf;
    DXYZ dCmf;

    // XYZ Force
    dCf.dZ = -0.5 * dAmbientDensity * DRAG_COEFF.dZ * SURFACE_AREA.dZ * SQRD(m_vBodyVelocity.dZ);
    dCf.dX = -0.5 * dAmbientDensity * DRAG_COEFF.dX * SURFACE_AREA.dX * SQRD(m_vBodyVelocity.dX);
    dCf.dY = -0.5 * dAmbientDensity * DRAG_COEFF.dY * SURFACE_AREA.dY * SQRD(m_vBodyVelocity.dY);

    // Pitch/Bank/Heading Moment
    dCmf.dX = CMDE;
    dCmf.dX += CMQ * m_vBodyRotationVelocity.dX;

    dCmf.dY = CNDR;
    dCmf.dY += CNR * m_vBodyRotationVelocity.dY;

    dCmf.dZ = CLDA;
    dCmf.dZ += CLP * m_vBodyRotationVelocity.dZ;

    //Cf -> Force
    m_vAeroForce.dX = dCf.dX;
    m_vAeroForce.dY = dCf.dY;
    m_vAeroForce.dZ = dCf.dZ;

    m_vAeroMoment.dX = dCmf.dX;
    m_vAeroMoment.dY = dCmf.dY;
    m_vAeroMoment.dZ = dCmf.dZ;
}

/****************************************************************
****************************************************************/
void Simulation::UpdateGroundReaction(double dDeltaT)
{
    STATIC_CONST float STATIC_HEIGHT_ABOVE_GROUND = 3.7f;

    memset(&m_vGroundReactionForce, 0, sizeof(m_vGroundReactionForce));
    memset(&m_vGroundReactionMoment, 0, sizeof(m_vGroundReactionMoment));

    double dHeightAboveGround = m_vdPos.dY - m_SurfaceInfo.m_fElevation - STATIC_HEIGHT_ABOVE_GROUND;

    if (dHeightAboveGround < 0)   // On ground
    {
        STATIC_CONST float SPRING_K = -17877.0f;
        STATIC_CONST float DAMPER_K = -1500.0f;

        STATIC_CONST float FRICTION_X = -5000.0f;
        STATIC_CONST float FRICTION_Z = -5000.0f;
        STATIC_CONST float FRICTION_YAW = -5000.0f;

        STATIC_CONST float PITCH_K = -1500.0f;
        STATIC_CONST float BANK_K = -1000.0f;

        m_bOnGround = TRUE;

        //Spring
        m_vGroundReactionForce.dY = dHeightAboveGround * SPRING_K + m_vdVel.dY * DAMPER_K;

        //Apply friction.  For simplicity this actually acts more like a drag resistance
        m_vGroundReactionForce.dX = m_vBodyVelocity.dX * FRICTION_X;
        m_vGroundReactionForce.dZ = m_vBodyVelocity.dZ * FRICTION_Z;

        //Rotating resistance
        m_vGroundReactionMoment.dZ = m_vBodyRotationVelocity.dZ * BANK_K;
        m_vGroundReactionMoment.dX = m_vBodyRotationVelocity.dX * PITCH_K;
        m_vGroundReactionMoment.dY = m_vBodyRotationVelocity.dY * FRICTION_YAW;

        //Level pitch and bank
        m_vdOrient.dX = 0;
        m_vdOrient.dZ = 0;
    }
    else //In air
    {
        m_bOnGround = FALSE;
    }
}

/****************************************************************
****************************************************************/
BOOL Simulation::IsNormalMode() const
{
    unsigned short usFlags =
        SIMOBJECT_MODE_PAUSE |
        SIMOBJECT_MODE_DISABLED |
        SIMOBJECT_MODE_SLEW |
        SIMOBJECT_MODE_CRASH |
        SIMOBJECT_MODE_FREEZE_POSITION |
        SIMOBJECT_MODE_FREEZE_ATTITUDE |
        SIMOBJECT_MODE_FREEZE_ALTITUDE |
        SIMOBJECT_MODE_FREEZE_LATLON;

    return (0 == (m_BaseObject.GetMode() & usFlags));
}

//Helper function to get from the ISimObject to the Simulation
static HRESULT SimulationGet(const __in ISimObject& Sim, __out Simulation** ppSimulation)
{
    if (!ppSimulation)
    {
        return E_POINTER;
    }
 
    return const_cast<ISimObject&>(Sim).QueryService(SID_Simulation, CLSID_Simulation, (void**)ppSimulation);
}

//Set property macro for axis events
#define SIMULATION_PROPERTY_AXIS(PROPNAME, SETFUNC)\
/*static*/ STDMETHODIMP Simulation::PROPNAME(__in ISimObject& Sim, __in double dProperty, __in int iIndex)\
{\
    Simulation* pSimulation;\
    if (SUCCEEDED(SimulationGet(Sim, &pSimulation)))\
    {\
        pSimulation->SETFUNC((0.5f * (1 - (float)dProperty)));\
        pSimulation->Release();\
        return S_OK;\
    }\
    else\
    {\
        dProperty = 0;\
        return E_FAIL;\
    }\
}\

//Set property macro for button events
#define SIMULATION_PROPERTY_SET(PROPNAME, SETFUNC)\
/*static*/ STDMETHODIMP Simulation::PROPNAME(__in ISimObject& Sim, __in double dProperty, __in int iIndex)\
{\
    Simulation* pSimulation;\
    if (SUCCEEDED(SimulationGet(Sim, &pSimulation)))\
    {\
        pSimulation->SETFUNC((float)dProperty);\
        pSimulation->Release();\
        return S_OK;\
    }\
    else\
    {\
        dProperty = 0;\
        return E_FAIL;\
    }\
}\

//Set property macro for button events
#define SIMULATION_PROPERTY_SET_NO_ARG(PROPNAME, SETFUNC)\
/*static*/ STDMETHODIMP Simulation::PROPNAME(__in ISimObject& Sim, __in double dProperty, __in int iIndex)\
{\
    Simulation* pSimulation;\
    if (SUCCEEDED(SimulationGet(Sim, &pSimulation)))\
    {\
        pSimulation->SETFUNC();\
        pSimulation->Release();\
        return S_OK;\
    }\
    else\
    {\
        dProperty = 0;\
        return E_FAIL;\
    }\
}\

//Get property macro
#define SIMULATION_PROPERTY_GET(PROPNAME, GETFUNC)\
/*static*/ STDMETHODIMP Simulation::PROPNAME(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex)\
{\
    Simulation* pSimulation;\
    if (SUCCEEDED(SimulationGet(Sim, &pSimulation)))\
    {\
        dProperty = pSimulation->GETFUNC();\
        pSimulation->Release();\
        return S_OK;\
    }\
    else\
    {\
        dProperty = 0;\
        return E_FAIL;\
    }\
}\

SIMULATION_PROPERTY_AXIS(SetCollective, SetCollective)
SIMULATION_PROPERTY_SET_NO_ARG(IncCollective, IncCollective)
SIMULATION_PROPERTY_SET_NO_ARG(DecCollective, DecCollective)

SIMULATION_PROPERTY_SET(SetCyclicPitch, SetCyclicPitch)
SIMULATION_PROPERTY_SET(SetCyclicBank, SetCyclicBank)
SIMULATION_PROPERTY_SET(SetPedals, SetPedals)

SIMULATION_PROPERTY_GET(GetMainRotorRotationPercent, GetMainRotorRotationPercent)
SIMULATION_PROPERTY_GET(GetTailRotorRotationPercent, GetTailRotorRotationPercent)

/****************************************************************
****************************************************************/
/*static*/
HRESULT Simulation::RegisterProperties(__in REFGUID guidCategory,
                                       __in __notnull P3D::ISimObjectManagerV400* pSimObjectMgr)
{
    pSimObjectMgr->RegisterProperty(guidCategory, TEXT("SetCyclicPitch"), TEXT("percent over 100"), Simulation::SetCyclicPitch, EVENTTYPE_AXIS);
    pSimObjectMgr->RegisterProperty(guidCategory, TEXT("SetCyclicBank"),  TEXT("percent over 100"), Simulation::SetCyclicBank,  EVENTTYPE_AXIS);
    pSimObjectMgr->RegisterProperty(guidCategory, TEXT("SetPedals"),      TEXT("percent over 100"), Simulation::SetPedals,      EVENTTYPE_AXIS);

    pSimObjectMgr->RegisterProperty(guidCategory, TEXT("SetCollective"),  TEXT("percent over 100"), Simulation::SetCollective,  EVENTTYPE_AXIS);
    pSimObjectMgr->RegisterProperty(guidCategory, TEXT("IncCollective"),  TEXT("percent over 100"), Simulation::IncCollective,  EVENTTYPE_NORMAL);
    pSimObjectMgr->RegisterProperty(guidCategory, TEXT("DecCollective"),  TEXT("percent over 100"), Simulation::DecCollective,  EVENTTYPE_NORMAL);

    pSimObjectMgr->RegisterProperty(guidCategory, TEXT("GetMainRotorRotationPercent"), TEXT("percent over 100"), Simulation::GetMainRotorRotationPercent);
    pSimObjectMgr->RegisterProperty(guidCategory, TEXT("GetTailRotorRotationPercent"), TEXT("percent over 100"), Simulation::GetTailRotorRotationPercent);

    return S_OK;
}
