/************************************************************************************************************** 
    Copyright (C) Lockheed Martin Corporation.  All rights reserved.
    
    Description: A sample demonstrating how to implement a simple helicopter ISimObject.
***************************************************************************************************************/

#pragma once

#include "ISimObject.h"

class Constants
{
public:

    static void Initialize(const P3D::WorldConstants& Constants) { ms_WorldConstants = Constants; }

    static const P3D::WorldConstants& GetWorldConstants() { return ms_WorldConstants; }

private:

    static P3D::WorldConstants ms_WorldConstants;
};

static float GetSpecificGasConstant()           { return Constants::GetWorldConstants().m_fSpecificGasConstant; }          ///aka R  
static float GetSpecificHeatRatio()             { return Constants::GetWorldConstants().m_fSpecificHeatRatio; }            ///aka Gamma (Cp/Cv for air (specific heat ratio) 
static float GetStandardSeaLevelDensity()       { return Constants::GetWorldConstants().m_fStandardSeaLevelDensity; }      ///slugs/ft^3
static float GetStandardSeaLevelTemperature()   { return Constants::GetWorldConstants().m_fStandardSeaLevelTemperature; }  ///Rankine
static float GetStandardSeaLevelPressure()      { return Constants::GetWorldConstants().m_fStandardSeaLevelPressure; }     ///Lbs/SqFt
static float GetGravitySeaLevel()               { return Constants::GetWorldConstants().m_fGravitySeaLevel; }              ///ft/s^2

static double GetPolarRadius()                  { return Constants::GetWorldConstants().m_dPolarRadius; }                  ///Feet
static double GetEquatorialRadius()             { return Constants::GetWorldConstants().m_dEquatorialRadius; }             ///Feet
