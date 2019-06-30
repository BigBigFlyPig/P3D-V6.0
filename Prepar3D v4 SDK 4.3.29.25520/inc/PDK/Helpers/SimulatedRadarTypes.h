//-----------------------------------------------------------------------------
// Copyright (c) Lockheed Martin Corporation. All Rights Reserved
//-----------------------------------------------------------------------------
#pragma once

namespace Radar
{
	//----------------------------------------------------------------------------
	// Static Variables
	//----------------------------------------------------------------------------
	static const char RADAR_CALLBACK_NAME[] = ("P3DRadar");
	static const char RADAR_INTERFACE_NAME[] = ("ISimulatedRadar");

	//Convenience definitions of common structures used within radar system

	typedef double DEGREES;
	typedef double RADIANS;
	typedef double FEET;
	typedef double SECONDS;
	struct LLA
	{
	public:
		LLA() {}
		LLA(double lat, double lon, double alt) : Lat(lat), Lon(lon), Alt(alt) {}
		RADIANS Lat;
		RADIANS Lon;
		FEET Alt;
	};

	struct PBH
	{
	public:
		DEGREES Pitch;
		DEGREES Bank;
		DEGREES Heading;
	};
}