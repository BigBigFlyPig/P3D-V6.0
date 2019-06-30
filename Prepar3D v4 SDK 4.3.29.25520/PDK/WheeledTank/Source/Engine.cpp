//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//------------------------------------------------------------------------------
//
//  WheeledTank ISimObject (SimObject API) Example
//  Engine.cpp
//  
//  Description:
//              Engine controlls the acceleration of the wheeled tank.  Manages
//              all engine related vairables and assists in the wheeled tank's
//              main simulation process.  Also, creates an exhaust effect.
//------------------------------------------------------------------------------


#include "Engine.h"
#include "WheeledTank.h"

using namespace P3D;


/***********************************************************************************************
    Engine Destructor
***********************************************************************************************/
Engine::~Engine()
{
    if (m_pVisualEffect)
    {
        m_spBaseObject->VisualEffectOff(m_pVisualEffect);
        m_pVisualEffect = NULL;
    }
}

/***********************************************************************************************
    Engine subsystem simulation update
***********************************************************************************************/
HRESULT Engine::Update(__in const WheeledTank& Veh, double dDeltaT)
{
    HRESULT hr = S_OK;

    if (m_bOn)
    {
        if (m_pVisualEffect == NULL)
        {
            hr = m_spBaseObject->VisualEffectOn(TEXT("fx_dieselExhaust"), &m_vExhaustOffset, &m_pVisualEffect);
        }
    }
    else
    {
        if (m_pVisualEffect != NULL)
        {
            hr = m_spBaseObject->VisualEffectOff(m_pVisualEffect);
            m_pVisualEffect = NULL;
        }

    }   

    static const float s_RPM_TC = 1.0f;
    m_fPctRpm += (float)((m_fPctRpmGoal - m_fPctRpm) * s_RPM_TC * dDeltaT);

    if (m_bSoundOn != m_bOn)
    {
        m_bSoundOn = m_bOn;
        m_spBaseObject->TriggerSound(TEXT("Engine"), m_bSoundOn);
    }

    if (m_fStarter > 0)
    {
        m_fStarter -= (float)dDeltaT;

        if (m_fStarter < 0)
        {
            m_fStarter = 0;
        }

        if (!m_bOn && m_fStarter < 0.5f)
        {
            m_bOn = TRUE;
        }
    }

    return hr;
}

/***********************************************************************************************
***********************************************************************************************/
HRESULT Engine::SetOn(BOOL bOn)
{
    if (bOn != m_bOn)
    {
        if (bOn)
        {
            if (m_fStarter == 0)
            {
                m_fStarter = 1.0f;  //seconds
            }

            m_spBaseObject->TriggerSound(TEXT("EngineStart"), NULL);
        }
        else
        {
            m_bOn = FALSE;
        }
    }

    return S_OK;
}

/***********************************************************************************************
***********************************************************************************************/
HRESULT Engine::Toggle()
{
    return SetOn(!m_bOn);
}

/***********************************************************************************************
    Engine properties and triggers
***********************************************************************************************/
/*static*/
STDMETHODIMP Engine::Toggle(__in ISimObject& Sim, __in double dProperty, __in int iIndex)
{
    return static_cast<WheeledTank&>(Sim).GetEngine().Toggle();
}

/***********************************************************************************************
***********************************************************************************************/
/*static*/
STDMETHODIMP Engine::GetPercentRpm(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex)
{
    dProperty = static_cast<const WheeledTank&>(Sim).GetEngine().GetPercentRpm();

    return S_OK;
}

/***********************************************************************************************
***********************************************************************************************/
/*static*/
STDMETHODIMP Engine::GetCombustion(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex)
{
    dProperty = static_cast<const WheeledTank&>(Sim).GetEngine().IsOn();

    return S_OK;
}

/***********************************************************************************************
***********************************************************************************************/
/*static*/
STDMETHODIMP Engine::GetStarter(__in const ISimObject& Sim, __out double& dProperty, __in int iIndex)
{
    dProperty = static_cast<const WheeledTank&>(Sim).GetEngine().IsStarterOn();

    return S_OK;
}

/***********************************************************************************************
***********************************************************************************************/
/*static*/
STDMETHODIMP Engine::GetEngineOffset(__in const ISimObject& Sim, __out DXYZ& vProperty, __in int iIndex)
{
    vProperty = static_cast<const WheeledTank&>(Sim).GetEngine().GetEngineOffset();

    return S_OK;
}
