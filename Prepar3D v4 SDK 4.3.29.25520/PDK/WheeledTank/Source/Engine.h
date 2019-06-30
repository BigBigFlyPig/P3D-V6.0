//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//------------------------------------------------------------------------------
//
//  WheeledTank ISimObject (SimObject API) Example
//  Engine.h
//  
//  Description:
//              Engine controlls the acceleration of the wheeled tank.  Manages
//              all engine related vairables and assists in the wheeled tank's
//              main simulation process.  Also, creates an exhaust effect.
//------------------------------------------------------------------------------

#pragma once

#include <atlcomcli.h>
#include "IUnknownHelper.h"

#include <ISimObject.h>

using namespace P3D;

class WheeledTank;

/***********************************************************************************************
    Engine Subsystem
***********************************************************************************************/
class Engine
{
public:
    Engine(IBaseObjectV400* pBaseObject) 
        :   m_spBaseObject(pBaseObject), 
            m_bOn(TRUE), 
            m_fStarter(0.0f), 
            m_fPctRpm(0), 
            m_fPctRpmGoal(0),
            m_bSoundOn(FALSE), 
            m_pVisualEffect(NULL) 
    {
        m_vExhaustOffset.fX = 0, m_vExhaustOffset.fY = 3; m_vExhaustOffset.fZ = -12;
        m_vEngineffset.dX   = 0, m_vEngineffset.dY   = 0, m_vEngineffset.dZ = 5.0;
    }
    ~Engine();

    static STDMETHODIMP Toggle(__in ISimObject& Sim, __in double dProperty, __in int iIndex);
    static STDMETHODIMP GetPercentRpm(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex);
    static STDMETHODIMP GetCombustion(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex);
    static STDMETHODIMP GetStarter   (__in const ISimObject& Sim, __out double& dProperty, __in int iIndex);
    static STDMETHODIMP GetEngineOffset(__in const ISimObject& Sim, __out DXYZ& vProperty, __in int iIndex);

    HRESULT Toggle();
    HRESULT SetOn(BOOL bOn);

    BOOL  IsOn()           const {return m_bOn;}
    BOOL  IsStarterOn()    const {return m_fStarter > 0;}
    float GetRpm()         const {return m_fPctRpm;}
    double GetPercentRpm() const {return m_fPctRpm;}

    const DXYZ& GetEngineOffset() const {return m_vEngineffset;}
    void SetPctRpm(float fPctRpm) {m_fPctRpmGoal = (m_bOn && fPctRpm < 0.2f) ? 0.2f : fPctRpm;}

    HRESULT Update(__in const WheeledTank& Veh, double dDeltaT);

private:
    CComPtr<IBaseObjectV400>    m_spBaseObject;
    WheeledTank*    m_pWheeledTank;

    BOOL            m_bOn;
    float           m_fStarter;
    float           m_fPctRpm;
    float           m_fPctRpmGoal;
    BOOL            m_bSoundOn;
    void*           m_pVisualEffect;
    FXYZ            m_vExhaustOffset;
    DXYZ            m_vEngineffset;
};
