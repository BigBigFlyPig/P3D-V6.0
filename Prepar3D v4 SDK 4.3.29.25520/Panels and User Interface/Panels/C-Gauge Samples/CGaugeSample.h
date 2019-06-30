//Copyright (c) Microsoft Corporation.  All rights reserved. 
// SDK.Attitude.h
//

#define         VERSION_MAJOR           1
#define         VERSION_MINOR           0
#define         VERSION_BUILD           0

// magic to get the preprocessor to do what we want
#define		lita(arg) #arg
#define		xlita(arg) lita(arg)
#define		cat3(w,x,z) w##.##x##.##z##\000
#define		xcat3(w,x,z) cat3(w,x,z)
#define		VERSION_STRING xlita(xcat3(VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD))

#ifndef		VS_VERSION_INFO
#define		VS_VERSION_INFO		0x0001
#endif


/////////////////////////////////////////////////////////////////////////////
//
// Attitude Bitmaps
//
#define		BMP_ATTITUDE_SMALL_BACKGROUND		0x1000
#define		BMP_ATTITUDE_SMALL_CARD1			0x1100
#define		BMP_ATTITUDE_SMALL_MASK1			0x1101
#define		BMP_ATTITUDE_SMALL_CARD2			0x1200
#define		BMP_ATTITUDE_SMALL_MASK2			0x1201

/////////////////////////////////////////////////////////////////////////////
//
// Control Surfaces Bitmaps
//
#define		BMP_CS_SMALL_BACKGROUND				0x1300
#define		BMP_CS_SMALL_TRIM					0x1400
#define		BMP_CS_SMALL_AILERONS				0x1500
#define		BMP_CS_SMALL_ELEVATOR				0x1600
#define		BMP_CS_SMALL_RUDDER					0x1700

/////////////////////////////////////////////////////////////////////////////
//
// Fuel Gauge Bitmaps
//
#define		BMP_FUEL_SMALL_BACKGROUND			0x1800
#define		BMP_FUEL_SMALL_NEEDLE				0x2000

/////////////////////////////////////////////////////////////////////////////
//
// Fuel Selector Bitmaps
//
#define		BMP_FUEL_SELECTOR_OFF				0x2100
#define		BMP_FUEL_SELECTOR_LEFT				0x2101
#define		BMP_FUEL_SELECTOR_BOTH				0x2102
#define		BMP_FUEL_SELECTOR_RIGHT				0x2103

/////////////////////////////////////////////////////////////////////////////
//
// Temperature Gauge Bitmaps
//
#define		BMP_TEMPERATURE_SMALL_BACKGROUND	0x2200
#define		BMP_TEMPERATURE_SMALL_F				0x2300
#define		BMP_TEMPERATURE_SMALL_C				0x2301

/////////////////////////////////////////////////////////////////////////////
//
// Whiskey CompassBitmaps
//
#define		BMP_COMPASS_SMALL_BACKGROUND		0x2400
#define		BMP_COMPASS_SMALL_CARD				0x2500
#define		BMP_COMPASS_SMALL_MASK				0x2501

/////////////////////////////////////////////////////////////////////////////
//
// FlightMap Bitmaps
//
#define		BMP_FLIGHT_MAP_BACKGROUND		0x2600
#define		BMP_FLIGHT_MAP_BLANK			0x2601


