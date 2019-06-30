/************************************************************************************************************** 
    ISimObject.Legacy.h

    Copyright (C) Lockheed Martin.  All rights reserved.
    
    Provides legacy interfaces for backward compatibility.  "0000" is the original base version from
    which all others are derived
***************************************************************************************************************/

#pragma once

#include "..\Types.h"

namespace P3D
{

interface IBaseObjectV400;
interface ISimulation;
interface IForceElement;
interface IMassElement;

class WorldConstants;
class WeatherInfoV400;
class SurfaceInfoV400;

/*****************  Add legacy interfaces here *************************************/

/** Handles tasks that are not associated with an instance of a simobject.  This includes:\n
* - Registration of implementation factories and associated properties\n
* - Global application properties (e.g. world constants)\n
* - Object queries */
DECLARE_INTERFACE_(ISimObjectManagerV400, IUnknown)
{
    /** Registers an ISimObject implementation at load time with: unique ID, friendly category name (e.g. "airplane"), and factory function pointer. 
    * The "pszCategoryName" is a high-level categorization used primarily for UI (e.g. "airplane"). Mainly, it is used as a filter to exclude 
    * objects from appearing in the Vehicle Select screen. If you create a unique category name, ensure you add the name to the User Objects key 
    * in the Prepar3D.cfg's [Main] section. */
    STDMETHOD (RegisterSimulationCategory) (__in GUID guidCategory, __in LPCWSTR pszCategoryName, __in __notnull PSimCreateFunc pcbCreateFunction) PURE;

    /** Property "simvar" and "event" registrations. For a specific simobject implementation (guid), associates: property string name, units, and callback pointer (defined above) @note Input: Double */
    STDMETHOD (RegisterProperty) (__in GUID guidCategory, __in LPCWSTR pszPropertyName, __in LPCWSTR pszPropertyBaseUnits, __in __notnull PPropertyCallback       pcbProperty) PURE;
    /** Property "simvar" and "event" registrations. For a specific simobject implementation (guid), associates: property string name, units, and callback pointer (defined above) @note Input: Vector (DXYZ) */
    STDMETHOD (RegisterProperty) (__in GUID guidCategory, __in LPCWSTR pszPropertyName, __in LPCWSTR pszPropertyBaseUnits, __in __notnull PPropertyVectorCallback pcbProperty) PURE;
    /** Property "simvar" and "event" registrations. For a specific simobject implementation (guid), associates: property string name, units, and callback pointer (defined above) @note Input: String */
    STDMETHOD (RegisterProperty) (__in GUID guidCategory, __in LPCWSTR pszPropertyName, __in __notnull PPropertyStringCallback pcbProperty)                                   PURE;
    /** Property "simvar" and "event" registrations. For a specific simobject implementation (guid), associates: property string name, units, and callback pointer (defined above) @note Input: Double (with secondary substring input) */
    STDMETHOD (RegisterProperty) (__in GUID guidCategory, __in LPCWSTR pszPropertyName, __in LPCWSTR pszPropertyBaseUnits, __in __notnull PPropertyCallbackWithSubString    pcbProperty) PURE;
    /** Property "simvar" and "event" registrations. For a specific simobject implementation (guid), associates: property string name, units, and callback pointer (defined above) @note Input: Event */
    STDMETHOD (RegisterProperty) (__in GUID guidCategory, __in LPCWSTR pszPropertyName, __in LPCWSTR pszPropertyBaseUnits, __in __notnull PEventCallback          pcbEvent, __in EVENTTYPE eType)    PURE;
    /** Property "simvar" and "event" registrations. For a specific simobject implementation (guid), associates: property string name, units, and callback pointer (defined above) @note Input: Event vector */
    STDMETHOD (RegisterProperty) (__in GUID guidCategory, __in LPCWSTR pszPropertyName, __in LPCWSTR pszPropertyBaseUnits, __in __notnull PEventVectorCallback)    PURE;
    /** Property "simvar" and "event" registrations. For a specific simobject implementation (guid), associates: property string name, units, and callback pointer (defined above) @note Input: Event string */
    STDMETHOD (RegisterProperty) (__in GUID guidCategory, __in LPCWSTR pszPropertyName, __in __notnull PEventStringCallback)    PURE;
    /** Property "simvar" and "event" registrations. For a specific simobject implementation (guid), associates: property string name, units, and callback pointer (defined above) @note Input: Event double (with secondary substring input) */
    STDMETHOD (RegisterProperty) (__in GUID guidCategory, __in LPCWSTR pszPropertyName, __in LPCWSTR pszPropertyBaseUnits, __in __notnull PEventCallbackWithSubString  pcbEvent)    PURE;

    /** World Constants are constant values describing the Earth atmosphere and geometry.\n
    * NOTE: While this is accessed through IBaseObject, these values will be constant for all SimObjects, and a single static copy could be shared across multiple instances. */
    STDMETHOD (GetWorldConstants)(__out WorldConstants&) const PURE;
    
    /** Decodes a string units to its integer ID. This can be useful to get at initialization
    * as it is less performanct to query properties using the string version. e.g. "feet per second" to ID. */
    STDMETHOD (GetUnitCode)(__in LPCWSTR pszPropertyUnits, __out int& iUnitCode) const PURE;

    /** Gets another IBaseObject ref for a given ID */
    STDMETHOD (GetObject)(__in UINT idObject, __out IBaseObjectV400** ppObject) const PURE;

    /** Gets an IBaseObject ref for the current user object.\n
    NOTE: If the user object is the Viewer and there is a previous user object, this will return the previous user object. Otherwise, it will return the Viewer. */
    STDMETHOD (GetUserObject)(__out IBaseObjectV400** ppUserObject) const PURE;

    /** Used to register a callback function that is called upon creation of any new object. See types.h for callback definition. */
    STDMETHOD (RegisterOnObjectCreateCallback) (__in __notnull POnObjectCreateCallback  pCb)    PURE;
    /** Used to register a callback function that is called upon destruction of any existing object. The call is just prior to destruction. See types.h for callback definition. */
    STDMETHOD (RegisterOnObjectRemoveCallback) (__in __notnull POnObjectRemoveCallback  pCb)    PURE;
    /** Used to register a callback function that is called whenever the user is moved from one object to another. See types.h for callback definition. */
    STDMETHOD (RegisterOnUserObjectChangedCallback) (__in __notnull POnUserObjectChangedCallback  pCb)    PURE;

    /** Returns a list of object IDs for a given radius.\n
    * @param nObjects IN:  the max number of elements requested.  This must be no smaller than the size of the array pointed to by rgObjectIDs\n
    * @param nObjects OUT: the actual number of objects found\n
    * @param rgObjectIDs address of array in which object IDs are returned\n
    * NOTE: It is the callers responsibility to allocate the array's required memory */
    STDMETHOD (GetObjectsInRadius)(__in const DXYZ& vLonAltLat, __in float fRadiusFeet, __inout UINT& nObjects, __out UINT* rgObjectIDs) const PURE;

    /** Returns a list of object IDs for a given radius.  Does not include traffic\n
    * @param nObjects IN:  The max number of elements requested.  This must be no smaller than the size of the array pointed to by rgObjectIDs\n
    * @param nObjects OUT: The actual number of objects found\n
    * @param rgObjectIDs Address of array in which object IDs are returned.\n
    * NOTE: It is the callers responsibility to allocate the array's required memory. */
    STDMETHOD(GetNonTrafficObjectsInRadius)(__in const DXYZ& vLonAltLat, __in float fRadiusFeet, __inout UINT& nObjects, __out UINT* rgObjectIDs) const PURE;

    /**The user-selected general realism scalar, where 0.0 is "easy" and 1.0 is "real". This can be used to scale your implementation as appropriate*/
    STDMETHOD_(float, GetRealismSetting)()            const PURE;     //Percent
    /**The user-selected flag that dictates whether to process a crash or not*/
    STDMETHOD_(BOOL,  IsCrashDetectionOn)()           const PURE;
    /**The user-selected flag for whether to detect crashes between simobjects or not*/
    STDMETHOD_(BOOL,  IsCollisionBetweenObjectsOn)()  const PURE;
    /**The user-selected scalar for determining crash tolerance*/
    STDMETHOD_(float, GetCrashToleranceScalar)()      const PURE;

    /** RemoveObject - Remove any local non-user object 
    * @param idObject The id of the object to remove. */
    STDMETHOD (RemoveObject)(__in UINT idObject) PURE;
};

DEFINE_GUID(IID_ISimObjectManagerV400,  0x8a645399, 0x229d, 0x4e7c, 0x8d, 0xef, 0x5b, 0x9, 0x12, 0x81, 0xf, 0x10);

/** Handles tasks that are not associated with an instance of a simobject.  This includes:\n
* - Registration of implementation factories and associated properties\n
* - Global application properties (e.g. world constants)\n
* - Object queries */
DECLARE_INTERFACE_(ISimObjectManagerV410, ISimObjectManagerV400)
{
    /** Attempts to create an object with the given container title.
    * @param pszTitle   The container title object to be created.
    * @param idObject   The object id of the newly created object.
    * @return           S_OK if the object was successfully created, E_FAIL otherwise.
    */
    STDMETHOD(CreateObject)(__in __notnull LPCWSTR pszTitle, __out UINT& idObject) PURE;
};

DEFINE_GUID(IID_ISimObjectManagerV410, 0x4ee8eb41, 0x3cc6, 0x4d2f, 0xb0, 0xc7, 0xb6, 0x38, 0x87, 0x29, 0x8e, 0xcd);

/*************************************************************************************************
*************************************************************************************************/
DECLARE_INTERFACE_ (ISimObject, IServiceProvider)
{
    STDMETHOD (LoadConstantData)(__inout void** ppConstantData)     PURE;       //on-disk data.  return loaded data
    STDMETHOD (UnloadConstantData)(__inout void** ppConstantData)   PURE;       //on-disk data.  should point to loaded data, and return NULL
    STDMETHOD (LoadDynamicData)()                                   PURE;       //e.g. create runtime subsystems based on disk data
    STDMETHOD (Init)()                                              PURE;       //e.g. init relationships between subsystems
    STDMETHOD (DeInit)()                                            PURE;       //e.g. release references between subsystems
    STDMETHOD_(BOOL, SupportsLabels)()                      const   PURE;       //Does this simobject support label display?
    STDMETHOD_(void, OnModeChange)(int bfNewModes)                  PURE;       //Called upon change in modes (pause, slew, etc...)
    STDMETHOD_(void, OnPositionInit)()                              PURE;       //Called upon change in non-realtime object positions (e.g. UI) call GetPosition() to get new position
    STDMETHOD_(void, OnSpeedInit)(float fSpeed)                     PURE;       //Called upon change in speed   (e.g. UI)
};

DEFINE_GUID(IID_ISimObject, 0x5c467f71, 0x3817, 0x4fff, 0x95, 0xa8, 0x44, 0x7c, 0x2b, 0xb5, 0xeb, 0x56);

DECLARE_INTERFACE_ (ISimObjectV01, ISimObject)
{
    //Identical to ISimObject
};

DEFINE_GUID(IID_ISimObjectV01,  0xa5177ac6, 0x84a8, 0x494b, 0xa2, 0xfc, 0xe2, 0x9f, 0x6d, 0xdf, 0xa7, 0x87);

DECLARE_INTERFACE_ (ISimObjectV02, ISimObjectV01)
{
    STDMETHOD (SetSupportsLabels)(BOOL bOn)                         PURE;       //Set if this simobject should support label display?
};

DEFINE_GUID(IID_ISimObjectV02,  0xc4c4f969, 0xe2c7, 0x48cb, 0xab, 0xfe, 0x53, 0x3, 0xd, 0xa2, 0xb0, 0xe8);

DECLARE_INTERFACE_(ISimObjectV03, ISimObjectV02)
{
    STDMETHOD(QueryBaseObject)(REFIID riid, void** ppv)     PURE;       //Accessor to get the base object from th ISimObject
};

DEFINE_GUID(IID_ISimObjectV03, 0xef0dda2e, 0x3d35, 0x4b4b, 0xb1, 0x12, 0xfa, 0x5c, 0xb, 0xe4, 0x84, 0x10);

/*************************************************************************************************
*************************************************************************************************/
DECLARE_INTERFACE_ (ISimulation, IUnknown)
{
    //Real-time "simulate" call
    STDMETHOD (Update)(double dDeltaT)           PURE;    
    //State saving / loading
    STDMETHOD (SaveLoadState)(__in __notnull PSaveLoadCallback pfnCallback, __in const BOOL bSave) PURE;  
};

DEFINE_GUID(IID_ISimulation, 0x5612b28f, 0x4f7, 0x4090, 0xab, 0x84, 0xb1, 0x86, 0xa3, 0x6, 0x1, 0x63);

DECLARE_INTERFACE_(ISimulationV01, ISimulation)
{
    //Identical to ISimulation
};

DEFINE_GUID(IID_ISimulationV01, 0xe950867e, 0xfb49, 0x4d0f, 0x83, 0x31, 0xe8, 0x9, 0x17, 0x50, 0x9c, 0x35);

/** Object interface on the host side for providing platform information and services for the object */
DECLARE_INTERFACE_ (IBaseObjectV400, IServiceProvider)
{
    /** The ID of this object:  NOTE: "0" is an invalid id */
    STDMETHOD_(UINT, GetId)() const PURE;

    /** The guid ID of the object defined in an object file. NOTE: If the object is not spawned by a scenario, the return will be E_FAIL and the ID will be GUID_NULL. */
    STDMETHOD(GetMissionId)(__out GUID& guid) const PURE;
    /** Returns if the object is the user or not. */
    STDMETHOD_(BOOL, IsUser)() const PURE;

    /** Group Association ID can be used to set/get IDs for Friend/Foe or other types of groupings.\n
    * Note: Group association is arbitrary. It could be used for things like alliances or squadrons. Default is 0, which signifies a neutral grouping. */
    STDMETHOD_(UINT, GetObjectGroupAssociationId)()                      const PURE;
    /** Group Association ID can be used to set/get IDs for Friend/Foe or other types of groupings.\n
    * Note: Group association is arbitrary. It could be used for things like alliances or squadrons. Default is 0, which signifies a neutral grouping. */
    STDMETHOD_(void, SetObjectGroupAssociationId)(UINT uAssociationId)         PURE;

    /** Returns bitwise flags for the current modes of the SimObject. */
    STDMETHOD_(int, GetMode)() const PURE;
    /** Should be called when it is desired to put Prepar3D into "crash" mode. By default, the application will go through it's 
    * crash cycle and reset. It is the developer's responsibility to program the behavior of the object when crash in crash mode. */
    STDMETHOD (SetCrashMode)(double dDeltaT)  PURE;

    /** Gets the current world relative position and velocity from the Prepar3D-side SimObject. This will provide the 
    * valid state upon initialization, as well as when another system such as a UI element or slew changes the position. 
    * @param vLonAltLat Longitude, altitude, latitude (radians) 
    * @param vPHB Pitch, heading, bank (radians) 
    * @param vLonAltLatVel Longitude, altitude, latitude velocity (feet / second) 
    * @param vPHBVel Pitch, heading, bank velocity (radians / second) */
    STDMETHOD (GetPosition)(__out      DXYZ& vLonAltLat, __out      DXYZ& vPHB, __out      DXYZ& vLonAltLatVel, __out      DXYZ& vPHBVel) const PURE;
    /** Sets the current world relative position and velocity to the Prepar3D-side SimObject. 
    * @param vLonAltLat Longitude, altitude, latitude (radians) 
    * @param vPHB Pitch, heading, bank (radians) 
    * @param vLonAltLatVel Longitude, altitude, latitude velocity (feet / second) 
    * @param vPHBVel Pitch, heading, bank velocity (radians / second) 
    * @param bIsOnGround Flag indicating if the object is on the ground. This is important during terrain updates. 
    * @param dDeltaT The time between object updates used to track how much time has accumulated between camera frames */
    STDMETHOD (SetPosition)(__in const DXYZ& vLonAltLat, __in const DXYZ& vPHB, __in const DXYZ& vLonAltLatVel, __in const DXYZ& vPHBVel, __in BOOL bIsOnGround, __in double dDeltaT) PURE;
    /** IsOnGround Returns the on-ground flag value currently in the core base object. This can be useful for determining 
    * if the object has been placed on the ground through a non-simulated means such as the UI. */
    STDMETHOD_(BOOL, IsOnGround)() const PURE;

    /** Rotates a vector from the world frame of reference to the body frame of reference. */
    STDMETHOD (RotateWorldToBody)(__in const DXYZ& vWorld, __out DXYZ& vBody)  const PURE;
    /** Rotates a vector from the body frame of reference to the world frame of reference. */
    STDMETHOD (RotateBodyToWorld)(__in const DXYZ& vBody,  __out DXYZ& vWorld) const PURE;

    /** Registers an ISimulation callback for real-time updates (discussed in Creating Behaviors.) ISimulation registration will 
    * be locked after the ISimObject Init() function has been called. All ISimulation objects must be registered before this point. 
    * @param pSimulation Address of simulation system 
    * @param fRateHz Specified iteration rate */
    STDMETHOD (RegisterSimulation)(__in __notnull ISimulation* pSimulation, float fRateHz) PURE;

    /** Registers a service that can be queried for on this object. A service should be an IUnknown-derived object and registered with a unique GUID. 
    * @param guidService Unique GUID to identify this service. 
    * @param punkService Reference to an instance of this service. */
    STDMETHOD (RegisterService)(__in REFGUID guidService, __in __notnull IUnknown* punkService) PURE;
    /** Removes a service that has been register with RegisterService(). 
    * @param guidService Unique GUID to identify this service. */
    STDMETHOD (UnregisterService)(__in REFGUID guidService) PURE;

    STDMETHOD (GetPropertyCodeAndIndex)(__in PROPERTY_TYPE eType, __in LPCWSTR pszPropertyName, __out int& iPropertyCode, __inout int& iIndex) const PURE; /**< Get Properties */
    
    STDMETHOD (GetProperty)(__in int     iPropertyCode,   __in int      iUnitCode,   __out double& dProperty, __in int index = 0) const PURE;   /**< Get Property - Doubles */
    STDMETHOD (GetProperty)(__in LPCWSTR pszPropertyName, __in int      iUnitCode,   __out double& dProperty, __in int index = 0) const PURE;   /**< Get Property - Doubles */
    STDMETHOD (GetProperty)(__in LPCWSTR pszPropertyName, __in LPCWSTR  pszUnitCode, __out double& dProperty, __in int index = 0) const PURE;   /**< Get Property - Doubles */
    
    STDMETHOD (GetProperty)(__in int     iPropertyCode,   __in int      iUnitCode,   __out DXYZ&   dProperty, __in int index = 0) const PURE;   /**< Get Property - Vectors */
    STDMETHOD (GetProperty)(__in LPCWSTR pszPropertyName, __in int      iUnitCode,   __out DXYZ&   dProperty, __in int index = 0) const PURE;   /**< Get Property - Vectors */
    STDMETHOD (GetProperty)(__in LPCWSTR pszPropertyName, __in LPCWSTR  pszUnitCode, __out DXYZ&   dProperty, __in int index = 0) const PURE;   /**< Get Property - Vectors */
    
    STDMETHOD (GetProperty)(__in int     iPropertyCode,   __out LPWSTR pszProperty, __in UINT uLength,       __in int index = 0) const PURE;   /**< Get Property - Strings */
    STDMETHOD (GetProperty)(__in LPCWSTR pszPropertyName, __out LPWSTR pszProperty, __in UINT uLength,       __in int index = 0) const PURE;   /**< Get Property - Strings */
    /** Get Property - Doubles (with secondary substring input) */
    STDMETHOD (GetProperty)(__in int     iPropertyCode,   __in LPCWSTR pszSecondarySubstring, __in int      iUnitCode,   __out double& dProperty, __in int index = 0) const PURE;
    /** Get Property - Doubles (with secondary substring input) */
    STDMETHOD (GetProperty)(__in LPCWSTR pszPropertyName, __in LPCWSTR pszSecondarySubstring, __in int      iUnitCode,   __out double& dProperty, __in int index = 0) const PURE;
    /** Get Property - Doubles (with secondary substring input) */
    STDMETHOD (GetProperty)(__in LPCWSTR pszPropertyName, __in LPCWSTR pszSecondarySubstring, __in LPCWSTR  pszUnitCode, __out double& dProperty, __in int index = 0) const PURE;

    STDMETHOD (TriggerProperty)(__in  int     iPropertyCode,    __in int iUnitCode,         __in double dData,   __in  int  index) const PURE;  /**< Numeric trigger */
    STDMETHOD (TriggerProperty)(__in  LPCWSTR pszPropertyName,  __in int iUnitCode,         __in double dData,   __in  int  index) const PURE;  /**< Numeric trigger */
    STDMETHOD (TriggerProperty)(__in  LPCWSTR pszPropertyName,  __in LPCWSTR  pszUnitCode,  __in double dData,   __in  int  index) const PURE;  /**< Numeric trigger */
    
    STDMETHOD (TriggerProperty)(__in  int     iPropertyCode,   __in int iUnitCode,         __in const DXYZ&   vData, __in  int  index) const PURE;  /**< Vector trigger */
    STDMETHOD (TriggerProperty)(__in  LPCWSTR pszPropertyName, __in int iUnitCode,         __in const DXYZ&   vData, __in  int  index) const PURE;  /**< Vector trigger */
    STDMETHOD (TriggerProperty)(__in  LPCWSTR pszPropertyName, __in LPCWSTR  pszUnitCode,  __in const DXYZ&   vData, __in  int  index) const PURE;  /**< Vector trigger */
    
    STDMETHOD (TriggerProperty)(__in int     iPropertyCode,   __in LPCWSTR pszData, __in  int  index) const PURE;   /**< String strigger */
    STDMETHOD (TriggerProperty)(__in LPCWSTR pszPropertyName, __in LPCWSTR pszData, __in  int  index) const PURE;   /**< String strigger */
    /** Trigger - Doubles (with secondary substring input) */
    STDMETHOD (TriggerProperty)(__in  int       iPropertyCode, __in LPCWSTR pszSecondarySubstring, __in int iUnitCode,         __in double dData, __in  int  index) const PURE; 
    /** Trigger - Doubles (with secondary substring input) */
    STDMETHOD (TriggerProperty)(__in  LPCWSTR pszPropertyName, __in LPCWSTR pszSecondarySubstring, __in int iUnitCode,         __in double dData, __in  int  index) const PURE;
    /** Trigger - Doubles (with secondary substring input) */
    STDMETHOD (TriggerProperty)(__in  LPCWSTR pszPropertyName, __in LPCWSTR pszSecondarySubstring, __in LPCWSTR  pszUnitCode,  __in double dData, __in  int  index) const PURE;

    /** Registers a simulation property for this instance of the object. This differs from the RegisterProperty method in the ISimObjectManager 
    * interface which registers a property that exists for all instances associated with the given guid. Properties registered here, as well as their ID, exist 
    * only for the lifetime of this object. Otherwise they function the same when used for gauges, animations, and missions. */
    STDMETHOD (RegisterProperty) (__in LPCWSTR pszPropertyName, __in LPCWSTR pszPropertyBaseUnits, __in __notnull PPropertyCallback       pcbProperty)                      PURE;   /**< @note Double "get" */
    STDMETHOD (RegisterProperty) (__in LPCWSTR pszPropertyName, __in LPCWSTR pszPropertyBaseUnits, __in __notnull PEventCallback          pcbEvent, __in EVENTTYPE eType)   PURE;   /**< @note Double "set" */
    STDMETHOD (RegisterProperty) (__in LPCWSTR pszPropertyName, __in LPCWSTR pszPropertyBaseUnits, __in __notnull PPropertyVectorCallback pcbProperty)                      PURE;   /**< @note Vector "get" */
    STDMETHOD (RegisterProperty) (__in LPCWSTR pszPropertyName, __in LPCWSTR pszPropertyBaseUnits, __in __notnull PEventVectorCallback)                                     PURE;   /**< @note Vector "set" */
    STDMETHOD (RegisterProperty) (__in LPCWSTR pszPropertyName, __in __notnull PPropertyStringCallback pcbProperty)                                                         PURE;   /**< @note String "get" */
    STDMETHOD (RegisterProperty) (__in LPCWSTR pszPropertyName, __in __notnull PEventStringCallback)                                                                        PURE;   /**< @note String "set" */

    // System malfunctions.  The successful handle (greater than 0) returned on registration is used to later reference health (0 - 1.0).\n
    // e.g. pszName would include index, such as Engine 1 */
    /** Registers a specific malfunction that can be set through the UI, scenarios, or missions. 
    * @param guidMalfunction Unique malfunction ID 
    * @param pszType UI Type. Choices: Instruments, Systems, Radios, Engines, Controls, Structural, Miscellaneous 
    * @param pszBaseName Name used for mission file reference. Should be generic (no index), such as "Engine" 
    * @param pszInstanceName Specific malfunction name for the UI, such as "Total Failure Engine 1" 
    * @param nSubIndex Sub-index. For example, engine 0, 1, etc... */
    STDMETHOD_(HANDLE, RegisterSystemMalfunction)(__in REFGUID guidMalfunction, __in LPCWSTR pszType,  __in LPCWSTR pszBaseName, __in LPCWSTR pszInstanceName, __in int nSubIndex) PURE;
    /** Returns the health percentage (0.0 - 1.0) for a given malfunction. */
    STDMETHOD_(float,  GetSystemHealth)(HANDLE hSystem) const PURE;

    /** Apply damage points.  Positive points passed in will be decremented from current health points, to a limit of zero. */
    STDMETHOD (DecrementHealthPoints)(__in float fDamagePoints)      PURE;
    /** Retrieves the current health of the object. */
    STDMETHOD_(float, GetHealthPoints)()                       const PURE;
    /** Sets current health of the object. */
    STDMETHOD_(void, SetHealthPoints)(float fHealthPoints)     PURE;

    /** Provides current surface information for the requested offset from the model center. See the ISimObject.h for the definition of the SurfaceInfo data 
    * structure. A return value of E_FAIL means that Prepar3D's terrain system failed to process the request properly. This could happen if it is not initialized fully.
    * @param SurfaceInfo Reference to local SurfaceInfo data structure
    * @param pvOffsetFeet The offset from model enter.A value of NULL will use the model's center */
    STDMETHOD (GetSurfaceInformation)(__out SurfaceInfoV400& SurfaceInfo, __in const FXYZ* pvOffsetFeet) PURE;  //Pass in NULL for center of object
    /** Provides current surface elevation (above Mean Sea Level) for the requested offset from the model center. This will be more efficient than 
    * GetSurfaceInformation when only the elevation is needed. A return value of E_FAIL means that Prepar3D's terrain system failed to process the 
    * request properly. This could happen if it is not initialized fully. 
    * @param fElevationFeet Reference to the elevation variable (Feet) 
    * @param pvOffsetFeet The offset from model enter. A value of NULL will use the model's center */
    STDMETHOD(GetSurfaceElevation)(__out float& fElevationFeet, __in const FXYZ* pvOffsetFeet) PURE;       //Pass in NULL for center of object 
    /** Provides current depth for the requested offset from the model center.
    * @param fDepthFeet Reference to the depth variable (Feet)
    * @param pvOffsetFeet The offset from model enter. A value of NULL will use the model's center */
    STDMETHOD(GetBathymetryElevation)(__out float& fDepthFeet,  __in const FXYZ* pvOffsetFeet) PURE;    //Pass in NULL for center of object 

    /** Provides current weather information for the object's current position. See the ISimObject.h for the definition of the WeatherInfo data structure. 
    * A return value of E_FAIL means that Prepar3D's weather system failed to process the request properly. This could happen if it is not initialized fully. 
    * @param WeatherInfo Reference to local WeatherInfo data structure */
    STDMETHOD (GetWeatherInformation)(__out WeatherInfoV400& WeatherInfo) PURE;
    /** Returns the object's current magnetic variation in radians. A positive is value is "east". */
    STDMETHOD_(float, GetMagneticVariation)() const PURE;

    /** Turns a visual effect on. The out parameter allows you to hold a reference to a visual effect to subsequently turn off. 
    * @param pszEffectName File name for requested visual effect 
    * @param pvOffsetFeet The offset from model center. A value of NULL will use the model's center 
    * @param ppEffect Reference pointer for turning the visual effect off with VisualEffectOff() */
    STDMETHOD (VisualEffectOn) (__in __notnull LPCWSTR pszEffectName, __in const FXYZ* pvOffsetFeet, __out void** ppEffect) PURE;
    /** Turns a visual effect off. 
    * @param pEffect Reference pointer obtained in out parameter of VisualEffectOn() */
    STDMETHOD (VisualEffectOff)(__in __notnull void* pEffect) PURE;

    /** Triggers a sound configured in the object's sound.cfg. 
    * @param pszName Sound reference name from Sound.cfg */
    STDMETHOD (TriggerSound)(__in __notnull LPCWSTR pszName, BOOL bOn) PURE;
    /** Triggers a sound specifically for a ground contact point. 
    * @param pszName Sound reference name from Sound.cfg
    * @param  pvOffset, The offset from model center. A value of NULL will use the model's center 
    * @param fImpactSpeed Speed used by sound system to scale sound */
    STDMETHOD (TriggerContactSound)(__in __notnull LPCWSTR pszName, __in const FXYZ* pvOffset, float fImpactSpeed /*, surface type*/) PURE;

    
    //Proffered services - A simulation service that is made available by Prepar3D
    //for use in your simobject implementation.  The SDK will provide any available services

    /** Invokes the loading of the relevant constant data. This should be called from your SimObject's LoadConstantData(); */
    STDMETHOD (LoadServiceConstantData)  (__in REFGUID guidService)                 PURE;   
    /** Causes Prepar3D to unload the relevant constant data. This should be called from your SimObject's UnLoadConstantData(); */
    STDMETHOD (UnloadServiceConstantData)(__in REFGUID guidService)                 PURE;
    /** Invokes the instantiation of the service, based on the loaded constant data. This should be called from your SimObject's LoadDynamicData(); */
    STDMETHOD (CreateServiceInstance)    (__in REFGUID guidService)                 PURE;
    /** Causes Prepar3D to destroy the instance of the service. This should be called from your SimObject's Deinit(); */
    STDMETHOD (DestroyServiceInstance)   (__in REFGUID guidService)                 PURE;
    /** The real-time update of the service. Your SimObject is responsible for calling it with an accurate delta time. */
    STDMETHOD (UpdateServiceInstance)    (__in REFGUID guidService, double dDeltaT) PURE;

    
    //Config items refer to the configuration of the content "container".
    //Title   is unique string identifier defined in sim.cfg
    //CfgFile is the sim.cfg or aircraft.cfg file path
    //CfgDir  is the top level folder path for the content
    //GetCfgSectionName is the name of the section loaded.  For example [FLTSIM.1]
    //NOTE:  A minimum length of 260 is required for a fully qualified path

    /** Returns the unique string that identifies this object. */
    STDMETHOD (GetTitle)         (__out LPWSTR pszCfgTitle,__in unsigned int uLength)   const PURE;
    /** Returns a fully qualified path to the object's content path. This is generally the folder where the sim.cfg (or aircraft.cfg) lives. */
    STDMETHOD (GetCfgDir)        (__out LPWSTR pszCfgDir,  __in unsigned int uLength)   const PURE;
    /** Returns a fully qualified path to the sim.cfg file. */
    STDMETHOD (GetCfgFilePath)   (__out LPWSTR pszCfgFile, __in unsigned int uLength)   const PURE;
    /** Returns the relevant section name in the sim.cfg. e.g. [fltsim.1]. */
    STDMETHOD (GetCfgSectionName)(__out LPWSTR pszCfgFile, __in unsigned int uLength)   const PURE;

    /** Destroy self.  This will not be immediate, so it can be called from within 
    * itself.  It will be destroyed as soon as the current simulation finishes. */
    STDMETHOD (Destroy)()     PURE;

    /** Check for this object colliding with building or other simobject.  
    * Note: A return of S_OK does not mean there is a collision, only that the query operation encountered no errors
    * The eCollision should be checked for a positive collision, and ppUnkHitObject for whether it involved an (IUnknown) object */
    STDMETHOD (CheckCollision)(float fRadiusFeet, __out COLLISIONTYPE& eCollision,__out IUnknown** ppUnkHitObject) const PURE;

    /** Returns the current network mode for this object. */
    STDMETHOD_(NET_MODE_TYPE, GetNetworkMode)() const PURE;
};

DEFINE_GUID(IID_IBaseObjectV400, 0x65ba5c17, 0x6e25, 0x4d2f, 0x92, 0x6c, 0x8, 0x4e, 0x5b, 0x98, 0x76, 0xaf);

/** Object interface on the host side for providing platform information and services for the object */
DECLARE_INTERFACE_(IBaseObjectV410, IBaseObjectV400)
{

    /** Attaches the given object via offsets.
    * @param vOffsetFeetParent      The offset in feet from the parent model center.
    * @param vOffsetRadiansParent   The orientation offset in radians from the parent model center.
    * @param idChild                The child object id to be attached to the parent.
    * @param vOffsetFeetChild       The offset in feet from the parent attach point.
    * @param vOffsetRadiansChild    The orientation offset in radians from the parent attach point.
    * @return                       S_OK if the objects were successfully attached, E_FAIL otherwise.
    */
    STDMETHOD(AttachObject)(__in const DXYZ& vOffsetFeetParent, __in const DXYZ& vOffsetRadiansParent,
        __in UINT idChild, __in const DXYZ& vOffsetFeetChild, __in const DXYZ& vOffsetRadiansChild) PURE;

    /** Attaches the given object via attach point name and offsets.
    * @param pszAttachPointName     The name of the parent attach point.
    * @param vOffsetRadiansParent   The orientation offset in radians from the parent attach point.
    * @param idChild                The child object id to be attached to the parent attach point.
    * @param vOffsetFeetChild       The offset in feet from the parent attach point.
    * @param vOffsetRadiansChild    The orientation offset in radians from the parent attach point.
    * @return                       S_OK if the objects were successfully attached, E_FAIL otherwise.
    */
    STDMETHOD(AttachObject)(__in LPCSTR pszAttachPointName, __in const DXYZ& vOffsetRadiansParent,
        __in UINT idChild, __in const DXYZ& vOffsetFeetChild, __in const DXYZ& vOffsetRadiansChild) PURE;

    /** Detaches the given object.
    * @param idChild    The child object id to be detached from the parent.
    * @return           S_OK if the object was successfully detached,
    *                   S_FALSE if the object to be removed was not attached to the parent,
    *                   E_FAIL otherwise.
    */
    STDMETHOD(DetachObject)(__in UINT idChild) PURE;
};

DEFINE_GUID(IID_IBaseObjectV410, 0x474cdb85, 0x5288, 0x45e6, 0x89, 0x7, 0x2e, 0x3b, 0xb, 0xcf, 0xac, 0x1f);

/** Provides the current scenario (also known as "flight") and mission files. */
DECLARE_INTERFACE_(IScenarioManagerV400, IUnknown)
{
    /** Provides the fully qualified path to the scenario file. */
    STDMETHOD(GetScenarioFilePath)(__out LPWSTR pszFilePath, __in UINT uLength) const    PURE;
    /** Provides the fully qualified path to the associated object file. If one does not exist a zero-length string is returned. */
    STDMETHOD(GetObjectFilePath) (__out LPWSTR pszFilePath, __in UINT uLength)  const    PURE;
    /** Provides the file name of the associated object file. If one does not exist a zero-length string is returned. */
    STDMETHOD(GetObjectFileName) (__out LPWSTR pszFileName, __in UINT uLength)  const    PURE;
    /** Loads the specified scenario. */
    STDMETHOD(LoadScenario)(__in LPWSTR pszFilePath)                               const    PURE;

    //  Function callback registration for notification that a new mission or scenario (also known as a "flight") is loaded or saved.
    /** This allows registering a callback function for notification when the current scenario has been loaded or changed. See Types.h for the function profile of PNewScenarioNotify. */
    STDMETHOD(RegisterForNewScenarioNotification)(__in PNewScenarioNotify pcbNotifyFunction)      PURE;
};

DEFINE_GUID(IID_IScenarioManagerV400, 0xa2207a3b, 0x5ea1, 0x4187, 0xac, 0xca, 0x2f, 0xc1, 0xa9, 0xa1, 0xf4, 0xa3);

/**********************************************************************************************************
**********************************************************************************************************/
DECLARE_INTERFACE_ (IMassProperties, IUnknown)
{
    STDMETHOD_ (float,  GetWeight())                                        const PURE;
    STDMETHOD_ (BOOL,   RegisterMass(__in const IMassElement* pElement))          PURE;
    STDMETHOD_ (BOOL,   UnRegisterMass(__in const IMassElement* pElement))        PURE;
    STDMETHOD_ (void,   ForceUpdate())                                            PURE;
};

DEFINE_GUID(IID_IMassProperties, 0x3220df91, 0x6200, 0x4f68, 0xae, 0x5e, 0xee, 0xe8, 0x12, 0x69, 0x5b, 0x8b);

/**********************************************************************************************************
**********************************************************************************************************/
DECLARE_INTERFACE_ (IForceMoments, IUnknown)
{
    STDMETHOD_ (BOOL, RegisterElement(__in IForceElement* pElement))        PURE;
    STDMETHOD_ (BOOL, UnRegisterElement(__in IForceElement* pElement))      PURE;
    STDMETHOD_ (UINT, ElementCount())                                 const PURE;
    STDMETHOD_ (IForceElement*, GetElement(int index))                const PURE;
};

DEFINE_GUID(IID_IForceMoments, 0x663a2f20, 0x189d, 0x4797, 0xa2, 0x93, 0xb8, 0x45, 0x1d, 0x7, 0x8e, 0x0);

/**********************************************************************************************************
**********************************************************************************************************/
DECLARE_INTERFACE_ (ICollisionService, IUnknown)
{
    STDMETHOD_ (BOOL, InvokesCrashOnOtherObjects())                             PURE;
    STDMETHOD_ (void, SetInvokesCrashOnOtherObjects(__in BOOL invokesCrash))    PURE;
};

DEFINE_GUID(IID_ICollisionService, 0x231dfb4a, 0xf68f, 0x4b0e, 0x88, 0xaf, 0xbd, 0x97, 0x30, 0x8d, 0xd0, 0x12);

/**********************************************************************************************************
**********************************************************************************************************/
DECLARE_INTERFACE_ (IAircraftService, IUnknown)
{
    STDMETHOD_(float, GetIndicatedAirspeed)() const PURE;  //Feet per second
};

DEFINE_GUID(IID_IAircraftService, 0x402d68a6, 0x156a, 0x45ed, 0x92, 0x22, 0xdb, 0x8b, 0xd0, 0x1, 0xbd, 0x3);

/**********************************************************************************************************
**********************************************************************************************************/
DECLARE_INTERFACE_ (IBoatService, IUnknown)
{
};

DEFINE_GUID(IID_IBoatService, 0x6505f704, 0xf983, 0x4ccc, 0xb1, 0xed, 0xf4, 0xa4, 0x3, 0xf4, 0xb9, 0x8a);

/**********************************************************************************************************
**********************************************************************************************************/
DECLARE_INTERFACE_ (IGroundVehicleService, IUnknown)
{
};

DEFINE_GUID(IID_IGroundVehicleService, 0xc6661c56, 0xaa27, 0x4199, 0xa3, 0x97, 0x1c, 0xb9, 0x48, 0xfe, 0x84, 0x65);

/**********************************************************************************************************
**********************************************************************************************************/
DECLARE_INTERFACE_ (IAtcService, IUnknown)
{
};

DEFINE_GUID(IID_IAtcService, 0x8f23e25, 0xf169, 0x4d4e, 0xa3, 0x45, 0xe7, 0xb0, 0xa3, 0xdf, 0x46, 0x1f);

/**********************************************************************************************************
**********************************************************************************************************/
DECLARE_INTERFACE_ (IRadarSignatureService, IUnknown)
{
};
 
DEFINE_GUID(IID_IRadarSignatureService, 0x77dab198, 0xdf1d, 0x4fa8, 0x98, 0x18, 0xdb, 0x80, 0x41, 0x38, 0x71, 0x29);

/**********************************************************************************************************
**********************************************************************************************************/
DECLARE_INTERFACE_ (IDoorService, IUnknown)
{
    STDMETHOD_(float, GetDoorPercentOpen)(__in int doorIndex)                             const PURE;
};

DEFINE_GUID(IID_IDoorService, 0x4dcb87d6, 0xeea7, 0x4562, 0x94, 0x81, 0xa3, 0x94, 0x8b, 0x50, 0x1, 0x72);

/**********************************************************************************************************
**********************************************************************************************************/

DECLARE_INTERFACE_ (IMarkerManagerV01, IUnknown)
{
    STDMETHOD (CreateObjectMarker)      (__in UINT idObject, __out HANDLE& hHandle)            PURE;
    STDMETHOD (UpdateObjectMarkerOffset)(__in const HANDLE hHandle, __in const FXYZ& vOffset)  PURE;
    STDMETHOD (RemoveMarker)            (__inout HANDLE& hHandle)                              PURE;
};

DEFINE_GUID(IID_IMarkerManagerV01,    0xc2982c23, 0x5403, 0x4b8e, 0x87, 0xb6, 0xf7, 0x17, 0x1b, 0xa3, 0xef, 0xd0);

/**********************************************************************************************************
**********************************************************************************************************/

DECLARE_INTERFACE_(IAttachmentServiceV420, IUnknown)
{
    /** Gets the attach point index from the given name.
    * @param pszAttachPointName The name of the attach point.
    * @param uIndex             The index of the given attach point.
    * @return                   S_OK if the attach point index was successfully found, E_FAIL otherwise.
    */
    STDMETHOD(GetAttachPointIndex)(__in __notnull LPCSTR pszAttachPointName, __out UINT& uIndex) const PURE;
    /** Gets the attach point count.
    * @param uCount             The number of attach points.
    * @return                   S_OK if the attach point count was successfully found, E_FAIL otherwise.
    */
    STDMETHOD(GetAttachPointCount)(__out UINT& uCount) const PURE;
    /** Gets the offset of the attach point with the given index.
    * @param uIndex             The attach point index.
    * @param vOffsetFeet        The body offset of the attach point in feet.
    * @return                   S_OK if the attach point offset was successfully found, E_FAIL otherwise.
    */
    STDMETHOD(GetAttachPointOffset)(__in UINT uIndex, __out DXYZ& vOffsetFeet) const PURE;
    /** Gets the orientation of the attach point with the given index.
    * @param uIndex                 The attach point index.
    * @param vOrientationRadians    The body orientation offset of the attach point in radians.
    * @return                       S_OK if the attach point orientation was successfully found, E_FAIL otherwise.
    */
    STDMETHOD(GetAttachPointOrientation)(__in UINT uIndex, __out DXYZ& vOrientationRadians) const PURE;
    /** Gets the number of attached objects.
    * @param nObjects       The number of attached objects.
    * @return               S_OK if the attached objects were successfully found, E_FAIL otherwise.
    */
    STDMETHOD(GetAttachedObjectCount)(__out UINT& nObjects) const PURE;
    /** Gets a list of all attached objects.
    * @param nObjects IN:   The max number of objects requested. This must be no smaller than the size of the array pointed to by rgObjectIDs.
    * @param nObjects OUT:  The actual number of objects found.
    * @param rgObjectIDs    Address of array in which object IDs are returned.
    * @return               S_OK if the attached objects were successfully found, E_FAIL otherwise.
    * NOTE: It is the callers responsibility to allocate the array's required memory.
    */
    STDMETHOD(GetAttachedObjects)(__inout UINT& nObjects, __out UINT* rgObjectIDs) const PURE;
    /** Gets the attach point index for the given attached object id.
    * @param uObjectId  The attached object's id.
    * @param uIndex     The attached object's attach point index.
    * @return           S_OK if the attached object's attach point index is successfully found, E_FAIL otherwise.
    */
    STDMETHOD(GetAttachedObjectIndex)(__in UINT uObjectId, __out UINT& uIndex) const PURE;
    /** Gets the attached object id for the given attach point index.
    * @param uIndex     The attach point index.
    * @param uObjectId  The attached object's id.
    * @return           S_OK if the attach point index's attached object id is successfully found, E_FAIL otherwise.
    */
    STDMETHOD(GetAttachedObjectId)(__in UINT uIndex, __out UINT& uObjectId) const PURE;
    /** Updates the location and orientation of all attach points. Should be called after an
    *   object has simulated to ensure attached objects are in the correct location.
    * @return                   S_OK if at least on attach point was updated, S_FALSE otherwise.
    */
    STDMETHOD(UpdateAttachments)() PURE;
};

DEFINE_GUID(IID_IAttachmentServiceV420, 0x358adf81, 0x3ed2, 0x42dc, 0x82, 0x88, 0x67, 0xce, 0x14, 0x59, 0x78, 0xb8);

/**********************************************************************************************************
**********************************************************************************************************/

} //End namespace P3D
