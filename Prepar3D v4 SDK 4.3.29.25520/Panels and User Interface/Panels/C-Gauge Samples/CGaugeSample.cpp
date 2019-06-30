//Copyright (c) Microsoft Corporation.  All rights reserved. 
// SDK.c

#include "gauges.h"

#include "CGaugeSample.h"


/////////////////////////////////////////////////////////////////////////////
// Attitude
/////////////////////////////////////////////////////////////////////////////
#define     GAUGE_NAME          "Attitude"
#define     GAUGEHDR_VAR_NAME   gaugehdr_attitude
#define     GAUGE_W             100

#include "CGaugeSample.Attitude.hpp"

/////////////////////////////////////////////////////////////////////////////
// Control Surfaces
/////////////////////////////////////////////////////////////////////////////
#define     GAUGE_NAME          "Control_Surfaces"
#define     GAUGEHDR_VAR_NAME   gaugehdr_control_surfaces
#define     GAUGE_W             100

#include "CGaugeSample.Control_Surfaces.hpp"

/////////////////////////////////////////////////////////////////////////////
// Fuel
/////////////////////////////////////////////////////////////////////////////
#define     GAUGE_NAME          "Fuel"
#define     GAUGEHDR_VAR_NAME   gaugehdr_fuel
#define     GAUGE_W             100

#include "CGaugeSample.Fuel.hpp"

/////////////////////////////////////////////////////////////////////////////
// Fuel Selector
/////////////////////////////////////////////////////////////////////////////
#define     GAUGE_NAME          "Fuel_Selector"
#define     GAUGEHDR_VAR_NAME   gaugehdr_fuel_selector
#define     GAUGE_W             50

#include "CGaugeSample.Fuel_Selector.hpp"

/////////////////////////////////////////////////////////////////////////////
// Temperature
/////////////////////////////////////////////////////////////////////////////
#define     GAUGE_NAME          "Temperature"
#define     GAUGEHDR_VAR_NAME   gaugehdr_temperature
#define     GAUGE_W             100

#include "CGaugeSample.Temperature.hpp"

/////////////////////////////////////////////////////////////////////////////
// Whiskey
/////////////////////////////////////////////////////////////////////////////
#define     GAUGE_NAME          "Whiskey"
#define     GAUGEHDR_VAR_NAME   gaugehdr_whiskey
#define     GAUGE_W             100

#include "CGaugeSample.Whiskey.hpp"


/////////////////////////////////////////////////////////////////////////////
// FlightMap
/////////////////////////////////////////////////////////////////////////////
#define     GAUGE_NAME          "FlightMap"
#define     GAUGEHDR_VAR_NAME   gaugehdr_flightmap
#define     GAUGE_W         276

#include "CGaugeSample.FlightMap.hpp"

/////////////////////////////////////////////////////////////////////////////
// Gauge table entries
/////////////////////////////////////////////////////////////////////////////
GAUGE_TABLE_BEGIN()
    GAUGE_TABLE_ENTRY(&gaugehdr_attitude)
    GAUGE_TABLE_ENTRY(&gaugehdr_control_surfaces)
    GAUGE_TABLE_ENTRY(&gaugehdr_fuel)
    GAUGE_TABLE_ENTRY(&gaugehdr_fuel_selector)
    GAUGE_TABLE_ENTRY(&gaugehdr_temperature)
    GAUGE_TABLE_ENTRY(&gaugehdr_whiskey)
    GAUGE_TABLE_ENTRY(&gaugehdr_flightmap)
GAUGE_TABLE_END()

