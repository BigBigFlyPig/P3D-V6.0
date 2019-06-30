#pragma once

#include <map>
#include <utility>

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

// The external sim client GUID, used to register with SimConnect
// {3E6B47E7-C706-4A10-BC88-6E7C199ED5A6}
static const GUID g_guidExternalSim = 
{ 0x3e6b47e7, 0xc706, 0x4a10, { 0xbc, 0x88, 0x6e, 0x7c, 0x19, 0x9e, 0xd5, 0xa6 } };

#define M_PI 3.1415926535897932384626433832795  // PI
#define MAX_LEFTRIGHT_SPEED 75    // meters/sec (roughly 168 MPH)
#define MAX_FRONTBACK_SPEED 75    // meters/sec (roughly 168 MPH)
#define MAX_UPDOWN_SPEED 100       // meters/sec (roughly 225 MPH)

#pragma region stuff to convert from delta meters to delta radians

#define EARTH_CIRCUMFERENCE_AROUND_EQUATOR_IN_METERS 40075000.0		// FlightSim standard value
#define EARTH_CIRCUMFERENCE_THROUGH_POLES_IN_METERS  40007000.0		// FlightSim standard value
#define EARTH_SEMI_MAJOR_RADIUS_IN_METERS (EARTH_CIRCUMFERENCE_AROUND_EQUATOR_IN_METERS / (M_PI * 2.0))
#define EARTH_SEMI_MINOR_RADIUS_IN_METERS (EARTH_CIRCUMFERENCE_THROUGH_POLES_IN_METERS  / (M_PI * 2.0))
#define LAT_METERS_TO_POLE      (EARTH_CIRCUMFERENCE_THROUGH_POLES_IN_METERS / 4)

#define RADIANS_TO_LAT_METERS(fDeltaLatRad)      ((fDeltaLatRad) * ( LAT_METERS_TO_POLE / (M_PI / 2.0) ) )

#define RADIANS_TO_LON_METERS(fDeltaLonRad, fRefLatRad)  (fDeltaLonRad *        \
                                                   (EARTH_CIRCUMFERENCE_AROUND_EQUATOR_IN_METERS / (M_PI * 2.0))  \
                                                   * cos(fRefLatRad))

#define LAT_METERS_TO_RADIANS(fDeltaLatMeters)     ((fDeltaLatMeters) * (1.0 / EARTH_SEMI_MINOR_RADIUS_IN_METERS))

#define LON_METERS_TO_RADIANS(fDeltaLonMeters, fRefLatRad)     ((fDeltaLonMeters) / RADIANS_TO_LON_METERS(1, fRefLatRad))

#pragma endregion

//-----------------------------------------------------------------------------
// Data Defintiions
//-----------------------------------------------------------------------------

#pragma pack(push,1)

struct PerVehicleSimulate
{
    double Lat;
    double Lon;
    double Alt;
    double Pitch;
    double Bank;
    double Heading;
    double GroundAlt;
    double BathAlt;
    DWORD eSurfaceType;
};

struct PerVehicleOutput
{
    double Lat;
    double Lon;
    double Alt;
    double Pitch;
    double Bank;
    double Heading;
    double BodyVelX;
    double BodyVelY;
    double BodyVelZ;
    double WorldVelX;
    double WorldVelY;
    double WorldVelZ;
    double VerticalSpeed;
};

#pragma pack(pop)

class VehicleData
{
public:
    VehicleData()
    {
        bReset = false;

        fMaxLeftRightVelocity = MAX_LEFTRIGHT_SPEED;
        fMaxFrontBackVelocity = MAX_FRONTBACK_SPEED;
        fMaxUpDownVelocity = MAX_UPDOWN_SPEED;

        fLeftRight = 0;
        fFrontBack = 0;
        fUpDown = 0;

        fLeftRightVelocity = 0;
        fFrontBackVelocity = 0;
        fUpDownVelocity = 0;

        GroundAltOffset = 10;
    };

    DWORD dwObjectID;

    bool bReset;

    // max velocity values in meters/sec
    double fMaxLeftRightVelocity;
    double fMaxFrontBackVelocity;
    double fMaxUpDownVelocity;

    // input based vars
    double fLeftRight;
    double fFrontBack;
    double fUpDown;

    // linear velocity in meters/sec
    double fLeftRightVelocity;
    double fFrontBackVelocity;
    double fUpDownVelocity;

    // fixed distance offset from Ground Alt in meters
    double GroundAltOffset;

    // object create data
    int cntSimVars;
    int idxSimVarsBase;
};

enum DATA_DEFINITIONS
{
    PER_VEHICLE_SIMULATE_DEFINITION,
    PER_VEHICLE_OUTPUT_DEFINITION,
};

enum EVENTS
{
    EVENT_NONE,

    MOVEMENT_CENTER_EVENT,

    LEFTRIGHT_INCREMENT_EVENT,
    LEFTRIGHT_DECREMENT_EVENT,
    LEFTRIGHT_SET_EVENT,

    FRONTBACK_INCREMENT_EVENT,
    FRONTBACK_DECREMENT_EVENT,
    FRONTBACK_SET_EVENT,

    UPDOWN_MAX_EVENT,
    UPDOWN_MIN_EVENT,
    UPDOWN_INCREMENT_EVENT_1,
    UPDOWN_INCREMENT_EVENT_2,
    UPDOWN_INCREMENT_EVENT_3,
    UPDOWN_DECREMENT_EVENT_1,
    UPDOWN_DECREMENT_EVENT_2,
    UPDOWN_DECREMENT_EVENT_3,
    UPDOWN_SET_EVENT,

    LEFTRIGHT_AXIS_EVENT,
    FRONTBACK_AXIS_EVENT,
    UPDOWN_AXIS_EVENT,
};

typedef std::map<DWORD, VehicleData*> VehicleList;
typedef std::pair<DWORD, VehicleData*> VehicleListPair;
typedef std::map<DWORD, VehicleData*>::iterator VehicleListIterator;

