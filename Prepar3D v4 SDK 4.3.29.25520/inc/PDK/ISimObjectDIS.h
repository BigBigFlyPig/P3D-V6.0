//-----------------------------------------------------------------------------------
// ISimObjectDIS.h
// Copyright (C) Lockheed Martin.  All rights reserved.   
// Provides public interfaces for dll implementations
//-----------------------------------------------------------------------------------

#pragma once

#include "Legacy\ISimObjectDIS.Legacy.h"

namespace P3D
{

/** @addtogroup isim */ /** @{ */

class ArticulatedPart
{
public:

    unsigned char   m_yRecordType;
    unsigned char   m_yChangeIndicator;
    unsigned short  m_usAttachedToId;
    unsigned int    m_uiParameterType;
    float           m_fParameterValue;
    float           m_fPadding;
};

class ArticulatedParameter
{
public:
    union
    {
        ArticulatedPart m_ArticulatedPart;
    };
};

/** ## *Professional Plus Only*
* This service allows the developer to interact and retrieve information with a distributed interactive simulation (DIS) session. 
* Developers integrating with this interface should be familiar with and are expected to follow DIS IEEE standards. This service is 
* provided by the IPdk interface.*/
DECLARE_INTERFACE_(IDISManagerV420, IDISManagerV400)
{
    /** Returns TRUE if a DIS connection is active, FALSE otherwise. */
    STDMETHOD_(BOOL, IsConnected)() const PURE;

    /** Provides the EntityType for the given object ID if successful. */
    STDMETHOD (GetEntityTypeById)(__in UINT32 uID, __out P3D::EntityType& EntityType) const PURE;

    /** Used to issue a Fire PDU. */
    STDMETHOD (NotifyMunitionFired)(__in UINT32 uAttackerID,                        /**< The object ID of the firing entity */
                                    __in UINT32 uTargetID,                          /**< The object ID of the target entity if available, 0 otherwise */
                                    __in UINT32 uMunitionID,                        /**< The object ID of the munition entity if available, 0 otherwise */
                                    __in const P3D::EntityType& EntityType,         /**< The EntityType of the munition */
                                    __in const P3D::DXYZ& xyzLonAltLat,             /**< World location in radians and feet */
                                    __in const P3D::DXYZ& xyzLinearVelocity,        /**< World velocity in feet per second */
                                    __in unsigned short usWarheadType,              /**< The warhead type */
                                    __in unsigned short usFuseType,                 /**< The fuse type */
                                    __in unsigned short usQuantity,                 /**< The quantity of munitions represented */
                                    __in unsigned short usRate,                     /**< Rounds per minute */
                                    __in float fRange,                              /**< Meters */
                                    __inout unsigned short& usEventID               /**< Set to 0 for new Fire PDU or previously returned value to signify continuous firing */
                                    ) PURE;

    /** Used to issue a Detonation PDU. */
    STDMETHOD (NotifyMunitionDetonated)(__in UINT32 uAttackerID,                    /**< The object ID of the firing entity */
                                        __in UINT32 uTargetID,                      /**< The object ID of the target entity if available, 0 otherwise */
                                        __in UINT32 uMunitionID,                    /**< The object ID of the munition entity if available, 0 otherwise */
                                        __in const P3D::EntityType& EntityType,     /**< The EntityType of the munition */
                                        __in unsigned short usEventID,              /**< The event ID from an associated Fire PDU if available, 0 otherwise */
                                        __in const P3D::DXYZ& xyzLonAltLat,         /**< Radians/feet */
                                        __in const P3D::DXYZ& xyzLinearVelocity,    /**< World/FPS */
                                        __in unsigned short usWarheadType,          /**< The warhead type */
                                        __in unsigned short usFuseType,             /**< The fuse type */
                                        __in unsigned short usQuantity,             /**< The quantity of munitions represented */
                                        __in unsigned short usRate,                 /**< The rate of fire in rounds per minute */
                                        __in unsigned char yDetonationResult        /**< The result of the detonation */
                                        ) PURE;
                                        
    /** Returns the entity identifier for the given object id if successful. */
    STDMETHOD(GetEntityIdByObjectId)(__in UINT32 uObjectId,                         /**< The object id of the request */
                                     __out unsigned short& usSiteId,                /**< The site id of the entity identifier */
                                     __out unsigned short& usApplicationId,         /**< The application id of the entity identifier */
                                     __out unsigned short& usEntityId               /**< The entity/object id of the entity identifier */
                                     ) PURE;

    /** Returns the object id for the given entity identifier if successful. */
    STDMETHOD(GetObjectIdByEntityId)(__in unsigned short usSiteId,                  /**< The site id of the entity identifier */
                                     __in unsigned short usApplicationId,           /**< The application id of the entity identifier */
                                     __in unsigned short usEntityId,                /**< The entity/object id of the entity identifier */
                                     __out UINT32& uObjectId                        /**< The object id of the request */
                                     ) PURE;
};

DEFINE_GUID(IID_IDISManagerV420,  0x3cf01842, 0x3127, 0x4259, 0x83, 0xee, 0x71, 0x75, 0xfa, 0x2e, 0x26, 0xf5);
DEFINE_GUID(SID_DISManager,       0x84573c4c, 0x6422, 0x43f7, 0xb0, 0xe3, 0x26, 0x87, 0x68, 0x9e, 0x40, 0x9);

/** ## *Professional Plus Only*
* This service allows developers to provide Distributed Interactive Simulation (DIS) information to the core simulation. 
* Developers should implement this service and provide the requested information following DIS IEEE standards. */
DECLARE_INTERFACE_(IDISServiceV400, IUnknown)
{
    /** This function will be called when the application requires the object's entity appearance. The entity appearance is a 32-bit unsigned integer. 
    * The application expects the data to be packed according to DIS standards. The application expects the appearance type to match that of the entity 
    * type and domain (platform, air, land, munition, expendable, etc.). The function should return S_OK if the entity appearance is being provided by 
    * the ISimObject. */
    STDMETHOD (SerializeEntityAppearance)(__inout UINT& iAppearance) PURE;
    /** This function will be called on remote entities when the appearance needs to be updated. The data should be deserialized in the same manner as 
    * described above. The function should return S_OK if the entity appearance was correctly deserialized by the ISimObject. */
    STDMETHOD (DeserializeEntityAppearance)(__in UINT iAppearance) PURE;
    
    /** This function is called when requesting the articulated parameter count. The function should return S_OK and provide the number of articulated 
    * parameters if the ISimObject is providing articulated parameter support. */
    STDMETHOD (GetArticulatedParameterCount)(__inout UINT& iCount) PURE;

    /** This function is called on a given articulated parameter when the application is requesting an update. The ArticulatedParameter union class should be 
    * filled out in accordance to DIS standards. This function maybe called by the application when an entity state PDU is required due to heartbeat duration 
    * or articulated parameter position or rotation threshold values being exceeded. The function should return S_OK if the articulated parameter was correctly serialized. */
    STDMETHOD (SerializeArticulatedParameter)(__in UINT iIndex, __inout ArticulatedParameter& ArticulatedParam) PURE;
    /** This function is called on remote objects when the given articulated parameter needs to be updated. This function maybe called by the application 
    * when an entity state PDU is received or due to the dead reckoning of the parameter. The function should return S_OK if the articulated parameter was 
    * correctly deserialized. */
    STDMETHOD (DeserializeArticulatedParameter)(__in UINT iIndex, __in const ArticulatedParameter& ArticulatedParam) PURE;
};

DEFINE_GUID(IID_IDISServiceV400,    0xcd623b32, 0x5f8b, 0x409f, 0x84, 0x2a, 0x6, 0xd5, 0x20, 0xb7, 0x53, 0x17);
DEFINE_GUID(SID_DISService,         0xa2202ec4, 0xb81c, 0x4c0b, 0xbd, 0xb7, 0x9f, 0xc6, 0x82, 0x40, 0xc8, 0x43);

/**************************************************************************************************************
**************************************************************************************************************/

/** @} */

} //End namespace P3D
