///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) Lockheed Martin Corporation 2017
// All rights reserved
//
// HmdSample.h
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "initpdk.h"
#include "PdkPlugin.h"
#include "gauges.h"
#include "PDK.h"
#include "ISimObjectAttachments.h"

using namespace P3D;

static const char HMD_SAMPLE_CALLBACK_NAME[] = "HmdSample";
const double RADIUS_OF_EARTH_AT_EQUATOR = 20925646.9; // in feet

#define PI                      3.141592653589793238462643
#define TWO_PI                  (2.0 * PI)
#define DEG_TO_RAD(x)           (float)((x) * PI / 180.0f)
#define RAD_TO_DEG(x)           (float)((x) * 180.0f / PI)
#define FEET_TO_METERS(feet)    (float)(feet * 0.304800)
#define METERS_TO_FEET(meters)  (float)(meters * 1/0.3048)

// Note: The items in the property table correspond to the indices that
// will be returned in the Get/Set Property functions
struct PROPERTY_TABLE
{
    LPCSTR szPropertyName;
    LPCSTR szUnitsName;
    ENUM   units;
};

class HmdPanelCallback : public IPanelCCallback
{
    DECLARE_PANEL_CALLBACK_REFCOUNT(PanelCallback);
public:
    HmdPanelCallback();

    // ******* IPanelCCallback Methods *****************    
    virtual IPanelCCallback* QueryInterface(LPCSTR pszInterface) override { return nullptr; }
    virtual UINT32 GetVersion() override { return 1; }
    virtual bool ConvertStringToProperty(LPCSTR keyword, SINT32 * pID) override;
    virtual bool ConvertPropertyToString(SINT32 id, LPCSTR* pKeyword) override;
    virtual bool GetPropertyUnits(SINT32 id, ENUM * pEnum) override;
    virtual IAircraftCCallback * CreateAircraftCCallback(UINT32 ContainerID) override;
protected:
    const PROPERTY_TABLE *GetPropertyTable(UINT &uLength);
};

class HmdAircraftCallback : public IAircraftCCallback
{
    DECLARE_PANEL_CALLBACK_REFCOUNT(AircraftCallback);
public:
    HmdAircraftCallback(UINT32 containerId);

    // ******* IAircraftCCallback Methods ************* 
    virtual  IAircraftCCallback* QueryInterface(LPCSTR pszInterface) override { return nullptr; }
    virtual void Update() override {}
    virtual IGaugeCCallback * CreateGaugeCCallback() override;
protected:
    UINT32 GetContainerId()  const { return m_containerId; }

private:
    UINT32 m_containerId;
};

//
// Class that implements IGaugeCCallback
//
class HmdGaugeCallback : public IGaugeCCallback
{
    DECLARE_PANEL_CALLBACK_REFCOUNT(P3DRADARGaugeCallback);

public:
    HmdGaugeCallback(UINT32 containerId);

    // ************* IGaugeCCallback Methods ***************
    bool GetPropertyValue(SINT32 id, FLOAT64* pValue);
    bool SetPropertyValue(SINT32 id, FLOAT64 value);
    IGaugeCCallback* QueryInterface(LPCSTR pszInterface);
    void Update();
    bool GetPropertyValue(SINT32 id, LPCSTR* pszValue) { return false; }
    bool SetPropertyValue(SINT32 id, LPCSTR szValue) { return false; }
    bool GetPropertyValue(SINT32 id, LPCWSTR* pszValue) { return false; }
    bool SetPropertyValue(SINT32 id, LPCWSTR szValue) { return false; }
    IGaugeCDrawable* CreateGaugeCDrawable(SINT32 id, const IGaugeCDrawableCreateParameters* pParameters) { return nullptr; }

protected:
    ~HmdGaugeCallback();

private:
    void UpdateVisibility();
    void UpdatePbh();
    void UpdateTarget();
    bool IsActive();
    void SetActive(bool bActive);

    float GetPitch();
    float GetBank();
    float GetHeading();

    float GetHFov();
    float GetVFov();
    float GetZoom();

    bool HasTarget();
    float GetTargetX();
    float GetTargetY();
    float GetTargetAlt();
    float GetTargetAspectAngle();
    float GetTargetDistance();

private:
    UINT32 m_containerId;
    bool m_bActive;
    float m_fCameraPitch;
    float m_fCameraBank;
    float m_fCameraHeading;
    float m_fHFov;
    float m_fVFov;
    float m_fZoom;
    bool m_bHasTarget;
    float m_fTargetX;
    float m_fTargetY;
    float m_fTargetAlt;
    float m_fTargetAspectAngle;
    float m_fTargetDistance;
    ObjectWorldTransform m_CameraTransform;
    ObjectWorldTransform m_TargetTransform;
};

// Enum that contains the properties 
enum HMD_VAR
{
    HmdActive,
    HmdPitch,
    HmdBank,
    HmdHeading,
    HmdHFov,
    HmdVFov,
    HmdZoom,
    HmdTarget,
    HmdTargetX,
    HmdTargetY,
    HmdTargetAlt,
    HmdTargetAspectAngle,
    HmdTargetDistance,
};
// table of poerprty info.  Must stay lined up with enum above
static PROPERTY_TABLE HMD_PROPERTY_TABLE[] =
{
    { "Active",             "Number", UNITS_NUMBER },
    { "Pitch",              "Number", UNITS_NUMBER },
    { "Bank",               "Number", UNITS_NUMBER },
    { "Heading",            "Number", UNITS_NUMBER },
    { "HFov",               "Number", UNITS_NUMBER },
    { "VFov",               "Number", UNITS_NUMBER },
    { "Zoom",               "Number", UNITS_NUMBER },
    { "Target",             "Number", UNITS_NUMBER },
    { "TargetX",            "Number", UNITS_NUMBER },
    { "TargetY",            "Number", UNITS_NUMBER },
    { "TargetAltitude",     "Number", UNITS_NUMBER },
    { "TargetAspectAngle",  "Number", UNITS_NUMBER },
    { "TargetDistance",     "Number", UNITS_NUMBER }
};
